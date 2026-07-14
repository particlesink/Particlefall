.. index:: fix rigid

fix rigid command
=================

fix rigid/nve command
=====================

fix rigid/nvt command
=====================

fix rigid/npt command
=====================

fix rigid/nph command
=====================

fix rigid/small command
=======================

Syntax
""""""


.. parsed-literal::

   fix ID group-ID style bodystyle args keyword values ...

* ID, group-ID are documented in :doc:`fix <fix>` command
* style = *rigid*
* bodystyle = *single* or *molecule* or *group*
  
  .. parsed-literal::
  
       *single* args = none
       *molecule* args = none
       *group* args = N groupID1 groupID2 ...
         N = # of groups
         groupID1, groupID2, ... = list of N group IDs

* zero or more keyword/value pairs may be appended
* keyword =  or *temp* or *iso* or *aniso* or *x* or *y* or *z* or *couple* or *tparam* or *pchain* or *dilate* or *force* or *torque* or *infile*
  
  .. parsed-literal::
  
       *langevin* values = Tstart Tstop Tperiod seed
         Tstart,Tstop = desired temperature at start/stop of run (temperature units)
         Tdamp = temperature damping parameter (time units)
         seed = random number seed to use for white noise (prime number greater 10000)
       *temp* values = Tstart Tstop Tdamp
         Tstart,Tstop = desired temperature at start/stop of run (temperature units)
         Tdamp = temperature damping parameter (time units)
       *iso* or *aniso* values = Pstart Pstop Pdamp
         Pstart,Pstop = scalar external pressure at start/end of run (pressure units)
         Pdamp = pressure damping parameter (time units)
       *x* or *y* or *z* values = Pstart Pstop Pdamp
         Pstart,Pstop = external stress tensor component at start/end of run (pressure units)
         Pdamp = stress damping parameter (time units)
       *couple* = *none* or *xyz* or *xy* or *yz* or *xz*
       *tparam* values = Tchain Titer Torder
         Tchain = length of Nose/Hoover thermostat chain
         Titer = number of thermostat iterations performed
         Torder = 3 or 5 = Yoshida-Suzuki integration parameters
       *pchain* values = Pchain
         Pchain = length of the Nose/Hoover thermostat chain coupled with the barostat
       *dilate* value = dilate-group-ID
         dilate-group-ID = only dilate atoms in this group due to barostat volume changes
       *force* values = M xflag yflag zflag
         M = which rigid body from 1-Nbody (see asterisk form below)
         xflag,yflag,zflag = off/on if component of center-of-mass force is active
       *torque* values = M xflag yflag zflag
         M = which rigid body from 1-Nbody (see asterisk form below)
         xflag,yflag,zflag = off/on if component of center-of-mass torque is active
       *infile* filename
         filename = file with per-body values of mass, center-of-mass, moments of inertia



Examples
""""""""


.. parsed-literal::

   fix 1 clump rigid single
   fix 2 fluid rigid group 3 clump1 clump2 clump3 torque \* off off off

