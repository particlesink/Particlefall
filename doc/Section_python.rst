Python interface to Packfall
======================================

This section describes how to build and use Packfall via a Python
interface.


.. contents:: 
		:local:
		:depth: 1
   
The Packfall distribution includes the file python/Packfall.py which wraps
the library interface to Packfall.  This file makes it is possible to
run Packfall, invoke Packfall commands or give it an input script, extract
Packfall results, an modify internal Packfall variables, either from a
Python script or interactively from a Python prompt.  You can do the
former in serial or parallel.  Running Python interactively in
parallel does not generally work, unless you have a package installed
that extends your Python to enable multiple instances of Python to
read what you type.

`Python <http://www.python.org>`_ is a powerful scripting and programming
language which can be used to wrap software like Packfall and other
packages.  It can be used to glue multiple pieces of software
together, e.g. to run a coupled or multiscale model.  See :ref:`Section section <howto_10>` of the manual and the couple
directory of the distribution for more ideas about coupling Packfall to
other codes.  See :ref:`Section\_start 4 <start_5>` about
how to build Packfall as a library, and :ref:`Section\_howto 19 <howto_10>` for a description of the library
interface provided in src/library.cpp and src/library.h and how to
extend it for your needs.  As described below, that interface is what
is exposed to Python.  It is designed to be easy to add functions to.
This can easily extend the Python inteface as well.  See details
below.

By using the Python interface, Packfall can also be coupled with a GUI
or other visualization tools that display graphs or animations in real
time as Packfall runs.  Examples of such scripts are inlcluded in the
python directory.

Two advantages of using Python are how concise the language is, and
that it can be run interactively, enabling rapid development and
debugging of programs.  If you use it to mostly invoke costly
operations within Packfall, such as running a simulation for a
reasonable number of timesteps, then the overhead cost of invoking
Packfall thru Python will be negligible.

Before using Packfall from a Python script, you need to do two things.
You need to build Packfall as a dynamic shared library, so it can be
loaded by Python.  And you need to tell Python how to find the library
and the Python wrapper file python/Packfall.py.  Both these steps are
discussed below.  If you wish to run Packfall in parallel from Python,
you also need to extend your Python with MPI.  This is also discussed
below.

The Python wrapper for Packfall uses the amazing and magical (to me)
"ctypes" package in Python, which auto-generates the interface code
needed between Python and a set of C interface routines for a library.
Ctypes is part of standard Python for versions 2.5 and later.  You can
check which version of Python you have installed, by simply typing
"python" at a shell prompt.





.. _py\_1:

Building Packfall as a shared library
-----------------------------------------------

Instructions on how to build Packfall as a shared library are given in
:ref:`Section\_start 5 <start_5>`.  A shared library is one
that is dynamically loadable, which is what Python requires.  On Linux
this is a library file that ends in ".so", not ".a".

From the src directory, type


.. parsed-literal::

   make -f Makefile.linux shlib

or on Windows/MSYS2:

.. parsed-literal::

   make -f Makefile.msys2 shlib

This creates the shared library under the corresponding build directory,
such as ``build/linux/libpackfall.so`` or ``build/msys2/libpackfall.dll``.
The Python wrapper loads ``libpackfall`` from the usual shared-library
search path.

If this fails, see :ref:`Section\_start 5 <start_5>` for
more details, especially if your Packfall build uses auxiliary libraries
like MPI or FFTW which may not be built as shared libraries on your
system.


----------


.. _py\_2:

Installing the Python wrapper into Python
-----------------------------------------

For Python to invoke Packfall, there are 2 files it needs to know about:

* python/Packfall.py
* src/libpackfall.so

Lammps.py is the Python wrapper on the Packfall library interface.
Libpackfall.so is the shared Packfall library that Python loads, as
described above.

You can insure Python can find these files in one of two ways:

* set two environment variables
* run the python/install.py script

If you set the paths to these files as environment variables, you only
have to do it once.  For the csh or tcsh shells, add something like
this to your ~/.cshrc file, one line for each of the two files:


