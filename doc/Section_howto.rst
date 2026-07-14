How-to discussions
==================

This section describes how to perform common tasks using Packfall.


.. contents:: 
		:local:
		:depth: 1
   

.. _howto\_1:

Restarting a simulation
-----------------------

There are 3 ways to continue a long Packfall simulation.  Multiple
:doc:`run <run>` commands can be used in the same input script.  Each
run will continue from where the previous run left off.  Or binary
restart files can be saved to disk using the :doc:`restart <restart>`
command.  At a later time, these binary files can be read via a
:doc:`read\_restart <read_restart>` command in a new script.  Or they can
be converted to text data files using the :ref:`-r command-line switch <start_7>` and read by a
:doc:`read\_data <read_data>` command in a new script.

The :doc:`read\_restart <read_restart>` command typically replaces
the "create\_box"create\_box.html command in the input script that restarts
the simulation.

Note that while the internal state of the simulation is stored in the
restart file, most material properties and simulation settings are
not stored in the restart file. Be careful when changing settings and/or
material parameters when restarting a simulation.

The following commands do not need to be repeated because
their settings are included in the restart file: *units, atom\_style*.

As an alternate approach, the restart file could be converted to a data
file as follows:


.. parsed-literal::

   lmp_g++ -r tmp.restart.50 tmp.restart.data

Then, the "read\_data"read\_data.html command can be used to restart the
simulation.

:doc:`reset\_timestep <reset_timestep>` command can be used to tell
Packfall the current timestep.


----------


.. _howto\_2:

2d simulations
--------------

Use the :doc:`dimension <dimension>` command to specify a 2d simulation.

Make the simulation box periodic in z via the :doc:`boundary <boundary>`
command.  This is the default.

If using the :doc:`create box <create_box>` command to define a
simulation box, set the z dimensions narrow, but finite, so that the
create\_atoms command will tile the 3d simulation box with a single z
plane of atoms - e.g.


.. parsed-literal::

   :doc:`create box <create_box>` 1 -10 10 -10 10 -0.25 0.25

If using the :doc:`read data <read_data>` command to read in a file of
atom coordinates, set the "zlo zhi" values to be finite but narrow,
similar to the create\_box command settings just described.  For each
atom in the file, assign a z coordinate so it falls inside the
z-boundaries of the box - e.g. 0.0.

Use the :doc:`fix enforce2d <fix_enforce2d>` command as the last
defined fix to insure that the z-components of velocities and forces
are zeroed out every timestep.  The reason to make it the last fix is
so that any forces induced by other fixes will be zeroed out.

Many of the example input scripts included in the Packfall distribution
are for 2d models.

.. warning::

   Some models in Packfall treat particles as finite-size
   spheres, as opposed to point particles.  In 2d, the particles will
   still be spheres, not disks, meaning their moment of inertia will be
   the same as in 3d.


----------


.. _howto\_3:

Running multiple simulations from one input script
--------------------------------------------------

This can be done in several ways.  See the documentation for
individual commands for more details on how these examples work.

If "multiple simulations" means continue a previous simulation for
more timesteps, then you simply use the :doc:`run <run>` command
multiple times.  For example, this script


.. parsed-literal::

   units lj
   atom_style atomic
   read_data data.lj
   run 10000
   run 10000
   run 10000
   run 10000
   run 10000

would run 5 successive simulations of the same system for a total of
50,000 timesteps.

If you wish to run totally different simulations, one after the other,
the :doc:`clear <clear>` command can be used in between them to
re-initialize Packfall.  For example, this script


.. parsed-literal::

   units lj
   atom_style atomic
   read_data data.lj
   run 10000
   clear
   units lj
   atom_style atomic
   read_data data.lj.new
   run 10000

would run 2 independent simulations, one after the other.

For large numbers of independent simulations, you can use
:doc:`variables <variable>` and the :doc:`next <next>` and
:doc:`jump <jump>` commands to loop over the same input script
multiple times with different settings.  For example, this
script, named in.polymer


.. parsed-literal::

   variable d index run1 run2 run3 run4 run5 run6 run7 run8
   shell cd $d
   read_data data.polymer
   run 10000
   shell cd ..
   clear
   next d
   jump in.polymer

would run 8 simulations in different directories, using a data.polymer
file in each directory.  The same concept could be used to run the
same system at 8 different temperatures, using a temperature variable
and storing the output in different log and dump files, for example


.. parsed-literal::

   variable a loop 8
   variable t index 0.8 0.85 0.9 0.95 1.0 1.05 1.1 1.15
   log log.$a
   read data.polymer
   velocity all create $t 352839
   fix 1 all nvt $t $t 100.0
   dump 1 all atom 1000 dump.$a
   run 100000
   next t
   next a
   jump in.polymer

All of the above examples work whether you are running on 1 or
multiple processors, but assumed you are running Packfall on a single
partition of processors.  Packfall can be run on multiple partitions via
the "-partition" command-line switch as described in :ref:`this section <start_7>` of the manual.

In the last 2 examples, if Packfall were run on 3 partitions, the same
scripts could be used if the "index" and "loop" variables were
replaced with *universe*\ -style variables, as described in the
:doc:`variable <variable>` command.  Also, the "next t" and "next a"
commands would need to be replaced with a single "next a t" command.
With these modifications, the 8 simulations of each script would run
on the 3 partitions one after the other until all were finished.
Initially, 3 simulations would be started simultaneously, one on each
partition.  When one finished, that partition would then start
the 4th simulation, and so forth, until all 8 were completed.


----------


.. _howto\_4:

Granular models
---------------

Granular system are composed of spherical particles with a diameter,
as opposed to point particles.  This means they have an angular
velocity and torque can be imparted to them to cause them to rotate.

To run a simulation of a granular model, you will want to use
the following commands:

