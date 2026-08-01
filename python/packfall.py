# Python wrapper for the Packfall shared library via ctypes.

from __future__ import annotations

import os
from pathlib import Path
import sys
from ctypes import CDLL, POINTER, RTLD_GLOBAL, byref, c_char_p, c_double, c_int, c_void_p


class Packfall:
    def __init__(self, name: str = "", cmdargs: list[str] | None = None):
        self.lmp = None
        self.lib = self._load_library(name)
        self._configure_api()

        argv = list(cmdargs) if cmdargs else []
        if argv:
            argv.insert(0, "packfall.py")
            encoded = [arg.encode() for arg in argv]
            cargs = (c_char_p * len(encoded))(*encoded)
            self.lmp = c_void_p()
            self.lib.packfall_open_no_mpi(len(encoded), cargs, byref(self.lmp))
        else:
            self.lmp = c_void_p()
            self.lib.packfall_open_no_mpi(0, None, byref(self.lmp))

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc, tb):
        self.close()
        return False

    def __del__(self):
        if getattr(self, "lmp", None):
            self.close()

    def close(self):
        if self.lmp:
            self.lib.packfall_close(self.lmp)
            self.lmp = None

    def _configure_api(self):
        self.lib.packfall_open_no_mpi.argtypes = [c_int, POINTER(c_char_p), POINTER(c_void_p)]
        self.lib.packfall_close.argtypes = [c_void_p]
        self.lib.packfall_file.argtypes = [c_void_p, c_char_p]
        self.lib.packfall_command.argtypes = [c_void_p, c_char_p]
        self.lib.packfall_command.restype = c_char_p
        self.lib.packfall_extract_global.argtypes = [c_void_p, c_char_p]
        self.lib.packfall_extract_atom.argtypes = [c_void_p, c_char_p]
        self.lib.packfall_extract_compute.argtypes = [c_void_p, c_char_p, c_int, c_int]
        self.lib.packfall_extract_fix.argtypes = [c_void_p, c_char_p, c_int, c_int, c_int, c_int]
        self.lib.packfall_extract_variable.argtypes = [c_void_p, c_char_p, c_char_p]
        self.lib.packfall_get_natoms.argtypes = [c_void_p]
        self.lib.packfall_gather_atoms.argtypes = [c_void_p, c_char_p, c_int, c_int, c_void_p]
        self.lib.packfall_scatter_atoms.argtypes = [c_void_p, c_char_p, c_int, c_int, c_void_p]
        self.lib.packfall_free.argtypes = [c_void_p]

    def _candidate_library_paths(self, name: str):
        root = Path(__file__).resolve().parents[1]
        stem = "libpackfall" if not name else f"libpackfall_{name}"

        if os.name == "nt":
            candidates = [root / "build" / "msys2" / f"{stem}.dll"]
        else:
            candidates = [root / "build" / "linux" / f"{stem}.so"]

        if os.name == "nt":
            candidates.append(Path(f"{stem}.dll"))
        else:
            candidates.append(Path(f"{stem}.so"))

        return candidates

    def _add_runtime_dirs(self, libpath: Path):
        if os.name != "nt":
            return

        runtime_dirs = [
            libpath.parent,
            Path("C:/apps/msys64/mingw64/bin"),
            Path("C:/msys64/mingw64/bin"),
        ]

        for runtime_dir in runtime_dirs:
            if runtime_dir.exists():
                os.add_dll_directory(str(runtime_dir))

    def _load_library(self, name: str):
        errors = []
        for candidate in self._candidate_library_paths(name):
            try:
                if candidate.is_absolute() or candidate.exists():
                    self._add_runtime_dirs(candidate)
                    return CDLL(str(candidate), RTLD_GLOBAL)
            except Exception as exc:
                errors.append(f"{candidate} -> {exc}")

        stem = "libpackfall" if not name else f"libpackfall_{name}"
        suffix = ".dll" if os.name == "nt" else ".so"
        try:
            return CDLL(stem + suffix, RTLD_GLOBAL)
        except Exception as exc:
            errors.append(f"{stem + suffix} -> {exc}")
            raise OSError("Could not load Packfall dynamic library. Tried:\n" + "\n".join(errors))

    def file(self, path: str):
        self.lib.packfall_file(self.lmp, path.encode())

    def command(self, command: str):
        self.lib.packfall_command(self.lmp, command.encode())

    def extract_global(self, name: str, value_type: int):
        encoded = name.encode()
        if value_type == 0:
            self.lib.packfall_extract_global.restype = POINTER(c_int)
        elif value_type == 1:
            self.lib.packfall_extract_global.restype = POINTER(c_double)
        else:
            return None
        ptr = self.lib.packfall_extract_global(self.lmp, encoded)
        return ptr[0]

    def extract_atom(self, name: str, value_type: int):
        encoded = name.encode()
        if value_type == 0:
            self.lib.packfall_extract_atom.restype = POINTER(c_int)
        elif value_type == 1:
            self.lib.packfall_extract_atom.restype = POINTER(POINTER(c_int))
        elif value_type == 2:
            self.lib.packfall_extract_atom.restype = POINTER(c_double)
        elif value_type == 3:
            self.lib.packfall_extract_atom.restype = POINTER(POINTER(c_double))
        else:
            return None
        return self.lib.packfall_extract_atom(self.lmp, encoded)

    def extract_compute(self, compute_id: str, style: int, value_type: int):
        encoded = compute_id.encode()
        if value_type == 0:
            if style > 0:
                return None
            self.lib.packfall_extract_compute.restype = POINTER(c_double)
            ptr = self.lib.packfall_extract_compute(self.lmp, encoded, style, value_type)
            return ptr[0]
        if value_type == 1:
            self.lib.packfall_extract_compute.restype = POINTER(c_double)
            return self.lib.packfall_extract_compute(self.lmp, encoded, style, value_type)
        if value_type == 2:
            self.lib.packfall_extract_compute.restype = POINTER(POINTER(c_double))
            return self.lib.packfall_extract_compute(self.lmp, encoded, style, value_type)
        return None

    def extract_fix(self, fix_id: str, style: int, value_type: int, i: int = 0, j: int = 0):
        encoded = fix_id.encode()
        if value_type == 0:
            if style > 0:
                return None
            self.lib.packfall_extract_fix.restype = POINTER(c_double)
            ptr = self.lib.packfall_extract_fix(self.lmp, encoded, style, value_type, i, j)
            result = ptr[0]
            self.lib.packfall_free(ptr)
            return result
        if value_type == 1:
            self.lib.packfall_extract_fix.restype = POINTER(c_double)
            return self.lib.packfall_extract_fix(self.lmp, encoded, style, value_type, i, j)
        if value_type == 2:
            self.lib.packfall_extract_fix.restype = POINTER(POINTER(c_double))
            return self.lib.packfall_extract_fix(self.lmp, encoded, style, value_type, i, j)
        return None

    def extract_variable(self, name: str, group: str | None, value_type: int):
        encoded_name = name.encode()
        encoded_group = group.encode() if group else None

        if value_type == 0:
            self.lib.packfall_extract_variable.restype = POINTER(c_double)
            ptr = self.lib.packfall_extract_variable(self.lmp, encoded_name, encoded_group)
            result = ptr[0]
            self.lib.packfall_free(ptr)
            return result

        if value_type == 1:
            self.lib.packfall_extract_global.restype = POINTER(c_int)
            nlocal = self.lib.packfall_extract_global(self.lmp, b"nlocal")[0]
            result = (c_double * nlocal)()
            self.lib.packfall_extract_variable.restype = POINTER(c_double)
            ptr = self.lib.packfall_extract_variable(self.lmp, encoded_name, encoded_group)
            for i in range(nlocal):
                result[i] = ptr[i]
            self.lib.packfall_free(ptr)
            return result

        return None

    def get_natoms(self):
        return self.lib.packfall_get_natoms(self.lmp)

    def gather_atoms(self, name: str, value_type: int, count: int):
        encoded = name.encode()
        natoms = self.lib.packfall_get_natoms(self.lmp)
        if value_type == 0:
            data = ((count * natoms) * c_int)()
        elif value_type == 1:
            data = ((count * natoms) * c_double)()
        else:
            return None
        self.lib.packfall_gather_atoms(self.lmp, encoded, value_type, count, data)
        return data

    def scatter_atoms(self, name: str, value_type: int, count: int, data):
        self.lib.packfall_scatter_atoms(self.lmp, name.encode(), value_type, count, data)


# Backward-compatible alias retained for the existing example scripts.
packfall = Packfall

__all__ = ["Packfall", "packfall"]
