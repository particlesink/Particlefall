Getting Started
===============

This section describes how to build and run Packfall.


.. contents:: 
		:local:
		:depth: 1
   
.. _start\_1:

How to obtain Packfall
-----------------------------------------------

.. parsed-literal::

   git clone https://github.com/particlesink/Packfall.git


----------


.. _start\_2:

Building Packfall
----------------------------------------

* :ref:`Steps to build a Packfall executable <start_2_2>`
* :ref:`Common errors that can occur when making Packfall <start_2_3>`
* :ref:`Additional build tips <start_2_4>`


----------


.. _start\_2\_1:


----------


.. _start\_2\_2:

**Steps to build a Packfall executable:**
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Currently tested and supported systems are Windows/msys2 and Linux.

**Step 0**

.. parsed-literal::

   make

or

.. parsed-literal::

   make MPI=1

to build with MPI support



**Step 2**



All model combinations. You can create a file called 
\*style\_contact\_model\_user.whitelist\*
in your Packfall *src/* folder and fill it with entries such as


.. parsed-literal::

   GRAN_MODEL(HERTZ, TANGENTIAL_OFF, COHESION_OFF, ROLLING_OFF, SURFACE_DEFAULT)
   GRAN_MODEL(HERTZ, TANGENTIAL_HISTORY, COHESION_OFF, ROLLING_EPSD, SURFACE_DEFAULT)

After creating or modifying it you will need to
recompile Packfall.

In addition you can automatically scan files or folders recursively for
Packfall input scripts and search them for their used contact model.
With this list an addition to the whitelist is automatically created.
Running the following commands within the Packfall src directory


.. parsed-literal::

   ./genAutoExamplesWhitelist.sh -h
   ./genAutoExamplesWhitelist.sh ../examples

The first will display the help message for genAutoExamplesWhitelist.sh.
The second will recursively search the examples folder of your Packfall
installation and scan all in.\* files for the used contact models. It will
also create the file \*style\_contact\_model\_autoExamples.whitelist\* containing
the unique used models in the whitelist format.

A third whitelist is automatically created during the compilation and is 
stored in \*style\_contact\_model.whitelist\* . If this file exists, the next
compilation will NOT change this file. You can manually delete it and 
recreate it with


.. parsed-literal::

   sh Make.sh models
   sh Make.sh models_full

During every compilation the three whitelist files 
\*style\_contact\_model\_user.whitelist\* ,
\*style\_contact\_model\_autoExamples.whitelist\* and 
\*style\_contact\_model.whitelist\* are
automatically merged and only unique entries are kept in the merged file.


----------


.. _start\_5:

Building Packfall as a library
-------------------------------------------------------

Packfall can be built as either a static or shared library, which can
then be called from another application or a scripting language.  See
:ref:`this section <howto_10>` for more info on coupling
Packfall to other codes.  See :doc:`this section <Section_python>` for
more info on wrapping and running Packfall from Python.

**Static library:**
^^^^^^^^^^^^^^^^^^^

To build Packfall as a static library (\*.a file on Linux), type


.. parsed-literal::

   make -f Makefile.msys2 lib 
   make -f Makefile.msys2 shlib

where foo is the machine name (if you normally compile with \*auto\* then use
this option).  This kind of library is typically used to statically link a
driver application to Packfall, so that you can insure all dependencies
are satisfied at compile time.  Note that inclusion or exclusion of any
desired optional packages should be done before typing "make makelib".  The
first "make" command will create a current Makefile.lib with all the file
names in your src dir.  The second "make" command will use it to build
Packfall as a static library, using the ARCHIVE and ARFLAGS settings in
src/MAKE/Makefile.foo.  

**Shared library:**
^^^^^^^^^^^^^^^^^^^

To build Packfall as a shared library (\*.so file on Linux), which can be
dynamically loaded, e.g. from Python, type


.. parsed-literal::

   make makeshlib
   make -f Makefile.shlib foo

where foo is the machine name (if you normally compile with \*auto\* then use
this option). This kind of library is required when wrapping Packfall
with Python; see :doc:`Section\_python <Section_python>` for details.  Again,
note that inclusion or exclusion of any desired optional packages should be
done before typing "make makelib".  The first "make" command will create a
current Makefile.shlib with all the file names in your src dir.  The second
"make" command will use it to build Packfall as a shared library, using
the SHFLAGS and SHLIBFLAGS settings in src/MAKE/Makefile.foo.  The build
will create the file libpackfall.so which another application can link to
dyamically.  It will also create a soft link libpackfall.so, which the Python
wrapper uses by default.

Note that for a shared library to be usable by a calling program, all
the auxiliary libraries it depends on must also exist as shared
libraries.  This will be the case for libraries included with Packfall,
such as the dummy MPI library in src/STUBS or any package libraries in
lib/packges, since they are always built as shared libraries with the
-fPIC switch.  However, if a library like MPI or FFTW does not exist
as a shared library, the second make command will generate an error.
This means you will need to install a shared library version of the
package.  The build instructions for the library should tell you how
to do this.


**Calling the Packfall library:**
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Either flavor of library (static or shared) allows one or more Packfall
objects to be instantiated from the calling program.

When used from a C++ program, all of Packfall is wrapped in a LAMMPS\_NS
namespace; you can safely use any of its classes and methods from
within the calling code, as needed.

When used from a C or Fortran program or a scripting language like
Python, the library has a simple function-style interface, provided in
src/library.cpp and src/library.h.

See the sample codes in examples/COUPLE/simple for examples of C++ and
C and Fortran codes that invoke Packfall thru its library interface.
There are other examples as well in the COUPLE directory which are
discussed in :ref:`Section\_howto 10 <howto_10>` of the
manual.  See :doc:`Section\_python <Section_python>` of the manual for a
description of the Python wrapper provided with Packfall that operates
through the Packfall library interface.

The files src/library.cpp and library.h define the C-style API for
using Packfall as a library.  See :ref:`Section\_howto 19 <howto_19>` 
of the manual for a description of the
interface and how to extend it for your needs.


----------


.. _start\_6:

Running Packfall
-----------------------------------------

By default, Packfall runs by reading commands from stdin; e.g. packfall
< in.file.  This means you first create an input script (e.g. in.file)
containing the desired commands.  :doc:`This section <Section_commands>`
describes how input scripts are structured and what commands they
contain.

You can test Packfall on any of the sample inputs provided in the
examples or bench directory.  Input scripts are named in.\* and sample
outputs are named log.\*.name.P where name is a machine and P is the
number of processors it was run on.

Here is how you might run a standard chute wear benchmark on a
Linux box, using mpirun to launch a parallel job:


.. parsed-literal::

   cd src
   make
   cd ../examples/Packfall/Tutorials_public/chute_wear
   mpirun -np 4 packfall < in.chute_wear


----------



The screen output from Packfall is described in the next section.  As it
runs, Packfall also writes a log file with the same information.

If Packfall encounters errors in the input script or while running a
simulation it will print an ERROR message and stop or a WARNING
message and continue.  See :doc:`Section\_errors <Section_errors>` for a
discussion of the various kinds of errors Packfall can or can't detect,
a list of all ERROR and WARNING messages, and what to do about them.

Packfall can run a problem on any number of processors, including a
single processor.  In theory you should get identical answers on any
number of processors and on any machine.  In practice, numerical
round-off can cause slight differences and eventual divergence of
molecular dynamics phase space trajectories.

Packfall can run as large a problem as will fit in the physical memory
of one or more processors.  If you run out of memory, you must run on
more processors or setup a smaller problem.


----------


.. _start\_7:

Command-line options
-----------------------------------

At run time, Packfall recognizes several optional command-line switches
which may be used in any order.  Either the full word or a one-or-two
letter abbreviation can be used:

* -e or -echo
* -i or -in
* -h or -help
* -l or -log
* -nc or -nocite
* -p or -partition
* -pl or -plog
* -ps or -pscreen
* -r or -restart
* -ro or -reorder
* -sc or -screen
* -sf or -suffix
* -v or -var

For example, packfall might be launched as follows:


.. parsed-literal::

   mpirun -np 16 packfall -v f tmp.out -l my.log -sc none < in.alloy
   mpirun -np 16 packfall -var f tmp.out -log my.log -screen none < in.alloy

Here are the details on the options:


.. parsed-literal::

   -echo style

Set the style of command echoing.  The style can be *none* or *screen*
or *log* or *both*\ .  Depending on the style, each command read from
the input script will be echoed to the screen and/or logfile.  This
can be useful to figure out which line of your script is causing an
input error.  The default value is *log*\ .  The echo style can also be
set by using the :doc:`echo <echo>` command in the input script itself.


.. parsed-literal::

   -in file

Specify a file to use as an input script.  This is an optional switch
when running Packfall in one-partition mode.  If it is not specified,
Packfall reads its input script from stdin - e.g. packfall < in.run.
This is a required switch when running Packfall in multi-partition mode,
since multiple processors cannot all read from stdin.


.. parsed-literal::

   -help

Print a list of options compiled into this executable for each Packfall
style (atom\_style, fix, compute, pair\_style, bond\_style, etc).  This
can help you know if the command you want to use was included via the
appropriate package.  Packfall will print the info and immediately exit
if this switch is used.


.. parsed-literal::

   -log file

Specify a log file for Packfall to write status information to.  In
one-partition mode, if the switch is not used, Packfall writes to the
file log.packfall.  If this switch is used, Packfall writes to the
specified file.  In multi-partition mode, if the switch is not used, a
log.packfall file is created with hi-level status information.  Each
partition also writes to a log.packfall.N file where N is the partition
ID.  If the switch is specified in multi-partition mode, the hi-level
logfile is named "file" and each partition also logs information to a
file.N.  For both one-partition and multi-partition mode, if the
specified file is "none", then no log files are created.  Using a
:doc:`log <log>` command in the input script will override this setting.
Option -plog will override the name of the partition log files file.N.


.. parsed-literal::

   -partition 8x2 4 5 ...

Invoke Packfall in multi-partition mode.  When Packfall is run on P
processors and this switch is not used, Packfall runs in one partition,
i.e. all P processors run a single simulation.  If this switch is
used, the P processors are split into separate partitions and each
partition runs its own simulation.  The arguments to the switch
specify the number of processors in each partition.  Arguments of the
form MxN mean M partitions, each with N processors.  Arguments of the
form N mean a single partition with N processors.  The sum of
processors in all partitions must equal P.  Thus the command
"-partition 8x2 4 5" has 10 partitions and runs on a total of 25
processors.

To run multiple independent simulatoins from one input script, using
multiple partitions, see :ref:`Section\_howto 4 <howto_4>`
of the manual.  World- and universe-style :doc:`variables <variable>`
are useful in this context.


.. parsed-literal::

   -plog file

Specify the base name for the partition log files, so partition N
writes log information to file.N. If file is none, then no partition
log files are created.  This overrides the filename specified in the
-log command-line option.  This option is useful when working with
large numbers of partitions, allowing the partition log files to be
suppressed (-plog none) or placed in a sub-directory (-plog
replica\_files/log.packfall) If this option is not used the log file for
partition N is log.packfall.N or whatever is specified by the -log
command-line option.


.. parsed-literal::

   -pscreen file

Specify the base name for the partition screen file, so partition N
writes screen information to file.N. If file is none, then no
partition screen files are created.  This overrides the filename
specified in the -screen command-line option.  This option is useful
when working with large numbers of partitions, allowing the partition
screen files to be suppressed (-pscreen none) or placed in a
sub-directory (-pscreen replica\_files/screen).  If this option is not
used the screen file for partition N is screen.N or whatever is
specified by the -screen command-line option.


.. parsed-literal::

   -restart restartfile datafile

Convert the restart file into a data file and immediately exit.  This
is the same operation as if the following 2-line input script were
run:


.. parsed-literal::

   read_restart restartfile
   write_data datafile

Note that the specified restartfile and datafile can have wild-card
characters ("\*",%") as described by the
:doc:`read\_restart <read_restart>` and :doc:`write\_data <write_data>`
commands.  But a filename such as file.\* will need to be enclosed in
quotes to avoid shell expansion of the "\*" character.


.. parsed-literal::

   -reorder nth N
   -reorder custom filename

Reorder the processors in the MPI communicator used to instantiate
Packfall, in one of several ways.  The original MPI communicator ranks
all P processors from 0 to P-1.  The mapping of these ranks to
physical processors is done by MPI before Packfall begins.  It may be
useful in some cases to alter the rank order.  E.g. to insure that
cores within each node are ranked in a desired order.  Or when using
the :doc:`run\_style verlet/split <run_style>` command with 2 partitions
to insure that a specific Kspace processor (in the 2nd partition) is
matched up with a specific set of processors in the 1st partition.
See the :doc:`Section\_accelerate <Section_accelerate>` doc pages for
more details.

If the keyword *nth* is used with a setting *N*\ , then it means every
Nth processor will be moved to the end of the ranking.  This is useful
when using the :doc:`run\_style verlet/split <run_style>` command with 2
partitions via the -partition command-line switch.  The first set of
processors will be in the first partition, the 2nd set in the 2nd
partition.  The -reorder command-line switch can alter this so that
the 1st N procs in the 1st partition and one proc in the 2nd partition
will be ordered consecutively, e.g. as the cores on one physical node.
This can boost performance.  For example, if you use "-reorder nth 4"
and "-partition 9 3" and you are running on 12 processors, the
processors will be reordered from


.. parsed-literal::

   0 1 2 3 4 5 6 7 8 9 10 11

to


.. parsed-literal::

   0 1 2 4 5 6 8 9 10 3 7 11

so that the processors in each partition will be


.. parsed-literal::

   0 1 2 4 5 6 8 9 10
   3 7 11

See the "processors" command for how to insure processors from each
partition could then be grouped optimally for quad-core nodes.

If the keyword is *custom*\ , then a file that specifies a permutation
of the processor ranks is also specified.  The format of the reorder
file is as follows.  Any number of initial blank or comment lines
(starting with a "#" character) can be present.  These should be
followed by P lines of the form:


.. parsed-literal::

   I J

where P is the number of processors Packfall was launched with.  Note
that if running in multi-partition mode (see the -partition switch
above) P is the total number of processors in all partitions.  The I
and J values describe a permutation of the P processors.  Every I and
J should be values from 0 to P-1 inclusive.  In the set of P I values,
every proc ID should appear exactly once.  Ditto for the set of P J
values.  A single I,J pairing means that the physical processor with
rank I in the original MPI communicator will have rank J in the
reordered communicator.

Note that rank ordering can also be specified by many MPI
implementations, either by environment variables that specify how to
order physical processors, or by config files that specify what
physical processors to assign to each MPI rank.  The -reorder switch
simply gives you a portable way to do this without relying on MPI
itself.  See the `processors out <processors>`_ command for how to output
info on the final assignment of physical processors to the Packfall
simulation domain.


.. parsed-literal::

   -screen file

Specify a file for Packfall to write its screen information to.  In
one-partition mode, if the switch is not used, Packfall writes to the
screen.  If this switch is used, Packfall writes to the specified file
instead and you will see no screen output.  In multi-partition mode,
if the switch is not used, hi-level status information is written to
the screen.  Each partition also writes to a screen.N file where N is
the partition ID.  If the switch is specified in multi-partition mode,
the hi-level screen dump is named "file" and each partition also
writes screen information to a file.N.  For both one-partition and
multi-partition mode, if the specified file is "none", then no screen
output is performed. Option -pscreen will override the name of the
partition screen files file.N.


.. parsed-literal::

   -var name value1 value2 ...

Specify a variable that will be defined for substitution purposes when
the input script is read.  "Name" is the variable name which can be a
single character (referenced as $x in the input script) or a full
string (referenced as ${abc}).  An :doc:`index-style variable <variable>` 
will be created and populated with the
subsequent values, e.g. a set of filenames.  Using this command-line
option is equivalent to putting the line "variable name index value1
value2 ..."  at the beginning of the input script.  Defining an index
variable as a command-line argument overrides any setting for the same
index variable in the input script, since index variables cannot be
re-defined.  See the :doc:`variable <variable>` command for more info on
defining index and other kinds of variables and :ref:`this section <cmd_2>` 
for more info on using variables
in input scripts.

.. note::

   Currently, the command-line parser looks for arguments that
   start with "-" to indicate new switches.  Thus you cannot specify
   multiple variable values if any of they start with a "-", e.g. a
   negative numeric value.  It is OK if the first value1 starts with a
   "-", since it is automatically skipped.


----------


.. _start\_8:

Packfall screen output
-----------------------------------------------

As Packfall reads an input script, it prints information to both the
screen and a log file about significant actions it takes to setup a
simulation.  When the simulation is ready to begin, Packfall performs
various initializations and prints the amount of memory (in MBytes per
processor) that the simulation requires.  It also prints details of
the initial thermodynamic state of the system.  During the run itself,
thermodynamic information is printed periodically, every few
timesteps.  When the run concludes, Packfall prints the final
thermodynamic state and a total run time for the simulation.  It then
appends statistics about the CPU time and storage requirements for the
simulation.  An example set of statistics is shown here:


.. parsed-literal::

   Loop time of 49.002 on 2 procs for 2004 atoms

   Pair   time (%) = 35.0495 (71.5267)
   Bond   time (%) = 0.092046 (0.187841)
   Kspce  time (%) = 6.42073 (13.103)
   Neigh  time (%) = 2.73485 (5.5811)
   Comm   time (%) = 1.50291 (3.06703)
   Outpt  time (%) = 0.013799 (0.0281601)
   Other  time (%) = 2.13669 (4.36041)

   Nlocal:    1002 ave, 1015 max, 989 min
   Histogram: 1 0 0 0 0 0 0 0 0 1
   Nghost:    8720 ave, 8724 max, 8716 min
   Histogram: 1 0 0 0 0 0 0 0 0 1
   Neighs:    354141 ave, 361422 max, 346860 min
   Histogram: 1 0 0 0 0 0 0 0 0 1

   Total # of neighbors = 708282
   Ave neighs/atom = 353.434
   Ave special neighs/atom = 2.34032
   Number of reneighborings = 42
   Dangerous reneighborings = 2

The first section gives the breakdown of the CPU run time (in seconds)
into major categories.  The second section lists the number of owned
atoms (Nlocal), ghost atoms (Nghost), and pair-wise neighbors stored
per processor.  The max and min values give the spread of these values
across processors with a 10-bin histogram showing the distribution.
The total number of histogram counts is equal to the number of
processors.

The last section gives aggregate statistics for pair-wise neighbors
and special neighbors that Packfall keeps track of (see the
:doc:`special\_bonds <special_bonds>` command).  The number of times
neighbor lists were rebuilt during the run is given as well as the
number of potentially "dangerous" rebuilds.  If atom movement
triggered neighbor list rebuilding (see the
:doc:`neigh\_modify <neigh_modify>` command), then dangerous
reneighborings are those that were triggered on the first timestep
atom movement was checked for.  If this count is non-zero you may wish
to reduce the delay factor to insure no force interactions are missed
by atoms moving beyond the neighbor skin distance before a rebuild
takes place.

The first line lists the initial and final energy, as well as the
energy on the next-to-last iteration.  The next 2 lines give a measure
of the gradient of the energy (force on all atoms).  The 2-norm is the
"length" of this force vector; the inf-norm is the largest component.
The last 2 lines are statistics on how many iterations and
force-evaluations the minimizer required.  Multiple force evaluations
are typically done at each iteration to perform a 1d line minimization
in the search direction.


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
