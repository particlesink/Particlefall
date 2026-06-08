.. index:: fix multisphere/break

fix multisphere/break command
=============================

Syntax
""""""


.. parsed-literal::

   fix ID group-ID multisphere/break keyword value ...

* ID, group-ID are documented in :doc:`fix <fix>` command
* multisphere/break = style name of this fix command
* keywords = same as for :doc:`fix multisphere <fix_multisphere>`, in addition: *trigger\_threshold* and *trigger\_timeStep* and *trigger\_name* (or the deprecated:*trigger\_fixName*)
  
  .. parsed-literal::
  
       *trigger_threshold* value = *x in units matching that of trigger_fixName*
         this allows the user the specify a threshold value that will be compared with the per-atom value provided by trigger_fixName, e.g., the residence time in a region (e.g., computed via a fix property/atom/tracer/residencetime). if the threshold is exceeded, the multisphere particle will break.
       *trigger_timeStep* value = *x in steps*
         this allows the user to activate the fix after a predefined number of steps.
       *trigger_name*
         the user must specify a reference to a fix property/atom or variable of style atom that is used as the trigger value.

  
  .. parsed-literal::
  
       *trigger_fixName* (deprecated!)
         the user must specify the name of the fix that specified per-atom information that is used as the trigger value.



Examples
""""""""


.. parsed-literal::

   fix integr  grpMulti1  multisphere/break allow_group_and_set yes trigger_threshold 0.1 trigger_timeStep 0 trigger_name f_resTimeMill
   fix integr  grpMulti2  multisphere/break allow_group_and_set yes trigger_threshold 1 trigger_timeStep 0 trigger_name f_pressure\ **1**
   fix integr  grpMulti3  multisphere/break allow_group_and_set yes trigger_threshold 50 trigger_timeStep 10 trigger_name v_myAtomVariable

Description
"""""""""""

The integration that is performed by this command is
euqivalent to :doc:`fix\_multisphere <fix_multisphere>`. However, the user can trigger breakage events, which will destroy the "body" the atoms are in. The trigger is imiplemented such that in case the trigger value of one atom in a body exceeds the threshold value, the whole body will break. There are no fragments: all atoms that constitutea body will be released upon breakage.

.. note::

   the atoms in a body MUST NOT overlap, since the atoms are simply released from the body without adjusting their size or their position.

.. note::

   A integrator for the spheres needs to be available (e.g. :doc:`fix nve/sphere <fix_nve_sphere>`) otherwise the multisphere will fragment but no futher integration of the particles will occur


----------


**Restart, fix\_modify, output, run start/stop, minimize info:**

Same as for :doc:`fix\_multisphere <fix_multisphere>`.

Restrictions
""""""""""""


Same as for :doc:`fix\_multisphere <fix_multisphere>`.

Related commands
""""""""""""""""

:doc:`fix\_multisphere <fix_multisphere>`

**Default:** none


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
