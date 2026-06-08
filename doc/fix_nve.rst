.. index:: fix nve

fix nve command
===============

Syntax
""""""


.. parsed-literal::

   fix ID group-ID nve

* ID, group-ID are documented in :doc:`fix <fix>` command
* nve = style name of this fix command

Examples
""""""""


.. parsed-literal::

   fix 1 all nve

Description
"""""""""""

Perform constant NVE integration to update position and velocity for
atoms in the group each timestep.  V is volume; E is energy.  This
creates a system trajectory consistent with the microcanonical
ensemble.


----------


**Restart, fix\_modify, output, run start/stop, minimize info:**

No information about this fix is written to :doc:`binary restart files <restart>`.  None of the :doc:`fix\_modify <fix_modify>` options
are relevant to this fix.  No global or per-atom quantities are stored
by this fix for access by various :ref:`output commands <howto_8>`.  No parameter of this fix can
be used with the *start/stop* keywords of the :doc:`run <run>` command.
This fix is not invoked during :doc:`energy minimization <minimize>`.

Restrictions
""""""""""""
 none

**Related commands:** none

**Default:** none


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
