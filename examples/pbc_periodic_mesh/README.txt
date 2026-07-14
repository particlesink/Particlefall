Periodic boundary condition tutorial examples
============================================

This folder contains a small tutorial suite for exercising Packfall's
periodic-boundary-condition implementation together with triangulated mesh
walls.

Each case demonstrates two things at once:

1. a particle wraps across a single periodic box direction, and
2. a `fix mesh/surface` wall geometry deliberately extends beyond that same
   periodic box face.

Files
-----

`in.pff`
`boundary p f f`

The x direction is periodic. A particle moves in +x and wraps across the x
boundary. The floor mesh is wider than the box in x, so part of the mesh lies
 outside the periodic x limits.

`in.fpf`
`boundary f p f`

The y direction is periodic. A particle moves in +y and wraps across the y
boundary. The floor mesh is wider than the box in y.

`in.ffp`
`boundary f f p`

The z direction is periodic. A particle moves in +z and wraps across the z
boundary. A vertical mesh plate is taller than the box in z, so part of the
mesh lies outside the periodic z limits.

How to use
----------

Run one of the input files with Packfall, for example:

`packfall -in in.pff`

Each case writes VTK particle dumps and mesh STL dumps in the local folder.
Inspect them in ParaView to confirm:

* the particle wraps cleanly across the periodic direction, and
* the mesh remains usable even though its geometry overhangs the periodic box.

Notes
-----

These are setup and visualization examples, not validation benchmarks. They
are meant to be easy to read and to provide a concrete starting point when
debugging or extending PBC-related behavior.
