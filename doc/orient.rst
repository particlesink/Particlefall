.. index:: orient

orient command
==============

Syntax
""""""


.. parsed-literal::

   orient dim i j k

* dim = *x* or *y* or *z*
* i,j,k = orientation of lattice that is along box direction dim

Examples
""""""""


.. parsed-literal::

   orient x 1 1 0
   orient y -1 1 0
   orient z 0 0 1

Description
"""""""""""

Specify the orientation of a cubic lattice along simulation box
directions *x* or *y* or *z*\ .  These 3 basis vectors are used when the
:doc:`create\_atoms <create_atoms>` command generates a lattice of atoms.

The 3 basis vectors B1, B2, B3 must be mutually orthogonal and form a
right-handed system such that B1 cross B2 is in the direction of B3.

The basis vectors should be specified in an irreducible form (smallest
possible integers), though LIGGGHTS(R)-PUBLIC does not check for this.

Restrictions
""""""""""""
 none

Related commands
""""""""""""""""

:doc:`origin <origin>`, :doc:`create\_atoms <create_atoms>`

Default
"""""""


.. parsed-literal::

   orient x 1 0 0
   orient y 0 1 0
   orient z 0 0 1


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
