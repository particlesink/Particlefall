Modifying & extending Packfall
========================================

This section describes how to customize Packfall by modifying
and extending its source code.


.. contents:: 
		:local:
		:depth: 1
   
Packfall is designed in a modular fashion so as to be easy to modify and
extend with new functionality.  In fact, about 75% of its source code
is files added in this fashion.

In this section, changes and additions users can make are listed along
with minimal instructions.  If you add a new feature to Packfall and
think it will be of interest to general users, we encourage you to
submit it to the developers for inclusion in the released version of
Packfall.  Information about how to do this is provided
:ref:`below <mod_14>`.

The best way to add a new feature is to find a similar feature in
Packfall and look at the corresponding source and header files to figure
out what it does.  You will need some knowledge of C++ to be able to
understand the hi-level structure of Packfall and its class
organization, but functions (class methods) that do actual
computations are written in vanilla C-style code and operate on simple
C-style data structures (vectors and arrays).

Most of the new features described in this section require you to
write a new C++ derived class (except for exceptions described below,
where you can make small edits to existing files).  Creating a new
class requires 2 files, a source code file (\*.cpp) and a header file
(\*.h).  The derived class must provide certain methods to work as a
new option.  Depending on how different your new feature is compared
to existing features, you can either derive from the base class
itself, or from a derived class that already exists.  Enabling Packfall
to invoke the new class is as simple as putting the two source
files in the src dir and re-building Packfall.

The advantage of C++ and its object-orientation is that all the code
and variables needed to define the new feature are in the 2 files you
write, and thus shouldn't make the rest of Packfall more complex or
cause side-effect bugs.

Here is a concrete example.  Suppose you write 2 files pair\_foo.cpp
and pair\_foo.h that define a new class PairFoo that computes pairwise
potentials described in the classic 1997 :ref:`paper <Foo>` by Foo, et al.
If you wish to invoke those potentials in a Packfall input script with a
command like


.. parsed-literal::

   pair_style foo 0.1 3.5

then your pair\_foo.h file should be structured as follows:


.. parsed-literal::

   #ifdef PAIR_CLASS
   PairStyle(foo,PairFoo)
   #else
   ...
   (class definition for PairFoo)
   ...
   #endif

where "foo" is the style keyword in the pair\_style command, and
PairFoo is the class name defined in your pair\_foo.cpp and pair\_foo.h
files.

When you re-build Packfall, your new pairwise potential becomes part of
the executable and can be invoked with a pair\_style command like the
example above.  Arguments like 0.1 and 3.5 can be defined and
processed by your new class.

As illustrated by this pairwise example, many kinds of options are
referred to in the Packfall documentation as the "style" of a particular
command.

The instructions below give the header file for the base class that
these styles are derived from.  Public variables in that file are ones
used and set by the derived classes which are also used by the base
class.  Sometimes they are also used by the rest of Packfall.  Virtual
functions in the base class header file which are set = 0 are ones you
must define in your new derived class to give it the functionality
Packfall expects.  Virtual functions that are not set to 0 are functions
you can optionally define.

Additionally, new output options can be added directly to the
thermo.cpp, dump\_custom.cpp, and variable.cpp files as explained
below.

Here are additional guidelines for modifying Packfall and adding new
functionality:

* Think about whether what you want to do would be better as a pre- or
  post-processing step.  Many computations are more easily and more
  quickly done that way.
* Don't do anything within the timestepping of a run that isn't
  parallel.  E.g. don't accumulate a bunch of data on a single processor
  and analyze it.  You run the risk of seriously degrading the parallel
  efficiency.
* If your new feature reads arguments or writes output, make sure you
  follow the unit conventions discussed by the :doc:`units <units>`
  command.
* If you add something you think is truly useful and doesn't impact
  Packfall performance when it isn't used, send an email to the
  `developers <http://lammps.sandia.gov/authors.html>`_.  We might be
  interested in adding it to the Packfall distribution.  See further
  details on this at the bottom of this page.





