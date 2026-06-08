.. index:: dump\_modify

dump\_modify command
====================

Syntax
""""""


.. parsed-literal::

   dump_modify dump-ID keyword values ...

* dump-ID = ID of dump to modify
* one or more keyword/value pairs may be appended
* these keywords apply to the dump custom/vtk style
* keyword = *binary* or *element* or *every* or *fileper* or *first* or *label* or *nfile* or *pad* or *region* or *sort* or *thresh*
  
  .. parsed-literal::
  
       *binary* arg = *yes* or *no*
       *element* args = E1 E2 ... EN, where N = # of atom types
         E1,...,EN = element name, e.g. C or Fe or Ga
       *every* arg = N
         N = dump every this many timesteps
         N can be a variable (see below)
       *fileper* arg = Np
         Np = write one file for every this many processors
       *first* arg = *yes* or *no*
       *label* arg = string
         string = character string to use in header of legacy VTK file
       *nfile* arg = Nf
         Nf = write this many files, one from each of Nf processors
       *pad* arg = Nchar = # of characters to convert timestep to
       *region* arg = region-ID or "none"
       *sort* arg = *off* or *id* or N or -N
          off = no sorting of per-atom lines within a snapshot
          id = sort per-atom lines by atom ID
          N = sort per-atom lines in ascending order by the Nth column
          -N = sort per-atom lines in descending order by the Nth column
       *thresh* args = attribute operation value
         attribute = same attributes (x,fy,etotal,sxx,etc) used by dump custom style
         operation = "<" or "<=" or ">" or ">=" or "==" or "!="
         value = numeric value to compare to
         these 3 args can be replaced by the word "none" to turn off thresholding



Examples
""""""""


.. parsed-literal::

   dump_modify dmpvtp binary yes
   dump_modify e_data region mySphere thresh x < 0.0 thresh ervel >= 0.2
   dump_modify dmpvtk every 1000 nfile 20
   dump_modify dmpvtk every v_myVar

