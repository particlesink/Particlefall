.. index:: print

print command
=============

Syntax
""""""

print string keyword value:pre

* string = text string to print, which may contain variables
* zero or more keyword/value pairs may be appended
  
  .. parsed-literal::
  
     keyword = *file* or *append* or *screen* or *newline*\ :l
       *file* value = filename
       *append* value = filename
       *screen* value = *yes* or *no*
       *newline* value = *yes* or *no*



Examples
""""""""


.. parsed-literal::

   print "Done with equilibration" file info.dat
   print Vol=$\ *volume* append info.dat screen no newline no
   print "The system volume is now $v"
   print 'The system volume is now $v'

Description
"""""""""""

Print a text string to the screen and logfile.  One line of output is
generated.  The text string must be a single argument, so it should be
enclosed in double quotes if it is more than one word.  If it contains
variables, they will be evaluated and their current values printed.

If the *file* or *append* keyword is used, a filename is specified to
which the output will be written.  If *file* is used, then the
filename is overwritten if it already exists.  If *append* is used,
then the filename is appended to if it already exists, or created if
it does not exist.

If the *screen* keyword is used, output to the screen and logfile can
be turned on or off as desired.

If the *newline* keyword is used and its value set to *no* then no newline
character will be printed at the end of the string. This can be useful if a
string should be concatenated.

If you want the print command to be executed multiple times (with
changing variable values), there are 3 options.  First, consider using
the :doc:`fix print <fix_print>` command, which will print a string
periodically during a simulation.  Second, the print command can be
used as an argument to the *every* option of the :doc:`run <run>`
command.  Third, the print command could appear in a section of the
input script that is looped over (see the :doc:`jump <jump>` and
:doc:`next <next>` commands).

See the :doc:`variable <variable>` command for a description of *equal*
style variables which are typically the most useful ones to use with
the print command.  Equal-style variables can calculate formulas
involving mathematical operations, atom properties, group properties,
thermodynamic properties, global values calculated by a
:doc:`compute <compute>` or :doc:`fix <fix>`, or references to other
:doc:`variables <variable>`.

Restrictions
""""""""""""
 none

Related commands
""""""""""""""""

:doc:`fix print <fix_print>`, :doc:`variable <variable>`

Default
"""""""

The option defaults are no file output, screen = yes and newline = yes.


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