.. _mod\_1:

Atom styles
-----------

Classes that define an :doc:`atom style <atom_style>` are derived from
the AtomVec class and managed by the Atom class.  The atom style
determines what attributes are associated with an atom.  A new atom
style can be created if one of the existing atom styles does not
define all the attributes you need to store and communicate with
atoms.

Atom\_vec\_atomic.cpp is a simple example of an atom style.

Here is a brief description of methods you define in your new derived
class.  See atom\_vec.h for details.

+-------------------------+--------------------------------------------------------------------------------+
| init                    | one time setup (optional)                                                      |
+-------------------------+--------------------------------------------------------------------------------+
| grow                    | re-allocate atom arrays to longer lengths (required)                           |
+-------------------------+--------------------------------------------------------------------------------+
| grow\_reset             | make array pointers in Atom and AtomVec classes consistent (required)          |
+-------------------------+--------------------------------------------------------------------------------+
| copy                    | copy info for one atom to another atom's array locations (required)            |
+-------------------------+--------------------------------------------------------------------------------+
| pack\_comm              | store an atom's info in a buffer communicated every timestep (required)        |
+-------------------------+--------------------------------------------------------------------------------+
| pack\_comm\_vel         | add velocity info to communication buffer (required)                           |
+-------------------------+--------------------------------------------------------------------------------+
| pack\_comm\_hybrid      | store extra info unique to this atom style (optional)                          |
+-------------------------+--------------------------------------------------------------------------------+
| unpack\_comm            | retrieve an atom's info from the buffer (required)                             |
+-------------------------+--------------------------------------------------------------------------------+
| unpack\_comm\_vel       | also retrieve velocity info (required)                                         |
+-------------------------+--------------------------------------------------------------------------------+
| unpack\_comm\_hybrid    | retreive extra info unique to this atom style (optional)                       |
+-------------------------+--------------------------------------------------------------------------------+
| pack\_reverse           | store an atom's info in a buffer communicating partial forces  (required)      |
+-------------------------+--------------------------------------------------------------------------------+
| pack\_reverse\_hybrid   | store extra info unique to this atom style (optional)                          |
+-------------------------+--------------------------------------------------------------------------------+
| unpack\_reverse         | retrieve an atom's info from the buffer (required)                             |
+-------------------------+--------------------------------------------------------------------------------+
| unpack\_reverse\_hybrid | retreive extra info unique to this atom style (optional)                       |
+-------------------------+--------------------------------------------------------------------------------+
| pack\_border            | store an atom's info in a buffer communicated on neighbor re-builds (required) |
+-------------------------+--------------------------------------------------------------------------------+
| pack\_border\_vel       | add velocity info to buffer (required)                                         |
+-------------------------+--------------------------------------------------------------------------------+
| pack\_border\_hybrid    | store extra info unique to this atom style (optional)                          |
+-------------------------+--------------------------------------------------------------------------------+
| unpack\_border          | retrieve an atom's info from the buffer (required)                             |
+-------------------------+--------------------------------------------------------------------------------+
| unpack\_border\_vel     | also retrieve velocity info (required)                                         |
+-------------------------+--------------------------------------------------------------------------------+
| unpack\_border\_hybrid  | retreive extra info unique to this atom style (optional)                       |
+-------------------------+--------------------------------------------------------------------------------+
| pack\_exchange          | store all an atom's info to migrate to another processor (required)            |
+-------------------------+--------------------------------------------------------------------------------+
| unpack\_exchange        | retrieve an atom's info from the buffer (required)                             |
+-------------------------+--------------------------------------------------------------------------------+
| size\_restart           | number of restart quantities associated with proc's atoms (required)           |
+-------------------------+--------------------------------------------------------------------------------+
| pack\_restart           | pack atom quantities into a buffer (required)                                  |
+-------------------------+--------------------------------------------------------------------------------+
| unpack\_restart         | unpack atom quantities from a buffer (required)                                |
+-------------------------+--------------------------------------------------------------------------------+
| create\_atom            | create an individual atom of this style (required)                             |
+-------------------------+--------------------------------------------------------------------------------+
| data\_atom              | parse an atom line from the data file (required)                               |
+-------------------------+--------------------------------------------------------------------------------+
| data\_atom\_hybrid      | parse additional atom info unique to this atom style (optional)                |
+-------------------------+--------------------------------------------------------------------------------+
| data\_vel               | parse one line of velocity information from data file (optional)               |
+-------------------------+--------------------------------------------------------------------------------+
| data\_vel\_hybrid       | parse additional velocity data unique to this atom style (optional)            |
+-------------------------+--------------------------------------------------------------------------------+
| memory\_usage           | tally memory allocated by atom arrays (required)                               |
+-------------------------+--------------------------------------------------------------------------------+