* :doc:`atom\_style sphere <atom_style>`
* :doc:`fix nve/sphere <fix_nve_sphere>`
* :doc:`fix gravity <fix_gravity>`

This compute

* :doc:`compute erotate/sphere <compute_erotate_sphere>`

calculates rotational kinetic energy which can be :ref:`output with thermodynamic info <howto_8>`.

Use a number of particle-particle and particle-wall contact models,
which compute forces and torques between interacting pairs of particles
and between particles and primitive walls or mesh elements. Details
can be found here:

* :doc:`pair\_style gran <pair_gran>`
* :doc:`List of all contact models <Section_gran_models>`

These commands implement fix options specific to granular systems:

* :doc:`fix freeze <fix_freeze>`
* :doc:`fix viscous <fix_viscous>`
* :doc:`fix wall/gran <fix_wall_gran>`

:doc:`fix wall/gran <fix_wall_gran>` commands can define two types of
walls: primitive and mesh. Mesh walls are defined using a
:doc:`fix mesh/surface <fix_mesh_surface>` or related command.

The fix style *freeze* zeroes both the force and torque of frozen
atoms, and should be used for granular system instead of the fix style
*setforce*\ .

For computational efficiency, you can eliminate needless pairwise
computations between frozen atoms by using this command:

* :doc:`neigh\_modify <neigh_modify>` exclude


----------


.. _howto\_5:

Coupling Packfall to other codes
------------------------------------------

Packfall is designed to allow it to be coupled to other codes.  For
example, a quantum mechanics code might compute forces on a subset of
atoms and pass those forces to Packfall.  Or a continuum finite element
(FE) simulation might use atom positions as boundary conditions on FE
nodal points, compute a FE solution, and return interpolated forces on
MD atoms.

Packfall can be coupled to other codes in at least 3 ways.  Each has
advantages and disadvantages, which you'll have to think about in the
context of your application.

(1) Define a new :doc:`fix <fix>` command that calls the other code.  In
this scenario, Packfall is the driver code.  During its timestepping,
the fix is invoked, and can make library calls to the other code,
which has been linked to Packfall as a library.  This is the way the
`POEMS <poems_>`_ package that performs constrained rigid-body motion on
groups of atoms is hooked to Packfall.  See the
:doc:`fix\_poems <fix_poems>` command for more details.  See :doc:`this section <Section_modify>` of the documentation for info on how to add
a new fix to Packfall.

.. _poems: http://www.rpi.edu/~anderk5/lab



(2) Define a new Packfall command that calls the other code.  This is
conceptually similar to method (1), but in this case Packfall and the
other code are on a more equal footing.  Note that now the other code
is not called during the timestepping of a Packfall run, but between
runs.  The Packfall input script can be used to alternate Packfall runs
with calls to the other code, invoked via the new command.  The
:doc:`run <run>` command facilitates this with its *every* option, which
makes it easy to run a few steps, invoke the command, run a few steps,
invoke the command, etc.

In this scenario, the other code can be called as a library, as in
(1), or it could be a stand-alone code, invoked by a system() call
made by the command (assuming your parallel machine allows one or more
processors to start up another program).  In the latter case the
stand-alone code could communicate with Packfall thru files that the
command writes and reads.

See :doc:`Section\_modify <Section_modify>` of the documentation for how
to add a new command to Packfall.

(3) Use Packfall as a library called by another code.  In this case the
other code is the driver and calls Packfall as needed.  Or a wrapper
code could link and call both Packfall and another code as libraries.
Again, the :doc:`run <run>` command has options that allow it to be
invoked with minimal overhead (no setup or clean-up) if you wish to do
multiple short runs, driven by another program.

Examples of driver codes that call Packfall as a library are included in
the examples/COUPLE directory of the Packfall distribution; see
examples/COUPLE/README for more details:

* simple: simple driver programs in C++ and C which invoke Packfall as a
  library
* lammps\_quest: coupling of Packfall and `Quest <quest_>`_, to run classical
  MD with quantum forces calculated by a density functional code
* lammps\_spparks: coupling of Packfall and `SPPARKS <spparks_>`_, to couple
  a kinetic Monte Carlo model for grain growth using MD to calculate
  strain induced across grain boundaries

.. _quest: http://dft.sandia.gov/Quest



.. _spparks: http://www.sandia.gov/~sjplimp/spparks.html



:ref:`This section <start_5>` of the documentation
describes how to build Packfall as a library.  Once this is done, you
can interface with Packfall either via C++, C, Fortran, or Python (or
any other language that supports a vanilla C-like interface).  For
example, from C++ you could create one (or more) "instances" of
Packfall, pass it an input script to process, or execute individual
commands, all by invoking the correct class methods in Packfall.  From C
or Fortran you can make function calls to do the same things.  See
:doc:`Section\_python <Section_python>` of the manual for a description
of the Python wrapper provided with Packfall that operates through the
Packfall library interface.

The files src/library.cpp and library.h contain the C-style interface
to Packfall.

Note that the lammps\_open() function that creates an instance of
Packfall takes an MPI communicator as an argument.  This means that
instance of Packfall will run on the set of processors in the
communicator.  Thus the calling code can run Packfall on all or a subset
of processors.  For example, a wrapper script might decide to
alternate between Packfall and another code, allowing them both to run
on all the processors.  Or it might allocate half the processors to
Packfall and half to the other code and run both codes simultaneously
before syncing them up periodically.  Or it might instantiate multiple
instances of Packfall to perform different calculations.


----------


.. _howto\_6:

Visualizing Packfall snapshots
----------------------------------------

Packfall itself does not do visualization, but snapshots from Packfall
simulations can be visualized (and analyzed) in a variety of ways.

