.. index:: compute com

compute com command
===================

Syntax
""""""


.. parsed-literal::

   compute ID group-ID com general_keyword general_values

* ID, group-ID are documented in :doc:`compute <compute>` command
* com = style name of this compute command
* general\_keywords general\_values are documented in `compute <compute.html">`_

Examples
""""""""


.. parsed-literal::

   compute 1 all com

Description
"""""""""""

Define a computation that calculates the center-of-mass of the group
of atoms, including all effects due to atoms passing thru periodic
boundaries.

A vector of three quantites is calculated by this compute, which
are the x,y,z coordinates of the center of mass.

.. warning::

   The coordinates of an atom contribute to the
   center-of-mass in "unwrapped" form, by using the image flags
   associated with each atom.  See the :doc:`dump custom <dump>` command
   for a discussion of "unwrapped" coordinates.  See the Atoms section of
   the :doc:`read\_data <read_data>` command for a discussion of image flags
   and how they are set for each atom.  You can reset the image flags
   (e.g. to 0) before invoking this compute by using the :doc:`set image <set>` command.

.. warning::

   If an atom is part of a rigid body (see the :doc:`fix rigid <fix_rigid>` command), it's periodic image flags are altered,
   and its contribution to the center-of-mass may not reflect its true
   contribution.  See the :doc:`fix rigid <fix_rigid>` command for details.
   Thus, to compute the center-of-mass of rigid bodies as they cross
   periodic boundaries, you will need to post-process a :doc:`dump file <dump>` containing coordinates of the atoms in the bodies.

**Output info:**

This compute calculates a global vector of length 3, which can be
accessed by indices 1-3 by any command that uses global vector values
from a compute as input.  See :ref:`this section <howto_8>` for an overview of Packfall output
options.

The vector values are "intensive".  The vector values will be in
distance :doc:`units <units>`.

Restrictions
""""""""""""
 none

Related commands
""""""""""""""""

:doc:`compute com/molecule <compute_com_molecule>`

**Default:** none


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
