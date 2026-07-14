.. index:: fix

fix command
===========

Syntax
""""""


.. parsed-literal::

   fix ID group-ID style args

* ID = user-assigned name for the fix
* group-ID = ID of the group of atoms to apply the fix to
* style = one of a long list of possible style names (see below)
* args = arguments used by a particular style

Examples
""""""""


.. parsed-literal::

   fix 1 all nve
   fix 3 all nvt temp 300.0 300.0 0.01
   fix mine top setforce 0.0 NULL 0.0

Description
"""""""""""

Set a fix that will be applied to a group of atoms.  In Packfall, a
"fix" is any operation that is applied to the system during
timestepping or minimization.  Examples include updating of atom
positions and velocities due to time integration, controlling
temperature, applying constraint forces to atoms, enforcing boundary
conditions, computing diagnostics, etc.  There are dozens of fixes
defined in Packfall and new ones can be added; see :doc:`this section <Section_modify>` for a discussion.

The full list of fixes defined in Packfall is on :ref:`this page <cmd_5>`.

Fixes perform their operations at different stages of the timestep.
If 2 or more fixes operate at the same stage of the timestep, they are
invoked in the order they were specified in the input script.

The ID of a fix can only contain alphanumeric characters and
underscores.

Fixes can be deleted with the :doc:`unfix <unfix>` command.

.. warning::

   The :doc:`unfix <unfix>` command is the only way to turn
   off a fix; simply specifying a new fix with a similar style will not
   turn off the first one.  This is especially important to realize for
   integration fixes.  For example, using a :doc:`fix nve <fix_nve>`
   command for a second run after using a :doc:`fix nvt <fix_nh>` command
   for the first run, will not cancel out the NVT time integration
   invoked by the "fix nvt" command.  Thus two time integrators would be
   in place!

If you specify a new fix with the same ID and style as an existing
fix, the old fix is deleted and the new one is created (presumably
with new settings).  This is the same as if an "unfix" command were
first performed on the old fix, except that the new fix is kept in the
same order relative to the existing fixes as the old one originally
was.  Note that this operation also wipes out any additional changes
made to the old fix via the :doc:`fix\_modify <fix_modify>` command.

The :doc:`fix modify <fix_modify>` command allows settings for some
fixes to be reset.  See the doc page for individual fixes for details.

Some fixes store an internal "state" which is written to binary
restart files via the :doc:`restart <restart>` or
:doc:`write\_restart <write_restart>` commands.  This allows the fix to
continue on with its calculations in a restarted simulation.  See the
:doc:`read\_restart <read_restart>` command for info on how to re-specify
a fix in an input script that reads a restart file.  See the doc pages
for individual fixes for info on which ones can be restarted.


----------


Some fixes calculate one of three styles of quantities: global,
per-atom, or local, which can be used by other commands or output as
described below.  A global quantity is one or more system-wide values,
e.g. the energy of a wall interacting with particles.  A per-atom
quantity is one or more values per atom, e.g. the displacement vector
for each atom since time 0.  Per-atom values are set to 0.0 for atoms
not in the specified fix group.  Local quantities are calculated by
each processor based on the atoms it owns, but there may be zero or
more per atoms.

Note that a single fix may produces either global or per-atom or local
quantities (or none at all), but never more than one of these.

Global, per-atom, and local quantities each come in three kinds: a
single scalar value, a vector of values, or a 2d array of values.  The
doc page for each fix describes the style and kind of values it
produces, e.g. a per-atom vector.  Some fixes produce more than one
kind of a single style, e.g. a global scalar and a global vector.

When a fix quantity is accessed, as in many of the output commands
discussed below, it can be referenced via the following bracket
notation, where ID is the ID of the fix:

+-------------+--------------------------------------------+
| f\_ID       | entire scalar, vector, or array            |
+-------------+--------------------------------------------+
| f\_ID[I]    | one element of vector, one column of array |
+-------------+--------------------------------------------+
| f\_ID[I][J] | one element of array                       |
+-------------+--------------------------------------------+

In other words, using one bracket reduces the dimension of the
quantity once (vector -> scalar, array -> vector).  Using two brackets
reduces the dimension twice (array -> scalar).  Thus a command that
uses scalar fix values as input can also process elements of a vector
or array.

Note that commands and :doc:`variables <variable>` which use fix
quantities typically do not allow for all kinds, e.g. a command may
require a vector of values, not a scalar.  This means there is no
ambiguity about referring to a fix quantity as f\_ID even if it
produces, for example, both a scalar and vector.  The doc pages for
various commands explain the details.


----------


In Packfall, the values generated by a fix can be used in several ways:

* Global values can be output via the :doc:`thermo\_style custom <thermo_style>` or :doc:`fix ave/time <fix_ave_time>` command.
  Or the values can be referenced in a :doc:`variable equal <variable>` or
  :doc:`variable atom <variable>` command.
* Per-atom values can be output via the :doc:`dump custom <dump>` command
  or the :doc:`fix ave/spatial <fix_ave_spatial>` command.  Or they can be
  time-averaged via the :doc:`fix ave/atom <fix_ave_atom>` command or
  reduced by the :doc:`compute reduce <compute_reduce>` command.  Or the
  per-atom values can be referenced in an :doc:`atom-style variable <variable>`.
* Local values can be reduced by the :doc:`compute reduce <compute_reduce>` command, or histogrammed by the :doc:`fix ave/histo <fix_ave_histo>` command.

See this :ref:`howto section <howto_8>` for a summary of
various Packfall output options, many of which involve fixes.

The results of fixes that calculate global quantities can be either
"intensive" or "extensive" values.  Intensive means the value is
independent of the number of atoms in the simulation,
e.g. temperature.  Extensive means the value scales with the number of
atoms in the simulation, e.g. total rotational kinetic energy.
:doc:`Thermodynamic output <thermo_style>` will normalize extensive
values by the number of atoms in the system, depending on the
"thermo\_modify norm" setting.  It will not normalize intensive values.
If a fix value is accessed in another way, e.g. by a
:doc:`variable <variable>`, you may want to know whether it is an
intensive or extensive value.  See the doc page for individual fixes
for further info.


----------


Each fix style has its own documentation page which describes its
arguments and what it does, as listed below.

The full list of fixes defined in Packfall is on :ref:`this page <cmd_5>`.

Restrictions
""""""""""""


Some fix styles are part of specific packages.  They are only enabled
if Packfall was built with that package.  See the :ref:`Making Packfall <start_3>` section for more info on packages.
The doc pages for individual fixes tell if it is part of a package.

Related commands
""""""""""""""""

:doc:`unfix <unfix>`, :doc:`fix\_modify <fix_modify>`

**Default:** none


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
