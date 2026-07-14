.. index:: pair\_style

pair\_style command
===================

Syntax
""""""


.. parsed-literal::

   pair_style style args

* style = one of the styles from the list :ref:`here <cmd_5>`
* args = arguments used by a particular style

Examples
""""""""


.. parsed-literal::

   pair_style gran model hertz tangential history
   pair_style none

Description
"""""""""""

Set the formula(s) Packfall uses to compute pairwise interactions.  In
Packfall, pair potentials are defined between pairs of atoms that are
within a cutoff distance and the set of active interactions typically
changes over time.  See the :doc:`bond\_style <bond_style>` command to
define potentials between pairs of bonded atoms, which typically
remain in place for the duration of a simulation.

The full list of pair styles defined in Packfall is on :ref:`this page <cmd_5>`.

Hybrid models where specified pairs of atom types interact via
different pair potentials can be setup using the *hybrid* pair style.

The coefficients associated with a pair style are typically set for
each pair of atom types, and are specified by the
:doc:`pair\_coeff <pair_coeff>` command or read from a file by the
:doc:`read\_data <read_data>` or :doc:`read\_restart <read_restart>`
commands.

The :doc:`pair\_modify <pair_modify>` command sets options for mixing of
type I-J interaction coefficients.

If the pair\_style command has a cutoff argument, it sets global
cutoffs for all pairs of atom types.  The distance(s) can be smaller
or larger than the dimensions of the simulation box.

Typically, the global cutoff value can be overridden for a specific
pair of atom types by the :doc:`pair\_coeff <pair_coeff>` command.  The
pair style settings (including global cutoffs) can be changed by a
subsequent pair\_style command using the same style.  This will reset
the cutoffs for all atom type pairs, including those previously set
explicitly by a :doc:`pair\_coeff <pair_coeff>` command.  The exceptions
to this are that pair\_style *table* and *hybrid* settings cannot be
reset.  A new pair\_style command for these styles will wipe out all
previously specified pair\_coeff values.


----------


The full list of pair styles defined in Packfall is on :ref:`this page <cmd_5>`.

Restrictions
""""""""""""


This command must be used before any coefficients are set by the
:doc:`pair\_coeff <pair_coeff>`, :doc:`read\_data <read_data>`, or
:doc:`read\_restart <read_restart>` commands.

Some pair styles are part of specific packages.  They are only enabled
if Packfall was built with that package.  See the :ref:`Making Packfall <start_3>` section for more info on packages.
The doc pages for individual pair potentials tell if it is part of a
package.

Related commands
""""""""""""""""

:doc:`pair\_coeff <pair_coeff>`, :doc:`read\_data <read_data>`,
:doc:`pair\_modify <pair_modify>`, :doc:`dielectric <dielectric>`,

Default
"""""""


.. parsed-literal::

   pair_style none


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