Packfall snapshots are created by the :doc:`dump <dump>` command which can
create files in several formats.  The native Packfall dump format is a
text file (see "dump atom" or "dump custom").

A Python-based toolkit (LPP) distributed by our group can read native Packfall
dump files, including custom dump files with additional columns of
user-specified atom information, and convert them to VTK file formats
that can be read with Paraview.

Also, there exist a number of :doc:`dump <dump>` commands that output to
VTK natively.


----------


.. _howto\_7:

Triclinic (non-orthogonal) simulation boxes
-------------------------------------------

By default, Packfall uses an orthogonal simulation box to encompass the
particles.  The :doc:`boundary <boundary>` command sets the boundary
conditions of the box (periodic, non-periodic, etc).  The orthogonal
box has its "origin" at (xlo,ylo,zlo) and is defined by 3 edge vectors
starting from the origin given by **a** = (xhi-xlo,0,0); **b** =
(0,yhi-ylo,0); **c** = (0,0,zhi-zlo).  The 6 parameters
(xlo,xhi,ylo,yhi,zlo,zhi) are defined at the time the simulation box
is created, e.g. by the :doc:`create\_box <create_box>` or
:doc:`read\_data <read_data>` or :doc:`read\_restart <read_restart>`
commands.  Additionally, Packfall defines box size parameters lx,ly,lz
where lx = xhi-xlo, and similarly in the y and z dimensions.  The 6
parameters, as well as lx,ly,lz, can be output via the :doc:`thermo\_style custom <thermo_style>` command.

Packfall also allows simulations to be performed in triclinic
(non-orthogonal) simulation boxes shaped as a parallelepiped with
triclinic symmetry.  The parallelepiped has its "origin" at
(xlo,ylo,zlo) and is defined by 3 edge vectors starting from the
origin given by **a** = (xhi-xlo,0,0); **b** = (xy,yhi-ylo,0); **c** =
(xz,yz,zhi-zlo).  *xy,xz,yz* can be 0.0 or positive or negative values
and are called "tilt factors" because they are the amount of
displacement applied to faces of an originally orthogonal box to
transform it into the parallelepiped.  In Packfall the triclinic
simulation box edge vectors **a**\ , **b**\ , and **c** cannot be arbitrary
vectors.  As indicated, **a** must lie on the positive x axis.  **b** must
lie in the xy plane, with strictly positive y component. **c** may have
any orientation with strictly positive z component.  The requirement
that **a**\ , **b**\ , and **c** have strictly positive x, y, and z components,
respectively, ensures that **a**\ , **b**\ , and **c** form a complete
right-handed basis.  These restrictions impose no loss of generality,
since it is possible to rotate/invert any set of 3 crystal basis
vectors so that they conform to the restrictions.

For example, assume that the 3 vectors **A**\ ,\ **B**\ ,\ **C** are the edge
vectors of a general parallelepiped, where there is no restriction on
**A**\ ,\ **B**\ ,\ **C** other than they form a complete right-handed basis i.e.
**A** x **B** . **C** > 0.  The equivalent Packfall **a**\ ,\ **b**\ ,\ **c** are a linear
rotation of **A**\ , **B**\ , and **C** and can be computed as follows:

.. image:: Eqs/transform.jpg
   :align: center

where A = \|\ **A**\ \| indicates the scalar length of **A**\ . The \^ hat symbol
indicates the corresponding unit vector. *beta* and *gamma* are angles
between the vectors described below. Note that by construction,
**a**\ , **b**\ , and **c** have strictly positive x, y, and z components, respectively.
If it should happen that
**A**\ , **B**\ , and **C** form a left-handed basis, then the above equations
are not valid for **c**\ . In this case, it is necessary
to first apply an inversion. This can be achieved
by interchanging two basis vectors or by changing the sign of one of them.

For consistency, the same rotation/inversion applied to the basis vectors
must also be applied to atom positions, velocities,
and any other vector quantities.
This can be conveniently achieved by first converting to
fractional coordinates in the
old basis and then converting to distance coordinates in the new basis.
The transformation is given by the following equation:

.. image:: Eqs/rotate.jpg
   :align: center

where *V* is the volume of the box, **X** is the original vector quantity and
**x** is the vector in the Packfall basis.

There is no requirement that a triclinic box be periodic in any
dimension, though it typically should be in at least the 2nd dimension
of the tilt (y in xy) if you want to enforce a shift in periodic
boundary conditions across that boundary.  Some commands that work
with triclinic boxes, e.g. the :doc:`fix deform <fix_deform>` and :doc:`fix npt <fix_nh>` commands, require periodicity or non-shrink-wrap
boundary conditions in specific dimensions.  See the command doc pages
for details.

The 9 parameters (xlo,xhi,ylo,yhi,zlo,zhi,xy,xz,yz) are defined at the
time the simluation box is created.  This happens in one of 3 ways.
If the :doc:`create\_box <create_box>` command is used with a region of
style *prism*\ , then a triclinic box is setup.  See the
:doc:`region <region>` command for details.  If the
:doc:`read\_data <read_data>` command is used to define the simulation
box, and the header of the data file contains a line with the "xy xz
yz" keyword, then a triclinic box is setup.  See the
:doc:`read\_data <read_data>` command for details.  Finally, if the
:doc:`read\_restart <read_restart>` command reads a restart file which
was written from a simulation using a triclinic box, then a triclinic
box will be setup for the restarted simulation.

Note that you can define a triclinic box with all 3 tilt factors =
0.0, so that it is initially orthogonal.  This is necessary if the box
will become non-orthogonal, e.g. due to the :doc:`fix npt <fix_nh>` or
:doc:`fix deform <fix_deform>` commands.  Alternatively, you can use the
:doc:`change\_box <change_box>` command to convert a simulation box from
orthogonal to triclinic and vice versa.

