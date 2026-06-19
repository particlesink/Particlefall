.. index:: fix couple/sphdem

fix couple/sphdem command
=========================

Syntax
""""""

.. parsed-literal::

   fix ID group-ID couple/sphdem fluid-group solid-group kernel-style h drag-coefficient

* `fluid-group` = group containing SPH carrier particles
* `solid-group` = group containing DEM particles
* `kernel-style` = SPH kernel name, e.g. `cubicspline`
* `h` = smoothing length used for interpolation
* `drag-coefficient` = linear drag prefactor

Description
"""""""""""

This fix is the first Packfall-side scaffold for SPH-DEM coupling.
It performs a kernel-weighted interpolation of SPH particle velocity onto each
DEM particle in `solid-group`, applies a linear drag force proportional to the
velocity difference, estimates a local fluid fraction from the kernel support,
adds a pressure-gradient contribution when SPH pressure data is available,
adds an Archimedes-style buoyancy term when a matching `fix gravity` acts on
the DEM group, and distributes the equal-and-opposite reaction force back to
neighboring SPH particles in `fluid-group`.

The current implementation now uses a small embedded sampler for spherical DEM
particles: one center sample and six offset samples at half-radius along the
Cartesian axes. Force contributions are reconstructed per sample and summed to
the rigid particle. This improves partial-immersion behavior compared with the
earlier center-only bridge and also generates a hydrodynamic torque.
Pressure-gradient and buoyancy forces are scaled by a per-sample support-weight
fill fraction, so they ramp in continuously as SPH support grows near a free
surface instead of switching on in an all-or-nothing way.

This gives a minimal conservative force bridge without modifying the existing
SPH pair styles or DEM pair styles.

Current limitations:

* only single-processor runs are supported
* the porosity estimate is a local kernel-support proxy rather than a resolved
  cell-based void-fraction model
* the pressure-gradient term is sampled from the SPH particle pressure field
  using normalized kernel interpolation
* the buoyancy term uses the kernel-interpolated local SPH density together
  with the DEM particle volume and gravity vector
* no added-mass, lift, or lubrication terms are included
* the fix assumes the atom style provides both SPH density and DEM mass/radius
  attributes, e.g. `atom_style hybrid sphere sph`

The fix provides a global 15-vector:

* component 1 = number of solid particles that found SPH support
* component 2 = total x drag applied to DEM particles
* component 3 = total y drag applied to DEM particles
* component 4 = total z drag applied to DEM particles
* component 5 = total x pressure-gradient force applied to DEM particles
* component 6 = total y pressure-gradient force applied to DEM particles
* component 7 = total z pressure-gradient force applied to DEM particles
* component 8 = summed local porosity estimate over coupled DEM particles
* component 9 = total x buoyancy force applied to DEM particles
* component 10 = total y buoyancy force applied to DEM particles
* component 11 = total z buoyancy force applied to DEM particles
* component 12 = total x bridge torque applied to DEM particles
* component 13 = total y bridge torque applied to DEM particles
* component 14 = total z bridge torque applied to DEM particles
* component 15 = summed support-weighted immersed-fraction estimate over coupled
  DEM particles

The fix also provides a per-atom 18-column array that can be dumped with
`dump custom` or `dump custom/vtk` as `f_ID[N]`:

* column 1 = drag force x
* column 2 = drag force y
* column 3 = drag force z
* column 4 = pressure-gradient force x
* column 5 = pressure-gradient force y
* column 6 = pressure-gradient force z
* column 7 = buoyancy force x
* column 8 = buoyancy force y
* column 9 = buoyancy force z
* column 10 = total bridge force x
* column 11 = total bridge force y
* column 12 = total bridge force z
* column 13 = local porosity estimate
* column 14 = coupling flag, 1.0 when SPH support was found and 0.0 otherwise
* column 15 = total bridge torque x
* column 16 = total bridge torque y
* column 17 = total bridge torque z
* column 18 = support-weighted immersed-fraction estimate

Example
"""""""

.. parsed-literal::

   atom_style hybrid sphere sph
   fix bridge all couple/sphdem fluid solid cubicspline 0.02 0.01
   dump d1 all custom/vtk 100 bridge_*.vtk id type x y z f_bridge[9] f_bridge[12] f_bridge[13] f_bridge[18]

Related commands
""""""""""""""""

:doc:`atom_style <atom_style>`, :doc:`fix nve/sphere <fix_nve_sphere>`,
:doc:`fix nve/sph <fix_nve_sph>`
