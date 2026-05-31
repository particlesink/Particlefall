## Packfall

Packfall is a Discrete Particle Simulation code based on LIGGGHTS-PUBLIC.

## Building

Only two build systems are supported in this tree:

```sh
make linux
make msys2
```

Both entry points compile `src/*.cpp` directly, generate the required style headers via `src/Make.sh`, and write artifacts under `build/linux` or `build/msys2`.

The default build is single-process and uses the bundled MPI shim in `src/STUBS`. Real MPI is optional and must be enabled explicitly:

```sh
make linux
make msys2
make linux USE_MPI=1
make msys2 USE_MPI=1
```

## Attribution

This repository contains code derived from LIGGGHTS-PUBLIC and LAMMPS. `LIGGGHTS` and `CFDEM` are registered trademarks of DCS Computing GmbH. Packfall is not approved or endorsed by DCS Computing GmbH. See `LICENSE` and source-file headers for license, copyright, and attribution details.
