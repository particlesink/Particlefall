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
EXAMPLES_DIR = ROOT / "examples"
ARTIFACTS_DIR = EXAMPLES_DIR / "artifacts"
MANIFEST_PATH = EXAMPLES_DIR / "manifest.json"


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
    parser = argparse.ArgumentParser(description="Run Packfall example integration checks.")
    parser.add_argument("--exe", help="Path to the Packfall executable.")
    parser.add_argument("--case", action="append", help="Run only the named case. May be provided multiple times.")
    parser.add_argument(
        "--group",
        action="append",
        help="Run only cases in the named group. May be provided multiple times.",
    )
    parser.add_argument("--clean", action="store_true", help="Clean generated example outputs and exit.")
    parser.add_argument("--list", action="store_true", help="List available cases and exit.")
    return parser.parse_args()


def ensure_artifacts_dir():
    ARTIFACTS_DIR.mkdir(parents=True, exist_ok=True)


def clean_directory_contents(path):
    for child in path.iterdir():
        if child.name.startswith("."):
            continue
        remove_generated_path(child)


def is_tracked(path):
    relpath = path.resolve().relative_to(ROOT)
    completed = subprocess.run(
        ["git", "ls-files", "--error-unmatch", str(relpath)],
        cwd=str(ROOT),
        stdout=subprocess.DEVNULL,
        stderr=subprocess.DEVNULL,
        check=False,
    )
    return completed.returncode == 0


def remove_generated_path(path):
    tracked = is_tracked(path)
    if tracked:
        subprocess.run(
            ["git", "restore", "--worktree", str(path.resolve().relative_to(ROOT))],
            cwd=str(ROOT),
            stdout=subprocess.DEVNULL,
            stderr=subprocess.DEVNULL,
            check=False,
        )
        return
    if not path.exists():
        return
    if path.is_dir():
        clean_directory_contents(path)
    else:
        path.unlink()


def clean_case_outputs(case_dir, case):
    for relpath in case.get("cleanup_paths", []):
        path = case_dir / relpath
        if path.exists():
            remove_generated_path(path)

    for pattern in case.get("cleanup_globs", []):
        for path in case_dir.glob(pattern):
            remove_generated_path(path)

    for path in case_dir.glob("log.*"):
        remove_generated_path(path)


def clean_runner_artifacts():
    if ARTIFACTS_DIR.exists():
        shutil.rmtree(ARTIFACTS_DIR)


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


def require_substrings(text, substrings, label):
    for substring in substrings:
        if substring not in text:
            return f"{label} missing substring {substring}"
    return None


def forbid_substrings(text, substrings, label):
    for substring in substrings:
        if substring in text:
            return f"{label} contains forbidden substring {substring}"
    return None


def require_regexes(text, patterns, label):
    for pattern in patterns:
        if re.search(pattern, text, flags=re.MULTILINE) is None:
            return f"{label} missing regex {pattern}"
    return None


def check_globs(case_dir, glob_specs):
    for spec in glob_specs:
        pattern = spec["pattern"]
        matches = sorted(case_dir.glob(pattern))
        min_count = spec.get("min_count", 1)
        max_count = spec.get("max_count")

        if len(matches) < min_count:
            return f"glob {pattern} matched {len(matches)} path(s), expected at least {min_count}"
        if max_count is not None and len(matches) > max_count:
            return f"glob {pattern} matched {len(matches)} path(s), expected at most {max_count}"

        substrings = spec.get("required_substrings", [])
        regexes = spec.get("required_regex", [])
        min_size = spec.get("min_size_bytes")
        for path in matches:
            if min_size is not None and path.stat().st_size < min_size:
                return f"glob match {path.relative_to(case_dir)} smaller than {min_size} bytes"
            if substrings or regexes:
                content = path.read_text(encoding="utf-8", errors="ignore")
                msg = require_substrings(content, substrings, f"file {path.relative_to(case_dir)}")
                if msg:
                    return msg
                msg = require_regexes(content, regexes, f"file {path.relative_to(case_dir)}")
                if msg:
                    return msg
    return None


def run_case(exe, case):
    case_dir = ROOT / case["cwd"]
    timeout_s = case.get("timeout_s", 120)
    command = case.get("command", [str(exe), "-in", case["input"]])
    if command and command[0] == "{exe}":
        command = [str(exe)] + command[1:]
    elif command:
        command = [str(exe) if token == "{exe}" else token for token in command]

    ensure_artifacts_dir()
    clean_case_outputs(case_dir, case)

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

    if expected_exit_code == 0:
        msg = forbid_substrings(output, case.get("forbidden_output_substrings", ["ERROR:"]), "output")
        if msg:
            return False, msg, elapsed

    msg = require_substrings(output, case.get("required_output_substrings", []), "output")
    if msg:
        return False, msg, elapsed

    msg = require_regexes(output, case.get("required_output_regex", []), "output")
    if msg:
        return False, msg, elapsed

    for relpath in case.get("required_files", []):
        path = case_dir / relpath
        if not path.exists():
            return False, f"missing required file {relpath}", elapsed

    for relpath, substrings in case.get("required_file_substrings", {}).items():
        path = case_dir / relpath
        if not path.exists():
            return False, f"missing required file {relpath}", elapsed
        content = path.read_text(encoding="utf-8", errors="ignore")
        msg = require_substrings(content, substrings, f"file {relpath}")
        if msg:
            return False, msg, elapsed

    for relpath, regexes in case.get("required_file_regex", {}).items():
        path = case_dir / relpath
        if not path.exists():
            return False, f"missing required file {relpath}", elapsed
        content = path.read_text(encoding="utf-8", errors="ignore")
        msg = require_regexes(content, regexes, f"file {relpath}")
        if msg:
            return False, msg, elapsed

    msg = check_globs(case_dir, case.get("required_globs", []))
    if msg:
        return False, msg, elapsed

    return True, "ok", elapsed


def main():
    args = parse_args()
    cases = load_manifest()

    if args.list:
        for case in cases:
            print(f"{case['name']} [{case.get('group', 'ungrouped')}]")
        return 0

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

    if args.clean:
        for case in cases:
            clean_case_outputs(ROOT / case["cwd"], case)
        clean_runner_artifacts()
        return 0

    exe = Path(args.exe).resolve() if args.exe else find_default_exe()
    if exe is None or not exe.exists():
        print("Could not find Packfall executable. Use --exe to specify one.", file=sys.stderr)
        return 2

    print(f"Using executable: {exe}")
    print(f"Running {len(cases)} example case(s)")

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
