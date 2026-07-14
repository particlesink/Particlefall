.. index:: compute msd/molecule

compute msd/molecule command
============================

Syntax
""""""


.. parsed-literal::

   compute ID group-ID msd/molecule general_keyword general_values

* ID, group-ID are documented in :doc:`compute <compute>` command
* msd/molecule = style name of this compute command
* general\_keywords general\_values are documented in `compute <compute.html">`_

Examples
""""""""


.. parsed-literal::

   compute 1 all msd/molecule

Description
"""""""""""

Define a computation that calculates the mean-squared displacement
(MSD) of individual molecules.  The calculation includes all effects
due to atoms passing thru periodic boundaries.

Four quantites are calculated by this compute for each molecule.  The
first 3 quantities are the squared dx,dy,dz displacements of the
center-of-mass.  The 4th component is the total squared displacement,
i.e. (dx\*dx + dy\*dy + dz\*dz) of the center-of-mass.

The slope of the mean-squared displacement (MSD) versus time is
proportional to the diffusion coefficient of the diffusing molecules.

The displacement of the center-of-mass of the molecule is from its
original center-of-mass position at the time the compute command was
issued.

The MSD for a particular molecule is only computed if one or more of
its atoms are in the specified group.  Normally all atoms in the
molecule should be in the group, however this is not required.  Packfall
will warn you if this is not the case.  Only atoms in the group
contribute to the center-of-mass calculation for the molecule, which
is used to caculate its initial and current position.

The ordering of per-molecule quantities produced by this compute is
consistent with the ordering produced by other compute commands that
generate per-molecule datums.  Conceptually, them molecule IDs will be
in ascending order for any molecule with one or more of its atoms in
the specified group.

.. warning::

   The initial coordinates of each molecule are stored in
   "unwrapped" form, by using the image flags associated with each atom.
   See the :doc:`dump custom <dump>` command for a discussion of
   "unwrapped" coordinates.  See the Atoms section of the
   :doc:`read\_data <read_data>` command for a discussion of image flags and
   how they are set for each atom.  You can reset the image flags
   (e.g. to 0) before invoking this compute by using the :doc:`set image <set>` command.

.. warning::

   If an atom is part of a rigid body (see the :doc:`fix rigid <fix_rigid>` command), it's periodic image flags are altered,
   and its contribution to the MSD may not reflect its true contribution.
   See the :doc:`fix rigid <fix_rigid>` command for details.  Thus, to
   compute the MSD of rigid bodies as they cross periodic boundaries, you
   will need to post-process a :doc:`dump file <dump>` containing
   coordinates of the atoms in the bodies.

.. warning::

   Unlike the :doc:`compute msd <compute_msd>` command,
   this compute does not store the initial center-of-mass coorindates of
   its molecules in a restart file.  Thus you cannot continue the MSD per
   molecule calculation of this compute when running from a :doc:`restart file <read_restart>`.

**Output info:**

This compute calculates a global array where the number of rows =
Nmolecules and the number of columns = 4 for dx,dy,dz and the total
displacement.  These values can be accessed by any command that uses
global array values from a compute as input.  See :ref:`this section <howto_8>` for an overview of Packfall output
options.

The array values are "intensive".  The array values will be in
distance\^2 :doc:`units <units>`.

Restrictions
""""""""""""
 none

Related commands
""""""""""""""""

:doc:`compute msd <compute_msd>`

**Default:** none


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
