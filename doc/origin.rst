.. index:: origin

origin command
==============

Syntax
""""""


.. parsed-literal::

   origin x y z

* x,y,z = origin of a lattice

Examples
""""""""


.. parsed-literal::

   origin 0.0 0.5 0.5

Description
"""""""""""

Set the origin of the lattice defined by the :doc:`lattice <lattice>`
command.  The lattice is used by the :doc:`create\_atoms <create_atoms>`
command to create new atoms and by other commands that use a lattice
spacing as a distance measure.  This command offsets the origin of the
lattice from the (0,0,0) coordinate of the simulation box by some
fraction of a lattice spacing in each dimension.

The specified values are in lattice coordinates from 0.0 to 1.0, so
that a value of 0.5 means the lattice is displaced 1/2 a cubic cell.

Restrictions
""""""""""""
 none

Related commands:**

:doc:`lattice <lattice>`, :doc:`orient <orient>`

Default
"""""""


.. parsed-literal::

   origin 0 0 0


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
