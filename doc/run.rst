.. index:: run

run command
===========

Syntax
""""""

run N keyword values ...

* N = # of timesteps
* N can also be specified as a physical time with a trailing *sec* or *s* suffix (prototype)
* zero or more keyword/value pairs may be appended
* keyword = *upto* or *start* or *stop* or *pre* or *post* or *every*
  
  .. parsed-literal::
  
       *upto* value = none 
       *start* value = N1
         N1 = timestep at which 1st run started
       *stop* value = N2
         N2 = timestep at which last run will end
       *pre* value = *no* or *yes*
       *post* value = *no* or *yes*
       *every* values = M c1 c2 ...
         M = break the run into M-timestep segments and invoke one or more commands between each segment
         c1,c2,...,cN = one or more LIGGGHTS(R)-PUBLIC commands, each enclosed in quotes
         c1 = NULL means no command will be invoked



Examples
""""""""


.. parsed-literal::

   run 10000
   run 1000000 upto
   run 100 start 0 stop 1000
   run 1000 pre no post yes
   run 0.5 sec
   run 2.6 s upto
   run 100000 start 0 stop 1000000 every 1000 "print 'Protein Rg = $r'"
   run 100000 every 1000 NULL

Description
"""""""""""

Run or continue dynamics for a specified number of timesteps ("run").
As a prototype, a physical time can also be specified directly by
adding a trailing *sec* or *s* suffix.

When the :doc:`run style <run_style>` is *respa*\ , N refers to outer
loop (largest) timesteps.

A value of N = 0 is acceptable; only the thermodynamics of the system
are computed and printed without taking a timestep.

For example, if the timestep is fixed at 1.0e-5 seconds, these two
commands are equivalent:


.. parsed-literal::

   run 100000
   run 1.0 sec

The explicit time form is intended to replace common input-script
patterns that manually convert physical time to timestep counts.

The *upto* keyword means to perform a run starting at the current
timestep up to the specified timestep.  E.g. if the current timestep
is 10,000 and "run 100000 upto" is used, then an additional 90,000
timesteps will be run.  This can be useful for very long runs on a
machine that allocates chunks of time and terminate your job when time
is exceeded.  If you need to restart your script multiple times
(reading in the last restart file), you can keep restarting your
script with the same run command until the simulation finally
completes.

For the prototype time-based form, *upto* compares against the current
simulation time instead of the current timestep.

The *start* or *stop* keywords can be used if multiple runs are being
performed and you want a :doc:`fix <fix>` command that changes some
value over time to make the change across the
entire set of runs and not just a single run.  See the doc page for
individual fixes to see which ones can be used with the *start/stop*
keywords.

The *pre* and *post* keywords can be used to streamline the setup,
clean-up, and associated output to the screen that happens before and
after a run.  This can be useful if you wish to do many short runs in
succession (e.g. LIGGGHTS(R)-PUBLIC is being called as a library which is doing
other computations between successive short LIGGGHTS(R)-PUBLIC runs).

By default (pre and post = yes), LIGGGHTS(R)-PUBLIC creates neighbor lists,
computes forces, and imposes fix constraints before every run.  And
after every run it gathers and prints timings statistics.  If a run is
just a continuation of a previous run (i.e. no settings are changed),
the initial computation is not necessary; the old neighbor list is
still valid as are the forces.  So if *pre* is specified as "no" then
the initial setup is skipped, except for printing thermodynamic info.
Note that if *pre* is set to "no" for the very 1st run LIGGGHTS(R)-PUBLIC
performs, then it is overridden, since the initial setup computations
must be done.

.. warning::

   If your input script changes settings between 2 runs
   (e.g. adds a :doc:`fix <fix>` or :doc:`dump <dump>` or
   :doc:`compute <compute>` or changes a :doc:`neighbor <neigh_modify>` list
   parameter), then the initial setup must be performed.  LIGGGHTS(R)-PUBLIC does not
   check for this, but it would be an error to use the *pre no* option in
   this case.

If *post* is specified as "no", the full timing summary is skipped;
only a one-line summary timing is printed.

The *every* keyword provides a means of breaking a LIGGGHTS(R)-PUBLIC run into a
series of shorter runs.  Optionally, one or more LIGGGHTS(R)-PUBLIC commands (c1,
c2, ..., cN) will be executed in between the short runs.  If used, the
*every* keyword must be the last keyword, since it has a variable
number of arguments.  Each of the trailing arguments is a single
LIGGGHTS(R)-PUBLIC command, and each command should be enclosed in quotes, so that
the entire command will be treated as a single argument.  This will
also prevent any variables in the command from being evaluated until
it is executed multiple times during the run.  Note that if a command
itself needs one of its arguments quoted (e.g. the :doc:`print <print>`
command), then you can use a combination of single and double quotes,
as in the example above or below.

The *every* keyword is a means to avoid listing a long series of runs
and interleaving commands in your input script.  For example, a
:doc:`print <print>` command could be invoked or a :doc:`fix <fix>` could
be redefined, e.g. to reset a thermostat temperature.  Or this could
be useful for invoking a command you have added to LIGGGHTS(R)-PUBLIC that wraps
some other code (e.g. as a library) to perform a computation
periodically during a long LIGGGHTS(R)-PUBLIC run.  See :doc:`this section <Section_modify>` of the documentation for info about how
to add new commands to LIGGGHTS(R)-PUBLIC.  See :ref:`this section <howto_10>` of the documentation for ideas
about how to couple LIGGGHTS(R)-PUBLIC to other codes.

With the *every* option, N total steps are simulated, in shorter runs
of M steps each.  After each M-length run, the specified commands are
invoked.  If only a single command is specified as NULL, then no
command is invoked.  Thus these lines:


.. parsed-literal::

   variable q equal x[100]
   run 6000 every 2000 "print Coord = $q"

are the equivalent of:


.. parsed-literal::

   variable q equal x[100]
   run 2000
   print Coord = $q
   run 2000
   print Coord = $q
   run 2000
   print Coord = $q

which does 3 runs of 2000 steps and prints the x-coordinate of a
particular atom between runs.  Note that the variable "$q" will
be evaluated afresh each time the print command is executed.

Note that by using the line continuation character "&", the run every
command can be spread across many lines, though it is still a single
command:


.. parsed-literal::

   run 100000 every 1000 &
     "print 'Minimum value = $a'" &
     "print 'Maximum value = $b'" &
     "print 'Temp = $c'" &
     "print 'Press = $d'"

If the *pre* and *post* options are set to "no" when used with the
*every* keyword, then the 1st run will do the full setup and the last
run will print the full timing summary, but these operations will be
skipped for intermediate runs.

.. warning::

   You might hope to specify a command that exits the run
   by jumping out of the loop, e.g.


.. parsed-literal::

   variable t equal temp
   run 10000 every 100 "if '$t < 300.0' then 'jump SELF afterrun'"

Unfortunately this will not currently work.  The run command simply
executes each command one at a time each time it pauses, then
continues the run.  You can replace the jump command with a simple
:doc:`quit <quit>` command and cause LIGGGHTS(R)-PUBLIC to exit during the
middle of a run when the condition is met.

Restrictions
""""""""""""


The prototype time-based form currently only supports the *upto*,
*pre*, and *post* keywords.  The *start*, *stop*, and *every* keywords
remain timestep-only for now.

The number of specified timesteps N must fit in a signed 32-bit
integer, so you are limited to slightly more than 2 billion steps
(2\^31) in a single run.  However, you can perform successive runs to
run a simulation for any number of steps (ok, up to 2\^63 steps).

Related commands
""""""""""""""""

:doc:`run\_style <run_style>`,

Default
"""""""

The option defaults are start = the current timestep, stop = current
timestep + N, pre = yes, and post = yes.


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