The constructor of the derived class sets values for several variables
that you must set when defining a new atom style, which are documented
in atom\_vec.h.  New atom arrays are defined in atom.cpp.  Search for
the word "customize" and you will find locations you will need to
modify.

.. warning::

   It is possible to add some attributes, such as a
   molecule ID, to atom styles that do not have them via the :doc:`fix property/atom <fix_property_atom>` command.  This command also
   allows new custom attributes consisting of extra integer or
   floating-point values to be added to atoms.  See the :doc:`fix property/atom <fix_property_atom>` doc page for examples of cases
   where this is useful and details on how to initialize, access, and
   output the custom values.

New :doc:`pair styles <pair_style>`, :doc:`fixes <fix>`, or
:doc:`computes <compute>` can be added to Packfall, as discussed below.
The code for these classes can use the per-atom properties defined by
fix property/atom.  The Atom class has a find\_custom() method that is
useful in this context:


.. parsed-literal::

   int index = atom->find_custom(char \*name, int &flag);

The "name" of a custom attribute, as specified in the :doc:`fix property/atom <fix_property_atom>` command, is checked to verify
that it exists and its index is returned.  The method also sets flag =
0/1 depending on whether it is an integer or floating-point attribute.
The vector of values associated with the attribute can then be
accessed using the returned index as


.. parsed-literal::

   int \*ivector = atom->ivector[index];
   double \*dvector = atom->dvector[index];

Ivector or dvector are vectors of length Nlocal = # of owned atoms,
which store the attributes of individual atoms.


----------


.. _mod\_2:

Compute styles
--------------

Classes that compute scalar and vector quantities like temperature
and the pressure tensor, as well as classes that compute per-atom
quantities like kinetic energy and the centro-symmetry parameter
are derived from the Compute class.  New styles can be created
to add new calculations to Packfall.

Compute\_temp.cpp is a simple example of computing a scalar
temperature.  Compute\_ke\_atom.cpp is a simple example of computing
per-atom kinetic energy.

Here is a brief description of methods you define in your new derived
class.  See compute.h for details.

+--------------------+------------------------------------------------------------------+
| init               | perform one time setup (required)                                |
+--------------------+------------------------------------------------------------------+
| init\_list         | neighbor list setup, if needed (optional)                        |
+--------------------+------------------------------------------------------------------+
| compute\_scalar    | compute a scalar quantity (optional)                             |
+--------------------+------------------------------------------------------------------+
| compute\_vector    | compute a vector of quantities (optional)                        |
+--------------------+------------------------------------------------------------------+
| compute\_peratom   | compute one or more quantities per atom (optional)               |
+--------------------+------------------------------------------------------------------+
| compute\_local     | compute one or more quantities per processor (optional)          |
+--------------------+------------------------------------------------------------------+
| pack\_comm         | pack a buffer with items to communicate (optional)               |
+--------------------+------------------------------------------------------------------+
| unpack\_comm       | unpack the buffer (optional)                                     |
+--------------------+------------------------------------------------------------------+
| pack\_reverse      | pack a buffer with items to reverse communicate (optional)       |
+--------------------+------------------------------------------------------------------+
| unpack\_reverse    | unpack the buffer (optional)                                     |
+--------------------+------------------------------------------------------------------+
| remove\_bias       | remove velocity bias from one atom (optional)                    |
+--------------------+------------------------------------------------------------------+
| remove\_bias\_all  | remove velocity bias from all atoms in group (optional)          |
+--------------------+------------------------------------------------------------------+
| restore\_bias      | restore velocity bias for one atom after remove\_bias (optional) |
+--------------------+------------------------------------------------------------------+
| restore\_bias\_all | same as before, but for all atoms in group (optional)            |
+--------------------+------------------------------------------------------------------+
| memory\_usage      | tally memory usage (optional)                                    |
+--------------------+------------------------------------------------------------------+


