import argparse
import json
import os
import re
import shutil
import subprocess
import sys
import time
from pathlib import Path


ROOT = Path(__file__).resolve().parent.parent
TESTS_DIR = ROOT / "tests"
ARTIFACTS_DIR = TESTS_DIR / "artifacts"
MANIFEST_PATH = TESTS_DIR / "manifest.json"
METRIC_RE = re.compile(r"^TEST_METRIC\s+([A-Za-z0-9_]+)=(.+?)\s*$")


def load_manifest():
    with MANIFEST_PATH.open("r", encoding="utf-8") as fh:
        return json.load(fh)["cases"]


def find_default_exe():
    candidates = [
        ROOT / "build" / "msys2" / "packfall.exe",
        ROOT / "build" / "linux" / "packfall",
        ROOT / "build" / "linux" / "packfall.exe",
    ]
    for candidate in candidates:
        if candidate.exists():
            return candidate
    return None


def build_runtime_env(exe):
    env = os.environ.copy()
    if exe.suffix.lower() == ".exe":
        candidates = [
            Path(r"C:\apps\msys64\mingw64\bin"),
            Path(r"C:\msys64\mingw64\bin"),
        ]
        existing = [str(path) for path in candidates if path.exists()]
        if existing:
            env["PATH"] = os.pathsep.join(existing + [env.get("PATH", "")])
    return env


def parse_args():
    parser = argparse.ArgumentParser(description="Run Packfall smoke/regression tests.")
    parser.add_argument("--exe", help="Path to the Packfall executable.")
    parser.add_argument("--case", action="append", help="Run only the named case. May be provided multiple times.")
    parser.add_argument(
        "--group",
        action="append",
        help="Run only cases in the named group. May be provided multiple times.",
    )
    parser.add_argument("--list", action="store_true", help="List available cases and exit.")
    return parser.parse_args()


def ensure_artifacts_dir():
    ARTIFACTS_DIR.mkdir(parents=True, exist_ok=True)


def coerce_metric_value(text):
    text = text.strip()
    try:
        if "." in text or "e" in text.lower():
            return float(text)
        return int(text)
    except ValueError:
        return text


def parse_metrics(output):
    metrics = {}
    for line in output.splitlines():
        match = METRIC_RE.match(line.strip())
        if match:
            metrics[match.group(1)] = coerce_metric_value(match.group(2))
    return metrics


def compare_metric(expected, actual):
    if isinstance(expected, dict):
        if "min" in expected and actual < expected["min"]:
            return False
        if "max" in expected and actual > expected["max"]:
            return False
        if "eq" in expected and actual != expected["eq"]:
            return False
        return True
    if isinstance(expected, (int, float)) and isinstance(actual, (int, float)):
        return expected == actual
    return expected == actual


def clean_required_files(case_dir, case):
    for relpath in case.get("required_files", []):
        path = case_dir / relpath
        if path.exists():
            if path.is_dir():
                shutil.rmtree(path)
            else:
                path.unlink()


def write_artifact_log(case_name, output):
    log_path = ARTIFACTS_DIR / f"{case_name}.log"
    last_error = None

    for _ in range(3):
        try:
            if log_path.exists():
                log_path.unlink()
            log_path.write_text(output, encoding="utf-8")
            return log_path
        except PermissionError as exc:
            last_error = exc
            time.sleep(0.1)

    fallback_path = ARTIFACTS_DIR / f"{case_name}.{int(time.time() * 1000)}.log"
    try:
        fallback_path.write_text(output, encoding="utf-8")
        print(
            f"Warning: could not overwrite {log_path.name}; wrote {fallback_path.name} instead.",
            file=sys.stderr,
        )
        return fallback_path
    except PermissionError:
        if last_error is not None:
            raise last_error
        raise


def run_case(exe, case):
    case_dir = ROOT / case["cwd"]
    input_name = case["input"]
    timeout_s = case.get("timeout_s", 20)
    ensure_artifacts_dir()
    clean_required_files(case_dir, case)

    command = [str(exe), "-in", input_name]
    env = build_runtime_env(exe)
    start = time.time()
    completed = subprocess.run(
        command,
        cwd=str(case_dir),
        capture_output=True,
        text=True,
        timeout=timeout_s,
        env=env,
    )
    elapsed = time.time() - start
    output = completed.stdout + completed.stderr
    expected_exit_code = case.get("expect_exit_code", 0)

    write_artifact_log(case["name"], output)

    if completed.returncode != expected_exit_code:
        return False, f"exit code expected {expected_exit_code} got {completed.returncode}", elapsed

    if expected_exit_code == 0 and "ERROR:" in output:
        return False, "simulation reported ERROR", elapsed

    for substring in case.get("required_output_substrings", []):
        if substring not in output:
            return False, f"missing output substring {substring}", elapsed

    if expected_exit_code == 0:
        metrics = parse_metrics(output)
        for key, expected_value in case.get("metrics", {}).items():
            if key not in metrics:
                return False, f"missing metric {key}", elapsed
            if not compare_metric(expected_value, metrics[key]):
                return False, f"metric {key} expected {expected_value} got {metrics[key]}", elapsed

        for relpath in case.get("required_files", []):
            path = case_dir / relpath
            if not path.exists():
                return False, f"missing required file {relpath}", elapsed

        for relpath, substrings in case.get("required_file_substrings", {}).items():
            path = case_dir / relpath
            if not path.exists():
                return False, f"missing required file {relpath}", elapsed
            content = path.read_text(encoding="utf-8", errors="ignore")
            for substring in substrings:
                if substring not in content:
                    return False, f"required file {relpath} missing substring {substring}", elapsed

    return True, "ok", elapsed


def main():
    args = parse_args()
    cases = load_manifest()

    if args.list:
        for case in cases:
            print(f"{case['name']} [{case.get('group', 'ungrouped')}]")
        return 0

    exe = Path(args.exe).resolve() if args.exe else find_default_exe()
    if exe is None or not exe.exists():
        print("Could not find Packfall executable. Use --exe to specify one.", file=sys.stderr)
        return 2

    selected_names = set(args.case or [])
    if selected_names:
        cases = [case for case in cases if case["name"] in selected_names]
        missing = selected_names.difference({case["name"] for case in cases})
        if missing:
            print("Unknown case(s): " + ", ".join(sorted(missing)), file=sys.stderr)
            return 2

    selected_groups = set(args.group or [])
    if selected_groups:
        known_groups = {case.get("group", "ungrouped") for case in load_manifest()}
        unknown_groups = selected_groups.difference(known_groups)
        if unknown_groups:
            print("Unknown group(s): " + ", ".join(sorted(unknown_groups)), file=sys.stderr)
            return 2
        cases = [case for case in cases if case.get("group", "ungrouped") in selected_groups]

    print(f"Using executable: {exe}")
    print(f"Running {len(cases)} test(s)")

    failures = []
    total_start = time.time()
    for case in cases:
        ok, message, elapsed = run_case(exe, case)
        status = "PASS" if ok else "FAIL"
        print(f"[{status}] {case['name']} ({elapsed:.2f}s) {message}")
        if not ok:
            failures.append(case["name"])

    total_elapsed = time.time() - total_start
    print(f"Completed in {total_elapsed:.2f}s")
    if failures:
        print("Failed cases: " + ", ".join(failures), file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