As with orthogonal boxes, Packfall defines triclinic box size parameters
lx,ly,lz where lx = xhi-xlo, and similarly in the y and z dimensions.
The 9 parameters, as well as lx,ly,lz, can be output via the
:doc:`thermo\_style custom <thermo_style>` command.

To avoid extremely tilted boxes (which would be computationally
inefficient), Packfall normally requires that no tilt factor can skew
the box more than half the distance of the parallel box length, which
is the 1st dimension in the tilt factor (x for xz).  This is required
both when the simulation box is created, e.g. via the
:doc:`create\_box <create_box>` or :doc:`read\_data <read_data>` commands,
as well as when the box shape changes dynamically during a simulation,
e.g. via the :doc:`fix deform <fix_deform>` command.

For example, if xlo = 2 and xhi = 12, then the x box length is 10 and
the xy tilt factor must be between -5 and 5.  Similarly, both xz and
yz must be between -(xhi-xlo)/2 and +(yhi-ylo)/2.  Note that this is
not a limitation, since if the maximum tilt factor is 5 (as in this
example), then configurations with tilt = ..., -15, -5, 5, 15, 25,
... are geometrically all equivalent.  If the box tilt exceeds this
limit during a dynamics run (e.g. via the :doc:`fix deform <fix_deform>`
command), then the box is "flipped" to an equivalent shape with a tilt
factor within the bounds, so the run can continue.  See the :doc:`fix deform <fix_deform>` doc page for further details.

One exception to this rule is if the 1st dimension in the tilt
factor (x for xy) is non-periodic.  In that case, the limits on the
tilt factor are not enforced, since flipping the box in that dimension
does not change the atom positions due to non-periodicity.  In this
mode, if you tilt the system to extreme angles, the simulation will
simply become inefficient, due to the highly skewed simulation box.

The limitation on not creating a simulation box with a tilt factor
skewing the box more than half the distance of the parallel box length
can be overridden via the :doc:`box <box>` command.  Setting the *tilt*
keyword to *large* allows any tilt factors to be specified.

Note that if a simulation box has a large tilt factor, Packfall will run
less efficiently, due to the large volume of communication needed to
acquire ghost atoms around a processor's irregular-shaped sub-domain.
For extreme values of tilt, Packfall may also lose atoms and generate an
error.

Triclinic crystal structures are often defined using three lattice
constants *a*\ , *b*\ , and *c*\ , and three angles *alpha*\ , *beta* and
*gamma*\ . Note that in this nomenclature, the a, b, and c lattice
constants are the scalar lengths of the edge vectors **a**\ , **b**\ , and **c**
defined above.  The relationship between these 6 quantities
(a,b,c,alpha,beta,gamma) and the Packfall box sizes (lx,ly,lz) =
(xhi-xlo,yhi-ylo,zhi-zlo) and tilt factors (xy,xz,yz) is as follows:

.. image:: Eqs/box.jpg
   :align: center

The inverse relationship can be written as follows:

.. image:: Eqs/box_inverse.jpg
   :align: center

The values of *a*\ , *b*\ , *c* , *alpha*\ , *beta* , and *gamma* can be printed
out or accessed by computes using the
:doc:`thermo\_style custom <thermo_style>` keywords
*cella*\ , *cellb*\ , *cellc*\ , *cellalpha*\ , *cellbeta*\ , *cellgamma*\ ,
respectively.

As discussed on the :doc:`dump <dump>` command doc page, when the BOX
BOUNDS for a snapshot is written to a dump file for a triclinic box,
an orthogonal bounding box which encloses the triclinic simulation box
is output, along with the 3 tilt factors (xy, xz, yz) of the triclinic
box, formatted as follows:


.. parsed-literal::

   ITEM: BOX BOUNDS xy xz yz
   xlo_bound xhi_bound xy
   ylo_bound yhi_bound xz
   zlo_bound zhi_bound yz

This bounding box is convenient for many visualization programs and is
calculated from the 9 triclinic box parameters
(xlo,xhi,ylo,yhi,zlo,zhi,xy,xz,yz) as follows:


.. parsed-literal::

   xlo_bound = xlo + MIN(0.0,xy,xz,xy+xz)
   xhi_bound = xhi + MAX(0.0,xy,xz,xy+xz)
   ylo_bound = ylo + MIN(0.0,yz)
   yhi_bound = yhi + MAX(0.0,yz)
   zlo_bound = zlo
   zhi_bound = zhi

These formulas can be inverted if you need to convert the bounding box
back into the triclinic box parameters, e.g. xlo = xlo\_bound -
MIN(0.0,xy,xz,xy+xz).


----------


.. _howto\_8:

Output from Packfall (thermo, dumps, computes, fixes, variables)
--------------------------------------------------------------------------

There are four basic kinds of Packfall output:

* :doc:`Thermodynamic output <thermo_style>`, which is a list
  of quantities printed every few timesteps to the screen and logfile.
* :doc:`Dump files <dump>`, which contain snapshots of atoms and various
  per-atom values and are written at a specified frequency.
* Certain fixes can output user-specified quantities to files: :doc:`fix ave/time <fix_ave_time>` for time averaging, :doc:`fix ave/spatial <fix_ave_spatial>` for spatial averaging, and :doc:`fix print <fix_print>` for single-line output of
  :doc:`variables <variable>`.  Fix print can also output to the
  screen.
* :doc:`Restart files <restart>`.

A simulation prints one set of thermodynamic output and (optionally)
restart files.  It can generate any number of dump files and fix
output files, depending on what :doc:`dump <dump>` and :doc:`fix <fix>`
commands you specify.

As discussed below, Packfall gives you a variety of ways to determine
what quantities are computed and printed when the thermodynamics,
dump, or fix commands listed above perform output.  Throughout this
discussion, note that users can also :doc:`add their own computes and fixes to Packfall <Section_modify>` which can then generate values that can
then be output with these commands.

