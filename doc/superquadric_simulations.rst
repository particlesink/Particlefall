How-To for "SUPERQUADRIC" simulations in Packfall
====================================================



Description:
------------

This text describes how to perform "SUPERQUADRIC" simulations in Packfall.

Introduction:
-------------

"SUPERQUADRIC" simulations allow Packfall to use superquadric bodies as particles.
Superquadrics are smooth convex aspherical bodies described by three
semi-axes and two blockiness parameters. The ellipsoidal case is recovered
for blockiness values equal to 2.

Compilation:
------------

.. _boost: http://www.boost.org/



To compile Packfall with this feature the `boost <boost_>`_ library must be available on the system. On most LINUX-distributions a recent version can be installed via the package managment systems.

Before starting the compilation, enable the SUPERQUADRIC model in the
active Packfall makefile by setting ``ENABLE_SUPERQUADRICS=1``. You can
do this on the command line, for example:


.. parsed-literal::

       make linux ENABLE_SUPERQUADRICS=1

If Boost is installed in a non-standard location, add its include path:

.. parsed-literal::

       make linux ENABLE_SUPERQUADRICS=1 EXTRA_CPPFLAGS=-IPath/to/your/Boost/include

.. note::

   Make sure that you add the definitions after the original assignments of LMP\_INC and EXTRA\_INC. Otherwise your changes will be overwritten.

.. note::

   "Most Boost libraries are header-only: they consist entirely of header files containing templates and inline functions, and require no separately-compiled library binaries or special treatment when linking." (`link <boost_>`_)

Detailed description:
---------------------

The overall simulation structure stays the same for SUPERQUADRIC simulations as used for standard Packfall simulations. In order to create a SUPERQUADRIC simulation based on an existing case you have to perform following steps:

* Use :doc:`atom\_style superquadric <atom_style>`
* For contact models (pair\_style gran as well as fix wall/gran) use :doc:`surface superquadric <gran_surface_superquadric>`
* The SUPERQUADRIC model requires a special integrator, namely :doc:`fix nve/superquadric <fix_nve_superquadric>`
* To define new SUPERQUADRIC particles use :doc:`fix particletemplate/superquadric <fix_particletemplate_superquadric>`
* To insert superquadrics from templates or template distributions use the standard insertion fixes such as :doc:`fix insert/pack <fix_insert_pack>`, :doc:`fix insert/rate/region <fix_insert_rate_region>`, or :doc:`fix insert/stream <fix_insert_stream>`

Important practical notes:

* Superquadric shape is controlled by the semi-axes *a*\ , *b*\ , *c* and the
  blockiness parameters *n1* and *n2*.
* The insertion fixes control the initial particle orientation. Use
  ``orientation random`` if particles should not all start with the same pose.
* Primitive and mesh walls both require the superquadric surface model when
  used with superquadric particles.
* For post-processing, :doc:`dump custom/vtk <dump_custom_vtk>` can write the
  semi-axes, blockiness values, and quaternion components needed to
  reconstruct particle geometry in a visualization tool.

Typical workflow:

* define :doc:`atom_style superquadric <atom_style>`
* define contact and wall models with :doc:`surface superquadric <gran_surface_superquadric>`
* define one or more :doc:`fix particletemplate/superquadric <fix_particletemplate_superquadric>` templates
* optionally combine them with :doc:`fix particledistribution/discrete <fix_particledistribution_discrete>`
* insert particles with an insertion fix, choosing ``orientation random`` or another orientation mode as needed
* integrate motion with :doc:`fix nve/superquadric <fix_nve_superquadric>`
* dump quaternion and shape data for post-processing if visualization of the real body geometry is desired

See also the example in
``examples/superquadric/in.particle_particle``.

Performance
-----------

The SUPERQUADRIC model uses the same parallel simulation framework as
Packfall itself. Compared to standard Packfall, the calculation of
contact points is computationally expensive for superquadric bodies,
which leads to a lower maximum number of particles per processor.


----------


Questions?
----------

If any questions remain, contact us.


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
