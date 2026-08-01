Superquadric tutorial examples
==============================

This folder contains a small tutorial suite for Packfall
superquadric support. Each input focuses on a specific feature set so the
examples stay readable.

Files
-----

`in.particle_particle`
Baseline contact example with primitive walls, two superquadric templates,
random and constant insertion orientations, and VTK output for ParaView.

`in.orientation_modes`
Compares the three insertion orientation modes:
`orientation template`, `orientation random`, and
`orientation constant q1 q2 q3 q4`.

`in.template_distributions`
Demonstrates the template-side geometry options:
`shape uniform`, per-axis `shapex/shapey/shapez`, `size uniform`,
and mixed blockiness values.

`in.set_superquadric`
Shows how to create superquadric atoms directly and configure their
shape, blockiness, density, and quaternion orientation with the
`set` command.

`in.mesh_wall`
Demonstrates superquadric interaction with a triangulated mesh wall
using `fix mesh/surface` and `fix wall/gran ... surface superquadric mesh`.
It reuses the closed cylinder STL from the nearby
`mesh_inclusion_insert` tutorial.

Visualization
-------------

The VTK/VTP dumps written by these cases contain quaternion, semi-axis,
and blockiness data. Use `paraview_superquadric_filter.py` in this folder
to reconstruct the actual superquadric geometry in ParaView.
