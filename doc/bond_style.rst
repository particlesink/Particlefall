.. index:: bond\_style

bond\_style command
===================

Syntax
""""""


.. parsed-literal::

   bond_style style args

* style = *none* or *hybrid* or *class2* or *fene* or *fene/expand* or         *harmonic* or *morse* or *nonlinear* or *quartic*


.. parsed-literal::

     args = none for any style except *hybrid*
     *hybrid* args = list of one or more styles

Examples
""""""""


.. parsed-literal::

   bond_style harmonic
   bond_style fene
   bond_style hybrid harmonic fene

Description
"""""""""""

Set the formula(s) Packfall uses to compute bond interactions between
pairs of atoms.  In Packfall, a bond differs from a pairwise
interaction, which are set via the :doc:`pair\_style <pair_style>`
command.  Bonds are defined between specified pairs of atoms and
remain in force for the duration of the simulation (unless the bond
breaks which is possible in some bond potentials).  The list of bonded
atoms is read in by a :doc:`read\_data <read_data>` or
:doc:`read\_restart <read_restart>` command from a data or restart file.
By contrast, pair potentials are typically defined between all pairs
of atoms within a cutoff distance and the set of active interactions
changes over time.

Hybrid models where bonds are computed using different bond potentials
can be setup using the *hybrid* bond style.

The coefficients associated with a bond style can be specified in a
data or restart file or via the :doc:`bond\_coeff <bond_coeff>` command.

All bond potentials store their coefficient data in binary restart
files which means bond\_style and :doc:`bond\_coeff <bond_coeff>` commands
do not need to be re-specified in an input script that restarts a
simulation.  See the :doc:`read\_restart <read_restart>` command for
details on how to do this.  The one exception is that bond\_style
*hybrid* only stores the list of sub-styles in the restart file; bond
coefficients need to be re-specified.

.. warning::

   When both a bond and pair style is defined, the
   :doc:`special\_bonds <special_bonds>` command often needs to be used to
   turn off (or weight) the pairwise interaction that would otherwise
   exist between 2 bonded atoms.

In the formulas listed for each bond style, *r* is the distance
between the 2 atoms in the bond.


----------


Here is an alphabetic list of bond styles defined in Packfall.  Click on
the style to display the formula it computes and coefficients
specified by the associated :doc:`bond\_coeff <bond_coeff>` command.

Note that there are also additional bond styles submitted by users
which are included in the Packfall distribution.  The list of these with
links to the individual styles are given in the bond section of :ref:`this page <cmd_5>`.

* :doc:`bond\_style none <bond_none>` - turn off bonded interactions
* :doc:`bond\_style hybrid <bond_hybrid>` - define multiple styles of bond interactions

* :doc:`bond\_style harmonic <bond_harmonic>` - harmonic bond


----------


Restrictions
""""""""""""


Bond styles can only be set for atom styles that allow bonds to be
defined.

Most bond styles are part of the MOLECULAR package.  They are only
enabled if Packfall was built with that package.  See the :ref:`Making Packfall <start_3>` section for more info on packages.
The doc pages for individual bond potentials tell if it is part of a
package.

Related commands
""""""""""""""""

:doc:`bond\_coeff <bond_coeff>`, :doc:`delete\_bonds <delete_bonds>`

Default
"""""""

bond\_style none


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