The following sub-sections discuss different Packfall command related
to output and the kind of data they operate on and produce:

* :ref:`Global/per-atom/local data <global>`
* :ref:`Scalar/vector/array data <scalar>`
* :ref:`Thermodynamic output <thermo>`
* :ref:`Dump file output <dump>`
* :ref:`Fixes that write output files <fixoutput1>`
* :ref:`Computes that process output quantities <computeoutput>`
* :ref:`Fixes that process output quantities <fixoutput2>`
* :ref:`Computes that generate values to output <compute>`
* :ref:`Fixes that generate values to output <fix>`
* :ref:`Variables that generate values to output <variable>`
* :ref:`Summary table of output options and data flow between commands <table>`

.. _global:

Global/per-atom/local data
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Various output-related commands work with three different styles of
data: global, per-atom, or local.  A global datum is one or more
system-wide values, e.g. the temperature of the system.  A per-atom
datum is one or more values per atom, e.g. the kinetic energy of each
atom.  Local datums are calculated by each processor based on the
atoms it owns, but there may be zero or more per atom, e.g. a list of
bond distances.

.. _scalar:

Scalar/vector/array data
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Global, per-atom, and local datums can each come in three kinds: a
single scalar value, a vector of values, or a 2d array of values.  The
doc page for a "compute" or "fix" or "variable" that generates data
will specify both the style and kind of data it produces, e.g. a
per-atom vector.

When a quantity is accessed, as in many of the output commands
discussed below, it can be referenced via the following bracket
notation, where ID in this case is the ID of a compute.  The leading
"c\_" would be replaced by "f\_" for a fix, or "v\_" for a variable:

+-------------+--------------------------------------------+
| c\_ID       | entire scalar, vector, or array            |
+-------------+--------------------------------------------+
| c\_ID[I]    | one element of vector, one column of array |
+-------------+--------------------------------------------+
| c\_ID[I][J] | one element of array                       |
+-------------+--------------------------------------------+

In other words, using one bracket reduces the dimension of the data
once (vector -> scalar, array -> vector).  Using two brackets reduces
the dimension twice (array -> scalar).  Thus a command that uses
scalar values as input can typically also process elements of a vector
or array.

.. _thermo:

Thermodynamic output
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The frequency and format of thermodynamic output is set by the
:doc:`thermo <thermo>`, :doc:`thermo\_style <thermo_style>`, and
:doc:`thermo\_modify <thermo_modify>` commands.  The
:doc:`thermo\_style <thermo_style>` command also specifies what values
are calculated and written out.  Pre-defined keywords can be specified
(e.g. press, etotal, etc).  Three additional kinds of keywords can
also be specified (c\_ID, f\_ID, v\_name), where a :doc:`compute <compute>`
or :doc:`fix <fix>` or :doc:`variable <variable>` provides the value to be
output.  In each case, the compute, fix, or variable must generate
global values for input to the :doc:`thermo\_style custom <dump>`
command.

Note that thermodynamic output values can be "extensive" or
"intensive".  The former scale with the number of atoms in the system
(e.g. total energy), the latter do not (e.g. temperature).  The
setting for :doc:`thermo\_modify norm <thermo_modify>` determines whether
extensive quantities are normalized or not.  Computes and fixes
produce either extensive or intensive values; see their individual doc
pages for details.  :doc:`Equal-style variables <variable>` produce only
intensive values; you can include a division by "natoms" in the
formula if desired, to make an extensive calculation produce an
intensive result.

.. _dump:

Dump file output
^^^^^^^^^^^^^^^^^^^^^^^^^^^

Dump file output is specified by the :doc:`dump <dump>` and
:doc:`dump\_modify <dump_modify>` commands.  There are several
pre-defined formats (dump atom, dump xtc, etc).

There is also a :doc:`dump custom <dump>` format where the user
specifies what values are output with each atom.  Pre-defined atom
attributes can be specified (id, x, fx, etc).  Three additional kinds
of keywords can also be specified (c\_ID, f\_ID, v\_name), where a
:doc:`compute <compute>` or :doc:`fix <fix>` or :doc:`variable <variable>`
provides the values to be output.  In each case, the compute, fix, or
variable must generate per-atom values for input to the :doc:`dump custom <dump>` command.

There is also a :doc:`dump local <dump>` format where the user specifies
what local values to output.  A pre-defined index keyword can be
specified to enumuerate the local values.  Two additional kinds of
keywords can also be specified (c\_ID, f\_ID), where a
:doc:`compute <compute>` or :doc:`fix <fix>` or :doc:`variable <variable>`
provides the values to be output.  In each case, the compute or fix
must generate local values for input to the :doc:`dump local <dump>`
command.

.. _fixoutput1:

Fixes that write output files
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Several fixes take various quantities as input and can write output
files: :doc:`fix ave/time <fix_ave_time>`, :doc:`fix ave/spatial <fix_ave_spatial>`, :doc:`fix ave/histo <fix_ave_histo>`,
:doc:`fix ave/correlate <fix_ave_correlate>`, and :doc:`fix print <fix_print>`.

The :doc:`fix ave/time <fix_ave_time>` command enables direct output to
a file and/or time-averaging of global scalars or vectors.  The user
specifies one or more quantities as input.  These can be global
:doc:`compute <compute>` values, global :doc:`fix <fix>` values, or
:doc:`variables <variable>` of any style except the atom style which
produces per-atom values.  Since a variable can refer to keywords used
by the :doc:`thermo\_style custom <thermo_style>` command (like temp or
press) and individual per-atom values, a wide variety of quantities
can be time averaged and/or output in this way.  If the inputs are one
or more scalar values, then the fix generate a global scalar or vector
of output.  If the inputs are one or more vector values, then the fix
generates a global vector or array of output.  The time-averaged
output of this fix can also be used as input to other output commands.

