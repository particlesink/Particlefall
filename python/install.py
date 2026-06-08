#!/usr/bin/env python3

"""Install the Packfall Python wrapper and shared library."""

from __future__ import annotations

import shutil
import site
import sys
from pathlib import Path


def find_repo_root() -> Path:
    return Path(__file__).resolve().parents[1]


def candidate_library_paths(root: Path) -> list[Path]:
    return [
        root / "build" / "msys2" / "libpackfall.dll",
        root / "build" / "linux" / "libpackfall.so",
    ]


def default_python_dir() -> Path:
    paths = site.getsitepackages()
    return Path(paths[0])


def main() -> int:
    if len(sys.argv) > 3 or (len(sys.argv) > 1 and sys.argv[1] == "-h"):
        print("Syntax: python install.py [libdir] [pydir]")
        return 0

    root = find_repo_root()
    libdir = Path(sys.argv[1]) if len(sys.argv) >= 2 else None
    pydir = Path(sys.argv[2]) if len(sys.argv) == 3 else default_python_dir()

    library = next((path for path in candidate_library_paths(root) if path.exists()), None)
    if library is None:
        tried = "\n".join(str(path) for path in candidate_library_paths(root))
        raise SystemExit(f"Could not find a built Packfall shared library. Tried:\n{tried}")

    wrapper = root / "python" / "packfall.py"
    if not wrapper.exists():
        raise SystemExit(f"Could not find wrapper module: {wrapper}")

    if libdir is not None:
        libdir.mkdir(parents=True, exist_ok=True)
        shutil.copy2(library, libdir / library.name)
        print(f"Installed {library.name} to {libdir}")

    pydir.mkdir(parents=True, exist_ok=True)
    shutil.copy2(wrapper, pydir / "packfall.py")
    print(f"Installed packfall.py to {pydir}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
