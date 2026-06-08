.. index:: fix enforce2d

fix enforce2d command
=====================

Syntax
""""""


.. parsed-literal::

   fix ID group-ID enforce2d

* ID, group-ID are documented in :doc:`fix <fix>` command
* enforce2d = style name of this fix command

Examples
""""""""


.. parsed-literal::

   fix 5 all enforce2d

Description
"""""""""""

Zero out the z-dimension velocity and force on each atom in the group.
This is useful when running a 2d simulation to insure that atoms do
not move from their initial z coordinate.


----------


**Restart, fix\_modify, output, run start/stop, minimize info:**

No information about this fix is written to :doc:`binary restart files <restart>`.  None of the :doc:`fix\_modify <fix_modify>` options
are relevant to this fix.  No global or per-atom quantities are stored
by this fix for access by various :ref:`output commands <howto_8>`.  No parameter of this fix can
be used with the *start/stop* keywords of the :doc:`run <run>` command.

The forces due to this fix are imposed during an energy minimization,
invoked by the :doc:`minimize <minimize>` command.

Restrictions
""""""""""""
 none

**Related commands:** none

**Default:** none


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