.. parsed-literal::

   setenv PYTHONPATH $\ *PYTHONPATH*\ :/home/sjplimp/Packfall/python
   setenv LD_LIBRARY_PATH $*LD_LIBRARY_PATH*:/home/sjplimp/Packfall/src

If you use the python/install.py script, you need to invoke it every
time you rebuild Packfall (as a shared library) or make changes to the
python/Packfall.py file.

You can invoke install.py from the python directory as


.. parsed-literal::

   % python install.py [libdir] [pydir]

The optional libdir is where to copy the Packfall shared library to; the
default is /usr/local/lib.  The optional pydir is where to copy the
Packfall.py file to; the default is the site-packages directory of the
version of Python that is running the install script.

Note that libdir must be a location that is in your default
LD\_LIBRARY\_PATH, like /usr/local/lib or /usr/lib.  And pydir must be a
location that Python looks in by default for imported modules, like
its site-packages dir.  If you want to copy these files to
non-standard locations, such as within your own user space, you will
need to set your PYTHONPATH and LD\_LIBRARY\_PATH environment variables
accordingly, as above.

If the install.py script does not allow you to copy files into system
directories, prefix the python command with "sudo".  If you do this,
make sure that the Python that root runs is the same as the Python you
run.  E.g. you may need to do something like


.. parsed-literal::

   % sudo /usr/local/bin/python install.py [libdir] [pydir]

You can also invoke install.py from the make command in the src
directory as


.. parsed-literal::

   % make install-python

In this mode you cannot append optional arguments.  Again, you may
need to prefix this with "sudo".  In this mode you cannot control
which Python is invoked by root.

Note that if you want Python to be able to load different versions of
the Packfall shared library (see :ref:`this section <py_5>` below), you will
need to manually copy files like liblmp\_auto.so into the appropriate
system directory.  This is not needed if you set the LD\_LIBRARY\_PATH
environment variable as described above.


----------


.. _py\_3:

Extending Python with MPI to run in parallel
--------------------------------------------

If you wish to run Packfall in parallel from Python, you need to extend
your Python with an interface to MPI.  This also allows you to
make MPI calls directly from Python in your script, if you desire.

There are several Python packages available that purport to wrap MPI
as a library and allow MPI functions to be called from Python.

These include

* `pyMPI <http://pympi.sourceforge.net/>`_
* `maroonmpi <http://code.google.com/p/maroonmpi/>`_
* `mpi4py <http://code.google.com/p/mpi4py/>`_
* `myMPI <http://nbcr.sdsc.edu/forum/viewtopic.php?t=89&sid=c997fefc3933bd66204875b436940f16>`_
* `Pypar <http://code.google.com/p/pypar>`_

All of these except pyMPI work by wrapping the MPI library and
exposing (some portion of) its interface to your Python script.  This
means Python cannot be used interactively in parallel, since they do
not address the issue of interactive input to multiple instances of
Python running on different processors.  The one exception is pyMPI,
which alters the Python interpreter to address this issue, and (I
believe) creates a new alternate executable (in place of "python"
itself) as a result.

In principle any of these Python/MPI packages should work to invoke
Packfall in parallel and MPI calls themselves from a Python script which
is itself running in parallel.  However, when I downloaded and looked
at a few of them, their documentation was incomplete and I had trouble
with their installation.  It's not clear if some of the packages are
still being actively developed and supported.

The one I recommend, since I have successfully used it with Packfall, is
Pypar.  Pypar requires the ubiquitous `Numpy package <http://numpy.scipy.org>`_ be installed in your Python.  After
launching python, type


.. parsed-literal::

   import numpy

to see if it is installed.  If not, here is how to install it (version
1.3.0b1 as of April 2009).  Unpack the numpy tarball and from its
top-level directory, type


.. parsed-literal::

   python setup.py build
   sudo python setup.py install

The "sudo" is only needed if required to copy Numpy files into your
Python distribution's site-packages directory.

To install Pypar (version pypar-2.1.4\_94 as of Aug 2012), unpack it
and from its "source" directory, type


.. parsed-literal::

   python setup.py build
   sudo python setup.py install