----------


.. _mod\_3:

Dump styles
-----------

.. _mod\_4:

Dump custom output options
--------------------------

Classes that dump per-atom info to files are derived from the Dump
class.  To dump new quantities or in a new format, a new derived dump
class can be added, but it is typically simpler to modify the
DumpCustom class contained in the dump\_custom.cpp file.

Dump\_atom.cpp is a simple example of a derived dump class.

Here is a brief description of methods you define in your new derived
class.  See dump.h for details.

+---------------+---------------------------------------------------+
| write\_header | write the header section of a snapshot of atoms   |
+---------------+---------------------------------------------------+
| count         | count the number of lines a processor will output |
+---------------+---------------------------------------------------+
| pack          | pack a proc's output data into a buffer           |
+---------------+---------------------------------------------------+
| write\_data   | write a proc's data to a file                     |
+---------------+---------------------------------------------------+

See the :doc:`dump <dump>` command and its *custom* style for a list of
keywords for atom information that can already be dumped by
DumpCustom.  It includes options to dump per-atom info from Compute
classes, so adding a new derived Compute class is one way to calculate
new quantities to dump.

Alternatively, you can add new keywords to the dump custom command.
Search for the word "customize" in dump\_custom.cpp to see the
half-dozen or so locations where code will need to be added.


----------


.. _mod\_5:

Fix styles
----------

In Packfall, a "fix" is any operation that is computed during
timestepping that alters some property of the system.  Essentially
everything that happens during a simulation besides force computation,
neighbor list construction, and output, is a "fix".  This includes
time integration (update of coordinates and velocities), force
constraints or boundary conditions (SHAKE or walls), and diagnostics
(compute a diffusion coefficient).  New styles can be created to add
new options to Packfall.

Fix\_setforce.cpp is a simple example of setting forces on atoms to
prescribed values.  There are dozens of fix options already in Packfall;
choose one as a template that is similar to what you want to
implement.

Here is a brief description of methods you can define in your new
derived class.  See fix.h for details.

