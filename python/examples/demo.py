#!/usr/bin/env python3

"""Minimal Packfall library wrapper demo."""

from __future__ import annotations

import sys
from pathlib import Path

THIS_DIR = Path(__file__).resolve().parent
PYTHON_DIR = THIS_DIR.parent
if str(PYTHON_DIR) not in sys.path:
    sys.path.insert(0, str(PYTHON_DIR))

from packfall import packfall


def main() -> int:
    if len(sys.argv) != 1:
        print("Syntax: demo.py")
        return 1

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

    with packfall() as sim:
        for command in commands:
            sim.command(command)

        print("\nPython output:")
        natoms = sim.extract_global("natoms", 0)
        mass = sim.extract_atom("rmass", 2)
        x = sim.extract_atom("x", 3)
        print(f"Natoms, mass, x[0][0] coord = {natoms} {mass[1]} {x[0][0]}")

        temp = sim.extract_compute("thermo_temp", 0, 0)
        print(f"Temperature from compute = {temp}")

        eng = sim.extract_variable("eng", None, 0)
        print(f"Energy from equal-style variable = {eng}")

        vy = sim.extract_variable("vy", "all", 1)
        print(f"Velocity component from atom-style variable = {vy[1]}")

        natoms = sim.get_natoms()
        print(f"Natoms from get_natoms = {natoms}")

        coords = sim.gather_atoms("x", 1, 3)
        print(f"Global coords from gather_atoms = {coords[0]} {coords[1]} {coords[5]}")

        coords[0] += 1.0
        sim.scatter_atoms("x", 1, 3, coords)
        print(f"Changed x[0][0] via scatter_atoms = {x[0][0]}")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
