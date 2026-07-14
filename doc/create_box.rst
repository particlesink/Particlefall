.. index:: create\_box

create\_box command
===================

Syntax
""""""


.. parsed-literal::

   create_box N region-ID

* N = # of atom types to use in this simulation
* region-ID = ID of region to use as simulation domain (optional)

Examples
""""""""


.. parsed-literal::

   create_box 2 mybox
   create_box 1

Description
"""""""""""

This command creates a simulation box based on a specified region.
Either a :doc:`region <region>` command must first be used to define a
geometric domain and its *region-ID* must be specified as argument,
or a :doc:`simulation\_domain <simulation_domain>` command must be used
first. In the latter case the box defined by the
:doc:`simulation\_domain <simulation_domain>` is automatically used if
no *region-ID* is specified.

The argument N is the number of atom types that will be used in the
simulation.

If the region is not of style *prism*\ , then Packfall encloses the region
(block, sphere, etc) with an axis-aligned orthogonal bounding box
which becomes the simulation domain.

If the region is of style *prism*\ , Packfall creates a non-orthogonal
simulation domain shaped as a parallelepiped with triclinic symmetry.
As defined by the :doc:`region prism <region>` command, the
parallelepiped has its "origin" at (xlo,ylo,zlo) and is defined by 3
edge vectors starting from the origin given by A = (xhi-xlo,0,0); B =
(xy,yhi-ylo,0); C = (xz,yz,zhi-zlo).  *Xy,xz,yz* can be 0.0 or
positive or negative values and are called "tilt factors" because they
are the amount of displacement applied to faces of an originally
orthogonal box to transform it into the parallelipiped.

A *prism* region used with the create\_box command must have tilt
factors (xy,xz,yz) that do not skew the box more than half the
distance of the parallel box length.  For example, if xlo = 2 and xhi
= 12, then the x box length is 10 and the xy tilt factor must be
between -5 and 5.  Similarly, both xz and yz must be between
-(xhi-xlo)/2 and +(yhi-ylo)/2.  Note that this is not a limitation,
since if the maximum tilt factor is 5 (as in this example), then
configurations with tilt = ..., -15, -5, 5, 15, 25, ... are all
geometrically equivalent.

See :ref:`Section\_howto 12 <howto_7>` of the doc pages
for a geometric description of triclinic boxes, as defined by Packfall,
and how to transform these parameters to and from other commonly used
triclinic representations.

When a prism region is used, the simulation domain must be periodic in
any dimensions with a non-zero tilt factor, as defined by the
:doc:`boundary <boundary>` command.  I.e. if the xy tilt factor is
non-zero, then both the x and y dimensions must be periodic.
Similarly, x and z must be periodic if xz is non-zero and y and z must
be periodic if yz is non-zero.  Also note that if your simulation will
tilt the box, e.g. via the :doc:`fix deform <fix_deform>` command, the
simulation box must be defined as triclinic, even if the tilt factors
are initially 0.0.

.. warning::

   If the system is non-periodic (in a dimension), then
   you should not make the lo/hi box dimensions (as defined in your
   :doc:`region <region>` command) radically smaller/larger than the extent
   of the atoms you eventually plan to create, e.g. via the
   :doc:`create\_atoms <create_atoms>` command.  For example, if your atoms
   extend from 0 to 50, you should not specify the box bounds as -10000
   and 10000. This is because Packfall uses the specified box size to
   layout the 3d grid of processors.  A huge (mostly empty) box will be
   sub-optimal for performance when using "fixed" boundary conditions
   (see the :doc:`boundary <boundary>` command).  When using "shrink-wrap"
   boundary conditions (see the :doc:`boundary <boundary>` command), a huge
   (mostly empty) box may cause a parallel simulation to lose atoms the
   first time that Packfall shrink-wraps the box around the atoms.

Restrictions
""""""""""""


An :doc:`atom\_style <atom_style>` and one of :doc:`region <region>` or
:doc:`simulation\_domain <simulation_domain>` must have been previously
defined to use this command.

Related commands
""""""""""""""""

:doc:`create\_atoms <create_atoms>`, :doc:`region <region>`, :doc:`simulation\_domain <simulation_domain>`

**Default:** none


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
