.. index:: compute group/group

compute group/group command
===========================

Syntax
""""""


.. parsed-literal::

   compute ID group-ID group/group general_keyword general_values group2-ID keyword value ...

* ID, group-ID are documented in :doc:`compute <compute>` command
* group/group = style name of this compute command
* general\_keywords general\_values are documented in `compute <compute.html">`_
* group2-ID = group ID of second (or same) group
* zero or more keyword/value pairs may be appended
* keyword = *pair* or *boundary*
  
  .. parsed-literal::
  
       *pair* value = *yes* or *no*
       *boundary* value = *yes* or *no*



Examples
""""""""


.. parsed-literal::

   compute 1 lower group/group upper
   compute mine fluid group/group wall

Description
"""""""""""

Define a computation that calculates the total energy and force
interaction between two groups of atoms: the compute group and the
specified group2.  The two groups can be the same.

If the *pair* keyword is set to *yes*\ , which is the default, then the
the interaction energy will include a pair component which is defined
as the pairwise energy between all pairs of atoms where one atom in
the pair is in the first group and the other is in the second group.
Likewise, the interaction force calculated by this compute will
include the force on the compute group atoms due to pairwise
interactions with atoms in the specified group2.

This compute does not calculate any bond interactions between atoms in
the two groups.


----------


The pairwise contributions to the group-group interactions are
calculated by looping over a neighbor list.

**Output info:**

This compute calculates a global scalar (the energy) and a global
vector of length 3 (force), which can be accessed by indices 1-3.
These values can be used by any command that uses global scalar or
vector values from a compute as input.  See :ref:`this section <howto_8>` for an overview of LIGGGHTS(R)-PUBLIC output
options.

Both the scalar and vector values calculated by this compute are
"extensive".  The scalar value will be in energy :doc:`units <units>`.
The vector values will be in force :doc:`units <units>`.

Restrictions
""""""""""""


**Related commands:** none

Default
"""""""

The option defaults are pair = yes, kspace = no, and boundary = yes.


----------


.. _Bogusz:



Bogusz et al, J Chem Phys, 108, 7070 (1998)


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
