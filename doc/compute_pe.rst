.. index:: compute pe

compute pe command
==================

Syntax
""""""


.. parsed-literal::

   compute ID group-ID pe general_keyword general_values keyword ...

* ID, group-ID are documented in :doc:`compute <compute>` command
* pe = style name of this compute command
* general\_keywords general\_values are documented in `compute <compute.html">`_
* zero or more keywords may be appended
* keyword = *pair* or *bond* or *angle* or *dihedral* or *improper* or *kspace*

Examples
""""""""


.. parsed-literal::

   compute 1 all pe
   compute molPE all pe bond angle dihedral improper

Description
"""""""""""

Define a computation that calculates the potential energy of the
entire system of atoms.  The specified group must be "all".  See the
:doc:`compute pe/atom <compute_pe_atom>` command if you want per-atom
energies.  These per-atom values could be summed for a group of atoms
via the :doc:`compute reduce <compute_reduce>` command.

The energy is calculated by the various pair, bond, etc potentials
defined for the simulation.  If no extra keywords are listed, then the
potential energy is the sum of pair, bond, angle, dihedral, improper,
and kspace (long-range) energy.  If any extra keywords are listed,
then only those components are summed to compute the potential energy.

Various fixes can contribute to the total potential energy of the
system.  See the doc pages for :doc:`individual fixes <fix>` for
details.  The *thermo* option of the
:doc:`compute\_modify <compute_modify>` command determines whether these
contributions are added into the computed potential energy.  If no
keywords are specified the default is *yes*\ .  If any keywords are
specified, the default is *no*\ .

**Output info:**

This compute calculates a global scalar (the potential energy).  This
value can be used by any command that uses a global scalar value from
a compute as input.  See :ref:`Section\_howto 15 <howto_8>` for an overview of LIGGGHTS(R)-PUBLIC output
options.

The scalar value calculated by this compute is "extensive".  The
scalar value will be in energy :doc:`units <units>`.

Restrictions
""""""""""""


This compute does not include the potential
energy due to the overlap of granular particles.

Related commands
""""""""""""""""

:doc:`compute pe/atom <compute_pe_atom>`

**Default:** none


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