Again, the "sudo" is only needed if required to copy Pypar files into
your Python distribution's site-packages directory.

If you have successully installed Pypar, you should be able to run
Python and type


.. parsed-literal::

   import pypar

without error.  You should also be able to run python in parallel
on a simple test script


.. parsed-literal::

   % mpirun -np 4 python test.py

where test.py contains the lines


.. parsed-literal::

   import pypar
   print "Proc %d out of %d procs" % (pypar.rank(),pypar.size())

and see one line of output for each processor you run on.

.. warning::

   To use Pypar and Packfall in parallel from Python, you
   must insure both are using the same version of MPI.  If you only have
   one MPI installed on your system, this is not an issue, but it can be
   if you have multiple MPIs.  Your Packfall build is explicit about which
   MPI it is using, since you choose whether to build with the MPI shim
   or a real MPI wrapper compiler in the active Packfall makefile.
   Pypar uses the "mpicc" command to find
   information about the MPI it uses to build against.  And it tries to
   load "libmpi.so" from the LD\_LIBRARY\_PATH.  This may or may not find
   the MPI library that Packfall is using.  If you have problems running
   both Pypar and Packfall together, this is an issue you may need to
   address, e.g. by moving other MPI installations so that Pypar finds
   the right one.


----------


.. _py\_4:

Testing the Python-Packfall interface
-----------------------------------------------

To test if Packfall is callable from Python, launch Python interactively
and type:


.. parsed-literal::

   >>> from Packfall import Packfall
   >>> lig = Packfall()

If you get no errors, you're ready to use Packfall from Python.  If the
2nd command fails, the most common error to see is


.. parsed-literal::

   OSError: Could not load Packfall dynamic library

which means Python was unable to load the Packfall shared library.  This
typically occurs if the system can't find the Packfall shared library or
one of the auxiliary shared libraries it depends on, or if something
about the library is incompatible with your Python.  The error message
should give you an indication of what went wrong.

You can also test the load directly in Python as follows, without
first importing from the Packfall.py file:


.. parsed-literal::

   >>> from ctypes import CDLL
   >>> CDLL("libpackfall.so")

If an error occurs, carefully go thru the steps in :ref:`Section\_start 5 <start_5>` and above about building a shared
library and about insuring Python can find the necessary two files
it needs.

**Test Packfall and Python in serial:**
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

To run a Packfall test in serial, type these lines into Python
interactively from the bench directory:


.. parsed-literal::

   >>> from Packfall import Packfall
   >>> lig = Packfall()
   >>> lig.file("in.lj")

Or put the same lines in the file test.py and run it as


.. parsed-literal::

   % python test.py

Either way, you should see the results of running the in.lj benchmark
on a single processor appear on the screen, the same as if you had
typed something like:


.. parsed-literal::

   lmp_auto < in.lj

**Test Packfall and Python in parallel:**
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

To run Packfall in parallel, assuming you have installed the
`Pypar <http://datamining.anu.edu.au/~ole/pypar>`_ package as discussed
above, create a test.py file containing these lines:


.. parsed-literal::

   import pypar
   from Packfall import Packfall
   lig = Packfall()
   lig.file("in.lj")
   print "Proc %d out of %d procs has" % (pypar.rank(),pypar.size()),lig
   pypar.finalize()

You can then run it in parallel as:


.. parsed-literal::

   % mpirun -np 4 python test.py

and you should see the same output as if you had typed


.. parsed-literal::

   % mpirun -np 4 lmp_auto < in.lj

Note that if you leave out the 3 lines from test.py that specify Pypar
commands you will instantiate and run Packfall independently on each of
the P processors specified in the mpirun command.  In this case you
should get 4 sets of output, each showing that a Packfall run was made
on a single processor, instead of one set of output showing that
Packfall ran on 4 processors.  If the 1-processor outputs occur, it
means that Pypar is not working correctly.

Also note that once you import the PyPar module, Pypar initializes MPI
for you, and you can use MPI calls directly in your Python script, as
described in the Pypar documentation.  The last line of your Python
script should be pypar.finalize(), to insure MPI is shut down
correctly.

