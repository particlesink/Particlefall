.. index:: read\_dump

read\_dump command
==================

Syntax
""""""


.. parsed-literal::

   read_dump file Nstep field1 field2 ... keyword values ...

* file = name of dump file to read
* Nstep = snapshot timestep to read from file
* one or more fields may be appended
  
  .. parsed-literal::
  
     field = *x* or *y* or *z* or *vx* or *vy* or *vz* or *q* or *ix* or *iy* or *iz*
       *x*\ ,\ *y*\ ,\ *z* = atom coordinates
       *radius* = atom radii
       *vx*\ ,\ *vy*\ ,\ *vz* = velocity components
       *q* = charge
       *ix*\ ,\ *iy*\ ,\ *iz* = image flags in each dimension

* zero or more keyword/value pairs may be appended
* keyword = *box* or *replace* or *purge* or *trim* or *add* or *label* or *scaled* or *wrapped* or *format*
  
  .. parsed-literal::
  
       *box* value = *yes* or *no* = replace simulation box with dump box
       *replace* value = *yes* or *no* = overwrite atoms with dump atoms
       *purge* value = *yes* or *no* = delete all atoms before adding dump atoms
       *trim* value = *yes* or *no* = trim atoms not in dump snapshot
       *add* value = *yes* or *no* = add new dump atoms to system
       *label* value = field column
         field = one of the listed fields or *id* or *type*
         column = label on corresponding column in dump file
       *scaled* value = *yes* or *no* = coords in dump file are scaled/unscaled
       *wrapped* value = *yes* or *no* = coords in dump file are wrapped/unwrapped
       *format* values = format of dump file, must be last keyword if used
         *native* = native LIGGGHTS(R)-PUBLIC dump file
         *xyz* = XYZ file



Examples
""""""""


.. parsed-literal::

   read_dump dump.file 5000 x y z radius
   read_dump dump.xyz 5 x y z radius format xyz box no
   read_dump dump.file 5000 x y vx vy radius trim yes
   read_dump ../run7/dump.file.gz 10000 x y z radius box yes
   read_dump dump.xyz 5 x y z radius box no format xyz