The :doc:`fix ave/spatial <fix_ave_spatial>` command enables direct
output to a file of spatial-averaged per-atom quantities like those
output in dump files, within 1d layers of the simulation box.  The
per-atom quantities can be atom density (mass or number) or atom
attributes such as position, velocity, force.  They can also be
per-atom quantities calculated by a :doc:`compute <compute>`, by a
:doc:`fix <fix>`, or by an atom-style :doc:`variable <variable>`.  The
spatial-averaged output of this fix can also be used as input to other
output commands.

The :doc:`fix ave/histo <fix_ave_histo>` command enables direct output
to a file of histogrammed quantities, which can be global or per-atom
or local quantities.  The histogram output of this fix can also be
used as input to other output commands.

The :doc:`fix ave/correlate <fix_ave_correlate>` command enables direct
output to a file of time-correlated quantities, which can be global
scalars.  The correlation matrix output of this fix can also be used
as input to other output commands.

The :doc:`fix print <fix_print>` command can generate a line of output
written to the screen and log file or to a separate file, periodically
during a running simulation.  The line can contain one or more
:doc:`variable <variable>` values for any style variable except the atom
style).  As explained above, variables themselves can contain
references to global values generated by :doc:`thermodynamic keywords <thermo_style>`, :doc:`computes <compute>`,
:doc:`fixes <fix>`, or other :doc:`variables <variable>`, or to per-atom
values for a specific atom.  Thus the :doc:`fix print <fix_print>`
command is a means to output a wide variety of quantities separate
from normal thermodynamic or dump file output.

.. _computeoutput:

Computes that process output quantities
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The :doc:`compute reduce <compute_reduce>` and :doc:`compute reduce/region <compute_reduce>` commands take one or more per-atom
or local vector quantities as inputs and "reduce" them (sum, min, max,
ave) to scalar quantities.  These are produced as output values which
can be used as input to other output commands.

The :doc:`compute slice <compute_slice>` command take one or more global
vector or array quantities as inputs and extracts a subset of their
values to create a new vector or array.  These are produced as output
values which can be used as input to other output commands.

The :doc:`compute property/atom <compute_property_atom>` command takes a
list of one or more pre-defined atom attributes (id, x, fx, etc) and
stores the values in a per-atom vector or array.  These are produced
as output values which can be used as input to other output commands.
The list of atom attributes is the same as for the :doc:`dump custom <dump>` command.

The :doc:`compute property/local <compute_property_local>` command takes
a list of one or more pre-defined local attributes (bond info, angle
info, etc) and stores the values in a local vector or array.  These
are produced as output values which can be used as input to other
output commands.

The :doc:`compute atom/molecule <compute_atom_molecule>` command takes a
list of one or more per-atom quantities (from a compute, fix, per-atom
variable) and sums the quantities on a per-molecule basis.  It
produces a global vector or array as output values which can be used
as input to other output commands.

.. _fixoutput2:

Fixes that process output quantities
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The :doc:`fix ave/atom <fix_ave_atom>` command performs time-averaging
of per-atom vectors.  The per-atom quantities can be atom attributes
such as position, velocity, force.  They can also be per-atom
quantities calculated by a :doc:`compute <compute>`, by a
:doc:`fix <fix>`, or by an atom-style :doc:`variable <variable>`.  The
time-averaged per-atom output of this fix can be used as input to
other output commands.

The :doc:`fix store/state <fix_store_state>` command can archive one or
more per-atom attributes at a particular time, so that the old values
can be used in a future calculation or output.  The list of atom
attributes is the same as for the :doc:`dump custom <dump>` command,
including per-atom quantities calculated by a :doc:`compute <compute>`,
by a :doc:`fix <fix>`, or by an atom-style :doc:`variable <variable>`.
The output of this fix can be used as input to other output commands.

.. _compute:

Computes that generate values to output
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Every :doc:`compute <compute>` in Packfall produces either global or
per-atom or local values.  The values can be scalars or vectors or
arrays of data.  These values can be output using the other commands
described in this section.  The doc page for each compute command
describes what it produces.  Computes that produce per-atom or local
values have the word "atom" or "local" in their style name.  Computes
without the word "atom" or "local" produce global values.

.. _fix:

Fixes that generate values to output
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Some :doc:`fixes <fix>` in Packfall produces either global or per-atom or
local values which can be accessed by other commands.  The values can
be scalars or vectors or arrays of data.  These values can be output
using the other commands described in this section.  The doc page for
each fix command tells whether it produces any output quantities and
describes them.

.. _variable:

Variables that generate values to output
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Every :doc:`variables <variable>` defined in an input script generates
either a global scalar value or a per-atom vector (only atom-style
variables) when it is accessed.  The formulas used to define equal-
and atom-style variables can contain references to the thermodynamic
keywords and to global and per-atom data generated by computes, fixes,
and other variables.  The values generated by variables can be output
using the other commands described in this section.

.. _table:

Summary table of output options and data flow between commands
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This table summarizes the various commands that can be used for
generating output from Packfall.  Each command produces output data of
some kind and/or writes data to a file.  Most of the commands can take
data from other commands as input.  Thus you can link many of these
commands together in pipeline form, where data produced by one command
is used as input to another command and eventually written to the
screen or to a file.  Note that to hook two commands together the
output and input data types must match, e.g. global/per-atom/local
data and scalar/vector/array data.

Also note that, as described above, when a command takes a scalar as
input, that could be an element of a vector or array.  Likewise a
vector input could be a column of an array.