**Running Python scripts:**
^^^^^^^^^^^^^^^^^^^^^^^^^^^

Note that any Python script (not just for Packfall) can be invoked in
one of several ways:


.. parsed-literal::

   % python foo.script
   % python -i foo.script
   % foo.script

The last command requires that the first line of the script be
something like this:


.. parsed-literal::

   #!/usr/local/bin/python
   #!/usr/local/bin/python -i

where the path points to where you have Python installed, and that you
have made the script file executable:


.. parsed-literal::

   % chmod +x foo.script

Without the "-i" flag, Python will exit when the script finishes.
With the "-i" flag, you will be left in the Python interpreter when
the script finishes, so you can type subsequent commands.  As
mentioned above, you can only run Python interactively when running
Python on a single processor, not in parallel.





.. _py\_5:

Using Packfall from Python
------------------------------------

The Python interface to Packfall consists of a Python "Packfall" module,
the source code for which is in python/Packfall.py, which creates a
"Packfall" object, with a set of methods that can be invoked on that
object.  The sample Python code below assumes you have first imported
the "Packfall" module in your Python script, as follows:


.. parsed-literal::

   from Packfall import Packfall

These are the methods defined by the Packfall module.  If you look
at the file src/library.cpp you will see that they correspond
one-to-one with calls you can make to the Packfall library from a C++ or
C or Fortran program.


.. parsed-literal::

   lig = Packfall()           # create a Packfall object using the default libpackfall.so library
   lig = Packfall("auto")     # create a Packfall object using the liblmp_auto.so library
   lig = Packfall("",list)    # ditto, with command-line args, e.g. list = ["-echo","screen"]
   lig = Packfall("auto",list)

   lig.close()              # destroy a Packfall object

   lig.file(file)           # run an entire input script, file = "in.lj"
   lig.command(cmd)         # invoke a single Packfall command, cmd = "run 100"

   xlo = lig.extract_global(name,type)  # extract a global quantity
                                        # name = "boxxlo", "nlocal", etc
   				     # type = 0 = int
   				     #        1 = double

   coords = lig.extract_atom(name,type)      # extract a per-atom quantity
                                             # name = "x", "type", etc
   				          # type = 0 = vector of ints
   				          #        1 = array of ints
   				          #        2 = vector of doubles
   				          #        3 = array of doubles

   eng = lig.extract_compute(id,style,type)  # extract value(s) from a compute
   v3 = lig.extract_fix(id,style,type,i,j)   # extract value(s) from a fix
                                             # id = ID of compute or fix
   					  # style = 0 = global data
   					  #	    1 = per-atom data
   					  #         2 = local data
   					  # type = 0 = scalar
   					  #	   1 = vector
   					  #        2 = array
   					  # i,j = indices of value in global vector or array

   var = lig.extract_variable(name,group,flag)  # extract value(s) from a variable
   	                                     # name = name of variable
   					     # group = group ID (ignored for equal-style variables)
   					     # flag = 0 = equal-style variable
   					     #        1 = atom-style variable

   natoms = lig.get_natoms()                 # total # of atoms as int
   data = lig.gather_atoms(name,type,count)  # return atom attribute of all atoms gathered into data, ordered by atom ID
                                             # name = "x", "charge", "type", etc
                                             # count = # of per-atom values, 1 or 3, etc
   lig.scatter_atoms(name,type,count,data)   # scatter atom attribute of all atoms from data, ordered by atom ID
                                             # name = "x", "charge", "type", etc
                                             # count = # of per-atom values, 1 or 3, etc


----------


.. warning::

   Currently, the creation of a Packfall object from within
   Packfall.py does not take an MPI communicator as an argument.  There
   should be a way to do this, so that the Packfall instance runs on a
   subset of processors if desired, but I don't know how to do it from
   Pypar.  So for now, it runs with MPI\_COMM\_WORLD, which is all the
   processors.  If someone figures out how to do this with one or more of
   the Python wrappers for MPI, like Pypar, please let us know and we
   will amend these doc pages.

