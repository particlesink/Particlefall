.. index:: fix nve/sphere

fix nve/sphere command
======================

Syntax
""""""


.. parsed-literal::

   fix ID group-ID nve/sphere

* ID, group-ID are documented in :doc:`fix <fix>` command
* nve/sphere = style name of this fix command
* zero or more keyword/value pairs may be appended
* keyword = *update*
  
  .. parsed-literal::
  
       *update* value = *dipole*
         dipole = update orientation of dipole moment during integration



Examples
""""""""


.. parsed-literal::

   fix 1 all nve/sphere
   fix 1 all nve/sphere update dipole

Description
"""""""""""

Perform constant NVE integration to update position, velocity, and
angular velocity for finite-size spherical particles in the group each
timestep.  V is volume; E is energy.  This creates a system trajectory
consistent with the microcanonical ensemble.

This fix differs from the :doc:`fix nve <fix_nve>` command, which
assumes point particles and only updates their position and velocity.

If the *update* keyword is used with the *dipole* value, then the
orientation of the dipole moment of each particle is also updated
during the time integration.  This option should be used for models
where a dipole moment is assigned to particles via use of the
:doc:`atom\_style dipole <atom_style>` command.


----------


**Restart, fix\_modify, output, run start/stop, minimize info:**

No information about this fix is written to :doc:`binary restart files <restart>`.  None of the :doc:`fix\_modify <fix_modify>` options
are relevant to this fix.  No global or per-atom quantities are stored
by this fix for access by various :ref:`output commands <howto_8>`.  No parameter of this fix can
be used with the *start/stop* keywords of the :doc:`run <run>` command.
This fix is not invoked during :doc:`energy minimization <minimize>`.

Restrictions
""""""""""""


This fix requires that atoms store torque and angular velocity (omega)
and a radius as defined by the :doc:`atom\_style sphere <atom_style>`
command.  If the *dipole* keyword is used, then they must also store a
dipole moment as defined by the :doc:`atom\_style dipole <atom_style>`
command.

All particles in the group must be finite-size spheres.  They cannot
be point particles.

Related commands
""""""""""""""""

:doc:`fix nve <fix_nve>`, :doc:`fix nve/asphere <fix_nve_asphere>`

**Default:** none


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