+--------------------------------------------------------+-------------------------------------------+-------------------------------------------+--+
| Command                                                | Input                                     | Output                                    |  |
+--------------------------------------------------------+-------------------------------------------+-------------------------------------------+--+
| :doc:`thermo\_style custom <thermo_style>`             | global scalars                            | screen, log file                          |  |
+--------------------------------------------------------+-------------------------------------------+-------------------------------------------+--+
| :doc:`dump custom <dump>`                              | per-atom vectors                          | dump file                                 |  |
+--------------------------------------------------------+-------------------------------------------+-------------------------------------------+--+
| :doc:`dump local <dump>`                               | local vectors                             | dump file                                 |  |
+--------------------------------------------------------+-------------------------------------------+-------------------------------------------+--+
| :doc:`fix print <fix_print>`                           | global scalar from variable               | screen, file                              |  |
+--------------------------------------------------------+-------------------------------------------+-------------------------------------------+--+
| :doc:`print <print>`                                   | global scalar from variable               | screen                                    |  |
+--------------------------------------------------------+-------------------------------------------+-------------------------------------------+--+
| :doc:`computes <compute>`                              | N/A                                       | global/per-atom/local scalar/vector/array |  |
+--------------------------------------------------------+-------------------------------------------+-------------------------------------------+--+
| :doc:`fixes <fix>`                                     | N/A                                       | global/per-atom/local scalar/vector/array |  |
+--------------------------------------------------------+-------------------------------------------+-------------------------------------------+--+
| :doc:`variables <variable>`                            | global scalars, per-atom vectors          | global scalar, per-atom vector            |  |
+--------------------------------------------------------+-------------------------------------------+-------------------------------------------+--+
| :doc:`compute reduce <compute_reduce>`                 | per-atom/local vectors                    | global scalar/vector                      |  |
+--------------------------------------------------------+-------------------------------------------+-------------------------------------------+--+
| :doc:`compute slice <compute_slice>`                   | global vectors/arrays                     | global vector/array                       |  |
+--------------------------------------------------------+-------------------------------------------+-------------------------------------------+--+
| :doc:`compute property/atom <compute_property_atom>`   | per-atom vectors                          | per-atom vector/array                     |  |
+--------------------------------------------------------+-------------------------------------------+-------------------------------------------+--+
| :doc:`compute property/local <compute_property_local>` | local vectors                             | local vector/array                        |  |
+--------------------------------------------------------+-------------------------------------------+-------------------------------------------+--+
| :doc:`compute atom/molecule <compute_atom_molecule>`   | per-atom vectors                          | global vector/array                       |  |
+--------------------------------------------------------+-------------------------------------------+-------------------------------------------+--+
| :doc:`fix ave/atom <fix_ave_atom>`                     | per-atom vectors                          | per-atom vector/array                     |  |
+--------------------------------------------------------+-------------------------------------------+-------------------------------------------+--+
| :doc:`fix ave/time <fix_ave_time>`                     | global scalars/vectors                    | global scalar/vector/array, file          |  |
+--------------------------------------------------------+-------------------------------------------+-------------------------------------------+--+
| :doc:`fix ave/spatial <fix_ave_spatial>`               | per-atom vectors                          | global array, file                        |  |
+--------------------------------------------------------+-------------------------------------------+-------------------------------------------+--+
| :doc:`fix ave/histo <fix_ave_histo>`                   | global/per-atom/local scalars and vectors | global array, file                        |  |
+--------------------------------------------------------+-------------------------------------------+-------------------------------------------+--+
| :doc:`fix ave/correlate <fix_ave_correlate>`           | global scalars                            | global array, file                        |  |
+--------------------------------------------------------+-------------------------------------------+-------------------------------------------+--+
| :doc:`fix store/state <fix_store_state>`               | per-atom vectors                          | per-atom vector/array                     |  |
+--------------------------------------------------------+-------------------------------------------+-------------------------------------------+--+
|                                                        |                                           |                                           |  |
+--------------------------------------------------------+-------------------------------------------+-------------------------------------------+--+


----------


.. _howto\_9:

Walls
-----

Walls are typically used to bound particle motion,
i.e. to serve as a boundary condition.

Walls in Packfall for granular simulations are
typically defined using :doc:`fix wall/gran <fix_wall_gran>`.
This command can define two types of
walls: primitive and mesh. Mesh walls are defined using a
:doc:`fix mesh/surface <fix_mesh_surface>` or related command.

Also. walls can be constructed made of particles

Rough walls, built of particles, can be created in various ways.  The
particles themselves can be generated like any other particle, via the
:doc:`lattice <lattice>` and :doc:`create\_atoms <create_atoms>` commands,
or read in via the :doc:`read\_data <read_data>` command.

Their motion can be constrained by many different commands, so that
they do not move at all, move together as a group at constant velocity
or in response to a net force acting on them, move in a prescribed
fashion (e.g. rotate around a point), etc.  Note that if a time
integration fix like :doc:`fix nve <fix_nve>`
is not used with the group that contains wall particles, their
positions and velocities will not be updated.

* :doc:`fix aveforce <fix_aveforce>` - set force on particles to average value, so they move together
* :doc:`fix setforce <fix_setforce>` - set force on particles to a value, e.g. 0.0
* :doc:`fix freeze <fix_freeze>` - freeze particles for use as granular walls
* :doc:`fix nve/noforce <fix_nve_noforce>` - advect particles by their velocity, but without force
* :doc:`fix move <fix_move>` - prescribe motion of particles by a linear velocity, oscillation, rotation, variable

The :doc:`fix move <fix_move>` command offers the most generality, since
the motion of individual particles can be specified with
:doc:`variable <variable>` formula which depends on time and/or the
particle position.

For rough walls, it may be useful to turn off pairwise interactions
between wall particles via the :doc:`neigh\_modify exclude <neigh_modify>` command.