Note that you can create multiple Packfall objects in your Python
script, and coordinate and run multiple simulations, e.g.


.. parsed-literal::

   from Packfall import Packfall
   lig1 = Packfall()
   lig2 = Packfall()
   lig1.file("in.file1")
   lig2.file("in.file2")

The file() and command() methods allow an input script or single
commands to be invoked.

The extract\_global(), extract\_atom(), extract\_compute(),
extract\_fix(), and extract\_variable() methods return values or
pointers to data structures internal to Packfall.

For extract\_global() see the src/library.cpp file for the list of
valid names.  New names could easily be added.  A double or integer is
returned.  You need to specify the appropriate data type via the type
argument.

For extract\_atom(), a pointer to internal Packfall atom-based data is
returned, which you can use via normal Python subscripting.  See the
extract() method in the src/atom.cpp file for a list of valid names.
Again, new names could easily be added.  A pointer to a vector of
doubles or integers, or a pointer to an array of doubles (double \*\*)
or integers (int \*\*) is returned.  You need to specify the appropriate
data type via the type argument.

For extract\_compute() and extract\_fix(), the global, per-atom, or
local data calulated by the compute or fix can be accessed.  What is
returned depends on whether the compute or fix calculates a scalar or
vector or array.  For a scalar, a single double value is returned.  If
the compute or fix calculates a vector or array, a pointer to the
internal Packfall data is returned, which you can use via normal Python
subscripting.  The one exception is that for a fix that calculates a
global vector or array, a single double value from the vector or array
is returned, indexed by I (vector) or I and J (array).  I,J are
zero-based indices.  The I,J arguments can be left out if not needed.
See :ref:`Section\_howto 15 <howto_8>` of the manual for a
discussion of global, per-atom, and local data, and of scalar, vector,
and array data types.  See the doc pages for individual
:doc:`computes <compute>` and :doc:`fixes <fix>` for a description of what
they calculate and store.

For extract\_variable(), an :doc:`equal-style or atom-style variable <variable>` is evaluated and its result returned.

For equal-style variables a single double value is returned and the
group argument is ignored.  For atom-style variables, a vector of
doubles is returned, one value per atom, which you can use via normal
Python subscripting. The values will be zero for atoms not in the
specified group.

The get\_natoms() method returns the total number of atoms in the
simulation, as an int.

The gather\_atoms() method returns a ctypes vector of ints or doubles
as specified by type, of length count\*natoms, for the property of all
the atoms in the simulation specified by name, ordered by count and
then by atom ID.  The vector can be used via normal Python
subscripting.  If atom IDs are not consecutively ordered within
Packfall, a None is returned as indication of an error.

Note that the data structure gather\_atoms("x") returns is different
from the data structure returned by extract\_atom("x") in four ways.
(1) Gather\_atoms() returns a vector which you index as x[i];
extract\_atom() returns an array which you index as x[i][j].  (2)
Gather\_atoms() orders the atoms by atom ID while extract\_atom() does
not.  (3) Gathert\_atoms() returns a list of all atoms in the
simulation; extract\_atoms() returns just the atoms local to each
processor.  (4) Finally, the gather\_atoms() data structure is a copy
of the atom coords stored internally in Packfall, whereas extract\_atom()
returns an array that effectively points directly to the internal
data.  This means you can change values inside Packfall from Python by
assigning a new values to the extract\_atom() array.  To do this with
the gather\_atoms() vector, you need to change values in the vector,
then invoke the scatter\_atoms() method.

The scatter\_atoms() method takes a vector of ints or doubles as
specified by type, of length count\*natoms, for the property of all the
atoms in the simulation specified by name, ordered by bount and then
by atom ID.  It uses the vector of data to overwrite the corresponding
properties for each atom inside Packfall.  This requires Packfall to have
its "map" option enabled; see the :doc:`atom\_modify <atom_modify>`
command for details.  If it is not, or if atom IDs are not
consecutively ordered, no coordinates are reset.