+---------------------------+-------------------------------------------------------------------------------------------+
| setmask                   | determines when the fix is called during the timestep (required)                          |
+---------------------------+-------------------------------------------------------------------------------------------+
| init                      | initialization before a run (optional)                                                    |
+---------------------------+-------------------------------------------------------------------------------------------+
| setup\_pre\_exchange      | called before atom exchange in setup (optional)                                           |
+---------------------------+-------------------------------------------------------------------------------------------+
| setup\_pre\_force         | called before force computation in setup (optional)                                       |
+---------------------------+-------------------------------------------------------------------------------------------+
| setup                     | called immediately before the 1st timestep and after forces are computed (optional)       |
+---------------------------+-------------------------------------------------------------------------------------------+
| min\_setup\_pre\_force    | like setup\_pre\_force, but for minimizations instead of MD runs (optional)               |
+---------------------------+-------------------------------------------------------------------------------------------+
| min\_setup                | like setup, but for minimizations instead of MD runs (optional)                           |
+---------------------------+-------------------------------------------------------------------------------------------+
| initial\_integrate        | called at very beginning of each timestep (optional)                                      |
+---------------------------+-------------------------------------------------------------------------------------------+
| pre\_exchange             | called before atom exchange on re-neighboring steps (optional)                            |
+---------------------------+-------------------------------------------------------------------------------------------+
| pre\_neighbor             | called before neighbor list build (optional)                                              |
+---------------------------+-------------------------------------------------------------------------------------------+
| pre\_force                | called after pair & molecular forces are computed (optional)                              |
+---------------------------+-------------------------------------------------------------------------------------------+
| post\_force               | called after pair & molecular forces are computed and communicated (optional)             |
+---------------------------+-------------------------------------------------------------------------------------------+
| final\_integrate          | called at end of each timestep (optional)                                                 |
+---------------------------+-------------------------------------------------------------------------------------------+
| end\_of\_step             | called at very end of timestep (optional)                                                 |
+---------------------------+-------------------------------------------------------------------------------------------+
| write\_restart            | dumps fix info to restart file (optional)                                                 |
+---------------------------+-------------------------------------------------------------------------------------------+
| restart                   | uses info from restart file to re-initialize the fix (optional)                           |
+---------------------------+-------------------------------------------------------------------------------------------+
| grow\_arrays              | allocate memory for atom-based arrays used by fix (optional)                              |
+---------------------------+-------------------------------------------------------------------------------------------+
| copy\_arrays              | copy atom info when an atom migrates to a new processor (optional)                        |
+---------------------------+-------------------------------------------------------------------------------------------+
| pack\_exchange            | store atom's data in a buffer (optional)                                                  |
+---------------------------+-------------------------------------------------------------------------------------------+
| unpack\_exchange          | retrieve atom's data from a buffer (optional)                                             |
+---------------------------+-------------------------------------------------------------------------------------------+
| pack\_restart             | store atom's data for writing to restart file (optional)                                  |
+---------------------------+-------------------------------------------------------------------------------------------+
| unpack\_restart           | retrieve atom's data from a restart file buffer (optional)                                |
+---------------------------+-------------------------------------------------------------------------------------------+
| size\_restart             | size of atom's data (optional)                                                            |
+---------------------------+-------------------------------------------------------------------------------------------+
| maxsize\_restart          | max size of atom's data (optional)                                                        |
+---------------------------+-------------------------------------------------------------------------------------------+
| setup\_pre\_force\_respa  | same as setup\_pre\_force, but for rRESPA (optional)                                      |
+---------------------------+-------------------------------------------------------------------------------------------+
| initial\_integrate\_respa | same as initial\_integrate, but for rRESPA (optional)                                     |
+---------------------------+-------------------------------------------------------------------------------------------+
| post\_integrate\_respa    | called after the first half integration step is done in rRESPA (optional)                 |
+---------------------------+-------------------------------------------------------------------------------------------+
| pre\_force\_respa         | same as pre\_force, but for rRESPA (optional)                                             |
+---------------------------+-------------------------------------------------------------------------------------------+
| post\_force\_respa        | same as post\_force, but for rRESPA (optional)                                            |
+---------------------------+-------------------------------------------------------------------------------------------+
| final\_integrate\_respa   | same as final\_integrate, but for rRESPA (optional)                                       |
+---------------------------+-------------------------------------------------------------------------------------------+
| min\_pre\_force           | called after pair & molecular forces are computed in minimizer (optional)                 |
+---------------------------+-------------------------------------------------------------------------------------------+
| min\_post\_force          | called after pair & molecular forces are computed and communicated in minmizer (optional) |
+---------------------------+-------------------------------------------------------------------------------------------+
| min\_store                | store extra data for linesearch based minimization on a LIFO stack (optional)             |
+---------------------------+-------------------------------------------------------------------------------------------+
| min\_pushstore            | push the minimization LIFO stack one element down (optional)                              |
+---------------------------+-------------------------------------------------------------------------------------------+
| min\_popstore             | pop the minimization LIFO stack one element up (optional)                                 |
+---------------------------+-------------------------------------------------------------------------------------------+
| min\_clearstore           | clear minimization LIFO stack (optional)                                                  |
+---------------------------+-------------------------------------------------------------------------------------------+
| min\_step                 | reset or move forward on line search minimization (optional)                              |
+---------------------------+-------------------------------------------------------------------------------------------+
| min\_dof                  | report number of degrees of freedom *added* by this fix in minimization (optional)        |
+---------------------------+-------------------------------------------------------------------------------------------+
| max\_alpha                | report maximum allowed step size during linesearch minimization (optional)                |
+---------------------------+-------------------------------------------------------------------------------------------+
| pack\_comm                | pack a buffer to communicate a per-atom quantity (optional)                               |
+---------------------------+-------------------------------------------------------------------------------------------+
| unpack\_comm              | unpack a buffer to communicate a per-atom quantity (optional)                             |
+---------------------------+-------------------------------------------------------------------------------------------+
| pack\_reverse\_comm       | pack a buffer to reverse communicate a per-atom quantity (optional)                       |
+---------------------------+-------------------------------------------------------------------------------------------+
| unpack\_reverse\_comm     | unpack a buffer to reverse communicate a per-atom quantity (optional)                     |
+---------------------------+-------------------------------------------------------------------------------------------+
| dof                       | report number of degrees of freedom *removed* by this fix during MD (optional)            |
+---------------------------+-------------------------------------------------------------------------------------------+
| compute\_scalar           | return a global scalar property that the fix computes (optional)                          |
+---------------------------+-------------------------------------------------------------------------------------------+
| compute\_vector           | return a component of a vector property that the fix computes (optional)                  |
+---------------------------+-------------------------------------------------------------------------------------------+
| compute\_array            | return a component of an array property that the fix computes (optional)                  |
+---------------------------+-------------------------------------------------------------------------------------------+
| deform                    | called when the box size is changed (optional)                                            |
+---------------------------+-------------------------------------------------------------------------------------------+
| reset\_target             | called when a change of the target temperature is requested during a run (optional)       |
+---------------------------+-------------------------------------------------------------------------------------------+
| reset\_dt                 | is called when a change of the time step is requested during a run (optional)             |
+---------------------------+-------------------------------------------------------------------------------------------+
| modify\_param             | called when a fix\_modify request is executed (optional)                                  |
+---------------------------+-------------------------------------------------------------------------------------------+
| memory\_usage             | report memory used by fix (optional)                                                      |
+---------------------------+-------------------------------------------------------------------------------------------+
| thermo                    | compute quantities for thermodynamic output (optional)                                    |
+---------------------------+-------------------------------------------------------------------------------------------+