Description
"""""""""""

Modify the parameters of a previously defined dump command.

These keywords apply to the :doc:`dump custom/vtk <dump_custom_vtk>` style.
The description gives details.


----------


The *binary* keyword, if specified as *yes*\ , causes the output to be
written in binary format. If specified as *no*\ , which is the default,
the data is written in ASCII format to the output file.


----------


The *element* keyword associates element names (e.g. H, C, Fe) with
LIGGGHTS(R)-PUBLIC atom types.  There are no restrictions to what
label can be used as an element name.  Any whitespace separated text
will be accepted.


----------


The *every* keyword changes the dump frequency originally specified by
the :doc:`dump custom/vtk <dump_custom_vtk>` command to a new value.
The every keyword can be specified in one of two ways.
It can be a numeric value in which case it must be > 0.
Or it can be an :doc:`equal-style variable <variable>`,
which should be specified as v\_name, where name is the variable name.

In this case, the variable is evaluated at the beginning of a run to
determine the next timestep at which a dump snapshot will be written
out.  On that timestep the variable will be evaluated again to
determine the next timestep, etc.  Thus the variable should return
timestep values.  See the stagger() and logfreq() and stride() math
functions for :doc:`equal-style variables <variable>`, as examples of
useful functions to use in this context.  Other similar math functions
could easily be added as options for :doc:`equal-style variables <variable>`.  Also see the next() function, which allows
use of a file-style variable which reads successive values from a
file, each time the variable is evaluated.  Used with the *every*
keyword, if the file contains a list of ascending timesteps, you can
output snapshots whenever you wish.

Note that when using the variable option with the *every* keyword, you
need to use the *first* option if you want an initial snapshot written
to the dump file.

For example, the following commands will
write snapshots at timesteps 0,10,20,30,100,200,300,1000,2000,etc:


.. parsed-literal::

   variable	s equal logfreq(10,3,10)
   dump		1 all custom/vtk 100 tmp.dump\*.vtk vx vy vz
   dump_modify	1 every v_s first yes

The following commands would write snapshots at the timesteps listed
in file tmp.times:


.. parsed-literal::

   variable        f file tmp.times
   variable	s equal next(f)
   dump		1 all custom/vtk 100 tmp.dump\*.vtk vx vy vz
   dump_modify	1 every v_s

.. warning::

   When using a file-style variable with the *every*
   keyword, the file of timesteps must list a first timestep that is
   beyond the current timestep (e.g. it cannot be 0).  And it must list
   one or more timesteps beyond the length of the run you perform.  This
   is because the dump command will generate an error if the next
   timestep it reads from the file is not a value greater than the
   current timestep.  Thus if you wanted output on steps 0,15,100 of a
   100-timestep run, the file should contain the values 15,100,101 and
   you should also use the dump\_modify first command.  Any final value >
   100 could be used in place of 101.


----------


The *first* keyword determines whether a dump snapshot is written on
the very first timestep after the dump command is invoked.  This will
always occur if the current timestep is a multiple of N, the frequency
specified in the :doc:`dump custom/vtk <dump_custom_vtk>` command,
including timestep 0.  But if this is not the case, a dump snapshot
will only be written if the setting of this keyword is *yes*\ .
If it is *no*\ , which is the default, then it will not be written.


----------


The *fileper* keyword is documented below with the *nfile* keyword.


----------


When writing to legacy VTK files, the dump *custom/vtk* style will
use the specified *label* as the header line.  By default this header line is:


.. parsed-literal::

   Generated by LIGGGHTS(R)-PUBLIC


----------


The *nfile* or *fileper* keywords can be used in conjunction with the
"%" wildcard character in the specified dump file name, if an XML file
format was specified.  As explained on the :doc:`dump custom\_vtk <dump_custom_vtk>`
command doc page, the "%" character causes the dump file to be written
in pieces, one piece for each of P processors.  By default P = the
number of processors the simulation is running on.  The *nfile* or
*fileper* keyword can be used to set P to a smaller value, which can
be more efficient when running on a large number of processors.

The *nfile* keyword sets P to the specified Nf value.  For example, if
Nf = 4, and the simulation is running on 100 processors, 4 files will
be written, by processors 0,25,50,75.  Each will collect information
from itself and the next 24 processors and write it to a dump file.

For the *fileper* keyword, the specified value of Np means write one
file for every Np processors.  For example, if Np = 4, every 4th
processor (0,4,8,12,etc) will collect information from itself and the
next 3 processors and write it to a dump file.


----------


The *pad* keyword only applies when the dump filename is specified
with a wildcard "\*" character which becomes the timestep.  If *pad* is
0, which is the default, the timestep is converted into a string of
unpadded length, e.g. 100 or 12000 or 2000000.  When *pad* is
specified with *Nchar* > 0, the string is padded with leading zeroes
so they are all the same length = *Nchar*\ .  For example, pad 7 would
yield 0000100, 0012000, 2000000.  This can be useful so that
post-processing programs can easily read the files in ascending
timestep order.


----------


If the *region* keyword is specified, only atoms in the region
will be written to the dump file.  Only one region can be applied as
a filter (the last one specified).  See the :doc:`region <region>`
command for more details.  Note that a region can be defined as the
"inside" or "outside" of a geometric shape, and it can be the "union"
or "intersection" of a series of simpler regions.


----------


The *sort* keyword determines whether lines of per-atom output in a
snapshot are sorted or not.  A sort value of *off* means they will
typically be written in indeterminate order, either in serial or
parallel.  This is the case even in serial if the :doc:`atom\_modify sort <atom_modify>` option is turned on, which it is by default, to
improve performance.  A sort value of *id* means sort the output by
atom ID.  A sort value of N or -N means sort the output by the value
in the Nth column of per-atom info in either ascending or descending
order.

.. warning::

   Unless it is required by the dump style, sorting dump
   file output requires extra overhead in terms of CPU and communication
   cost, as well as memory, versus unsorted output.


----------


Using the *thresh* keyword, multiple thresholds can be specified.
Specifying "none" turns off all threshold criteria.  If thresholds are
specified, only atoms whose attributes meet all the threshold criteria
are written to the dump file.  The possible attributes that can be
tested for are the same as those that can be specified in the
:doc:`dump custom/vtk <dump_custom_vtk>` command, with the exception
of the *element* attribute, since it is not a numeric value.  Note
that different attributes can be output by the dump custom/vtk command
than are used as threshold criteria by the dump\_modify command.
E.g. you can output the coordinates and stress of atoms whose energy
is above some threshold.





Restrictions
""""""""""""
 none

Related commands
""""""""""""""""

:doc:`dump <dump>`, :doc:`dump custom/vtk <dump_custom_vtk>`, :doc:`dump image <dump_image>`, :doc:`undump <undump>`

Default
"""""""

The option defaults are

* binary = no
* element = "C" for every atom type
* every = whatever it was set to via the :doc:`dump custom/vtk <dump_custom_vtk>` command
* fileper = # of processors
* first = no
* label = "Generated by LIGGGHTS"
* nfile = 1
* pad = 0
* region = none
* sort = off
* thresh = none


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
