.. index:: compute gyration

compute gyration command
========================

Syntax
""""""


.. parsed-literal::

   compute ID group-ID gyration general_keyword general_values

* ID, group-ID are documented in :doc:`compute <compute>` command
* gyration = style name of this compute command
* general\_keywords general\_values are documented in `compute <compute.html">`_

Examples
""""""""


.. parsed-literal::

   compute 1 molecule gyration

Description
"""""""""""

Define a computation that calculates the radius of gyration Rg of the
group of atoms, including all effects due to atoms passing thru
periodic boundaries.

Rg is a measure of the size of the group of atoms, and is computed by
this formula

.. image:: Eqs/compute_gyration.jpg
   :align: center

where M is the total mass of the group, Rcm is the center-of-mass
position of the group, and the sum is over all atoms in the group.

A Rg tensor, stored as a 6-element vector, is also calculated by this
compute.  The formula for the components of the tensor is the same as
the above formula, except that (Ri - Rcm)\^2 is replaced by (Rix -
Rcmx) \* (Riy - Rcmy) for the xy component, etc.  The 6 components of
the vector are ordered xx, yy, zz, xy, xz, yz.

.. warning::

   The coordinates of an atom contribute to Rg in
   "unwrapped" form, by using the image flags associated with each atom.
   See the :doc:`dump custom <dump>` command for a discussion of
   "unwrapped" coordinates.  See the Atoms section of the
   :doc:`read\_data <read_data>` command for a discussion of image flags and
   how they are set for each atom.  You can reset the image flags
   (e.g. to 0) before invoking this compute by using the :doc:`set image <set>` command.

**Output info:**

This compute calculates a global scalar (Rg) and a global vector of
length 6 (Rg tensor), which can be accessed by indices 1-6.  These
values can be used by any command that uses a global scalar value or
vector values from a compute as input.  See :ref:`Section\_howto 15 <howto_8>` for an overview of Packfall output
options.

The scalar and vector values calculated by this compute are
"intensive".  The scalar and vector values will be in distance
:doc:`units <units>`.

Restrictions
""""""""""""
 none

Related commands
""""""""""""""""

:doc:`compute gyration/molecule <compute_gyration_molecule>`

**Default:** none


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