The array of coordinates passed to scatter\_atoms() must be a ctypes
vector of ints or doubles, allocated and initialized something like
this:


.. parsed-literal::

   from ctypes import \*
   natoms = lig.get_natoms()
   n3 = 3\*natoms
   x = (n3\*c_double)()
   x\ **0** = x coord of atom with ID 1
   x\ **1** = y coord of atom with ID 1
   x\ **2** = z coord of atom with ID 1
   x\ **3** = x coord of atom with ID 2
   ...
   x\ **n3-1** = z coord of atom with ID natoms
   lig.scatter_coords("x",1,3,x)

Alternatively, you can just change values in the vector returned by
gather\_atoms("x",1,3), since it is a ctypes vector of doubles.


----------


As noted above, these Python class methods correspond one-to-one with
the functions in the Packfall library interface in src/library.cpp and
library.h.  This means you can extend the Python wrapper via the
following steps:

* Add a new interface function to src/library.cpp and
  src/library.h.
* Rebuild Packfall as a shared library.
* Add a wrapper method to python/Packfall.py for this interface
  function.
* You should now be able to invoke the new interface function from a
  Python script.  Isn't ctypes amazing?





.. _py\_6:

Example Python scripts that use Packfall
--------------------------------------------------

These are the Python scripts included as demos in the python/examples
directory of the Packfall distribution, to illustrate the kinds of
things that are possible when Python wraps Packfall.  If you create your
own scripts, send them to us and we can include them in the Packfall
distribution.

+---------------------+--------------------------------------------------------------+
| trivial.py          | read/run a Packfall input script thru Python                 |
+---------------------+--------------------------------------------------------------+
| demo.py             | invoke various Packfall library interface routines           |
+---------------------+--------------------------------------------------------------+
| simple.py           | mimic operation of couple/simple/simple.cpp in Python        |
+---------------------+--------------------------------------------------------------+
| gui.py              | GUI go/stop/temperature-slider to control Packfall           |
+---------------------+--------------------------------------------------------------+
| plot.py             | real-time temeperature plot with GnuPlot via Pizza.py        |
+---------------------+--------------------------------------------------------------+
| viz\_tool.py        | real-time viz via some viz package                           |
+---------------------+--------------------------------------------------------------+
| vizplotgui\_tool.py | combination of viz\_tool.py and plot.py and gui.py           |
+---------------------+--------------------------------------------------------------+


----------


For the viz\_tool.py and vizplotgui\_tool.py commands, replace "tool"
with "gl" or "atomeye" or "pymol" or "vmd", depending on what
visualization package you have installed.

Note that for GL, you need to be able to run the Pizza.py GL tool,
which is included in the pizza sub-directory.  See the `Pizza.py doc pages <pizza_>`_ for more info:

.. _pizza: http://www.sandia.gov/~sjplimp/pizza.html



Note that for AtomEye, you need version 3, and there is a line in the
scripts that specifies the path and name of the executable.  See the
AtomEye WWW pages `here <atomeye_>`_ or `here <atomeye3_>`_ for more details:


.. parsed-literal::

   http://mt.seas.upenn.edu/Archive/Graphics/A
   http://mt.seas.upenn.edu/Archive/Graphics/A3/A3.html

.. _atomeye: http://mt.seas.upenn.edu/Archive/Graphics/A



.. _atomeye3: http://mt.seas.upenn.edu/Archive/Graphics/A3/A3.html



The latter link is to AtomEye 3 which has the scriping
capability needed by these Python scripts.

Note that for PyMol, you need to have built and installed the
open-source version of PyMol in your Python, so that you can import it
from a Python script.  See the PyMol WWW pages `here <pymol_>`_ or
`here <pymolopen_>`_ for more details:


.. parsed-literal::

   http://www.pymol.org
   http://sourceforge.net/scm/?type=svn&group_id=4546

.. _pymol: http://www.pymol.org



.. _pymolopen: http://sourceforge.net/scm/?type=svn&group\_id=4546



The latter link is to the open-source version.


----------


See the python/README file for instructions on how to run them and the
source code for individual scripts for comments about what they do.

.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