Typically, only a small fraction of these methods are defined for a
particular fix.  Setmask is mandatory, as it determines when the fix
will be invoked during the timestep.  Fixes that perform time
integration (\ *nve*\ , *nvt*\ , *npt*\ ) implement initial\_integrate() and
final\_integrate() to perform velocity Verlet updates.  Fixes that
constrain forces implement post\_force().

Fixes that perform diagnostics typically implement end\_of\_step().  For
an end\_of\_step fix, one of your fix arguments must be the variable
"nevery" which is used to determine when to call the fix and you must
set this variable in the constructor of your fix.  By convention, this
is the first argument the fix defines (after the ID, group-ID, style).

If the fix needs to store information for each atom that persists from
timestep to timestep, it can manage that memory and migrate the info
with the atoms as they move from processors to processor by
implementing the grow\_arrays, copy\_arrays, pack\_exchange, and
unpack\_exchange methods.  Similarly, the pack\_restart and
unpack\_restart methods can be implemented to store information about
the fix in restart files.  If you wish an integrator or force
constraint fix to work with rRESPA (see the :doc:`run\_style <run_style>`
command), the initial\_integrate, post\_force\_integrate, and
final\_integrate\_respa methods can be implemented.  The thermo method
enables a fix to contribute values to thermodynamic output, as printed
quantities and/or to be summed to the potential energy of the system.


