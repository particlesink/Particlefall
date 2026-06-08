.. index:: compute inertia/molecule

compute inertia/molecule command
================================

Syntax
""""""


.. parsed-literal::

   compute ID group-ID inertia/molecule general_keyword general_values

* ID, group-ID are documented in :doc:`compute <compute>` command
* inertia/molecule = style name of this compute command
* general\_keywords general\_values are documented in `compute <compute.html">`_

Examples
""""""""


.. parsed-literal::

   compute 1 fluid inertia/molecule

Description
"""""""""""

Define a computation that calculates the inertia tensor of individual
molecules.  The calculation includes all effects due to atoms passing
thru periodic boundaries.

The symmetric intertia tensor has 6 components, ordered
Ixx,Iyy,Izz,Ixy,Iyz,Ixz.  The tensor for a particular molecule is only
computed if one or more of its atoms is in the specified group.
Normally all atoms in the molecule should be in the group, however
this is not required.  LIGGGHTS(R)-PUBLIC will warn you if this is not the case.
Only atoms in the group contribute to the inertia tensor and
associated center-of-mass calculation for the molecule.

The ordering of per-molecule quantities produced by this compute is
consistent with the ordering produced by other compute commands that
generate per-molecule datums.  Conceptually, the molecule IDs will be
in ascending order for any molecule with one or more of its atoms in
the specified group.

.. warning::

   The coordinates of an atom contribute to the
   molecule's inertia tensor in "unwrapped" form, by using the image
   flags associated with each atom.  See the :doc:`dump custom <dump>`
   command for a discussion of "unwrapped" coordinates.  See the Atoms
   section of the :doc:`read\_data <read_data>` command for a discussion of
   image flags and how they are set for each atom.  You can reset the
   image flags (e.g. to 0) before invoking this compute by using the :doc:`set image <set>` command.

.. warning::

   If an atom is part of a rigid body (see the :doc:`fix rigid <fix_rigid>` command), it's periodic image flags are altered,
   and its contribution to the inertia tensor may not reflect its true
   contribution.  See the :doc:`fix rigid <fix_rigid>` command for details.
   Thus, to compute the inertia tensor of rigid bodies as they cross
   periodic boundaries, you will need to post-process a :doc:`dump file <dump>` containing coordinates of the atoms in the bodies.

**Output info:**

This compute calculates a global array where the number of rows =
Nmolecules and the number of columns = 6 for the 6 components of the
inertia tensor of each molecule, ordered as listed above.  These
values can be accessed by any command that uses global array values
from a compute as input.  See :ref:`Section\_howto 15 <howto_8>` for an overview of LIGGGHTS(R)-PUBLIC output
options.

The array values are "intensive".  The array values will be in
distance :doc:`units <units>`.

Restrictions
""""""""""""
 none

Related commands
""""""""""""""""

:doc:`variable inertia() function <variable>`

**Default:** none


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
