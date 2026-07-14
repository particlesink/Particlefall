## Packfall

Packfall is a Discrete Particle Simulation code derived from LIGGGHTS-PUBLIC.

## Building

Only two build systems are supported in this tree:

```sh
make linux
make msys2
```

Both entry points compile `src/*.cpp` directly, generate the required style headers via `src/Make.sh`, and write artifacts under `build/linux` or `build/msys2`.

The historical upstream `lib/*` package-library workflow is not used by the
supported Packfall build, with the exception of the retained `lib/poems`
library that backs the in-tree `POEMS` package.

The default build is single-process and uses the bundled MPI shim in `src/STUBS`. Real MPI is optional and must be enabled explicitly:

```sh
make linux
make msys2
make linux USE_MPI=1
make msys2 USE_MPI=1
```

The default release path currently targets sphere, SPH, and SPH-DEM workflows.
Superquadric support remains work in progress and is not part of the default
build-and-smoke gate yet.

## Documentation Status

Packfall currently ships a mix of validated workflows, inherited examples, and
legacy manual content carried forward from the LIGGGHTS-PUBLIC codebase.

- The default supported release path is the build-and-smoke workflow described
  above.
- Additional examples and tutorials may still ship even when they are not part
  of the current validation set.
- Built documentation may include inherited LIGGGHTS-era material that remains
  useful for reference, but does not by itself imply that every documented
  feature is currently validated in Packfall.

## Attribution

This repository contains code derived from LIGGGHTS-PUBLIC and LAMMPS. `LIGGGHTS` and `CFDEM` are registered trademarks of DCS Computing GmbH. Packfall is not approved or endorsed by DCS Computing GmbH. See `LICENSE` and source-file headers for license, copyright, and attribution details.
