.. index:: reset\_timestep

reset\_timestep command
=======================

Syntax
""""""


.. parsed-literal::

   reset_timestep N

* N = timestep number

Examples
""""""""


.. parsed-literal::

   reset_timestep 0
   reset_timestep 4000000

Description
"""""""""""

Set the timestep counter to the specified value.  This command
normally comes after the timestep has been set by reading a restart
file via the :doc:`read\_restart <read_restart>` command, or a previous
simulation advanced the timestep.

The :doc:`read\_data <read_data>` and :doc:`create\_box <create_box>`
commands set the timestep to 0; the :doc:`read\_restart <read_restart>`
command sets the timestep to the value it had when the restart file
was written.

Restrictions
""""""""""""


This command cannot be used when any fixes are defined that keep track
of elapsed time to perform certain kinds of time-dependent operations.
All insertion commands are typically using such operations.

Various fixes use the current timestep to calculate related
quantities.  If the timestep is reset, this may produce unexpected
behavior, but Packfall allows the fixes to be defined even if the
timestep is reset.

Resetting the timestep clears flags for :doc:`computes <compute>` that
may have calculated some quantity from a previous run.  This means
these quantity cannot be accessed by a variable in between runs until
a new run is performed.  See the :doc:`variable <variable>` command for
more details.

Related commands
""""""""""""""""

:doc:`rerun <rerun>`

**Default:** none


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
