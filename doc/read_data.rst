.. index:: read\_data

read\_data command
==================

Syntax
""""""


.. parsed-literal::

   read_data file keyword args ...

* file = name of data file to read in
* zero or more keyword/arg pairs may be appended
* keyword = *fix*
  
  .. parsed-literal::
  
       *fix* args = fix-ID header-string section-string
         fix-ID = ID of fix to process header lines and sections of data file
         header-string = header lines containing this string will be passed to fix
         section-string = section names with this string will be passed to fix



Examples
""""""""


.. parsed-literal::

   read_data data.lj
   read_data ../run7/data.polymer.gz
   read_data data.protein fix mycmap crossterm CMAP

Description
"""""""""""

Read in a data file containing information Packfall needs to run a
simulation.  The file can be ASCII text or a gzipped text file
(detected by a .gz suffix).  This is one of 3 ways to specify initial
atom coordinates; see the :doc:`read\_restart <read_restart>` and
:doc:`create\_atoms <create_atoms>` commands for alternative methods.

The structure of the data file is important, though many settings and
sections are optional or can come in any order.  See the examples
directory for sample data files for different problems.

A data file has a header and a body.  The header appears first.  The
first line of the header is always skipped; it typically contains a
description of the file.  Then lines are read one at a time.  Lines
can have a trailing comment starting with '#' that is ignored.  If the
line is blank (only whitespace after comment is deleted), it is
skipped.  If the line contains a header keyword, the corresponding
value(s) is read from the line.  If it doesn't contain a header
keyword, the line begins the body of the file.

The body of the file contains zero or more sections.  The first line
of a section has only a keyword.  The next line is skipped.  The
remaining lines of the section contain values.  The number of lines
depends on the section keyword as described below.  Zero or more blank
lines can be used between sections.  Sections can appear in any order,
with a few exceptions as noted below.

The keyword *fix* can be used one or more times.  Each usage specifies
a fix that will be used to process a specific portion of the data
file.  Any header line containing *header-string* and any section with
a name containing *section-string* will be passed to the specified
fix.  See the :doc:`fix property/atom <fix_property_atom>` command for
an example of a fix that operates in this manner.  The doc page for
the fix defines the syntax of the header line(s) and section(s) that
it reads from the data file.  Note that the *header-string* can be
specified as NULL, in which case no header lines are passed to the
fix.  This means that it can infer the length of its Section from
standard header settings, such as the number of atoms.

The formatting of individual lines in the data file (indentation,
spacing between words and numbers) is not important except that header
and section keywords (e.g. atoms, xlo xhi, Masses, Bond Coeffs) must
be capitalized as shown and can't have extra white space between their
words - e.g. two spaces or a tab between the 2 words in "xlo xhi" or
the 2 words in "Bond Coeffs", is not valid.


----------


These are the recognized header keywords.  Header lines can come in
any order.  The value(s) are read from the beginning of the line.
Thus the keyword *atoms* should be in a line like "1000 atoms"; the
keyword *ylo yhi* should be in a line like "-10.0 10.0 ylo yhi"; the
keyword *xy xz yz* should be in a line like "0.0 5.0 6.0 xy xz yz".
All these settings have a default value of 0, except the lo/hi box
size defaults are -0.5 and 0.5.  A line need only appear if the value
is different than the default.

* *atoms* = # of atoms in system
* *bonds* = # of bonds in system
* *atom types* = # of atom types in system
* *bond types* = # of bond types in system
* *extra bond per atom* = leave space for this many new bonds per atom
* *ellipsoids* = # of ellipsoids in system
* *lines* = # of line segments in system
* *xlo xhi* = simulation box boundaries in x dimension
* *ylo yhi* = simulation box boundaries in y dimension
* *zlo zhi* = simulation box boundaries in z dimension
* *xy xz yz* = simulation box tilt factors for triclinic system

The initial simulation box size is determined by the lo/hi settings.
In any dimension, the system may be periodic or non-periodic; see the
:doc:`boundary <boundary>` command.

If the *xy xz yz* line does not appear, Packfall will set up an
axis-aligned (orthogonal) simulation box.  If the line does appear,
Packfall creates a non-orthogonal simulation domain shaped as a
parallelepiped with triclinic symmetry.  The parallelepiped has its
"origin" at (xlo,ylo,zlo) and is defined by 3 edge vectors starting
from the origin given by A = (xhi-xlo,0,0); B = (xy,yhi-ylo,0); C =
(xz,yz,zhi-zlo).  *Xy,xz,yz* can be 0.0 or positive or negative values
and are called "tilt factors" because they are the amount of
displacement applied to faces of an originally orthogonal box to
transform it into the parallelepiped.

The tilt factors (xy,xz,yz) can not skew the box more than half the
distance of the corresponding parallel box length.  For example, if
xlo = 2 and xhi = 12, then the x box length is 10 and the xy tilt
factor must be between -5 and 5.  Similarly, both xz and yz must be
between -(xhi-xlo)/2 and +(yhi-ylo)/2.  Note that this is not a
limitation, since if the maximum tilt factor is 5 (as in this
example), then configurations with tilt = ..., -15, -5, 5, 15, 25,
... are all geometrically equivalent.

See :ref:`Section\_howto 12 <howto_7>` of the doc pages
for a geometric description of triclinic boxes, as defined by Packfall,
and how to transform these parameters to and from other commonly used
triclinic representations.

When a triclinic system is used, the simulation domain must be
periodic in any dimensions with a non-zero tilt factor, as defined by
the :doc:`boundary <boundary>` command.  I.e. if the xy tilt factor is
non-zero, then both the x and y dimensions must be periodic.
Similarly, x and z must be periodic if xz is non-zero and y and z must
be periodic if yz is non-zero.  Also note that if your simulation will
tilt the box, e.g. via the :doc:`fix deform <fix_deform>` command, the
simulation box must be defined as triclinic, even if the tilt factors
are initially 0.0.

For 2d simulations, the *zlo zhi* values should be set to bound the z
coords for atoms that appear in the file; the default of -0.5 0.5 is
valid if all z coords are 0.0.  For 2d triclinic simulations, the xz
and yz tilt factors must be 0.0.

If the system is periodic (in a dimension), then atom coordinates can
be outside the bounds (in that dimension); they will be remapped (in a
periodic sense) back inside the box.

.. warning::

   If the system is non-periodic (in a dimension), then
   all atoms in the data file must have coordinates (in that dimension)
   that are "greater than or equal to" the lo value and "less than or
   equal to" the hi value.  If the non-periodic dimension is of style
   "fixed" (see the :doc:`boundary <boundary>` command), then the atom
   coords must be strictly "less than" the hi value, due to the way
   Packfall assign atoms to processors.  Note that you should not make the
   lo/hi values radically smaller/larger than the extent of the atoms.
   For example, if your atoms extend from 0 to 50, you should not specify
   the box bounds as -10000 and 10000.  This is because Packfall uses the
   specified box size to layout the 3d grid of processors.  A huge
   (mostly empty) box will be sub-optimal for performance when using
   "fixed" boundary conditions (see the :doc:`boundary <boundary>`
   command).  When using "shrink-wrap" boundary conditions (see the
   :doc:`boundary <boundary>` command), a huge (mostly empty) box may cause
   a parallel simulation to lose atoms the first time that Packfall
   shrink-wraps the box around the atoms.

The "extra bond per atom" setting should be used if new bonds will be
added to the system when a simulation runs, e.g. by using the :doc:`fix bond/create <fix_bond_create>` command.  This will pre-allocate
space in Packfall data structures for storing the new bonds.


----------


These are the section keywords for the body of the file.

* *Atoms, Velocities, Masses, Ellipsoids, Lines* = atom-property sections
* *Bonds* = molecular topology sections
* *Pair Coeffs, PairIJ Coeffs, Bond Coeffs* = force field sections
* *BondBond Coeffs, BondAngle Coeffs, MiddleBondTorsion Coeffs,    EndBondTorsion Coeffs* = class 2 force field sections

Each section is listed below in alphabetic order.  The format of each
section is described including the number of lines it must contain and
rules (if any) for where it can appear in the data file.

Any individual line in the various sections can have a trailing
comment starting with "#" for annotation purposes.  E.g. in the
Atoms section:


.. parsed-literal::

   10 1 17 -1.0 10.0 5.0 6.0   # salt ion


----------


*Atoms* section:

* one line per atom
* line syntax: depends on atom style

An *Atoms* section must appear in the data file if natoms > 0 in the
header section.  The atoms can be listed in any order.  These are the
line formats for each :doc:`atom style <atom_style>` in Packfall.  As
discussed below, each line can optionally have 3 flags (nx,ny,nz)
appended to it, which indicate which image of a periodic simulation
box the atom is in.  These may be important to include for some kinds
of analysis.

+-----------+------------------------------------------------------+
| bond      | atom-ID molecule-ID atom-type x y z                  |
+-----------+------------------------------------------------------+
| ellipsoid | atom-ID atom-type ellipsoidflag density x y z        |
+-----------+------------------------------------------------------+
| sphere    | atom-ID atom-type diameter density x y z             |
+-----------+------------------------------------------------------+
| line      | atom-ID molecule-ID atom-type lineflag density x y z |
+-----------+------------------------------------------------------+
| molecular | atom-ID molecule-ID atom-type x y z                  |
+-----------+------------------------------------------------------+
| hybrid    | atom-ID atom-type x y z sub-style1 sub-style2 ...    |
+-----------+------------------------------------------------------+

The keywords have these meanings:

* atom-ID = integer ID of atom
* molecule-ID = integer ID of molecule the atom belongs to
* atom-type = type of atom (1-Ntype)
* q = charge on atom (charge units)
* diameter = diameter of spherical atom (distance units)
* ellipsoidflag = 1 for ellipsoidal particles, 0 for point particles
* lineflag = 1 for line segment particles, 0 for point particles
* triangleflag = 1 for triangular particles, 0 for point particles
* density = density of particle (mass/distance\^3 or mass/distance\^2 or mass/distance units, depending on dimensionality of particle)
* mass = mass of particle (mass units)
* volume = volume of particle (distance\^3 units)
* x,y,z = coordinates of atom

The units for these quantities depend on the unit style; see the
:doc:`units <units>` command for details.

For 2d simulations specify z as 0.0, or a value within the *zlo zhi*
setting in the data file header.

The atom-ID is used to identify the atom throughout the simulation and
in dump files.  Normally, it is a unique value from 1 to Natoms for
each atom.  Unique values larger than Natoms can be used, but they
will cause extra memory to be allocated on each processor, if an atom
map array is used (see the :doc:`atom\_modify <atom_modify>` command).
If an atom map array is not used (e.g. an atomic system with no
bonds), and velocities are not assigned in the data file, and you
don't care if unique atom IDs appear in dump files, then the atom-IDs
can all be set to 0.

The molecule ID is a 2nd identifier attached to an atom.  Normally, it
is a number from 1 to N, identifying which molecule the atom belongs
to.  It can be 0 if it is an unbonded atom or if you don't care to
keep track of molecule assignments.

The diameter specifies the size of a finite-size spherical particle.
It can be set to 0.0, which means that atom is a point particle.

The ellipsoidflag, lineflag, triangleflag determine
whether the particle is a finite-size ellipsoid or line or triangle or
body of finite size, or whether the particle is a point particle.
Additional attributes must be defined for each ellipsoid, line,
triangle, or body in the corresponding *Ellipsoids*\ , *Lines*\ ,
*Triangles* section.

Some pair styles and fixes and computes that operate on finite-size
particles allow for a mixture of finite-size and point particles.  See
the doc pages of individual commands for details.

For finite-size particles, the density is used in conjunction with the
particle volume to set the mass of each particle as mass = density \*
volume.  In this context, volume can be a 3d quantity (for spheres or
ellipsoids), a 2d quantity (for triangles), or a 1d quantity (for line
segments).  If the volume is 0.0, meaning a point particle, then the
density value is used as the mass.  One exception is for the body atom
style, in which case the mass of each particle (body or point
particle) is specified explicitly.  This is because the volume of the
body is unknown.

For atom\_style hybrid, following the 5 initial values (ID,type,x,y,z),
specific values for each sub-style must be listed.  The order of the
sub-styles is the same as they were listed in the
:doc:`atom\_style <atom_style>` command.  The sub-style specific values
are those that are not the 5 standard ones (ID,type,x,y,z).  For
example, for the "charge" sub-style, a "q" value would appear.  For
the "full" sub-style, a "molecule-ID" and "q" would appear.  These are
listed in the same order they appear as listed above.  Thus if


.. parsed-literal::

   atom_style hybrid charge sphere

were used in the input script, each atom line would have these fields:


.. parsed-literal::

   atom-ID atom-type x y z q diameter density

Note that if a non-standard value is defined by multiple sub-styles,
it must appear mutliple times in the atom line.  E.g. the atom line
for atom\_style hybrid dipole full would list "q" twice:


.. parsed-literal::

   atom-ID atom-type x y z q mux muy myz molecule-ID q

Atom lines (all lines or none of them) can optionally list 3 trailing
integer values: nx,ny,nz.  For periodic dimensions, they specify which
image of the simulation box the atom is considered to be in.  An image
of 0 means it is inside the box as defined.  A value of 2 means add 2
box lengths to get the true value.  A value of -1 means subtract 1 box
length to get the true value.  Packfall updates these flags as atoms
cross periodic boundaries during the simulation.  The flags can be
output with atom snapshots via the :doc:`dump <dump>` command.

If nx,ny,nz values are not set in the data file, Packfall initializes
them to 0.  If image information is needed for later analysis and they
are not all initially 0, it's important to set them correctly in the
data file.  Also, if you plan to use the :doc:`replicate <replicate>`
command to generate a larger system, these flags must be listed
correctly for bonded atoms when the bond crosses a periodic boundary.
I.e. the values of the image flags should be different by 1 (in the
appropriate dimension) for the two atoms in such a bond.

Atom velocities and other atom quantities not defined above are set to
0.0 when the *Atoms* section is read.  Velocities can be set later by
a *Velocities* section in the data file or by a
:doc:`velocity <velocity>` or :doc:`set <set>` command in the input
script.


----------


*Bond Coeffs* section:

* one line per bond type
* line syntax: ID coeffs
  
  .. parsed-literal::
  
       ID = bond type (1-N)
       coeffs = list of coeffs

* example:
  
  .. parsed-literal::
  
       4 250 1.49



The number and meaning of the coefficients are specific to the defined
bond style.  See the :doc:`bond\_style <bond_style>` and
:doc:`bond\_coeff <bond_coeff>` commands for details.  Coefficients can
also be set via the :doc:`bond\_coeff <bond_coeff>` command in the input
script.


----------


*BondBond Coeffs* section:

* one line per angle type
* line syntax: ID coeffs
  
  .. parsed-literal::
  
       ID = angle type (1-N)
       coeffs = list of coeffs (see class 2 section of :doc:`angle_coeff <angle_coeff>`)




----------


*BondBond13 Coeffs* section:

* one line per dihedral type
* line syntax: ID coeffs
  
  .. parsed-literal::
  
       ID = dihedral type (1-N)
       coeffs = list of coeffs (see class 2 section of :doc:`dihedral_coeff <dihedral_coeff>`)




----------


*Bonds* section:

* one line per bond
* line syntax: ID type atom1 atom2
  
  .. parsed-literal::
  
       ID = bond number (1-Nbonds)
       type = bond type (1-Nbondtype)
       atom1,atom2 = IDs of 1st,2nd atoms in bond

* example:
  
  .. parsed-literal::
  
       12 3 17 29



The *Bonds* section must appear after the *Atoms* section.  All values
in this section must be integers (1, not 1.0).


----------


*Ellipsoids* section:

* one line per ellipsoid
* line syntax: atom-ID shapex shapey shapez quatw quati quatj quatk
  
  .. parsed-literal::
  
       atom-ID = ID of atom which is an ellipsoid
       shapex,shapey,shapez = 3 diameters of ellipsoid (distance units)
       quatw,quati,quatj,quatk = quaternion components for orientation of atom

* example:
  
  .. parsed-literal::
  
       12 1 2 1 1 0 0 0



The *Ellipsoids* section must appear if :doc:`atom\_style ellipsoid <atom_style>` is used and any atoms are listed in the
*Atoms* section with an ellipsoidflag = 1.  The number of ellipsoids
should be specified in the header section via the "ellipsoids"
keyword.

The 3 shape values specify the 3 diameters or aspect ratios of a
finite-size ellipsoidal particle, when it is oriented along the 3
coordinate axes.  They must all be non-zero values.

The values *quatw*\ , *quati*\ , *quatj*\ , and *quatk* set the orientation
of the atom as a quaternion (4-vector).  Note that the shape
attributes specify the aspect ratios of an ellipsoidal particle, which
is oriented by default with its x-axis along the simulation box's
x-axis, and similarly for y and z.  If this body is rotated (via the
right-hand rule) by an angle theta around a unit vector (a,b,c), then
the quaternion that represents its new orientation is given by
(cos(theta/2), a\*sin(theta/2), b\*sin(theta/2), c\*sin(theta/2)).  These
4 components are quatw, quati, quatj, and quatk as specified above.
Packfall normalizes each atom's quaternion in case (a,b,c) is not
specified as a unit vector.

The *Ellipsoids* section must appear after the *Atoms* section.


----------


*EndBondTorsion Coeffs* section:

* one line per dihedral type
* line syntax: ID coeffs
  
  .. parsed-literal::
  
       ID = dihedral type (1-N)
       coeffs = list of coeffs (see class 2 section of :doc:`dihedral_coeff <dihedral_coeff>`)




----------


*Lines* section:

* one line per line segment
* line syntax: atom-ID x1 y1 x2 y2
  
  .. parsed-literal::
  
       atom-ID = ID of atom which is a line segment
       x1,y1 = 1st end point
       x2,y2 = 2nd end point

* example:
  
  .. parsed-literal::
  
       12 1.0 0.0 2.0 0.0



The *Lines* section must appear if :doc:`atom\_style line <atom_style>`
is used and any atoms are listed in the *Atoms* section with a
lineflag = 1.  The number of lines should be specified in the header
section via the "lines" keyword.

The 2 end points are the end points of the line segment.  The ordering
of the 2 points should be such that using a right-hand rule to cross
the line segment with a unit vector in the +z direction, gives an
"outward" normal vector perpendicular to the line segment.
I.e. normal = (c2-c1) x (0,0,1).  This orientation may be important
for defining some interactions.

The *Lines* section must appear after the *Atoms* section.


----------


*Masses* section:

* one line per atom type
* line syntax: ID mass
  
  .. parsed-literal::
  
       ID = atom type (1-N)
       mass = mass value

* example:
  
  .. parsed-literal::
  
       3 1.01



This defines the mass of each atom type.  This can also be set via the
:doc:`mass <mass>` command in the input script.  This section cannot be
used for atom styles that define a mass for individual atoms -
e.g. :doc:`atom\_style sphere <atom_style>`.


----------


*MiddleBondTorsion Coeffs* section:

* one line per dihedral type
* line syntax: ID coeffs
  
  .. parsed-literal::
  
       ID = dihedral type (1-N)
       coeffs = list of coeffs (see class 2 section of :doc:`dihedral_coeff <dihedral_coeff>`)




----------


*Pair Coeffs* section:

* one line per atom type
* line syntax: ID coeffs
  
  .. parsed-literal::
  
       ID = atom type (1-N)
       coeffs = list of coeffs

* example:
  
  .. parsed-literal::
  
       3 0.022 2.35197 0.022 2.35197



The number and meaning of the coefficients are specific to the defined
pair style.  See the :doc:`pair\_style <pair_style>` and
:doc:`pair\_coeff <pair_coeff>` commands for details.  Since pair
coefficients for types I != J are not specified, these will be
generated automatically by the pair style's mixing rule.  See the
individual pair\_style doc pages and the :doc:`pair\_modify mix <pair_modify>` command for details.  Pair coefficients can also
be set via the :doc:`pair\_coeff <pair_coeff>` command in the input
script.


----------


*PairIJ Coeffs* section:

* one line per pair of atom types for all I,J with I <= J
* line syntax: ID1 ID2 coeffs
  
  .. parsed-literal::
  
       ID1 = atom type I = 1-N
       ID2 = atom type J = I-N, with I <= J
       coeffs = list of coeffs

* examples:
  
  .. parsed-literal::
  
       3 3 0.022 2.35197 0.022 2.35197
       3 5 0.022 2.35197 0.022 2.35197



This section must have N\*(N+1)/2 lines where N = # of atom types.  The
number and meaning of the coefficients are specific to the defined
pair style.  See the :doc:`pair\_style <pair_style>` and
:doc:`pair\_coeff <pair_coeff>` commands for details.  Since pair
coefficients for types I != J are all specified, these values will
turn off the default mixing rule defined by the pair style.  See the
individual pair\_style doc pages and the :doc:`pair\_modify mix <pair_modify>` command for details.  Pair coefficients can also
be set via the :doc:`pair\_coeff <pair_coeff>` command in the input
script.


----------


*Triangles* section:

* one line per triangle
* line syntax: atom-ID x1 y1 x2 y2
  
  .. parsed-literal::
  
       atom-ID = ID of atom which is a line segment
       x1,y1,z1 = 1st corner point
       x2,y2,z2 = 2nd corner point
       x3,y3,z3 = 3rd corner point

* example:
  
  .. parsed-literal::
  
       12 0.0 0.0 0.0 2.0 0.0 1.0 0.0 2.0 1.0



The *Triangles* section must appear if :doc:`atom\_style tri <atom_style>` is used and any atoms are listed in the *Atoms*
section with a triangleflag = 1.  The number of lines should be
specified in the header section via the "triangles" keyword.

The 3 corner points are the corner points of the triangle.  The
ordering of the 3 points should be such that using a right-hand rule
to go from point1 to point2 to point3 gives an "outward" normal vector
to the face of the triangle.  I.e. normal = (c2-c1) x (c3-c1).  This
orientation may be important for defining some interactions.

The *Triangles* section must appear after the *Atoms* section.


----------


*Velocities* section:

* one line per atom
* line syntax: depends on atom style

+--------------------------------+--------------------------------------------+
| all styles except those listed | atom-ID vx vy vz                           |
+--------------------------------+--------------------------------------------+
| electron                       | atom-ID vx vy vz ervel                     |
+--------------------------------+--------------------------------------------+
| ellipsoid                      | atom-ID vx vy vz lx ly lz                  |
+--------------------------------+--------------------------------------------+
| sphere                         | atom-ID vx vy vz wx wy wz                  |
+--------------------------------+--------------------------------------------+
| hybrid                         | atom-ID vx vy vz sub-style1 sub-style2 ... |
+--------------------------------+--------------------------------------------+

where the keywords have these meanings:

vx,vy,vz = translational velocity of atom
lx,ly,lz = angular momentum of aspherical atom
wx,wy,wz = angular velocity of spherical atom
ervel = electron radial velocity (0 for fixed-core):ul

The velocity lines can appear in any order.  This section can only be
used after an *Atoms* section.  This is because the *Atoms* section
must have assigned a unique atom ID to each atom so that velocities
can be assigned to them.

Vx, vy, vz, and ervel are in :doc:`units <units>` of velocity.  Lx, ly,
lz are in units of angular momentum (distance-velocity-mass).  Wx, Wy,
Wz are in units of angular velocity (radians/time).

For atom\_style hybrid, following the 4 initial values (ID,vx,vy,vz),
specific values for each sub-style must be listed.  The order of the
sub-styles is the same as they were listed in the
:doc:`atom\_style <atom_style>` command.  The sub-style specific values
are those that are not the 5 standard ones (ID,vx,vy,vz).  For
example, for the "sphere" sub-style, "wx", "wy", "wz" values would
appear.  These are listed in the same order they appear as listed
above.  Thus if


.. parsed-literal::

   atom_style hybrid electron sphere

were used in the input script, each velocity line would have these
fields:


.. parsed-literal::

   atom-ID vx vy vz ervel wx wy wz

Translational velocities can also be set by the
:doc:`velocity <velocity>` command in the input script.


----------


Restrictions
""""""""""""


To read gzipped data files, you must compile Packfall with the
-DLAMMPS\_GZIP option - see the :ref:`Making Packfall <start_2>` section of the documentation.

Related commands
""""""""""""""""

:doc:`read\_dump <read_dump>`, :doc:`read\_restart <read_restart>`,
:doc:`create\_atoms <create_atoms>`, :doc:`write\_data <write_data>`

**Default:** none


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
