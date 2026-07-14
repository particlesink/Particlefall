Input Script
============

This section describes how a Packfall input script is formatted and the
input script commands used to define a Packfall simulation.


.. contents:: 
		:local:
		:depth: 1
   




.. _inp\_1:

Packfall input script
-------------------------------

Packfall executes by reading commands from a input script (text file),
one line at a time.  When the input script ends, Packfall exits.  Each
command causes Packfall to take some action.  It may set an internal
variable, read in a file, or run a simulation.  Most commands have
default settings, which means you only need to use the command if you
wish to change the default.

In many cases, the ordering of commands in an input script is not
important.  However the following rules apply:

(1) Packfall does not read your entire input script and then perform a
simulation with all the settings.  Rather, the input script is read
one line at a time and each command takes effect when it is read.
Thus this sequence of commands:


.. parsed-literal::

   timestep 0.5
   run      100
   run      100

does something different than this sequence:


.. parsed-literal::

   run      100
   timestep 0.5
   run      100

In the first case, the specified timestep (0.5 fmsec) is used for two
simulations of 100 timesteps each.  In the 2nd case, the default
timestep (1.0 fmsec) is used for the 1st 100 step simulation and a 0.5
fmsec timestep is used for the 2nd one.

(2) Some commands are only valid when they follow other commands.  For
example you cannot set the temperature of a group of atoms until atoms
have been defined and a group command is used to define which atoms
belong to the group.

(3) Sometimes command B will use values that can be set by command A.
This means command A must precede command B in the input script if it
is to have the desired effect.  For example, the
:doc:`read\_data <read_data>` command initializes the system by setting
up the simulation box and assigning atoms to processors.  If default
values are not desired, the :doc:`processors <processors>` and
:doc:`boundary <boundary>` commands need to be used before read\_data to
tell Packfall how to map processors to the simulation box.

Many input script errors are detected by Packfall and an ERROR or
WARNING message is printed.  :doc:`This section <Section_errors>` gives
more information on what errors mean.  The documentation for each
command lists restrictions on how the command can be used.


----------


.. _inp\_2:

Parsing rules
-------------

Each non-blank line in the input script is treated as a command.
Packfall commands are case sensitive.  Command names are lower-case, as
are specified command arguments.  Upper case letters may be used in
file names or user-chosen ID strings.

Here is how each line in the input script is parsed by Packfall:

(1) If the last printable character on the line is a "&" character
(with no surrounding quotes), the command is assumed to continue on
the next line.  The next line is concatenated to the previous line by
removing the "&" character and newline.  This allows long commands to
be continued across two or more lines.

(2) All characters from the first "#" character onward are treated as
comment and discarded.  See an exception in (6).  Note that a
comment after a trailing "&" character will prevent the command from
continuing on the next line.  Also note that for multi-line commands a
single leading "#" will comment out the entire command.

(3) The line is searched repeatedly for $ characters, which indicate
variables that are replaced with a text string.  See an exception in
(6).

If the $ is followed by curly brackets, then the variable name is the
text inside the curly brackets.  If no curly brackets follow the $,
then the variable name is the single character immediately following
the $.  Thus ${myTemp} and $x refer to variable names "myTemp" and
"x".

If the $ is followed by parenthesis, then the text inside the
parenthesis is treated as an "immediate" variable and evaluated as an
:doc:`equal-style variable <variable>`.  This is a way to use numeric
formulas in an input script without having to assign them to variable
names.  For example, these 3 input script lines:


.. parsed-literal::

   variable X equal (xlo+xhi)/2+sqrt(v_area)
   region 1 block $X 2 INF INF EDGE EDGE
   variable X delete

can be replaced by


.. parsed-literal::

   region 1 block $((xlo+xhi)/2+sqrt(v_area)) 2 INF INF EDGE EDGE

so that you do not have to define (or discard) a temporary variable X.

Note that neither the curly-bracket or immediate form of variables can
contain nested $ characters for other variables to substitute for.
Thus you cannot do this:


.. parsed-literal::

   variable        a equal 2
   variable        b2 equal 4
   print           "B2 = ${b$a}"

Nor can you specify this $($x-1.0) for an immediate variable, but
you could use $(v\_x-1.0), since the latter is valid syntax for an
:doc:`equal-style variable <variable>`.

See the :doc:`variable <variable>` command for more details of how
strings are assigned to variables and evaluated, and how they can be
used in input script commands.

(4) The line is broken into "words" separated by whitespace (tabs,
spaces).  Note that words can thus contain letters, digits,
underscores, or punctuation characters.

(5) The first word is the command name.  All successive words in the
line are arguments.

(6) If you want text with spaces to be treated as a single argument,
it can be enclosed in either double or single quotes.  E.g.


.. parsed-literal::

   print "Volume = $v"
   print 'Volume = $v'
   if "$\ *steps* > 1000" then quit

The quotes are removed when the single argument is stored internally.
See the :doc:`dump modify format <dump_modify>` or :doc:`print <print>` or
:doc:`if <if>` commands for examples.  A "#" or "$" character that is
between quotes will not be treated as a comment indicator in (2) or
substituted for as a variable in (3).

.. warning::

   If the argument is itself a command that requires a
   quoted argument (e.g. using a :doc:`print <print>` command as part of an
   :doc:`if <if>` or :doc:`run every <run>` command), then the double and
   single quotes can be nested in the usual manner.  See the doc pages
   for those commands for examples.  Only one of level of nesting is
   allowed, but that should be sufficient for most use cases.


----------


.. _inp\_3:

Input script structure
-----------------------------------

