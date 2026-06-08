.. index:: fix freeze

fix freeze command
==================

Syntax
""""""


.. parsed-literal::

   fix ID group-ID freeze

* ID, group-ID are documented in :doc:`fix <fix>` command
* freeze = style name of this fix command

Examples
""""""""


.. parsed-literal::

   fix 2 bottom freeze

Description
"""""""""""

Zero out the force and torque on a granular particle.  This is useful
for preventing certain particles from moving in a simulation.  The
:doc:`granular pair styles <pair_gran>` also detect if this fix has been
defined and compute interactions between frozen and non-frozen
particles appropriately, as if the frozen particle has infinite mass.


----------


**Restart, fix\_modify, output, run start/stop, minimize info:**

No information about this fix is written to :doc:`binary restart files <restart>`.  None of the :doc:`fix\_modify <fix_modify>` options
are relevant to this fix.

This fix computes a global 3-vector of forces, which can be accessed
by various :ref:`output commands <howto_8>`.  This is the
total force on the group of atoms before the forces on individual
atoms are changed by the fix.  The vector values calculated by this
fix are "extensive".

No parameter of this fix can be used with the *start/stop* keywords of
the :doc:`run <run>` command.  This fix is not invoked during :doc:`energy minimization <minimize>`.

Restrictions
""""""""""""


There can only be a single freeze fix defined.  This is because other
the :doc:`granular pair styles <pair_gran>` treat frozen particles
differently and need to be able to reference a single group to which
this fix is applied.

**Related commands:** none

:doc:`atom\_style sphere <atom_style>`

**Default:** none


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