----------


.. _mod\_6:

Input script commands
---------------------

New commands can be added to Packfall input scripts by adding new
classes that have a "command" method.  For example, the create\_atoms,
read\_data, velocity, and run commands are all implemented in this
fashion.  When such a command is encountered in the Packfall input
script, Packfall simply creates a class with the corresponding name,
invokes the "command" method of the class, and passes it the arguments
from the input script.  The command method can perform whatever
operations it wishes on Packfall data structures.

The single method your new class must define is as follows:

+---------+-----------------------------------------+
| command | operations performed by the new command |
+---------+-----------------------------------------+

Of course, the new class can define other methods and variables as
needed.


----------


.. _mod\_7:

Pairwise potentials
-------------------

Classes that compute pairwise interactions are derived from the Pair
class.  In Packfall, pairwise calculation include manybody potentials
such as EAM or Tersoff where particles interact without a static bond
topology.  New styles can be created to add new pair potentials to
Packfall.

Here is a brief description of the class methods in pair.h:

+---------------------------------+-------------------------------------------------------------------+
| compute                         | workhorse routine that computes pairwise interactions             |
+---------------------------------+-------------------------------------------------------------------+
| settings                        | reads the input script line with arguments you define             |
+---------------------------------+-------------------------------------------------------------------+
| coeff                           | set coefficients for one i,j type pair                            |
+---------------------------------+-------------------------------------------------------------------+
| init\_one                       | perform initialization for one i,j type pair                      |
+---------------------------------+-------------------------------------------------------------------+
| init\_style                     | initialization specific to this pair style                        |
+---------------------------------+-------------------------------------------------------------------+
| write & read\_restart           | write/read i,j pair coeffs to restart files                       |
+---------------------------------+-------------------------------------------------------------------+
| write & read\_restart\_settings | write/read global settings to restart files                       |
+---------------------------------+-------------------------------------------------------------------+
| single                          | force and energy of a single pairwise interaction between 2 atoms |
+---------------------------------+-------------------------------------------------------------------+
| compute\_inner/middle/outer     | versions of compute used by rRESPA                                |
+---------------------------------+-------------------------------------------------------------------+

The inner/middle/outer routines are optional.


----------


.. _mod\_8:

Region styles
-------------

Classes that define geometric regions are derived from the Region
class.  Regions are used elsewhere in Packfall to group atoms, delete
atoms to create a void, insert atoms in a specified region, etc.  New
styles can be created to add new region shapes to Packfall.

Region\_sphere.cpp is an example of a spherical region.

Here is a brief description of methods you define in your new derived
class.  See region.h for details.

+-------+--------------------------------------------+
| match | determine whether a point is in the region |
+-------+--------------------------------------------+


----------


.. _mod\_9:

Thermodynamic output options
----------------------------

There is one class that computes and prints thermodynamic information
to the screen and log file; see the file thermo.cpp.

There are two styles defined in thermo.cpp: "one" and "multi".  There
is also a flexible "custom" style which allows the user to explicitly
list keywords for quantities to print when thermodynamic info is
output.  See the :doc:`thermo\_style <thermo_style>` command for a list
of defined quantities.

The thermo styles (one, multi, etc) are simply lists of keywords.
Adding a new style thus only requires defining a new list of keywords.
Search for the word "customize" with references to "thermo style" in
thermo.cpp to see the two locations where code will need to be added.

New keywords can also be added to thermo.cpp to compute new quantities
for output.  Search for the word "customize" with references to
"keyword" in thermo.cpp to see the several locations where code will
need to be added.

