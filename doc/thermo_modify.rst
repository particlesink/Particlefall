.. index:: thermo\_modify

thermo\_modify command
======================

Syntax
""""""


.. parsed-literal::

   thermo_modify keyword value ...

* one or more keyword/value pairs may be listed
* keyword = *lost* or *norm* or *flush* or *line* or *format*
  
  .. parsed-literal::
  
       *lost* value = *error* or *warn* or *ignore*
       *norm* value = *yes* or *no*
       *flush* value = *yes* or *no*
       *line* value = *one* or *multi*
       *format* values = *int* string or *float* string or M string
         M = integer from 1 to N, where N = # of quantities being printed
         string = C-style format string



Examples
""""""""


.. parsed-literal::

   thermo_modify lost ignore flush yes
   thermo_modify line multi format float %g

Description
"""""""""""

Set options for how thermodynamic information is computed and printed
by LIGGGHTS(R)-PUBLIC.

.. warning::

   These options apply to the currently defined thermo
   style.  When you specify a :doc:`thermo\_style <thermo_style>` command,
   all thermodynamic settings are restored to their default values,
   including those previously reset by a thermo\_modify command.  Thus if
   your input script specifies a thermo\_style command, you should use the
   thermo\_modify command after it.

The *lost* keyword determines whether LIGGGHTS(R)-PUBLIC checks for lost atoms
each time it computes thermodynamics and what it does if atoms are
lost.  An atom can be "lost" if it moves across a non-periodic
simulation box :doc:`boundary <boundary>` or if it moves more than a box
length outside the simulation domain (or more than a processor
sub-domain length) before reneighboring occurs.  The latter case is
typically due to bad dynamics, e.g. too large a timestep or huge
forces and velocities.  If the value is *ignore*\ , LIGGGHTS(R)-PUBLIC does not
check for lost atoms.  If the value is *error* or *warn*\ , LIGGGHTS(R)-PUBLIC
checks and either issues an error or warning.  The code will exit with
an error and continue with a warning.  A warning will only be issued
once, the first time an atom is lost.  This can be a useful debugging
option.

.. warning::

   For compuatational efficiency, this *lost* check is just
   a simple check if the total number of atoms in the system decreases.
   So for simulations where particles are continuously inserted, "lost"
   atoms might not be detected.

The *norm* keyword determines whether various thermodynamic output
values are normalized by the number of atoms or not, depending on
whether it is set to *yes* or *no*\ .  Different unit styles have
different defaults for this setting (see below).  Even if *norm* is
set to *yes*\ , a value is only normalized if it is an "extensive"
quantity, meaning that it scales with the number of atoms in the
system.  For the thermo keywords described by the doc page for the
:doc:`thermo\_style <thermo_style>` command, all energy-related keywords
are extensive. Other keywords such are "intensive" meaning their value is
independent (in a statistical sense) of the number of atoms in the
system and thus are never normalized.  For thermodynamic output values
extracted from fixes and computes in a :doc:`thermo\_style custom <thermo_style>` command, the doc page for the individual
:doc:`fix <fix>` or :doc:`compute <compute>` lists whether the value is
"extensive" or "intensive" and thus whether it is normalized.
Thermodynamic output values calculated by a variable formula are
assumed to be "intensive" and thus are never normalized.  You can
always include a divide by the number of atoms in the variable formula
if this is not the case.

The *flush* keyword invokes a flush operation after thermodynamic info
is written to the log file.  This insures the output in that file is
current (no buffering by the OS), even if LIGGGHTS(R)-PUBLIC halts before the
simulation completes.

The *line* keyword determines whether thermodynamics will be printed
as a series of numeric values on one line or in a multi-line format
with 3 quantities with text strings per line and a dashed-line header
containing the timestep and CPU time.  This modify option overrides
the *one* and *multi* thermo\_style settings.

The *format* keyword sets the numeric format of individual printed
quantities.  The *int* and *float* keywords set the format for all
integer or floating-point quantities printed.  The setting with a
numeric value M (e.g. format 5 %10.4g) sets the format of the Mth
value printed in each output line, e.g. the 5th column of output in
this case.  If the format for a specific column has been set, it will
take precedent over the *int* or *float* setting.

.. warning::

   The thermo output values *step* and *atoms* are stored
   internally as 8-byte signed integers, rather than the usual 4-byte
   signed integers.  When specifying the "format int" keyword you can use
   a "%d"-style format identifier in the format string and LIGGGHTS(R)-PUBLIC will
   convert this to the corresponding "%lu" form when it is applied to
   those keywords.  However, when specifying the "format M string"
   keyword for *step* and *natoms*\ , you should specify a string
   appropriate for an 8-byte signed integer, e.g. one with "%ld".

Restrictions
""""""""""""
 none

Related commands
""""""""""""""""

:doc:`thermo <thermo>`, :doc:`thermo\_style <thermo_style>`

Default
"""""""

The option defaults are lost = ignore, norm = yes for unit style of
*lj*\ , norm = no for unit style of *real* and *metal*\ , flush = no.

The defaults for the line and format options depend on the thermo
style.  For styles "one" and "custom", the line and format defaults
are "one", "%8d", and "%12.8g".  For style "multi", the line and
format defaults are "multi", "%8d", and "%14.4f".


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