Description
"""""""""""

Read atom information from a dump file to overwrite the current atom
coordinates, and optionally the atom velocities and image flags and
the simluation box dimensions.  This is useful for restarting a run
from a particular snapshot in a dump file.  See the
:doc:`read\_restart <read_restart>` and :doc:`read\_data <read_data>`
commands for alternative methods to do this.  Also see the
:doc:`rerun <rerun>` command for a means of reading multiple snapshots
from a dump file.

Note that a simulation box must already be defined before using the
read\_dump command.  This can be done by the
:doc:`create\_box <create_box>`, :doc:`read\_data <read_data>`, or
:doc:`read\_restart <read_restart>` commands.  The read\_dump command can
reset the simulation box dimensions, as explained below.

Since LIGGGHTS(R)-PUBLIC needs valid radii, the radius field is mandatory
to be read.

Also note that reading per-atom information from a dump snapshot is
limited to the atom coordinates, radii, velocities and image flags, as
explained below.  Other atom properties, which may be necessary to run
a valid simulation, such as atom charge, or bond topology information
for a molecular system, are not read from (or even contained in) dump
files.  Thus this auxiliary information should be defined in the usual
way, e.g. in a data file read in by a :doc:`read\_data <read_data>`
command, before using the read\_dump command, or by the :doc:`set <set>`
command, after the dump snapshot is read.


----------


If the dump filename specified as *file* ends with ".gz", the dump
file is read in gzipped format.  You cannot (yet) read a dump file
that was written in binary format with a ".bin" suffix, or to multiple
files via the "%" option in the dump file name.  See the
:doc:`dump <dump>` command for details.

The format of the dump file is selected through the *format* keyword.
If specified, it must be the last keyword used, since all remaining
arguments are passed on to the dump reader.  The *native* format is
for native LIGGGHTS(R)-PUBLIC dump files, written with a "dump atom".html or :doc:`dump custom <dump>` command.  The *xyz* format is for generic XYZ
formatted dump files,

Support for other dump format readers may be added in the future.


----------


Global information is first read from the dump file, namely timestep
and box information.

The dump file is scanned for a snapshot with a time stamp that matches
the specified *Nstep*\ .  This means the LIGGGHTS(R)-PUBLIC timestep the dump file
snapshot was written on for the *native* format.  However, the *xyz*
formats do not store the timestep.  For these formats,
timesteps are numbered logically, in a sequential manner, starting
from 0.  Thus to access the 10th snapshot in an *xyz* or *mofile*
formatted dump file, use *Nstep* = 9.

The dimensions of the simulation box for the selected snapshot are
also read; see the *box* keyword discussion below.  For the *native*
format, an error is generated if the snapshot is for a triclinic box
and the current simulation box is orthogonal or vice versa.  A warning
will be generated if the snapshot box boundary conditions (periodic,
shrink-wrapped, etc) do not match the current simulation boundary
conditions, but the boundary condition information in the snapshot is
otherwise ignored.  See the "boundary" command for more details.

For the *xyz* format, no information about the box is available, so
you must set the *box* flag to *no*\ .  See details below.


----------


Per-atom information from the dump file snapshot is then read from the
dump file snapshot.  This corresponds to the specified *fields* listed
in the read\_dump command.  It is an error to specify a z-dimension
field, namely *z*\ , *vz*\ , or *iz*\ , for a 2d simulation.

For dump files in *native* format, each column of per-atom data has a
text label listed in the file.  A matching label for each field must
appear, e.g. the label "vy" for the field *vy*\ .  For the *x*\ , *y*\ , *z*
fields any of the following labels are considered a match:


.. parsed-literal::

   x, xs, xu, xsu for field *x*
   y, ys, yu, ysu for field *y*
   z, zs, zu, zsu for field *z*

The meaning of xs (scaled), xu (unwrapped), and xsu (scaled and
unwrapped) is explained on the :doc:`dump <dump>` command doc page.
These labels are searched for in the list of column labels in the dump
file, in order, until a match is found.

The dump file must also contain atom IDs, with a column label of "id".

If the *add* keyword is specified with a value of *yes*\ , as discussed
below, the dump file must contain atom types, with a column label of
"type".

If a column label you want to read from the dump file is not a match
to a specified field, the *label* keyword can be used to specify the
specific column label from the dump file to associate with that field.
An example is if a time-averaged coordinate is written to the dump
file via the :doc:`fix ave/atom <fix_ave_atom>` command.  The column
will then have a label corresponding to the fix-ID rather than "x" or
"xs".  The *label* keyword can also be used to specify new column
labels for fields *id* and *type*\ .

For dump files in *xyz* format, only the *x*\ , *y*\ , and *z* fields are
supported.  The dump file does not store atom IDs, so these are
assigned consecutively to the atoms as they appear in the dump file,
starting from 1.  Thus you should insure that order of atoms is
consistent from snapshot to snapshot in the the XYZ dump file.  See
the :doc:`dump\_modify sort <dump_modify>` command if the XYZ dump file
was written by LIGGGHTS(R)-PUBLIC.


----------


Information from the dump file snapshot is used to overwrite or
replace properties of the current system.  There are various options
for how this is done, determined by the specified fields and optional
keywords.

The timestep of the snapshot becomes the current timestep for the
simulation.  See the :doc:`reset\_timestep <reset_timestep>` command if
you wish to change this after the dump snapshot is read.

If the *box* keyword is specified with a *yes* value, then the current
simulation box dimensions are replaced by the dump snapshot box
dimensions.  If the *box* keyword is specified with a *no* value, the
current simulatoin box is unchanged.

If the *purge* keyword is specified with a *yes* value, then all
current atoms in the system are deleted before any of the operations
invoked by the *replace*\ , *trim*\ , or *add* keywords take place.

If the *replace* keyword is specified with a *yes* value, then atoms
with IDs that are in both the current system and the dump snapshot
have their properties overwritten by field values.  If the *replace*
keyword is specified with a *no* value, atoms with IDs that are in
both the current system and the dump snapshot are not modified.

If the *trim* keyword is specified with a *yes* value, then atoms with
IDs that are in the current system but not in the dump snapshot are
deleted.  These atoms are unaffected if the *trim* keyword is
specified with a *no* value.

If the *add* keyword is specified with a *yes* value, then atoms with
IDs that are in the dump snapshot, but not in the current system are
added to the system.  These dump atoms are ignored if the *add*
keyword is specified with a *no* value.

Note that atoms added via the *add* keyword will have only the
attributes read from the dump file due to the *field* arguments.  If
*x* or *y* or *z* is not specified as a field, a value of 0.0 is used
for added atoms.  Added atoms must have an atom type, so this value
must appear in the dump file.

Any other attributes (e.g. charge or particle diameter for spherical
particles) will be set to default values, the same as if the
:doc:`create\_atoms <create_atoms>` command were used.

Note that atom IDs are not preserved for new dump snapshot atoms added
via the *add* keyword.  The procedure for assigning new atom IDS to
added atoms is the same as is described for the
:doc:`create\_atoms <create_atoms>` command.


----------


Atom coordinates read from the dump file are first converted into
unscaled coordinates, relative to the box dimensions of the snapshot.
These coordinates are then be assigned to an existing or new atom in
the current simulation.  The coordinates will then be remapped to the
simulation box, whether it is the original box or the dump snapshot
box.  If periodic boundary conditions apply, this means the atom will
be remapped back into the simulation box if necessary.  If shrink-wrap
boundary conditions apply, the new coordinates may change the
simulation box dimensions.  If fixed boundary conditions apply, the
atom will be lost if it is outside the simulation box.

For *native* format dump files, the 3 xyz image flags for an atom in
the dump file are set to the corresponding values appearing in the
dump file if the *ix*\ , *iy*\ , *iz* fields are specified.  If not
specified, the image flags for replaced atoms are not changed and
image flags for new atoms are set to default values.  If coordinates
read from the dump file are in unwrapped format (e.g. *xu*\ ) then the
image flags for read-in atoms are also set to default values.  The
remapping procedure described in the previous paragraph will then
change images flags for all atoms (old and new) if periodic boundary
conditions are applied to remap an atom back into the simulation box.

.. warning::

   If you get a warning about inconsistent image flags
   after reading in a dump snapshot, it means one or more pairs of bonded
   atoms now have inconsistent image flags.  As discussed in :doc:`Section errors <Section_errors>` this may or may not cause problems for
   subsequent simulations, One way this can happen is if you read image
   flag fields from the dump file but do not also use the dump file box
   parameters.

LIGGGHTS(R)-PUBLIC knows how to compute unscaled and remapped coordinates for the
snapshot column labels discussed above, e.g. *x*\ , *xs*\ , *xu*\ , *xsu*\ .
If another column label is assigned to the *x* or *y* or *z* field via
the *label* keyword, e.g. for coordinates output by the :doc:`fix ave/atom <fix_ave_atom>` command, then LIGGGHTS(R)-PUBLIC needs to know whether
the coordinate information in the dump file is scaled and/or wrapped.
This can be set via the *scaled* and *wrapped* keywords.  Note that
the value of the *scaled* and *wrapped* keywords is ignored for fields
*x* or *y* or *z* if the *label* keyword is not used to assign a
column label to that field.

The scaled/unscaled and wrapped/unwrapped setting must be identical
for any of the *x*\ , *y*\ , *z* fields that are specified.  Thus you
cannot read *xs* and *yu* from the dump file.  Also, if the dump file
coordinates are scaled and the simulation box is triclinic, then all 3
of the *x*\ , *y*\ , *z* fields must be specified, since they are all
needed to generate absolute, unscaled coordinates.


----------


Restrictions
""""""""""""


To read gzipped dump files, you must compile LIGGGHTS(R)-PUBLIC with the
-DLAMMPS\_GZIP option - see the :ref:`Making LIGGGHTS(R)-PUBLIC <start_2>` section of the documentation.

Related commands
""""""""""""""""

:doc:`dump <dump>`,
:doc:`read\_data <read_data>`, :doc:`read\_restart <read_restart>`,
:doc:`rerun <rerun>`

Default
"""""""

The option defaults are box = yes, replace = yes, purge = no, trim =
no, add = no, scaled = no, wrapped = yes, and format = native.


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
