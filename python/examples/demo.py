#!/usr/bin/env python3

"""Minimal Packfall library demo.

Run from the repo root or from python/examples:

    python python/examples/demo.py
"""

from __future__ import annotations

import ctypes
import os
from pathlib import Path
import sys


def find_repo_root() -> Path:
    here = Path(__file__).resolve()
    return here.parents[2]


def candidate_library_paths(repo_root: Path) -> list[Path]:
    return [
        repo_root / "build" / "msys2" / "libpackfall.dll",
        repo_root / "build" / "linux" / "libpackfall.so",
    ]


def candidate_runtime_paths(repo_root: Path) -> list[Path]:
    return [
        repo_root / "build" / "msys2",
        Path("C:/apps/msys64/mingw64/bin"),
        Path("C:/msys64/mingw64/bin"),
    ]


def load_packfall_library(repo_root: Path) -> ctypes.CDLL:
    if os.name == "nt":
        for runtime_path in candidate_runtime_paths(repo_root):
            if runtime_path.exists():
                os.add_dll_directory(str(runtime_path))

    for path in candidate_library_paths(repo_root):
        if path.exists():
            return ctypes.CDLL(str(path))

    tried = "\n".join(str(path) for path in candidate_library_paths(repo_root))
    raise OSError(f"Could not find a Packfall shared library. Tried:\n{tried}")


def main() -> int:
    if len(sys.argv) != 1:
        print("Syntax: demo.py")
        return 1

    repo_root = find_repo_root()
    demo_dir = Path(__file__).resolve().parent
    lib = load_packfall_library(repo_root)

    lammps_open_no_mpi = lib.lammps_open_no_mpi
    lammps_open_no_mpi.argtypes = [ctypes.c_int, ctypes.POINTER(ctypes.c_char_p), ctypes.POINTER(ctypes.c_void_p)]

    lammps_close = lib.lammps_close
    lammps_close.argtypes = [ctypes.c_void_p]

    lammps_file = lib.lammps_file
    lammps_file.argtypes = [ctypes.c_void_p, ctypes.c_char_p]

    lammps_command = lib.lammps_command
    lammps_command.argtypes = [ctypes.c_void_p, ctypes.c_char_p]

    lib.lammps_extract_global.argtypes = [ctypes.c_void_p, ctypes.c_char_p]
    lib.lammps_extract_atom.argtypes = [ctypes.c_void_p, ctypes.c_char_p]
    lib.lammps_extract_compute.argtypes = [ctypes.c_void_p, ctypes.c_char_p, ctypes.c_int, ctypes.c_int]
    lib.lammps_extract_variable.argtypes = [ctypes.c_void_p, ctypes.c_char_p, ctypes.c_char_p]
    lib.lammps_get_natoms.argtypes = [ctypes.c_void_p]
    lib.lammps_gather_atoms.argtypes = [ctypes.c_void_p, ctypes.c_char_p, ctypes.c_int, ctypes.c_int, ctypes.c_void_p]
    lib.lammps_scatter_atoms.argtypes = [ctypes.c_void_p, ctypes.c_char_p, ctypes.c_int, ctypes.c_int, ctypes.c_void_p]
    lib.lammps_free.argtypes = [ctypes.c_void_p]

    lmp = ctypes.c_void_p()
    lammps_open_no_mpi(0, None, ctypes.byref(lmp))

    try:
        commands = [
            "atom_style granular",
            "atom_modify map array",
            "boundary f f f",
            "newton off",
            "communicate single vel yes",
            "units si",
            "region box block 0 0.05 0 0.05 0 0.05 units box",
            "create_box 1 box",
            "neighbor 0.001 bin",
            "neigh_modify delay 0",
            "fix m1 all property/global youngsModulus peratomtype 5.e6",
            "fix m2 all property/global poissonsRatio peratomtype 0.45",
            "fix m3 all property/global coefficientRestitution peratomtypepair 1 0.3",
            "fix m4 all property/global coefficientFriction peratomtypepair 1 0.5",
            "pair_style gran model hertz tangential history",
            "pair_coeff * *",
            "create_atoms 1 single 0.015 0.015 0.015 units box",
            "create_atoms 1 single 0.03 0.03 0.03 units box",
            "set atom 1 diameter 0.01 density 2500.0",
            "set atom 2 diameter 0.01 density 2500.0",
            "velocity all set 0.1 0.0 0.0 sum no units box",
            "fix 1 all nve/sphere",
            "variable eng equal ke",
            "variable vy atom vy",
            "run 10",
        ]

        for command in commands:
            lammps_command(lmp, command.encode())

        print("\nPython output:")

        lib.lammps_extract_global.restype = ctypes.POINTER(ctypes.c_int)
        natoms = lib.lammps_extract_global(lmp, b"natoms")[0]

        lib.lammps_extract_atom.restype = ctypes.POINTER(ctypes.c_double)
        mass = lib.lammps_extract_atom(lmp, b"rmass")

        lib.lammps_extract_atom.restype = ctypes.POINTER(ctypes.POINTER(ctypes.c_double))
        x = lib.lammps_extract_atom(lmp, b"x")
        print(f"Natoms, mass, x[0][0] coord = {natoms} {mass[1]} {x[0][0]}")

        lib.lammps_extract_compute.restype = ctypes.POINTER(ctypes.c_double)
        temp = lib.lammps_extract_compute(lmp, b"thermo_temp", 0, 0)[0]
        print(f"Temperature from compute = {temp}")

        lib.lammps_extract_variable.restype = ctypes.POINTER(ctypes.c_double)
        eng_ptr = lib.lammps_extract_variable(lmp, b"eng", None)
        eng = eng_ptr[0]
        lib.lammps_free(eng_ptr)
        print(f"Energy from equal-style variable = {eng}")

        lib.lammps_extract_global.restype = ctypes.POINTER(ctypes.c_int)
        nlocal = lib.lammps_extract_global(lmp, b"nlocal")[0]
        vy_ptr = lib.lammps_extract_variable(lmp, b"vy", b"all")
        vy = [vy_ptr[i] for i in range(nlocal)]
        lib.lammps_free(vy_ptr)
        print(f"Velocity component from atom-style variable = {vy[1]}")

        natoms = lib.lammps_get_natoms(lmp)
        print(f"Natoms from get_natoms = {natoms}")

        coords = (ctypes.c_double * (3 * natoms))()
        lib.lammps_gather_atoms(lmp, b"x", 1, 3, coords)
        print(f"Global coords from gather_atoms = {coords[0]} {coords[1]} {coords[5]}")

        coords[0] += 1.0
        lib.lammps_scatter_atoms(lmp, b"x", 1, 3, coords)
        print(f"Changed x[0][0] via scatter_atoms = {x[0][0]}")

    finally:
        lammps_close(lmp)

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