Rough walls can also be created by specifying frozen particles that do
not move and do not interact with mobile particles, and then tethering
other particles to the fixed particles, via a :doc:`bond <bond_style>`.
The bonded particles do interact with other mobile particles.

* :doc:`fix wall/reflect <fix_wall_reflect>` - reflective flat walls
* :doc:`fix wall/region <fix_wall_region>` - use region surface as wall

The :doc:`fix wall/region <fix_wall_region>` command offers the most
generality, since the region surface is treated as a wall, and the
geometry of the region can be a simple primitive volume (e.g. a
sphere, or cube, or plane), or a complex volume made from the union
and intersection of primitive volumes.  :doc:`Regions <region>` can also
specify a volume "interior" or "exterior" to the specified primitive
shape or *union* or *intersection*\ .  :doc:`Regions <region>` can also be
"dynamic" meaning they move with constant velocity, oscillate, or
rotate.


----------


.. _howto\_10:

Library interface to Packfall
---------------------------------------

As described in :ref:`Section\_start 5 <start_5>`, Packfall
can be built as a library, so that it can be called by another code,
used in a :ref:`coupled manner <howto_10>` with other
codes, or driven through a :doc:`Python interface <Section_python>`.

All of these methodologies use a C-style interface to Packfall that is
provided in the files src/library.cpp and src/library.h.  The
functions therein have a C-style argument list, but contain C++ code
you could write yourself in a C++ application that was invoking Packfall
directly.  The C++ code in the functions illustrates how to invoke
internal Packfall operations.  Note that Packfall classes are defined
within a Packfall namespace (LAMMPS\_NS) if you use them from another C++
application.

Library.cpp contains these 4 functions:


.. parsed-literal::

   void lammps_open(int, char \*\*, MPI_Comm, void \*\*);
   void lammps_close(void \*);
   void lammps_file(void \*, char \*);
   char \*lammps_command(void \*, char \*);

The lammps\_open() function is used to initialize Packfall, passing in a
list of strings as if they were :ref:`command-line arguments <start_7>` when Packfall is run in
stand-alone mode from the command line, and a MPI communicator for
Packfall to run under.  It returns a ptr to the Packfall object that is
created, and which is used in subsequent library calls.  The
lammps\_open() function can be called multiple times, to create
multiple instances of Packfall.

Packfall will run on the set of processors in the communicator.  This
means the calling code can run Packfall on all or a subset of
processors.  For example, a wrapper script might decide to alternate
between Packfall and another code, allowing them both to run on all the
processors.  Or it might allocate half the processors to Packfall and
half to the other code and run both codes simultaneously before
syncing them up periodically.  Or it might instantiate multiple
instances of Packfall to perform different calculations.

The lammps\_close() function is used to shut down an instance of Packfall
and free all its memory.

The lammps\_file() and lammps\_command() functions are used to pass a
file or string to Packfall as if it were an input script or single
command in an input script.  Thus the calling code can read or
generate a series of Packfall commands one line at a time and pass it
thru the library interface to setup a problem and then run it,
interleaving the lammps\_command() calls with other calls to extract
information from Packfall, perform its own operations, or call another
code's library.

Other useful functions are also included in library.cpp.  For example:


.. parsed-literal::

   void \*lammps_extract_global(void \*, char \*)
   void \*lammps_extract_atom(void \*, char \*)
   void \*lammps_extract_compute(void \*, char \*, int, int)
   void \*lammps_extract_fix(void \*, char \*, int, int, int, int)
   void \*lammps_extract_variable(void \*, char \*, char \*)
   int lammps_get_natoms(void \*)
   void lammps_get_coords(void \*, double \*)
   void lammps_put_coords(void \*, double \*)

These can extract various global or per-atom quantities from Packfall as
well as values calculated by a compute, fix, or variable.  The "get"
and "put" operations can retrieve and reset atom coordinates.
See the library.cpp file and its associated header file library.h for
details.

The key idea of the library interface is that you can write any
functions you wish to define how your code talks to Packfall and add
them to src/library.cpp and src/library.h, as well as to the :doc:`Python interface <Section_python>`.  The routines you add can access or
change any Packfall data you wish.  The examples/COUPLE and python
directories have example C++ and C and Python codes which show how a
driver code can link to Packfall as a library, run Packfall on a subset of
processors, grab data from Packfall, change it, and put it back into
Packfall.





.. _Berendsen:



**(Berendsen)** Berendsen, Grigera, Straatsma, J Phys Chem, 91,
6269-6271 (1987).

.. _Cornell:



**(Cornell)** Cornell, Cieplak, Bayly, Gould, Merz, Ferguson,
Spellmeyer, Fox, Caldwell, Kollman, JACS 117, 5179-5197 (1995).

.. _Horn:



**(Horn)** Horn, Swope, Pitera, Madura, Dick, Hura, and Head-Gordon,
J Chem Phys, 120, 9665 (2004).

.. _Ikeshoji:



**(Ikeshoji)** Ikeshoji and Hafskjold, Molecular Physics, 81, 251-261
(1994).

.. _MacKerell:



**(MacKerell)** MacKerell, Bashford, Bellott, Dunbrack, Evanseck, Field,
Fischer, Gao, Guo, Ha, et al, J Phys Chem, 102, 3586 (1998).

.. _Mayo:



**(Mayo)** Mayo, Olfason, Goddard III, J Phys Chem, 94, 8897-8909
(1990).

.. _Jorgensen:



**(Jorgensen)** Jorgensen, Chandrasekhar, Madura, Impey, Klein, J Chem
Phys, 79, 926 (1983).

.. _Price:



**(Price)** Price and Brooks, J Chem Phys, 121, 10096 (2004).

.. _Shinoda:



**(Shinoda)** Shinoda, Shiga, and Mikami, Phys Rev B, 69, 134103 (2004).


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