Note that the :doc:`thermo\_style custom <thermo>` command already allows
for thermo output of quantities calculated by :doc:`fixes <fix>`,
:doc:`computes <compute>`, and :doc:`variables <variable>`.  Thus, it may
be simpler to compute what you wish via one of those constructs, than
by adding a new keyword to the thermo command.


----------


.. _mod\_10:

Variable options
----------------

There is one class that computes and stores :doc:`variable <variable>`
information in Packfall; see the file variable.cpp.  The value
associated with a variable can be periodically printed to the screen
via the :doc:`print <print>`, :doc:`fix print <fix_print>`, or
:doc:`thermo\_style custom <thermo_style>` commands.  Variables of style
"equal" can compute complex equations that involve the following types
of arguments:

thermo keywords = ke, vol, atoms, ...
other variables = v\_a, v\_myvar, ...
math functions = div(x,y), mult(x,y), add(x,y), ...
group functions = mass(group), xcm(group,x), ...
atom values = x\ **123**\ , y\ **3**\ , vx\ **34**\ , ...
compute values = c\_mytemp\ **0**\ , c\_thermo\_press\ **3**\ , ...

Adding keywords for the :doc:`thermo\_style custom <thermo_style>` command
(which can then be accessed by variables) was discussed
:ref:`here <thermo>` on this page.

Adding a new math function of one or two arguments can be done by
editing one section of the Variable::evaulate() method.  Search for
the word "customize" to find the appropriate location.

Adding a new group function can be done by editing one section of the
Variable::evaulate() method.  Search for the word "customize" to find
the appropriate location.  You may need to add a new method to the
Group class as well (see the group.cpp file).

Accessing a new atom-based vector can be done by editing one section
of the Variable::evaulate() method.  Search for the word "customize"
to find the appropriate location.

Adding new :doc:`compute styles <compute>` (whose calculated values can
then be accessed by variables) was discussed
:ref:`here <compute>` on this page.





.. _mod\_11:
.. _mod_14:

Submitting new features for inclusion in Packfall
-----------------------------------------------------------

We encourage users to submit new features that they add to Packfall, especially if
you think the features will be of interest to other users.  If they
are broadly useful we may add them as core files to Packfall.

The previous sections of this doc page describe how to add new
features of various kinds to Packfall.  Packages are simply collections
of one or more new class files which are invoked as a new "style"
within a Packfall input script.  If designed correctly, these additions
typically do not require changes to the main core of Packfall; they are
simply add-on files.

Here is what you need to do to submit a user package or single file
for our consideration.  Following these steps will save time for both
you and us.  See existing package files for examples.

* All source files you provide must compile with the most current
  version of Packfall.
* If you want your file(s) to be added to main Packfall or one of its
  standard packages, then it needs to be written in a style compatible
  with other Packfall source files.  This is so the developers can
  understand it and hopefully maintain it.  This basically means that
  the code accesses data structures, performs its operations, and is
  formatted similar to other Packfall source files, including the use of
  the error class for error and warning messages.
* Your new source files need to have the Packfall copyright, GPL notice,
  and your name at the top, like other Packfall source files.  They need
  to create a class that is inside the Packfall namespace.  I.e. they do not need to be
  in the same stylistic format and syntax as other Packfall files, though
  that would be nice.
* Finally, you must also send a documentation file for each new command
  or style you are adding to Packfall.  This will be one file for a
  single-file feature.  For a package, it might be several files.  These
  are simple text files which we will convert to HTML.  They must be in
  the same format as other manual source files in the ``doc/`` directory for
  similar commands and styles.  The txt2html
  tool we use to do the conversion can be downloaded from `this site <http://www.sandia.gov/~sjplimp/download.html>`_, so you can perform
  the HTML conversion yourself to proofread your doc page.

Note that the more clear and self-explanatory you make your doc and
README files, the more likely it is that users will try out your new
feature.





.. _Foo:



**(Foo)** Foo, Morefoo, and Maxfoo, J of Classic Potentials, 75, 345 (1997).


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
