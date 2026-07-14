.. index:: compute coord/gran

compute coord/gran command
==========================

Syntax
""""""


.. parsed-literal::

   compute ID group-ID coord/gran general_keyword general_values

* ID, group-ID are documented in :doc:`compute <compute>` command
* coord/atom = style name of this compute command
* general\_keywords general\_values are documented in `compute <compute.html">`_

Examples
""""""""


.. parsed-literal::

   compute 1 all coord/gran

Description
"""""""""""

Define a computation that calculates the coordination number for each
atom in a group. The value of the coordination number will be 0.0
for atoms not in the specified compute group.

The coordination number is defined as the number of neighbor atoms
within the granular cutoff distance from the central atom. The cutoff
distance for granular systems is the sum of the radii of the two particles.

The neighbor list needed to compute this quantity is constructed each
time the calculation is performed (i.e. each time a snapshot of atoms
is dumped). Thus it can be inefficient to compute/dump this quantity
too frequently or to have multiple compute/dump commands, each of a
coord/gran style.

**Output info:**

This compute calculates a per-atom vector, which can be accessed by
any command that uses per-atom values from a compute as input.  See
:ref:`this section <4_15>` for an overview of Packfall
output options.

The per-atom vector values will be a number >= 0.0, as explained
above.

Restrictions
""""""""""""


This command cannot be applied to multi-sphere simulations, as the
output will not be the per-body coordination number.

**Related commands:** none

**Default:** none


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