Description
"""""""""""

Treat one or more sets of atoms as independent rigid bodies.  This
means that each timestep the total force and torque on each rigid body
is computed as the sum of the forces and torques on its constituent
particles and the coordinates, velocities, and orientations of the
atoms in each body are updated so that the body moves and rotates as a
single entity.

Examples of large rigid bodies are a large colloidal particle, or
portions of a large biomolecule such as a protein.

Example of small rigid bodies are patchy nanoparticles, such as those
modeled in :ref:`this paper <Zhang>` by Sharon Glotzer's group, clumps of
granular particles, lipid molecules consiting of one or more point
dipoles connected to other spheroids or ellipsoids, irregular
particles built from line segments (2d) or triangles (3d), and
coarse-grain models of nano or colloidal particles consisting of a
small number of constituent particles.  Note that the :doc:`fix shake <fix_shake>` command can also be used to rigidify small
molecules of 2, 3, or 4 atoms, e.g. water molecules.  That fix treats
the constituent atoms as point masses.

These fixes also update the positions and velocities of the atoms in
each rigid body via time integration, in the NVE ensemble.

.. warning::

   Not all of the bodystyle options and keyword/value
   options are available for both the *rigid* and *rigid/small* variants.
   See details below.

The *rigid* variant is typically the best choice for a system with a
small number of large rigid bodies, each of which can extend across
the domain of many processors.  It operates by creating a single
global list of rigid bodies, which all processors contribute to.
MPI\_Allreduce operations are performed each timestep to sum the
contributions from each processor to the force and torque on all the
bodies.  This operation will not scale well in parallel if large
numbers of rigid bodies are simulated.

Which of the two variants is faster for a particular problem is hard
to predict.  The best way to decide is to perform a short test run.
Both variants should give identical numerical answers for short runs.
Long runs should give statistically similar results, but round-off
differences will accumulate to produce divergent trajectories.

.. warning::

   You should not update the atoms in rigid bodies via
   other time-integration fixes (e.g. :doc:`fix nve <fix_nve>`), or you will
   be integrating
   their motion more than once each timestep.  When performing a hybrid
   simulation with some atoms in rigid bodies, and some not, a separate
   time integration fix like :doc:`fix nve <fix_nve>` should be used for the
   non-rigid particles.

.. warning::

   These fixes are overkill if you simply want to hold a
   collection of atoms stationary or have them move with a constant
   velocity.  A simpler way to hold atoms stationary is to not include
   those atoms in your time integration fix.  E.g. use "fix 1 mobile nve"
   instead of "fix 1 all nve", where "mobile" is the group of atoms that
   you want to move.  You can move atoms with a constant velocity by
   assigning them an initial velocity (via the :doc:`velocity <velocity>`
   command), setting the force on them to 0.0 (via the :doc:`fix setforce <fix_setforce>` command), and integrating them as usual
   (e.g. via the :doc:`fix nve <fix_nve>` command).


----------


Each rigid body must have two or more atoms.  An atom can belong to at
most one rigid body.  Which atoms are in which bodies can be defined
via several options.

For bodystyle *single* the entire fix group of atoms is treated as one
rigid body.  This option is only allowed for fix rigid and its
sub-styles.

For bodystyle *molecule*\ , each set of atoms in the fix group with a
different molecule ID is treated as a rigid body.  This option is
allowed for fix rigid and fix rigid/small, and their sub-styles.  Note
that atoms with a molecule ID = 0 will be treated as a single rigid
body.  For a system with atomic solvent (typically this is atoms with
molecule ID = 0) surrounding rigid bodies, this may not be what you
want.  Thus you should be careful to use a fix group that only
includes atoms you want to be part of rigid bodies.

For bodystyle *group*\ , each of the listed groups is treated as a
separate rigid body.  Only atoms that are also in the fix group are
included in each rigid body.  This option is only allowed for fix
rigid and its sub-styles.

.. warning::

   To compute the initial center-of-mass position and
   other properties of each rigid body, the image flags for each atom in
   the body are used to "unwrap" the atom coordinates.  Thus you must
   insure that these image flags are consistent so that the unwrapping
   creates a valid rigid body (one where the atoms are close together),
   particularly if the atoms in a single rigid body straddle a periodic
   boundary.  This means the input data file or restart file must define
   the image flags for each atom consistently or that you have used the
   :doc:`set <set>` command to specify them correctly.  If a dimension is
   non-periodic then the image flag of each atom must be 0 in that
   dimension, else an error is generated.

The *force* and *torque* keywords discussed next are only allowed for
fix rigid and its sub-styles.

By default, each rigid body is acted on by other atoms which induce an
external force and torque on its center of mass, causing it to
translate and rotate.  Components of the external center-of-mass force
and torque can be turned off by the *force* and *torque* keywords.
This may be useful if you wish a body to rotate but not translate, or
vice versa, or if you wish it to rotate or translate continuously
unaffected by interactions with other particles.  Note that if you
expect a rigid body not to move or rotate by using these keywords, you
must insure its initial center-of-mass translational or angular
velocity is 0.0.  Otherwise the initial translational or angular
momentum the body has will persist.

An xflag, yflag, or zflag set to *off* means turn off the component of
force of torque in that dimension.  A setting of *on* means turn on
the component, which is the default.  Which rigid body(s) the settings
apply to is determined by the first argument of the *force* and
*torque* keywords.  It can be an integer M from 1 to Nbody, where
Nbody is the number of rigid bodies defined.  A wild-card asterisk can
be used in place of, or in conjunction with, the M argument to set the
flags for multiple rigid bodies.  This takes the form "\*" or "\*n" or
"n\*" or "m\*n".  If N = the number of rigid bodies, then an asterisk
with no numeric values means all bodies from 1 to N.  A leading
asterisk means all bodies from 1 to n (inclusive).  A trailing
asterisk means all bodies from n to N (inclusive).  A middle asterisk
means all types from m to n (inclusive).  Note that you can use the
*force* or *torque* keywords as many times as you like.  If a
particular rigid body has its component flags set multiple times, the
settings from the final keyword are used.

For computational efficiency, you may wish to turn off pairwise and
bond interactions within each rigid body, as they no longer contribute
to the motion.  The :doc:`neigh\_modify exclude <neigh_modify>` and
:doc:`delete\_bonds <delete_bonds>` commands are used to do this.

For computational efficiency, you should typically define one fix
rigid or fix rigid/small command which includes all the desired rigid
bodies.  Packfall will allow multiple rigid fixes to be defined, but it
is more expensive.


----------


The constituent particles within a rigid body can be point particles
(the default in Packfall) or finite-size particles, such as spheres or
ellipsoids or line segments or triangles.  See the :doc:`atom\_style sphere and ellipsoid and line and tri <atom_style>` commands for more
details on these kinds of particles.  Finite-size particles contribute
differently to the moment of inertia of a rigid body than do point
particles.  Finite-size particles can also experience torque (e.g. due
to :doc:`frictional granular interactions <pair_gran>`) and have an
orientation.  These contributions are accounted for by these fixes.

Forces between particles within a body do not contribute to the
external force or torque on the body.  Thus for computational
efficiency, you may wish to turn off pairwise and bond interactions
between particles within each rigid body.  The :doc:`neigh\_modify exclude <neigh_modify>` and :doc:`delete\_bonds <delete_bonds>`
commands are used to do this.  For finite-size particles this also
means the particles can be highly overlapped when creating the rigid
body.


----------


The *rigid* style performs constant
NVE time integration based on Richardson
iterations.


----------


The *infile* keyword allows a file of rigid body attributes to be read
in from a file, rather then having Packfall compute them.  There are 3
such attributes: the total mass of the rigid body, its center-of-mass
position, and its 6 moments of inertia.  For rigid bodies consisting
of point particles or non-overlapping finite-size particles, Packfall
can compute these values accurately.  However, for rigid bodies
consisting of finite-size particles which overlap each other, Packfall
will ignore the overlaps when computing these 3 attributes.  The
amount of error this induces depends on the amount of overlap.  To
avoid this issue, the values can be pre-computed (e.g. using Monte
Carlo integration).

The format of the file is as follows.  Note that the file does not
have to list attributes for every rigid body integrated by fix rigid.
Only bodies which the file specifies will have their computed
attributes overridden.  The file can contain initial blank lines or
comment lines starting with "#" which are ignored.  The first
non-blank, non-comment line should list N = the number of lines to
follow.  The N successive lines contain the following information:


.. parsed-literal::

   ID1 masstotal xcm ycm zcm ixx iyy izz ixy ixz iyz
   ID2 masstotal xcm ycm zcm ixx iyy izz ixy ixz iyz
   ...
   IDN masstotal xcm ycm zcm ixx iyy izz ixy ixz iyz

The rigid body IDs are all positive integers.  For the *single*
bodystyle, only an ID of 1 can be used.  For the *group* bodystyle,
IDs from 1 to Ng can be used where Ng is the number of specified
groups.  For the *molecule* bodystyle, use the molecule ID for the
atoms in a specific rigid body as the rigid body ID.

The masstotal and center-of-mass coordinates (xcm,ycm,zcm) are
self-explanatory.  The center-of-mass should be consistent with what
is calculated for the position of the rigid body with all its atoms
unwrapped by their respective image flags.  If this produces a
center-of-mass that is outside the simulation box, Packfall wraps it
back into the box.  The 6 moments of inertia (ixx,iyy,izz,ixy,ixz,iyz)
should be the values consistent with the current orientation of the
rigid body around its center of mass.  The values are with respect to
the simulation box XYZ axes, not with respect to the prinicpal axes of
the rigid body itself.  Packfall performs the latter calculation
internally.

.. warning::

   If you use the *infile* keyword and write restart
   files during a simulation, then each time a restart file is written,
   the fix also write an auxiliary restart file with the name
   rfile.rigid, where "rfile" is the name of the restart file,
   e.g. tmp.restart.10000 and tmp.restart.10000.rigid.  This auxiliary
   file is in the same format described above and contains info on the
   current center-of-mass and 6 moments of inertia.  Thus it can be used
   in a new input script that restarts the run and re-specifies a rigid
   fix using an *infile* keyword and the appropriate filename.  Note that
   the auxiliary file will contain one line for every rigid body, even if
   the original file only listed a subset of the rigid bodies.


----------


.. warning::

   The periodic image flags of atoms in rigid bodies are
   altered so that the rigid body can be reconstructed correctly when it
   straddles periodic boundaries.  The atom image flags are not
   incremented/decremented as they would be for non-rigid atoms as the
   rigid body crosses periodic boundaries.  Specifically, they are set so
   that the center-of-mass (COM) of the rigid body always remains inside
   the simulation box.

This means that if you output per-atom image flags you cannot
interpret them as you normally would.  I.e. the image flag values
written to a :doc:`dump file <dump>` will be different than they would
be if the atoms were not in a rigid body.  Likewise the :doc:`compute msd <compute_msd>` will not compute the expected mean-squared
displacement for such atoms if the body moves across periodic
boundaries.  It also means that if you have bonds between a pair of
rigid bodies and the bond straddles a periodic boundary, you cannot
use the :doc:`replicate <replicate>` command to increase the system
size.

Here are details on how, you can post-process a dump file to calculate
a diffusion coefficient for rigid bodies, using the altered per-atom
image flags written to a dump file.  The image flags for atoms in the
same rigid body can be used to unwrap the body and calculate its
center-of-mass (COM).  As mentioned above, this COM will always be
inside the simulation box.  Thus it will "jump" from one side of the
box to the other when the COM crosses a periodic boundary.  If you
keep track of the jumps, you can effectively "unwrap" the COM and use
that value to track the displacement of each rigid body, and thus the
mean-squared displacement (MSD) of an ensemble of bodies, and thus a
diffusion coefficient.

Note that fix rigid does define image flags for each rigid body, which
are incremented when the center-of-mass of the rigid body crosses a
periodic boundary in the usual way.  These image flags have the same
meaning as atom images (see the "dump" command) and can be accessed
and output as described below.


----------


**Restart, fix\_modify, output, run start/stop, minimize info:**

No information is written to :doc:`binary restart files <restart>`.
:doc:`read\_restart <read_restart>` command for info on how to re-specify
a fix in an input script that reads a restart file, so that the
operation of the fix continues in an uninterrupted fashion.

The :doc:`fix\_modify <fix_modify>` *energy* option is supported by the
rigid/nvt fix to add the energy change induced by the thermostatting
to the system's potential energy as part of :doc:`thermodynamic output <thermo_style>`.

The :doc:`fix\_modify <fix_modify>` *temp* and *press* options are
supported by the rigid/npt and rigid/nph fixes to change the computes used
to calculate the instantaneous pressure tensor. Note that the rigid/nvt fix
does not use any external compute to compute instantaneous temperature.

The  fixes compute a global
scalar which can be accessed by various :ref:`output commands <howto_8>`.  The scalar value calculated by
these fixes is "intensive".  The scalar is the current temperature of
the collection of rigid bodies.  This is averaged over all rigid
bodies and their translational and rotational degrees of freedom.  The
translational energy of a rigid body is 1/2 m v\^2, where m = total
mass of the body and v = the velocity of its center of mass.  The
rotational energy of a rigid body is 1/2 I w\^2, where I = the moment
of inertia tensor of the body and w = its angular velocity.  Degrees
of freedom constrained by the *force* and *torque* keywords are
removed from this calculation, but only for the *rigid* and
*rigid/nve* fixes.

This fix computes a global array
of values which can be accessed by various :ref:`output commands <howto_8>`.  The number of rows in the
array is equal to the number of rigid bodies.  The number of columns
is 15.  Thus for each rigid body, 15 values are stored: the xyz coords
of the center of mass (COM), the xyz components of the COM velocity,
the xyz components of the force acting on the COM, the xyz components
of the torque acting on the COM, and the xyz image flags of the COM,
which have the same meaning as image flags for atom positions (see the
"dump" command).  The force and torque values in the array are not
affected by the *force* and *torque* keywords in the fix rigid
command; they reflect values before any changes are made by those
keywords.

The ordering of the rigid bodies (by row in the array) is as follows.
For the *single* keyword there is just one rigid body.  For the
*molecule* keyword, the bodies are ordered by ascending molecule ID.
For the *group* keyword, the list of group IDs determines the ordering
of bodies.

The array values calculated by these fixes are "intensive", meaning
they are independent of the number of atoms in the simulation.

No parameter of these fixes can be used with the *start/stop* keywords
of the :doc:`run <run>` command.  These fixes are not invoked during
:doc:`energy minimization <minimize>`.


----------


Restrictions
""""""""""""
 none

Related commands
""""""""""""""""

:doc:`delete\_bonds <delete_bonds>`, :doc:`neigh\_modify <neigh_modify>`
exclude

Default
"""""""

The option defaults are force \* on on on and torque \* on on on,
meaning all rigid bodies are acted on by center-of-mass force and
torque.  Also Tchain = Pchain = 10, Titer = 1, Torder = 3.


----------


.. _Hoover:



**(Hoover)** Hoover, Phys Rev A, 31, 1695 (1985).

.. _Kamberaj:



**(Kamberaj)** Kamberaj, Low, Neal, J Chem Phys, 122, 224114 (2005).

.. _Martyna:



**(Martyna)** Martyna, Klein, Tuckerman, J Chem Phys, 97, 2635 (1992);
Martyna, Tuckerman, Tobias, Klein, Mol Phys, 87, 1117.

.. _Miller:



**(Miller)** Miller, Eleftheriou, Pattnaik, Ndirango, and Newns,
J Chem Phys, 116, 8649 (2002).

.. _Zhang:



**(Zhang)** Zhang, Glotzer, Nanoletters, 4, 1407-1413 (2004).


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