This section describes the structure of a typical Packfall input script.
The "examples" directory in the Packfall distribution contains many
sample input scripts.

A Packfall input script typically has 4 parts:

1. Initialization
2. Atom/particle definition and insertion
3. Settings
4. Run a simulation

The last 2 parts can be repeated as many times as desired.  I.e. run a
simulation, change some settings, run some more, etc.  Each of the 4
parts is now described in more detail.  Remember that almost all the
commands need only be used if a non-default value is desired.

(1) Initialization

Set parameters that need to be defined before atoms/particles are created or
read-in from a file.

The relevant commands are :doc:`units <units>`,
:doc:`dimension <dimension>`, :doc:`newton <newton>`,
:doc:`processors <processors>`, :doc:`boundary <boundary>`,
:doc:`atom\_style <atom_style>`, :doc:`atom\_modify <atom_modify>`.

If force-field parameters appear in the files that will be read, these
commands tell Packfall what kinds of force fields are being used:
:doc:`pair\_style <pair_style>`, :doc:`bond\_style <bond_style>`,
:doc:`fix wall/gran <fix_wall_gran>` .

Granular walls typically require meshes to be used. See
:doc:`fix mesh/surface <fix_mesh_surface>` for details.

(2) Atom/particle definition and insertion

There are 3 ways to define atoms in Packfall.  Read them in from a data
or restart file via the :doc:`read\_data <read_data>` or
:doc:`read\_restart <read_restart>` commands. Or create atoms on a lattice
using these commands: :doc:`lattice <lattice>`,
:doc:`region <region>`, :doc:`create\_box <create_box>`,
:doc:`create\_atoms <create_atoms>`.

However, the most common way to insert granular particles is
to use one of the fix insert/\* commands e.g.: :doc:`fix insert/pack <fix_insert_pack>`.

Before these insertion commands can be used, particle distributions
(`fix particledistribution/discrete <fix_particledistribution_discrete.hmtl>`_)
are built up using particle templates. For spherical particles, such
particle templates are defined using :doc:`fix particletemplate/sphere <fix_particletemplate_sphere>`.

(3) Settings

Once atoms are defined, a variety of settings can be specified:
simulation parameters, output options, etc.

Material parameters and force field coefficients are set by these commands
`fix property/global <fix_property_global.hmtl>`_,
or :doc:`pair\_coeff <pair_coeff>`, :doc:`bond\_coeff <bond_coeff>`.

Various simulation parameters are set by these commands:
:doc:`neighbor <neighbor>`, :doc:`neigh\_modify <neigh_modify>`,
:doc:`group <group>`, :doc:`timestep <timestep>`, :doc:`region <region>`,
:doc:`reset\_timestep <reset_timestep>`, :doc:`run\_style <run_style>`,

Fixes impose a variety of boundary conditions, time integration, and
diagnostic options.  The :doc:`fix <fix>` command comes in many flavors.

Various computations can be specified for execution during a
simulation using the :doc:`compute <compute>`,
:doc:`compute\_modify <compute_modify>`, and :doc:`variable <variable>`
commands.

Output options are set by the :doc:`thermo <thermo>`, :doc:`dump <dump>`,
and :doc:`restart <restart>` commands.

(4) Run a simulation

A simulation is run using the :doc:`run <run>` command.


----------


.. _cmd\_4:

An example input script
-----------------------

This shows an example input script for a Packfall simulation.


.. parsed-literal::

   #Contact model example
   atom_style   granular
   atom_modify  map array
   boundary     m m m
   newton       off
   communicate	 single vel yes
   units        si
   region       reg block -0.05 0.05 -0.05 0.05 0. 0.15 units box
   create_box   1 reg
   neighbor     0.002 bin
   neigh_modify delay 0
   #Material properties required for pair style
   fix          m1 all property/global youngsModulus peratomtype 5.e6
   fix          m2 all property/global poissonsRatio peratomtype 0.45
   fix          m3 all property/global coefficientRestitution peratomtypepair 1 0.95
   fix          m4 all property/global coefficientFriction peratomtypepair 1 0.05
   pair_style   gran model hertz tangential history
   pair_coeff	 \* \*
   timestep     0.00001
   fix          gravi all gravity 9.81 vector 0.0 0.0 -1.0
   fix          zwalls1 all wall/gran model hertz tangential history primitive type 1 zplane 0.0
   fix          zwalls2 all wall/gran model hertz tangential history primitive type 1 zplane 0.15
   fix          cylwalls all wall/gran model hertz tangential history primitive type 1 zcylinder 0.05 0. 0.
   #region of insertion
   region       bc cylinder z 0. 0. 0.045 0.00 0.15 units box
   #particle distributions
   fix          pts1 all particletemplate/sphere 12345787 1 density constant 2500 radius constant 0.0025
   fix          pdd1 all particledistribution/discrete 17903  1 pts1 1.0
   fix          ins all insert/pack seed 123457 distributiontemplate pdd1 vel constant 0. 0. -0.5 &
                insert_every once overlapcheck yes all_in yes particles_in_region 1800 region bc
   #apply nve integration to all particles
   fix          integr all nve/sphere
   #output settings, include total thermal energy
   compute         rke all erotate/sphere
   thermo_style    custom step atoms ke c_rke vol
   thermo          1000
   thermo_modify   lost ignore norm no
   compute_modify  thermo_temp dynamic yes
   dump            dmp all custom 800 post/dump\*.newModels id type x y z ix iy iz vx vy vz fx fy fz omegax omegay omegaz radius
   #insert particles and run
   run             5000


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
