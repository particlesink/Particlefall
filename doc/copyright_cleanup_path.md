# Copyright cleanup path

This note defines the first release-oriented copyright cleanup path for
Packfall. The goal is not to rewrite every inherited header immediately. The
goal is to make the supported release surfaces accurate, attributed, and
non-misleading.

## Public release model

GitHub does not receive the full local development history. Public releases are
prepared by squashing the intended release snapshot onto a release branch and
pushing that branch only.

That means the cleanup target is the release snapshot itself:

- files present in the squashed public branch
- runtime strings and identifiers emitted by the shipped executable
- top-level docs, tests, and tutorials that public users will actually read

It does not require rewriting or sanitizing the entire private local history
before a first GitHub release.

## Release-first scope

Finish these surfaces before the next GitHub release:

1. Runtime identity
   Ensure the executable banner and top-level build docs identify the program as
   Packfall rather than presenting the release as LIGGGHTS.
2. Top-level docs
   Keep `README.md`, build notes, and release notes explicit that Packfall is a
   derived work and that superquadrics are currently out of the default release
   scope.
3. User-facing examples and tutorials
   Prioritize tutorial folders and example READMEs that are likely to be linked
   from the release notes or copied by new users. These may still ship even if
   they are not part of the currently validated support surface, provided that
   the release framing makes that distinction clear.
4. Tests and release gates
   Keep only supported features in the default smoke and regression guidance so
   the advertised release path matches what is actually shipped.

## Initial public-snapshot hit list

These are the first files to audit for the squashed release branch:

- `README.md`
- `src/Make.sh`
- `src/Makefile.msys2`
- `tests/README.md`
- `tests/manifest.json`
- `examples/LIGGGHTS/Tutorials_public/mn_example/in.hopper_2d.liggghts`
- `examples/LIGGGHTS/Tutorials_public/pbc_periodic_mesh/README.txt`

These files either define the public identity of the release, the supported
build/test path, or an example likely to be copied by a new user.

The full built manual can also ship in the public snapshot, even when it still
contains inherited LIGGGHTS-specific material, so long as the top-level release
framing explains that the manual is a reference snapshot rather than a guarantee
that every page describes a currently validated Packfall workflow.

## Deferred scope

These items remain important, but they do not block the first release:

- bulk normalization of inherited source-file headers across `src/`
- full doc-tree terminology cleanup
- large legacy manual sections that still describe historical LIGGGHTS-specific
  workflows
- superquadric-specific docs and examples while that feature remains WIP

## Working method

Use this order for the cleanup:

1. fix the default runtime/build/test path
2. fix the docs a new user reads first
3. fix the examples a new user is likely to run
4. only then start broader header sweeps across inherited source files

When in doubt, prefer editing the small set of files that will be present in the
public squashed branch rather than trying to normalize the whole private tree at
once.

## Audit targets

Search for these terms during each pass:

- `LIGGGHTS`
- `CFDEM`
- `DCS Computing`
- `cfdem.com`
- `liggghts.com`

For each hit, classify it before editing:

- required legal attribution to keep
- historically accurate provenance that can remain in source headers
- user-facing branding that should become `Packfall`
- outdated support or endorsement wording that should be removed or rewritten
