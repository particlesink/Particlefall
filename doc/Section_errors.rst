Errors
======

This section describes the errors you can encounter when using LIGGGHTS(R)-PUBLIC,
either conceptually, or as printed out by the program.


.. contents:: 
		:local:
		:depth: 1
   




.. _err\_1:

Common problems
---------------

If two runs do not produce the same answer on different
machines or different numbers of processors, this is typically not a
bug.  In theory you should get identical answers on any number of
processors and on any machine.  In practice, numerical round-off can
cause slight differences and eventual divergence of molecular dynamics
phase space trajectories within a few 100s or few 1000s of timesteps.
However, the statistical properties of the two runs (e.g. average
energy or temperature) should still be the same.

If the :doc:`velocity <velocity>` command is used to set initial atom
velocities, a particular atom can be assigned a different velocity
when the problem is run on a different number of processors or on
different machines.  If this happens, the phase space trajectories of
the two simulations will rapidly diverge.  See the discussion of the
*loop* option in the :doc:`velocity <velocity>` command for details and
options that avoid this issue.

Similarly, the :doc:`create\_atoms <create_atoms>` command generates a
lattice of atoms.  For the same physical system, the ordering and
numbering of atoms by atom ID may be different depending on the number
of processors.

Some commands use random number generators which may be setup to
produce different random number streams on each processor and hence
will produce different effects when run on different numbers of
processors.  A commonly-used example is the :doc:`fix langevin <fix_langevin>` command for thermostatting.

A simulation typically has two stages, setup and run.  Most
errors are detected at setup time; others like a bond
stretching too far may not occur until the middle of a run.

The code tries to flag errors and print informative error messages so
you can fix the problem.  Of course, it cannot figure out your
physics or numerical mistakes, like choosing too big a timestep,
specifying erroneous force field coefficients, or putting 2 atoms on
top of each other! 

If you get an error message about an invalid command in your input
script, you can determine what command is causing the problem by
looking in the log.lammps file or using the :doc:`echo command <echo>`
to see it on the screen.  For a given command, the code expects certain
arguments in a specified order.  If you mess this up, the code will
often flag the error, but it may read a bogus argument and assign a
value that is valid, but not what you wanted.  E.g. trying to read the
string "abc" as an integer value and assigning the associated variable
a value of 0.

Generally, the code will print a message to the screen and logfile and
exit gracefully when it encounters a fatal error.  Sometimes it will
print a WARNING to the screen and logfile and continue on; you can
decide if the WARNING is important or not.  A WARNING message that is
generated in the middle of a run is only printed to the screen, not to
the logfile, to avoid cluttering up thermodynamic output.  If the code
crashes or hangs without spitting out an error message first then it
could be a bug (see :ref:`this section <err_2>`) or one of the following
cases:

The code runs in the available memory a processor allows to be
allocated.  Most reasonable MD runs are compute limited, not memory
limited, so this shouldn't be a bottleneck on most platforms.  Almost
all large memory allocations in the code are done via C-style malloc's
which will generate an error message if you run out of memory.
Smaller chunks of memory are allocated via C++ "new" statements.  If
you are unlucky you could run out of memory just when one of these
small requests is made, in which case the code will crash or hang (in
parallel), since the code doesn't trap on those errors.

Illegal arithmetic can cause the code to run slow or crash.  This is
typically due to invalid physics and numerics that your simulation is
computing.  If you see wild thermodynamic values or NaN values in your
output, something is wrong with your simulation.  If you
suspect this is happening, it is a good idea to print out
thermodynamic info frequently (e.g. every timestep) via the
:doc:`thermo <thermo>` so you can monitor what is happening.
Visualizing the atom movement is also a good idea to insure your model
is behaving as you expect.

In parallel, one way the code can hang is due to how different MPI
implementations handle buffering of messages.  If the code hangs
without an error message, it may be that you need to specify an MPI
setting or two (usually via an environment variable) to enable
buffering or boost the sizes of messages that can be buffered.


----------


.. _err\_2:

Reporting bugs
--------------

Try and isolate
the problem.  Run it on the smallest number of particles and fewest number
of processors and with the simplest input script that reproduces the
bug and try to identify what command or combination of commands is
causing the problem.

----------


.. _err\_3:

Error & warning messages
-------------------------------------

These are two alphabetic lists of the :ref:`ERROR <error>` and
:ref:`WARNING <warn>` messages the code prints out and the reason why.  If the
explanation here is not sufficient, the documentation for the
offending command may help.
Error and warning messages also list the source file and line number
where the error was generated.  For example, this message

ERROR: Illegal velocity command (velocity.cpp:78)

means that line #78 in the file src/velocity.cpp generated the error.
Looking in the source code may help you figure out what went wrong.

Note that error messages from :ref:`user-contributed packages <start_3>` are not listed here.  If such an
error occurs and is not self-explanatory, you'll need to look in the
source code or contact the author of the package.

.. _error:

Errors:
-------------------



*1-3 bond count is inconsistent*
   An inconsistency was detected when computing the number of 1-3
   neighbors for each atom.  This likely means something is wrong with
   the bond topologies you have defined.

*1-4 bond count is inconsistent*
   An inconsistency was detected when computing the number of 1-4
   neighbors for each atom.  This likely means something is wrong with
   the bond topologies you have defined.

*Accelerator sharing is not currently supported on system*
   Multiple MPI processes cannot share the accelerator on your
   system. For NVIDIA GPUs, see the nvidia-smi command to change this
   setting.

*All angle coeffs are not set*
   All angle coefficients must be set in the data file or by the
   angle\_coeff command before running a simulation.

*All bond coeffs are not set*
   All bond coefficients must be set in the data file or by the
   bond\_coeff command before running a simulation.

*All dihedral coeffs are not set*
   All dihedral coefficients must be set in the data file or by the
   dihedral\_coeff command before running a simulation.

*All improper coeffs are not set*
   All improper coefficients must be set in the data file or by the
   improper\_coeff command before running a simulation.

*All masses are not set*
   For atom styles that define masses for each atom type, all masses must
   be set in the data file or by the mass command before running a
   simulation.  They must also be set before using the velocity
   command.

*All mol IDs should be set for fix gcmc group atoms*
   The molecule flag is on, yet not all molecule ids in the fix group have
   been set to non-zero positive values by the user. This is an error since
   all atoms in the fix gcmc group are eligible for deletion, rotation, and
   translation and therefore must have valid molecule ids.

*All pair coeffs are not set*
   All pair coefficients must be set in the data file or by the
   pair\_coeff command before running a simulation.

*All read\_dump x,y,z fields must be specified for scaled, triclinic coords*
   For triclinic boxes and scaled coordinates you must specify all 3 of
   the x,y,z fields, else LIGGGHTS(R)-PUBLIC cannot reconstruct the unscaled
   coordinates.

*All universe/uloop variables must have same # of values*
   Self-explanatory.

*All variables in next command must be same style*
   Self-explanatory.

*Angle atom missing in delete\_bonds*
   The delete\_bonds command cannot find one or more atoms in a particular
   angle on a particular processor.  The pairwise cutoff is too short or
   the atoms are too far apart to make a valid angle.

*Angle atom missing in set command*
   The set command cannot find one or more atoms in a particular angle on
   a particular processor.  The pairwise cutoff is too short or the atoms
   are too far apart to make a valid angle.

*Angle atoms %d %d %d missing on proc %d at step %ld*
   One or more of 3 atoms needed to compute a particular angle are
   missing on this processor.  Typically this is because the pairwise
   cutoff is set too short or the angle has blown apart and an atom is
   too far away.

*Angle coeff for hybrid has invalid style*
   Angle style hybrid uses another angle style as one of its
   coefficients.  The angle style used in the angle\_coeff command or read
   from a restart file is not recognized.

*Angle coeffs are not set*
   No angle coefficients have been assigned in the data file or via the
   angle\_coeff command.

*Angle extent > half of periodic box length*
   This error was detected by the neigh\_modify check yes setting.  It is
   an error because the angle atoms are so far apart it is ambiguous how
   it should be defined.

*Angle potential must be defined for SHAKE*
   When shaking angles, an angle\_style potential must be used.

*Angle style hybrid cannot have hybrid as an argument*
   Self-explanatory.

*Angle style hybrid cannot have none as an argument*
   Self-explanatory.

*Angle style hybrid cannot use same angle style twice*
   Self-explanatory.

*Angle table must range from 0 to 180 degrees*
   Self-explanatory.

*Angle table parameters did not set N*
   List of angle table parameters must include N setting.

*Angle\_coeff command before angle\_style is defined*
   Coefficients cannot be set in the data file or via the angle\_coeff
   command until an angle\_style has been assigned.

*Angle\_coeff command before simulation box is defined*
   The angle\_coeff command cannot be used before a read\_data,
   read\_restart, or create\_box command.

*Angle\_coeff command when no angles allowed*
   The chosen atom style does not allow for angles to be defined.

*Angle\_style command when no angles allowed*
   The chosen atom style does not allow for angles to be defined.

*Angles assigned incorrectly*
   Angles read in from the data file were not assigned correctly to
   atoms.  This means there is something invalid about the topology
   definitions.

*Angles defined but no angle types*
   The data file header lists angles but no angle types.

*Another input script is already being processed*
   Cannot attempt to open a 2nd input script, when the original file is
   still being processed.

*Append boundary must be shrink/minimum*
   The boundary style of the face where atoms are added
   must be of type m (shrink/minimum).

*Arccos of invalid value in variable formula*
   Argument of arccos() must be between -1 and 1.

*Arcsin of invalid value in variable formula*
   Argument of arcsin() must be between -1 and 1.

*Assigning body parameters to non-body atom*
   Self-explanatory.

*Assigning ellipsoid parameters to non-ellipsoid atom*
   Self-explanatory.

*Assigning line parameters to non-line atom*
   Self-explanatory.

*Assigning tri parameters to non-tri atom*
   Self-explanatory.

*Atom IDs must be consecutive for velocity create loop all*
   Self-explanatory.

*Atom count changed in fix neb*
   This is not allowed in a NEB calculation.

*Atom count is inconsistent, cannot write restart file*
   Sum of atoms across processors does not equal initial total count.
   This is probably because you have lost some atoms.

*Atom in too many rigid bodies - boost MAXBODY*
   Fix poems has a parameter MAXBODY (in fix\_poems.cpp) which determines
   the maximum number of rigid bodies a single atom can belong to (i.e. a
   multibody joint).  The bodies you have defined exceed this limit.

*Atom sort did not operate correctly*
   This is an internal error.  Please report it to the
   developers.

*Atom sorting has bin size = 0.0*
   The neighbor cutoff is being used as the bin size, but it is zero.
   Thus you must explicitly list a bin size in the atom\_modify sort
   command or turn off sorting.

*Atom style hybrid cannot have hybrid as an argument*
   Self-explanatory.

*Atom style hybrid cannot use same atom style twice*
   Self-explanatory.

*Atom vector in equal-style variable formula*
   Atom vectors generate one value per atom which is not allowed
   in an equal-style variable.

*Atom-style variable in equal-style variable formula*
   Atom-style variables generate one value per atom which is not allowed
   in an equal-style variable.

*Atom\_modify map command after simulation box is defined*
   The atom\_modify map command cannot be used after a read\_data,
   read\_restart, or create\_box command.

*Atom\_modify sort and first options cannot be used together*
   Self-explanatory.

*Atom\_style command after simulation box is defined*
   The atom\_style command cannot be used after a read\_data,
   read\_restart, or create\_box command.

*Atom\_style line can only be used in 2d simulations*
   Self-explanatory.

*Atom\_style tri can only be used in 3d simulations*
   Self-explanatory.

*Attempt to pop empty stack in fix box/relax*
   Internal error.  Please report it to the developers.

*Attempt to push beyond stack limit in fix box/relax*
   Internal error.  Please report it to the developers.

*Attempting to rescale a 0.0 temperature*
   Cannot rescale a temperature that is already 0.0.

*Bad FENE bond*
   Two atoms in a FENE bond have become so far apart that the bond cannot
   be computed.

*Bad TIP4P angle type for PPPM/TIP4P*
   Specified angle type is not valid.

*Bad TIP4P angle type for PPPMDisp/TIP4P*
   Specified angle type is not valid.

*Bad TIP4P bond type for PPPM/TIP4P*
   Specified bond type is not valid.

*Bad TIP4P bond type for PPPMDisp/TIP4P*
   Specified bond type is not valid.

*Bad fix ID in fix append/atoms command*
   The value of the fix\_id for keyword spatial must start with the suffix
   f\_.

*Bad grid of processors*
   The 3d grid of processors defined by the processors command does not
   match the number of processors the code is being run on.

*Bad kspace\_modify slab parameter*
   Kspace\_modify value for the slab/volume keyword must be >= 2.0.

*Bad matrix inversion in mldivide3*
   This error should not occur unless the matrix is badly formed.

*Bad principal moments*
   Fix rigid did not compute the principal moments of inertia of a rigid
   group of atoms correctly.

*Bad quadratic solve for particle/line collision*
   This is an internal error.  It should nornally not occur.

*Bad quadratic solve for particle/tri collision*
   This is an internal error.  It should nornally not occur.

*Balance command before simulation box is defined*
   The balance command cannot be used before a read\_data, read\_restart,
   or create\_box command.

*Balance dynamic string is invalid*
   The string can only contain the characters "x", "y", or "z".

*Balance produced bad splits*
   This should not occur.  It means two or more cutting plane locations
   are on top of each other or out of order.  Report the problem to the
   developers.

*Bias compute does not calculate a velocity bias*
   The specified compute must compute a bias for temperature.

*Bias compute does not calculate temperature*
   The specified compute must compute temperature.

*Bias compute group does not match compute group*
   The specified compute must operate on the same group as the parent
   compute.

*Big particle in fix srd cannot be point particle*
   Big particles must be extended spheriods or ellipsoids.

*Bigint setting in lmptype.h is invalid*
   Size of bigint is less than size of tagint.

*Bigint setting in lmptype.h is not compatible*
   Bigint stored in restart file is not consistent with version
   you are running.

*Bitmapped lookup tables require int/float be same size*
   Cannot use pair tables on this machine, because of word sizes.  Use
   the pair\_modify command with table 0 instead.

*Bitmapped table in file does not match requested table*
   Setting for bitmapped table in pair\_coeff command must match table
   in file exactly.

*Bitmapped table is incorrect length in table file*
   Number of table entries is not a correct power of 2.

*Bond and angle potentials must be defined for TIP4P*
   Cannot use TIP4P pair potential unless bond and angle potentials
   are defined.

*Bond atom missing in box size check*
   The 2nd atoms needed to compute a particular bond is missing on this
   processor.  Typically this is because the pairwise cutoff is set too
   short or the bond has blown apart and an atom is too far away.

*Bond atom missing in delete\_bonds*
   The delete\_bonds command cannot find one or more atoms in a particular
   bond on a particular processor.  The pairwise cutoff is too short or
   the atoms are too far apart to make a valid bond.

*Bond atom missing in image check*
   The 2nd atom in a particular bond is missing on this processor.
   Typically this is because the pairwise cutoff is set too short or the
   bond has blown apart and an atom is too far away.

*Bond atom missing in set command*
   The set command cannot find one or more atoms in a particular bond on
   a particular processor.  The pairwise cutoff is too short or the atoms
   are too far apart to make a valid bond.

*Bond atoms %d %d missing on proc %d at step %ld*
   The 2nd atom needed to compute a particular bond is missing on this
   processor.  Typically this is because the pairwise cutoff is set too
   short or the bond has blown apart and an atom is too far away.

*Bond coeff for hybrid has invalid style*
   Bond style hybrid uses another bond style as one of its coefficients.
   The bond style used in the bond\_coeff command or read from a restart
   file is not recognized.

*Bond coeffs are not set*
   No bond coefficients have been assigned in the data file or via the
   bond\_coeff command.

*Bond extent > half of periodic box length*
   This error was detected by the neigh\_modify check yes setting.  It is
   an error because the bond atoms are so far apart it is ambiguous how
   it should be defined.

*Bond potential must be defined for SHAKE*
   Cannot use fix shake unless bond potential is defined.

*Bond style hybrid cannot have hybrid as an argument*
   Self-explanatory.

*Bond style hybrid cannot have none as an argument*
   Self-explanatory.

*Bond style hybrid cannot use same bond style twice*
   Self-explanatory.

*Bond style quartic cannot be used with 3,4-body interactions*
   No angle, dihedral, or improper styles can be defined when using
   bond style quartic.

*Bond style quartic requires special\_bonds = 1,1,1*
   This is a restriction of the current bond quartic implementation.

*Bond table parameters did not set N*
   List of bond table parameters must include N setting.

*Bond table values are not increasing*
   The values in the tabulated file must be monotonically increasing.

*Bond\_coeff command before bond\_style is defined*
   Coefficients cannot be set in the data file or via the bond\_coeff
   command until an bond\_style has been assigned.

*Bond\_coeff command before simulation box is defined*
   The bond\_coeff command cannot be used before a read\_data,
   read\_restart, or create\_box command.

*Bond\_coeff command when no bonds allowed*
   The chosen atom style does not allow for bonds to be defined.

*Bond\_style command when no bonds allowed*
   The chosen atom style does not allow for bonds to be defined.

*Bonds assigned incorrectly*
   Bonds read in from the data file were not assigned correctly to atoms.
   This means there is something invalid about the topology definitions.

*Bonds defined but no bond types*
   The data file header lists bonds but no bond types.

*Both sides of boundary must be periodic*
   Cannot specify a boundary as periodic only on the lo or hi side.  Must
   be periodic on both sides.

*Boundary command after simulation box is defined*
   The boundary command cannot be used after a read\_data, read\_restart,
   or create\_box command.

*Box bounds are invalid*
   The box boundaries specified in the read\_data file are invalid.  The
   lo value must be less than the hi value for all 3 dimensions.

*Box command after simulation box is defined*
   The box command cannot be used after a read\_data, read\_restart, or
   create\_box command.

*CPU neighbor lists must be used for ellipsoid/sphere mix*
   When using Gay-Berne or RE-squared pair styles with both ellipsoidal and
   spherical particles, the neighbor list must be built on the CPU

*Can not specify Pxy/Pxz/Pyz in fix box/relax with non-triclinic box*
   Only triclinic boxes can be used with off-diagonal pressure components.
   See the region prism command for details.

*Can not specify Pxy/Pxz/Pyz in fix nvt/npt/nph with non-triclinic box*
   Only triclinic boxes can be used with off-diagonal pressure components.
   See the region prism command for details.

*Can only use -plog with multiple partitions*
   Self-explanatory.  See doc page discussion of command-line switches.

*Can only use -pscreen with multiple partitions*
   Self-explanatory.  See doc page discussion of command-line switches.

*Can only use NEB with 1-processor replicas*
   This is current restriction for NEB as implemented.

*Can only use TAD with 1-processor replicas for NEB*
   This is current restriction for NEB as implemented.

*Cannot (yet) do analytic differentiation with pppm/gpu*
   This is a current restriction of this command.

*Cannot (yet) use K-space slab correction with compute group/group*
   This option is not yet supported.

*Cannot (yet) use Kspace slab correction with compute group/group*
   This option is not yet supported.

*Cannot (yet) use MSM with 2d simulation*
   This feature is not yet supported.

*Cannot (yet) use MSM with triclinic box*
   This feature is not yet supported.

*Cannot (yet) use PPPM with triclinic box*
   This feature is not yet supported.

*Cannot (yet) use PPPMDisp with triclinic box*
   This feature is not yet supported.

*Cannot (yet) use single precision with MSM (remove -DFFT\_SINGLE from Makefile and recompile)*
   Single precision cannot be used with MSM.

*Cannot add atoms to fix move variable*
   Atoms can not be added afterwards to this fix option.

*Cannot append atoms to a triclinic box*
   The simulation box must be defined with edges alligned with the
   Cartesian axes.

*Cannot balance in z dimension for 2d simulation*
   Self-explanatory.

*Cannot change box ortho/triclinic with certain fixes defined*
   This is because those fixes store the shape of the box.  You need to
   use unfix to discard the fix, change the box, then redefine a new
   fix.

*Cannot change box ortho/triclinic with dumps defined*
   This is because some dumps store the shape of the box.  You need to
   use undump to discard the dump, change the box, then redefine a new
   dump.

*Cannot change box tilt factors for orthogonal box*
   Cannot use tilt factors unless the simulation box is non-orthogonal.

*Cannot change box to orthogonal when tilt is non-zero*
   Self-explanatory.

*Cannot change box z boundary to nonperiodic for a 2d simulation*
   Self-explanatory.

*Cannot change dump\_modify every for dump dcd*
   The frequency of writing dump dcd snapshots cannot be changed.

*Cannot change dump\_modify every for dump xtc*
   The frequency of writing dump xtc snapshots cannot be changed.

*Cannot change timestep once fix srd is setup*
   This is because various SRD properties depend on the timestep
   size.

*Cannot change timestep with fix pour*
   This fix pre-computes some values based on the timestep, so it cannot
   be changed during a simulation run.

*Cannot change\_box after reading restart file with per-atom info*
   This is because the restart file info cannot be migrated with the
   atoms.  You can get around this by performing a 0-timestep run which
   will assign the restart file info to actual atoms.

*Cannot change\_box in xz or yz for 2d simulation*
   Self-explanatory.

*Cannot change\_box in z dimension for 2d simulation*
   Self-explanatory.

*Cannot compute initial g\_ewald\_disp*
   LIGGGHTS(R)-PUBLIC failed to compute an initial guess for the PPPM\_disp g\_ewald\_6
   factor that partitions the computation between real space and k-space
   for Disptersion interactions.

*Cannot create an atom map unless atoms have IDs*
   The simulation requires a mapping from global atom IDs to local atoms,
   but the atoms that have been defined have no IDs.

*Cannot create atoms with undefined lattice*
   Must use the lattice command before using the create\_atoms
   command.

*Cannot create/grow a vector/array of pointers for %s*
   LIGGGHTS(R)-PUBLIC code is making an illegal call to the templated memory
   allocaters, to create a vector or array of pointers.

*Cannot create\_atoms after reading restart file with per-atom info*
   The per-atom info was stored to be used when by a fix that you
   may re-define.  If you add atoms before re-defining the fix, then
   there will not be a correct amount of per-atom info.

*Cannot create\_box after simulation box is defined*
   The create\_box command cannot be used after a read\_data, read\_restart,
   or create\_box command.

*Cannot currently use pair reax with pair hybrid*
   This is not yet supported.

*Cannot currently use pppm/gpu with fix balance.*
   Self-explanatory.

*Cannot delete group all*
   Self-explanatory.

*Cannot delete group currently used by a compute*
   Self-explanatory.

*Cannot delete group currently used by a dump*
   Self-explanatory.

*Cannot delete group currently used by a fix*
   Self-explanatory.

*Cannot delete group currently used by atom\_modify first*
   Self-explanatory.

*Cannot displace\_atoms after reading restart file with per-atom info*
   This is because the restart file info cannot be migrated with the
   atoms.  You can get around this by performing a 0-timestep run which
   will assign the restart file info to actual atoms.

*Cannot do GCMC on atoms in atom\_modify first group*
   This is a restriction due to the way atoms are organized in a list to
   enable the atom\_modify first command.

*Cannot dump JPG file*
   LIGGGHTS(R)-PUBLIC was not built with the -DLAMMPS\_JPEG switch in the Makefile.

*Cannot dump sort on atom IDs with no atom IDs defined*
   Self-explanatory.

*Cannot evaporate atoms in atom\_modify first group*
   This is a restriction due to the way atoms are organized in
   a list to enable the atom\_modify first command.

*Cannot find delete\_bonds group ID*
   Group ID used in the delete\_bonds command does not exist.

*Cannot have both pair\_modify shift and tail set to yes*
   These 2 options are contradictory.

*Cannot open -reorder file*
   Self-explanatory.

*Cannot open ADP potential file %s*
   The specified ADP potential file cannot be opened.  Check that the
   path and name are correct.

*Cannot open AIREBO potential file %s*
   The specified AIREBO potential file cannot be opened.  Check that the
   path and name are correct.

*Cannot open BOP potential file %s*
   The specified BOP potential file cannot be opened.  Check that the
   path and name are correct.

*Cannot open COMB potential file %s*
   The specified COMB potential file cannot be opened.  Check that the
   path and name are correct.

*Cannot open EAM potential file %s*
   The specified EAM potential file cannot be opened.  Check that the
   path and name are correct.

*Cannot open EIM potential file %s*
   The specified EIM potential file cannot be opened.  Check that the
   path and name are correct.

*Cannot open LCBOP potential file %s*
   The specified LCBOP potential file cannot be opened.  Check that the
   path and name are correct.

*Cannot open MEAM potential file %s*
   The specified MEAM potential file cannot be opened.  Check that the
   path and name are correct.

*Cannot open Stillinger-Weber potential file %s*
   The specified SW potential file cannot be opened.  Check that the path
   and name are correct.

*Cannot open Tersoff potential file %s*
   The specified Tersoff potential file cannot be opened.  Check that the
   path and name are correct.

*Cannot open balance output file*
   Self-explanatory.

*Cannot open custom file*
   Self-explanatory.

*Cannot open dir to search for restart file*
   Using a "\*" in the name of the restart file will open the current
   directory to search for matching file names.

*Cannot open dump file*
   The output file for the dump command cannot be opened.  Check that the
   path and name are correct.

*Cannot open file %s*
   The specified file cannot be opened.  Check that the path and name are
   correct.

*Cannot open file variable file %s*
   The specified file cannot be opened.  Check that the path and name are
   correct.

*Cannot open fix ave/correlate file %s*
   The specified file cannot be opened.  Check that the path and name are
   correct.

*Cannot open fix ave/histo file %s*
   The specified file cannot be opened.  Check that the path and name are
   correct.

*Cannot open fix ave/spatial file %s*
   The specified file cannot be opened.  Check that the path and name are
   correct.

*Cannot open fix ave/time file %s*
   The specified file cannot be opened.  Check that the path and name are
   correct.

*Cannot open fix balance output file*
   Self-explanatory.

*Cannot open fix poems file %s*
   The specified file cannot be opened.  Check that the path and name are
   correct.

*Cannot open fix print file %s*
   The output file generated by the fix print command cannot be opened

*Cannot open fix qeq/comb file %s*
   The output file for the fix qeq/combs command cannot be opened.
   Check that the path and name are correct.

*Cannot open fix reax/bonds file %s*
   The output file for the fix reax/bonds command cannot be opened.
   Check that the path and name are correct.

*Cannot open fix rigid infile %s*
   The specified file cannot be opened.  Check that the path and name are
   correct.

*Cannot open fix tmd file %s*
   The output file for the fix tmd command cannot be opened.  Check that
   the path and name are correct.

*Cannot open fix ttm file %s*
   The output file for the fix ttm command cannot be opened.  Check that
   the path and name are correct.

*Cannot open gzipped file*
   LIGGGHTS(R)-PUBLIC is attempting to open a gzipped version of the specified file
   but was unsuccessful.  Check that the path and name are correct.

*Cannot open input script %s*
   Self-explanatory.

*Cannot open log.lammps*
   The default LIGGGHTS(R)-PUBLIC log file cannot be opened.  Check that the
   directory you are running in allows for files to be created.

*Cannot open logfile*
   The LIGGGHTS(R)-PUBLIC log file named in a command-line argument cannot be opened.
   Check that the path and name are correct.

*Cannot open logfile %s*
   The LIGGGHTS(R)-PUBLIC log file specified in the input script cannot be opened.
   Check that the path and name are correct.

*Cannot open pair\_write file*
   The specified output file for pair energies and forces cannot be
   opened.  Check that the path and name are correct.

*Cannot open processors output file*
   Self-explanatory.

*Cannot open restart file %s*
   Self-explanatory.

*Cannot open screen file*
   The screen file specified as a command-line argument cannot be
   opened.  Check that the directory you are running in allows for files
   to be created.

*Cannot open universe log file*
   For a multi-partition run, the master log file cannot be opened.
   Check that the directory you are running in allows for files to be
   created.

*Cannot open universe screen file*
   For a multi-partition run, the master screen file cannot be opened.
   Check that the directory you are running in allows for files to be
   created.

*Cannot read\_data after simulation box is defined*
   The read\_data command cannot be used after a read\_data,
   read\_restart, or create\_box command.

*Cannot read\_restart after simulation box is defined*
   The read\_restart command cannot be used after a read\_data,
   read\_restart, or create\_box command.

*Cannot redefine variable as a different style*
   An equal-style variable can be re-defined but only if it was
   originally an equal-style variable.

*Cannot replicate 2d simulation in z dimension*
   The replicate command cannot replicate a 2d simulation in the z
   dimension.

*Cannot replicate with fixes that store atom quantities*
   Either fixes are defined that create and store atom-based vectors or a
   restart file was read which included atom-based vectors for fixes.
   The replicate command cannot duplicate that information for new atoms.
   You should use the replicate command before fixes are applied to the
   system.

*Cannot reset timestep with a dynamic region defined*
   Dynamic regions (see the region command) have a time dependence.
   Thus you cannot change the timestep when one or more of these
   are defined.

*Cannot reset timestep with a time-dependent fix defined*
   You cannot reset the timestep when a fix that keeps track of elapsed
   time is in place.

*Cannot run 2d simulation with nonperiodic Z dimension*
   Use the boundary command to make the z dimension periodic in order to
   run a 2d simulation.

*Cannot set both respa pair and inner/middle/outer*
   In the rRESPA integrator, you must compute pairwise potentials either
   all together (pair), or in pieces (inner/middle/outer).  You can't do
   both.

*Cannot set dump\_modify flush for dump xtc*
   Self-explanatory.

*Cannot set mass for this atom style*
   This atom style does not support mass settings for each atom type.
   Instead they are defined on a per-atom basis in the data file.

*Cannot set meso\_rho for this atom style*
   Self-explanatory.

*Cannot set non-zero image flag for non-periodic dimension*
   Self-explanatory.

*Cannot set non-zero z velocity for 2d simulation*
   Self-explanatory.

*Cannot set quaternion for atom that has none*
   Self-explanatory.

*Cannot set respa middle without inner/outer*
   In the rRESPA integrator, you must define both a inner and outer
   setting in order to use a middle setting.

*Cannot set temperature for fix rigid/nph*
   The temp keyword cannot be specified.

*Cannot set theta for atom that is not a line*
   Self-explanatory.

*Cannot set this attribute for this atom style*
   The attribute being set does not exist for the defined atom style.

*Cannot set variable z velocity for 2d simulation*
   Self-explanatory.

*Cannot skew triclinic box in z for 2d simulation*
   Self-explanatory.

*Cannot use -cuda on without USER-CUDA installed*
   The USER-CUDA package must be installed via "make yes-user-cuda"
   before LIGGGHTS(R)-PUBLIC is built.

*Cannot use -reorder after -partition*
   Self-explanatory.  See doc page discussion of command-line switches.

*Cannot use Ewald with 2d simulation*
   The kspace style ewald cannot be used in 2d simulations.  You can use
   2d Ewald in a 3d simulation; see the kspace\_modify command.

*Cannot use Ewald with triclinic box*
   This feature is not yet supported.

*Cannot use Ewald/disp solver on system with no charge or LJ particles*
   No atoms in system have a non-zero charge or are LJ particles.  Change
   charges or change options of the kspace solver/pair style.

*Cannot use EwaldDisp with 2d simulation*
   This is a current restriction of this command.

*Cannot use NEB unless atom map exists*
   Use the atom\_modify command to create an atom map.

*Cannot use NEB with a single replica*
   Self-explanatory.

*Cannot use NEB with atom\_modify sort enabled*
   This is current restriction for NEB implemented in LIGGGHTS(R)-PUBLIC.

*Cannot use PPPM with 2d simulation*
   The kspace style pppm cannot be used in 2d simulations.  You can use
   2d PPPM in a 3d simulation; see the kspace\_modify command.

*Cannot use PPPMDisp with 2d simulation*
   The kspace style pppm/disp cannot be used in 2d simulations.  You can
   use 2d PPPM in a 3d simulation; see the kspace\_modify command.

*Cannot use PRD with a time-dependent fix defined*
   PRD alters the timestep in ways that will mess up these fixes.

*Cannot use PRD with a time-dependent region defined*
   PRD alters the timestep in ways that will mess up these regions.

*Cannot use PRD with atom\_modify sort enabled*
   This is a current restriction of PRD.  You must turn off sorting,
   which is enabled by default, via the atom\_modify command.

*Cannot use PRD with multi-processor replicas unless atom map exists*
   Use the atom\_modify command to create an atom map.

*Cannot use TAD unless atom map exists for NEB*
   See atom\_modify map command to set this.

*Cannot use TAD with a single replica for NEB*
   NEB requires multiple replicas.

*Cannot use TAD with atom\_modify sort enabled for NEB*
   This is a current restriction of NEB.

*Cannot use a damped dynamics min style with fix box/relax*
   This is a current restriction in LIGGGHTS(R)-PUBLIC.  Use another minimizer
   style.

*Cannot use a damped dynamics min style with per-atom DOF*
   This is a current restriction in LIGGGHTS(R)-PUBLIC.  Use another minimizer
   style.

*Cannot use append/atoms in periodic dimension*
   The boundary style of the face where atoms are added can not be of
   type p (periodic).

*Cannot use compute cluster/atom unless atoms have IDs*
   Atom IDs are used to identify clusters.

*Cannot use cwiggle in variable formula between runs*
   This is a function of elapsed time.

*Cannot use delete\_atoms unless atoms have IDs*
   Your atoms do not have IDs, so the delete\_atoms command cannot be
   used.

*Cannot use delete\_bonds with non-molecular system*
   Your choice of atom style does not have bonds.

*Cannot use fix GPU with USER-CUDA mode enabled*
   You cannot use both the GPU and USER-CUDA packages
   together.  Use one or the other.

*Cannot use fix TMD unless atom map exists*
   Using this fix requires the ability to lookup an atom index, which is
   provided by an atom map.  An atom map does not exist (by default) for
   non-molecular problems.  Using the atom\_modify map command will force
   an atom map to be created.

*Cannot use fix ave/spatial z for 2 dimensional model*
   Self-explanatory.

*Cannot use fix bond/break with non-molecular systems*
   Self-explanatory.

*Cannot use fix bond/create with non-molecular systems*
   Self-explanatory.

*Cannot use fix box/relax on a 2nd non-periodic dimension*
   When specifying an off-diagonal pressure component, the 2nd of the two
   dimensions must be periodic.  E.g. if the xy component is specified,
   then the y dimension must be periodic.

*Cannot use fix box/relax on a non-periodic dimension*
   When specifying a diagonal pressure component, the dimension must be
   periodic.

*Cannot use fix box/relax with both relaxation and scaling on a tilt factor*
   When specifying scaling on a tilt factor component, that component can not
   also be controlled by the barostat. E.g. if scalexy yes is specified and
   also keyword tri or xy, this is wrong.

*Cannot use fix box/relax with tilt factor scaling on a 2nd non-periodic dimension*
   When specifying scaling on a tilt factor component, the 2nd of the two
   dimensions must be periodic.  E.g. if the xy component is specified,
   then the y dimension must be periodic.

*Cannot use fix deform on a shrink-wrapped boundary*
   The x, y, z options cannot be applied to shrink-wrapped
   dimensions.

*Cannot use fix deform tilt on a shrink-wrapped 2nd dim*
   This is because the shrink-wrapping will change the value
   of the strain implied by the tilt factor.

*Cannot use fix deform trate on a box with zero tilt*
   The trate style alters the current strain.

*Cannot use fix enforce2d with 3d simulation*
   Self-explanatory.

*Cannot use fix gcmc in a 2d simulation*
   Fix gcmc is set up to run in 3d only. No 2d simulations with fix gcmc
   are allowed.

*Cannot use fix gcmc with a triclinic box*
   Fix gcmc is set up to run with othogonal boxes only. Simulations with
   triclinic boxes and fix gcmc are not allowed.

*Cannot use fix msst without per-type mass defined*
   Self-explanatory.

*Cannot use fix npt and fix deform on same component of stress tensor*
   This would be changing the same box dimension twice.

*Cannot use fix nvt/npt/nph on a 2nd non-periodic dimension*
   When specifying an off-diagonal pressure component, the 2nd of the two
   dimensions must be periodic.  E.g. if the xy component is specified,
   then the y dimension must be periodic.

*Cannot use fix nvt/npt/nph on a non-periodic dimension*
   When specifying a diagonal pressure component, the dimension must be
   periodic.

*Cannot use fix nvt/npt/nph with both xy dynamics and xy scaling*
   Self-explanatory.

*Cannot use fix nvt/npt/nph with both xz dynamics and xz scaling*
   Self-explanatory.

*Cannot use fix nvt/npt/nph with both yz dynamics and yz scaling*
   Self-explanatory.

*Cannot use fix nvt/npt/nph with xy scaling when y is non-periodic dimension*
   The 2nd dimension in the barostatted tilt factor must be periodic.

*Cannot use fix nvt/npt/nph with xz scaling when z is non-periodic dimension*
   The 2nd dimension in the barostatted tilt factor must be periodic.

*Cannot use fix nvt/npt/nph with yz scaling when z is non-periodic dimension*
   The 2nd dimension in the barostatted tilt factor must be periodic.

*Cannot use fix pour with triclinic box*
   This feature is not yet supported.

*Cannot use fix press/berendsen and fix deform on same component of stress tensor*
   These commands both change the box size/shape, so you cannot use both
   together.

*Cannot use fix press/berendsen on a non-periodic dimension*
   Self-explanatory.

*Cannot use fix press/berendsen with triclinic box*
   Self-explanatory.

*Cannot use fix reax/bonds without pair\_style reax*
   Self-explantory.

*Cannot use fix rigid npt/nph and fix deform on same component of stress tensor*
   This would be changing the same box dimension twice.

*Cannot use fix rigid npt/nph on a non-periodic dimension*
   When specifying a diagonal pressure component, the dimension must be
   periodic.

*Cannot use fix shake with non-molecular system*
   Your choice of atom style does not have bonds.

*Cannot use fix ttm with 2d simulation*
   This is a current restriction of this fix due to the grid it creates.

*Cannot use fix ttm with triclinic box*
   This is a current restriction of this fix due to the grid it creates.

*Cannot use fix wall in periodic dimension*
   Self-explanatory.

*Cannot use fix wall zlo/zhi for a 2d simulation*
   Self-explanatory.

*Cannot use fix wall/reflect in periodic dimension*
   Self-explanatory.

*Cannot use fix wall/reflect zlo/zhi for a 2d simulation*
   Self-explanatory.

*Cannot use fix wall/srd in periodic dimension*
   Self-explanatory.

*Cannot use fix wall/srd more than once*
   Nor is their a need to since multiple walls can be specified
   in one command.

*Cannot use fix wall/srd without fix srd*
   Self-explanatory.

*Cannot use fix wall/srd zlo/zhi for a 2d simulation*
   Self-explanatory.

*Cannot use force/hybrid\_neigh with triclinic box*
   Self-explanatory.

*Cannot use force/neigh with triclinic box*
   This is a current limitation of the GPU implementation
   in LIGGGHTS(R)-PUBLIC.

*Cannot use kspace solver on system with no charge*
   No atoms in system have a non-zero charge.

*Cannot use kspace solver with selected options on system with no charge*
   No atoms in system have a non-zero charge. Change charges or change
   options of the kspace solver/pair style.

*Cannot use lines with fix srd unless overlap is set*
   This is because line segements are connected to each other.

*Cannot use multiple fix wall commands with pair brownian*
   Self-explanatory.

*Cannot use multiple fix wall commands with pair lubricate*
   Self-explanatory.

*Cannot use multiple fix wall commands with pair lubricate/poly*
   Self-explanatory.

*Cannot use multiple fix wall commands with pair lubricateU*
   Self-explanatory.

*Cannot use neigh\_modify exclude with GPU neighbor builds*
   This is a current limitation of the GPU implementation
   in LIGGGHTS(R)-PUBLIC.

*Cannot use neighbor bins - box size << cutoff*
   Too many neighbor bins will be created.  This typically happens when
   the simulation box is very small in some dimension, compared to the
   neighbor cutoff.  Use the "nsq" style instead of "bin" style.

*Cannot use newton pair with born/coul/long/gpu pair style*
   Self-explanatory.

*Cannot use newton pair with born/coul/wolf/gpu pair style*
   Self-explanatory.

*Cannot use newton pair with born/gpu pair style*
   Self-explantory.

*Cannot use newton pair with buck/coul/cut/gpu pair style*
   Self-explanatory.

*Cannot use newton pair with buck/coul/long/gpu pair style*
   Self-explanatory.

*Cannot use newton pair with buck/gpu pair style*
   Self-explanatory.

*Cannot use newton pair with colloid/gpu pair style*
   Self-explanatory.

*Cannot use newton pair with coul/dsf/gpu pair style*
   Self-explanatory.

*Cannot use newton pair with coul/long/gpu pair style*
   Self-explanatory.

*Cannot use newton pair with dipole/cut/gpu pair style*
   Self-explanatory.

*Cannot use newton pair with eam/gpu pair style*
   Self-explanatory.

*Cannot use newton pair with gauss/gpu pair style*
   Self-explanatory.

*Cannot use newton pair with gayberne/gpu pair style*
   Self-explanatory.

*Cannot use newton pair with lj/charmm/coul/long/gpu pair style*
   Self-explanatory.

*Cannot use newton pair with lj/class2/coul/long/gpu pair style*
   Self-explanatory.

*Cannot use newton pair with lj/class2/gpu pair style*
   Self-explanatory.

*Cannot use newton pair with lj/cut/coul/cut/gpu pair style*
   Self-explanatory.

*Cannot use newton pair with lj/cut/coul/debye/gpu pair style*
   Self-explanatory.

*Cannot use newton pair with lj/cut/coul/dsf/gpu pair style*
   Self-explanatory.

*Cannot use newton pair with lj/cut/coul/long/gpu pair style*
   Self-explanatory.

*Cannot use newton pair with lj/cut/gpu pair style*
   Self-explanatory.

*Cannot use newton pair with lj/expand/gpu pair style*
   Self-explanatory.

*Cannot use newton pair with lj96/cut/gpu pair style*
   Self-explanatory.

*Cannot use newton pair with morse/gpu pair style*
   Self-explanatory.

*Cannot use newton pair with resquared/gpu pair style*
   Self-explanatory.

*Cannot use newton pair with table/gpu pair style*
   Self-explanatory.

*Cannot use newton pair with yukawa/colloid/gpu pair style*
   Self-explanatory.

*Cannot use newton pair with yukawa/gpu pair style*
   Self-explanatory.

*Cannot use non-zero forces in an energy minimization*
   Fix setforce cannot be used in this manner.  Use fix addforce
   instead.

*Cannot use nonperiodic boundares with fix ttm*
   This fix requires a fully periodic simulation box.

*Cannot use nonperiodic boundaries with Ewald*
   For kspace style ewald, all 3 dimensions must have periodic boundaries
   unless you use the kspace\_modify command to define a 2d slab with a
   non-periodic z dimension.

*Cannot use nonperiodic boundaries with EwaldDisp*
   For kspace style ewald/disp, all 3 dimensions must have periodic
   boundaries unless you use the kspace\_modify command to define a 2d
   slab with a non-periodic z dimension.

*Cannot use nonperiodic boundaries with PPPM*
   For kspace style pppm, all 3 dimensions must have periodic boundaries
   unless you use the kspace\_modify command to define a 2d slab with a
   non-periodic z dimension.

*Cannot use nonperiodic boundaries with PPPMDisp*
   For kspace style pppm/disp, all 3 dimensions must have periodic
   boundaries unless you use the kspace\_modify command to define a 2d
   slab with a non-periodic z dimension.

*Cannot use order greater than 8 with pppm/gpu.*
   Self-explanatory.

*Cannot use pair hybrid with GPU neighbor builds*
   See documentation for fix gpu.

*Cannot use pair tail corrections with 2d simulations*
   The correction factors are only currently defined for 3d systems.

*Cannot use processors part command without using partitions*
   See the command-line -partition switch.

*Cannot use ramp in variable formula between runs*
   This is because the ramp() function is time dependent.

*Cannot use region INF or EDGE when box does not exist*
   Regions that extend to the box boundaries can only be used after the
   create\_box command has been used.

*Cannot use set atom with no atom IDs defined*
   Atom IDs are not defined, so they cannot be used to identify an atom.

*Cannot use set mol with no molecule IDs defined*
   Self-explanatory.

*Cannot use slab correction with MSM*
   Slab correction can only be used with Ewald and PPPM, not MSM.

*Cannot use swiggle in variable formula between runs*
   This is a function of elapsed time.

*Cannot use tris with fix srd unless overlap is set*
   This is because triangles are connected to each other.

*Cannot use variable energy with constant force in fix addforce*
   This is because for constant force, LIGGGHTS(R)-PUBLIC can compute the change
   in energy directly.

*Cannot use variable every setting for dump dcd*
   The format of DCD dump files requires snapshots be output
   at a constant frequency.

*Cannot use variable every setting for dump xtc*
   The format of this file requires snapshots at regular intervals.

*Cannot use vdisplace in variable formula between runs*
   This is a function of elapsed time.

*Cannot use velocity create loop all unless atoms have IDs*
   Atoms in the simulation to do not have IDs, so this style
   of velocity creation cannot be performed.

*Cannot use wall in periodic dimension*
   Self-explanatory.

*Cannot wiggle and shear fix wall/gran*
   Cannot specify both options at the same time.

*Cannot zero Langevin force of 0 atoms*
   The group has zero atoms, so you cannot request its force
   be zeroed.

*Cannot zero momentum of 0 atoms*
   The collection of atoms for which momentum is being computed has no
   atoms.

*Change\_box command before simulation box is defined*
   Self-explanatory.

*Change\_box volume used incorrectly*
   The "dim volume" option must be used immediately following one or two
   settings for "dim1 ..." (and optionally "dim2 ...") and must be for a
   different dimension, i.e. dim != dim1 and dim != dim2.

*Communicate group != atom\_modify first group*
   Self-explanatory.

*Compute ID for compute atom/molecule does not exist*
   Self-explanatory.

*Compute ID for compute reduce does not exist*
   Self-explanatory.

*Compute ID for compute slice does not exist*
   Self-explanatory.

*Compute ID for fix ave/atom does not exist*
   Self-explanatory.

*Compute ID for fix ave/correlate does not exist*
   Self-explanatory.

*Compute ID for fix ave/histo does not exist*
   Self-explanatory.

*Compute ID for fix ave/spatial does not exist*
   Self-explanatory.

*Compute ID for fix ave/time does not exist*
   Self-explanatory.

*Compute ID for fix store/state does not exist*
   Self-explanatory.

*Compute ID must be alphanumeric or underscore characters*
   Self-explanatory.

*Compute angle/local used when angles are not allowed*
   The atom style does not support angles.

*Compute atom/molecule compute array is accessed out-of-range*
   Self-explanatory.

*Compute atom/molecule compute does not calculate a per-atom array*
   Self-explanatory.

*Compute atom/molecule compute does not calculate a per-atom vector*
   Self-explanatory.

*Compute atom/molecule compute does not calculate per-atom values*
   Self-explanatory.

*Compute atom/molecule fix array is accessed out-of-range*
   Self-explanatory.

*Compute atom/molecule fix does not calculate a per-atom array*
   Self-explanatory.

*Compute atom/molecule fix does not calculate a per-atom vector*
   Self-explanatory.

*Compute atom/molecule fix does not calculate per-atom values*
   Self-explanatory.

*Compute atom/molecule requires molecular atom style*
   Self-explanatory.

*Compute atom/molecule variable is not atom-style variable*
   Self-explanatory.

*Compute body/local requires atom style body*
   Self-explanatory.

*Compute bond/local used when bonds are not allowed*
   The atom style does not support bonds.

*Compute centro/atom requires a pair style be defined*
   This is because the computation of the centro-symmetry values
   uses a pairwise neighbor list.

*Compute cluster/atom cutoff is longer than pairwise cutoff*
   Cannot identify clusters beyond cutoff.

*Compute cluster/atom requires a pair style be defined*
   This is so that the pair style defines a cutoff distance which
   is used to find clusters.

*Compute cna/atom cutoff is longer than pairwise cutoff*
   Self-explantory.

*Compute cna/atom requires a pair style be defined*
   Self-explantory.

*Compute com/molecule requires molecular atom style*
   Self-explanatory.

*Compute contact/atom requires a pair style be defined*
   Self-explantory.

*Compute contact/atom requires atom style sphere*
   Self-explanatory.

*Compute coord/atom cutoff is longer than pairwise cutoff*
   Cannot compute coordination at distances longer than the pair cutoff,
   since those atoms are not in the neighbor list.

*Compute coord/atom requires a pair style be defined*
   Self-explantory.

*Compute damage/atom requires peridynamic potential*
   Damage is a Peridynamic-specific metric.  It requires you
   to be running a Peridynamics simulation.

*Compute dihedral/local used when dihedrals are not allowed*
   The atom style does not support dihedrals.

*Compute does not allow an extra compute or fix to be reset*
   This is an internal LIGGGHTS(R)-PUBLIC error.  Please report it to the
   developers.

*Compute erotate/asphere requires atom style ellipsoid or line or tri*
   Self-explanatory.

*Compute erotate/asphere requires extended particles*
   This compute cannot be used with point paritlces.

*Compute erotate/sphere requires atom style sphere*
   Self-explanatory.

*Compute erotate/sphere/atom requires atom style sphere*
   Self-explanatory.

*Compute event/displace has invalid fix event assigned*
   This is an internal LIGGGHTS(R)-PUBLIC error.  Please report it to the
   developers.

*Compute group/group group ID does not exist*
   Self-explanatory.

*Compute gyration/molecule requires molecular atom style*
   Self-explanatory.

*Compute heat/flux compute ID does not compute ke/atom*
   Self-explanatory.

*Compute heat/flux compute ID does not compute pe/atom*
   Self-explanatory.

*Compute heat/flux compute ID does not compute stress/atom*
   Self-explanatory.

*Compute improper/local used when impropers are not allowed*
   The atom style does not support impropers.

*Compute inertia/molecule requires molecular atom style*
   Self-explanatory.

*Compute msd/molecule requires molecular atom style*
   Self-explanatory.

*Compute nve/asphere requires atom style ellipsoid*
   Self-explanatory.

*Compute nvt/nph/npt asphere requires atom style ellipsoid*
   Self-explanatory.

*Compute pair must use group all*
   Pair styles accumlate energy on all atoms.

*Compute pe must use group all*
   Energies computed by potentials (pair, bond, etc) are computed on all
   atoms.

*Compute pressure must use group all*
   Virial contributions computed by potentials (pair, bond, etc) are
   computed on all atoms.

*Compute pressure temperature ID does not compute temperature*
   The compute ID assigned to a pressure computation must compute
   temperature.

*Compute property/atom for atom property that isn't allocated*
   Self-explanatory.

*Compute property/local cannot use these inputs together*
   Only inputs that generate the same number of datums can be used
   togther.  E.g. bond and angle quantities cannot be mixed.

*Compute property/local for property that isn't allocated*
   Self-explanatory.

*Compute property/molecule requires molecular atom style*
   Self-explanatory.

*Compute rdf requires a pair style be defined*
   Self-explanatory.

*Compute reduce compute array is accessed out-of-range*
   An index for the array is out of bounds.

*Compute reduce compute calculates global values*
   A compute that calculates peratom or local values is required.

*Compute reduce compute does not calculate a local array*
   Self-explanatory.

*Compute reduce compute does not calculate a local vector*
   Self-explanatory.

*Compute reduce compute does not calculate a per-atom array*
   Self-explanatory.

*Compute reduce compute does not calculate a per-atom vector*
   Self-explanatory.

*Compute reduce fix array is accessed out-of-range*
   An index for the array is out of bounds.

*Compute reduce fix calculates global values*
   A fix that calculates peratom or local values is required.

*Compute reduce fix does not calculate a local array*
   Self-explanatory.

*Compute reduce fix does not calculate a local vector*
   Self-explanatory.

*Compute reduce fix does not calculate a per-atom array*
   Self-explanatory.

*Compute reduce fix does not calculate a per-atom vector*
   Self-explanatory.

*Compute reduce replace requires min or max mode*
   Self-explanatory.

*Compute reduce variable is not atom-style variable*
   Self-explanatory.

*Compute slice compute array is accessed out-of-range*
   An index for the array is out of bounds.

*Compute slice compute does not calculate a global array*
   Self-explanatory.

*Compute slice compute does not calculate a global vector*
   Self-explanatory.

*Compute slice compute does not calculate global vector or array*
   Self-explanatory.

*Compute slice compute vector is accessed out-of-range*
   The index for the vector is out of bounds.

*Compute slice fix array is accessed out-of-range*
   An index for the array is out of bounds.

*Compute slice fix does not calculate a global array*
   Self-explanatory.

*Compute slice fix does not calculate a global vector*
   Self-explanatory.

*Compute slice fix does not calculate global vector or array*
   Self-explanatory.

*Compute slice fix vector is accessed out-of-range*
   The index for the vector is out of bounds.

*Compute temp/asphere requires atom style ellipsoid*
   Self-explanatory.

*Compute temp/asphere requires extended particles*
   This compute cannot be used with point paritlces.

*Compute temp/partial cannot use vz for 2d systemx*
   Self-explanatory.

*Compute temp/profile cannot bin z for 2d systems*
   Self-explanatory.

*Compute temp/profile cannot use vz for 2d systemx*
   Self-explanatory.

*Compute temp/sphere requires atom style sphere*
   Self-explanatory.

*Compute ti kspace style does not exist*
   Self-explanatory.

*Compute ti pair style does not exist*
   Self-explanatory.

*Compute ti tail when pair style does not compute tail corrections*
   Self-explanatory.

*Compute used in variable between runs is not current*
   Computes cannot be invoked by a variable in between runs.  Thus they
   must have been evaluated on the last timestep of the previous run in
   order for their value(s) to be accessed.  See the doc page for the
   variable command for more info.

*Compute used in variable thermo keyword between runs is not current*
   Some thermo keywords rely on a compute to calculate their value(s).
   Computes cannot be invoked by a variable in between runs.  Thus they
   must have been evaluated on the last timestep of the previous run in
   order for their value(s) to be accessed.  See the doc page for the
   variable command for more info.

*Compute voronoi/atom not allowed for triclinic boxes*
   This is a current restriction of this command.

*Computed temperature for fix temp/berendsen cannot be 0.0*
   Self-explanatory.

*Computed temperature for fix temp/rescale cannot be 0.0*
   Cannot rescale the temperature to a new value if the current
   temperature is 0.0.

*Could not adjust g\_ewald\_6*
   The Newton-Raphson solver failed to converge to a good value for
   g\_ewald\_6.  This error should not occur for typical problems.  Please
   send an email to the developers.

*Could not compute g\_ewald*
   The Newton-Raphson solver failed to converge to a good value for
   g\_ewald.  This error should not occur for typical problems.  Please
   send an email to the developers.

*Could not compute grid size*
   The code is unable to compute a grid size consistent with the desired
   accuracy.  This error should not occur for typical problems.  Please
   send an email to the developers.

*Could not compute grid size for Coulomb interaction*
   The code is unable to compute a grid size consistent with the desired
   accuracy.  This error should not occur for typical problems.  Please
   send an email to the developers.

*Could not compute grid size for dispersion*
   The code is unable to compute a grid size consistent with the desired
   accuracy.  This error should not occur for typical problems.  Please
   send an email to the developers.

*Could not count initial bonds in fix bond/create*
   Could not find one of the atoms in a bond on this processor.

*Could not create 3d FFT plan*
   The FFT setup for the PPPM solver failed, typically due
   to lack of memory.  This is an unusual error.  Check the
   size of the FFT grid you are requesting.

*Could not create 3d grid of processors*
   The specified constraints did not allow a Px by Py by Pz grid to be
   created where Px \* Py \* Pz = P = total number of processors.

*Could not create 3d remap plan*
   The FFT setup in pppm failed.

*Could not create numa grid of processors*
   The specified constraints did not allow this style of grid to be
   created.  Usually this is because the total processor count is not a
   multiple of the cores/node or the user specified processor count is >
   1 in one of the dimensions.

*Could not create twolevel 3d grid of processors*
   The specified constraints did not allow this style of grid to be
   created.

*Could not find atom\_modify first group ID*
   Self-explanatory.

*Could not find change\_box group ID*
   Group ID used in the change\_box command does not exist.

*Could not find compute ID for PRD*
   Self-explanatory.

*Could not find compute ID for TAD*
   Self-explanatory.

*Could not find compute ID for temperature bias*
   Self-explanatory.

*Could not find compute ID to delete*
   Self-explanatory.

*Could not find compute displace/atom fix ID*
   Self-explanatory.

*Could not find compute event/displace fix ID*
   Self-explanatory.

*Could not find compute group ID*
   Self-explanatory.

*Could not find compute heat/flux compute ID*
   Self-explanatory.

*Could not find compute msd fix ID*
   Self-explanatory.

*Could not find compute pressure temperature ID*
   The compute ID for calculating temperature does not exist.

*Could not find compute\_modify ID*
   Self-explanatory.

*Could not find delete\_atoms group ID*
   Group ID used in the delete\_atoms command does not exist.

*Could not find delete\_atoms region ID*
   Region ID used in the delete\_atoms command does not exist.

*Could not find displace\_atoms group ID*
   Group ID used in the displace\_atoms command does not exist.

*Could not find dump custom compute ID*
   The compute ID needed by dump custom to compute a per-atom quantity
   does not exist.

*Could not find dump custom fix ID*
   Self-explanatory.

*Could not find dump custom variable name*
   Self-explanatory.

*Could not find dump group ID*
   A group ID used in the dump command does not exist.

*Could not find dump local compute ID*
   Self-explanatory.

*Could not find dump local fix ID*
   Self-explanatory.

*Could not find dump modify compute ID*
   Self-explanatory.

*Could not find dump modify fix ID*
   Self-explanatory.

*Could not find dump modify variable name*
   Self-explanatory.

*Could not find fix ID to delete*
   Self-explanatory.

*Could not find fix gcmc rotation group ID*
   Self-explanatory.

*Could not find fix group ID*
   A group ID used in the fix command does not exist.

*Could not find fix msst compute ID*
   Self-explanatory.

*Could not find fix poems group ID*
   A group ID used in the fix poems command does not exist.

*Could not find fix recenter group ID*
   A group ID used in the fix recenter command does not exist.

*Could not find fix rigid group ID*
   A group ID used in the fix rigid command does not exist.

*Could not find fix srd group ID*
   Self-explanatory.

*Could not find fix\_modify ID*
   A fix ID used in the fix\_modify command does not exist.

*Could not find fix\_modify pressure ID*
   The compute ID for computing pressure does not exist.

*Could not find fix\_modify temperature ID*
   The compute ID for computing temperature does not exist.

*Could not find group delete group ID*
   Self-explanatory.

*Could not find set group ID*
   Group ID specified in set command does not exist.

*Could not find thermo compute ID*
   Compute ID specified in thermo\_style command does not exist.

*Could not find thermo custom compute ID*
   The compute ID needed by thermo style custom to compute a requested
   quantity does not exist.

*Could not find thermo custom fix ID*
   The fix ID needed by thermo style custom to compute a requested
   quantity does not exist.

*Could not find thermo custom variable name*
   Self-explanatory.

*Could not find thermo fix ID*
   Fix ID specified in thermo\_style command does not exist.

*Could not find thermo variable name*
   Self-explanatory.

*Could not find thermo\_modify pressure ID*
   The compute ID needed by thermo style custom to compute pressure does
   not exist.

*Could not find thermo\_modify temperature ID*
   The compute ID needed by thermo style custom to compute temperature does
   not exist.

*Could not find undump ID*
   A dump ID used in the undump command does not exist.

*Could not find velocity group ID*
   A group ID used in the velocity command does not exist.

*Could not find velocity temperature ID*
   The compute ID needed by the velocity command to compute temperature
   does not exist.

*Could not find/initialize a specified accelerator device*
   Could not initialize at least one of the devices specified for the gpu
   package

*Could not grab element entry from EIM potential file*
   Self-explanatory

*Could not grab global entry from EIM potential file*
   Self-explanatory.

*Could not grab pair entry from EIM potential file*
   Self-explanatory.

*Coulomb PPPMDisp order < minimum allowed order*
   The default minimum order is 2.  This can be reset by the
   kspace\_modify minorder command.

*Coulomb cut not supported in pair\_style buck/long/coul/coul*
   Must use long-range Coulombic interactions.

*Coulomb cut not supported in pair\_style lj/long/coul/long*
   Must use long-range Coulombic interactions.

*Coulomb cut not supported in pair\_style lj/long/tip4p/long*
   Must use long-range Coulombic interactions.

*Coulomb cutoffs of pair hybrid sub-styles do not match*
   If using a Kspace solver, all Coulomb cutoffs of long pair styles must
   be the same.

*Cound not find dump\_modify ID*
   Self-explanatory.

*Create\_atoms command before simulation box is defined*
   The create\_atoms command cannot be used before a read\_data,
   read\_restart, or create\_box command.

*Create\_atoms region ID does not exist*
   A region ID used in the create\_atoms command does not exist.

*Create\_box region ID does not exist*
   A region ID used in the create\_box command does not exist.

*Create\_box region does not support a bounding box*
   Not all regions represent bounded volumes.  You cannot use
   such a region with the create\_box command.

*Cutoffs missing in pair\_style buck/long/coul/long*
   Self-exlanatory.

*Cutoffs missing in pair\_style lj/long/coul/long*
   Self-explanatory.

*Cyclic loop in joint connections*
   Fix poems cannot (yet) work with coupled bodies whose joints connect
   the bodies in a ring (or cycle).

*Degenerate lattice primitive vectors*
   Invalid set of 3 lattice vectors for lattice command.

*Delete region ID does not exist*
   Self-explanatory.

*Delete\_atoms command before simulation box is defined*
   The delete\_atoms command cannot be used before a read\_data,
   read\_restart, or create\_box command.

*Delete\_atoms cutoff > neighbor cutoff*
   Cannot delete atoms further away than a processor knows about.

*Delete\_atoms requires a pair style be defined*
   This is because atom deletion within a cutoff uses a pairwise
   neighbor list.

*Delete\_bonds command before simulation box is defined*
   The delete\_bonds command cannot be used before a read\_data,
   read\_restart, or create\_box command.

*Delete\_bonds command with no atoms existing*
   No atoms are yet defined so the delete\_bonds command cannot be used.

*Deposition region extends outside simulation box*
   Self-explanatory.

*Did not assign all atoms correctly*
   Atoms read in from a data file were not assigned correctly to
   processors.  This is likely due to some atom coordinates being
   outside a non-periodic simulation box.

*Did not find all elements in MEAM library file*
   The requested elements were not found in the MEAM file.

*Did not find fix shake partner info*
   Could not find bond partners implied by fix shake command.  This error
   can be triggered if the delete\_bonds command was used before fix
   shake, and it removed bonds without resetting the 1-2, 1-3, 1-4
   weighting list via the special keyword.

*Did not find keyword in table file*
   Keyword used in pair\_coeff command was not found in table file.

*Did not set pressure for fix rigid/nph*
   The press keyword must be specified.

*Did not set temperature for fix rigid/nvt*
   The temp keyword must be specified.

*Did not set temperature or pressure for fix rigid/npt*
   The temp and press keywords must be specified.

*Dihedral atom missing in delete\_bonds*
   The delete\_bonds command cannot find one or more atoms in a particular
   dihedral on a particular processor.  The pairwise cutoff is too short
   or the atoms are too far apart to make a valid dihedral.

*Dihedral atom missing in set command*
   The set command cannot find one or more atoms in a particular dihedral
   on a particular processor.  The pairwise cutoff is too short or the
   atoms are too far apart to make a valid dihedral.

*Dihedral atoms %d %d %d %d missing on proc %d at step %ld*
   One or more of 4 atoms needed to compute a particular dihedral are
   missing on this processor.  Typically this is because the pairwise
   cutoff is set too short or the dihedral has blown apart and an atom is
   too far away.

*Dihedral charmm is incompatible with Pair style*
   Dihedral style charmm must be used with a pair style charmm
   in order for the 1-4 epsilon/sigma parameters to be defined.

*Dihedral coeff for hybrid has invalid style*
   Dihedral style hybrid uses another dihedral style as one of its
   coefficients.  The dihedral style used in the dihedral\_coeff command
   or read from a restart file is not recognized.

*Dihedral coeffs are not set*
   No dihedral coefficients have been assigned in the data file or via
   the dihedral\_coeff command.

*Dihedral style hybrid cannot have hybrid as an argument*
   Self-explanatory.

*Dihedral style hybrid cannot have none as an argument*
   Self-explanatory.

*Dihedral style hybrid cannot use same dihedral style twice*
   Self-explanatory.

*Dihedral/improper extent > half of periodic box length*
   This error was detected by the neigh\_modify check yes setting.  It is
   an error because the dihedral atoms are so far apart it is ambiguous
   how it should be defined.

*Dihedral\_coeff command before dihedral\_style is defined*
   Coefficients cannot be set in the data file or via the dihedral\_coeff
   command until an dihedral\_style has been assigned.

*Dihedral\_coeff command before simulation box is defined*
   The dihedral\_coeff command cannot be used before a read\_data,
   read\_restart, or create\_box command.

*Dihedral\_coeff command when no dihedrals allowed*
   The chosen atom style does not allow for dihedrals to be defined.

*Dihedral\_style command when no dihedrals allowed*
   The chosen atom style does not allow for dihedrals to be defined.

*Dihedrals assigned incorrectly*
   Dihedrals read in from the data file were not assigned correctly to
   atoms.  This means there is something invalid about the topology
   definitions.

*Dihedrals defined but no dihedral types*
   The data file header lists dihedrals but no dihedral types.

*Dimension command after simulation box is defined*
   The dimension command cannot be used after a read\_data,
   read\_restart, or create\_box command.

*Dispersion PPPMDisp order has been reduced below minorder*
   This may lead to a larger grid than desired.  See the kspace\_modify overlap
   command to prevent changing of the dipsersion order.

*Displace\_atoms command before simulation box is defined*
   The displace\_atoms command cannot be used before a read\_data,
   read\_restart, or create\_box command.

*Distance must be > 0 for compute event/displace*
   Self-explanatory.

*Divide by 0 in influence function of pair peri/lps*
   This should not normally occur.  It is likely a problem with your
   model.

*Divide by 0 in variable formula*
   Self-explanatory.

*Domain too large for neighbor bins*
   The domain has become extremely large so that neighbor bins cannot be
   used.  Most likely, one or more atoms have been blown out of the
   simulation box to a great distance.

*Double precision is not supported on this accelerator*
   Self-explanatory

*Dump cfg arguments can not mix xs\|ys\|zs with xsu\|ysu\|zsu*
   Self-explanatory.

*Dump cfg arguments must start with 'id type xs ys zs' or 'id type xsu ysu zsu'*
   This is a requirement of the CFG output format.

*Dump cfg requires one snapshot per file*
   Use the wildcard "\*" character in the filename.

*Dump custom and fix not computed at compatible times*
   The fix must produce per-atom quantities on timesteps that dump custom
   needs them.

*Dump custom compute does not calculate per-atom array*
   Self-explanatory.

*Dump custom compute does not calculate per-atom vector*
   Self-explanatory.

*Dump custom compute does not compute per-atom info*
   Self-explanatory.

*Dump custom compute vector is accessed out-of-range*
   Self-explanatory.

*Dump custom fix does not compute per-atom array*
   Self-explanatory.

*Dump custom fix does not compute per-atom info*
   Self-explanatory.

*Dump custom fix does not compute per-atom vector*
   Self-explanatory.

*Dump custom fix vector is accessed out-of-range*
   Self-explanatory.

*Dump custom variable is not atom-style variable*
   Only atom-style variables generate per-atom quantities, needed for
   dump output.

*Dump dcd of non-matching # of atoms*
   Every snapshot written by dump dcd must contain the same # of atoms.

*Dump dcd requires sorting by atom ID*
   Use the dump\_modify sort command to enable this.

*Dump every variable returned a bad timestep*
   The variable must return a timestep greater than the current timestep.

*Dump file does not contain requested snapshot*
   Self-explanatory.

*Dump file is incorrectly formatted*
   Self-explanatory.

*Dump image bond not allowed with no bond types*
   Self-explanatory.

*Dump image cannot perform sorting*
   Self-explanatory.

*Dump image persp option is not yet supported*
   Self-explanatory.

*Dump image requires one snapshot per file*
   Use a "\*" in the filename.

*Dump local and fix not computed at compatible times*
   The fix must produce per-atom quantities on timesteps that dump local
   needs them.

*Dump local attributes contain no compute or fix*
   Self-explanatory.

*Dump local cannot sort by atom ID*
   This is because dump local does not really dump per-atom info.

*Dump local compute does not calculate local array*
   Self-explanatory.

*Dump local compute does not calculate local vector*
   Self-explanatory.

*Dump local compute does not compute local info*
   Self-explanatory.

*Dump local compute vector is accessed out-of-range*
   Self-explanatory.

*Dump local count is not consistent across input fields*
   Every column of output must be the same length.

*Dump local fix does not compute local array*
   Self-explanatory.

*Dump local fix does not compute local info*
   Self-explanatory.

*Dump local fix does not compute local vector*
   Self-explanatory.

*Dump local fix vector is accessed out-of-range*
   Self-explanatory.

*Dump modify bcolor not allowed with no bond types*
   Self-explanatory.

*Dump modify bdiam not allowed with no bond types*
   Self-explanatory.

*Dump modify compute ID does not compute per-atom array*
   Self-explanatory.

*Dump modify compute ID does not compute per-atom info*
   Self-explanatory.

*Dump modify compute ID does not compute per-atom vector*
   Self-explanatory.

*Dump modify compute ID vector is not large enough*
   Self-explanatory.

*Dump modify element names do not match atom types*
   Number of element names must equal number of atom types.

*Dump modify fix ID does not compute per-atom array*
   Self-explanatory.

*Dump modify fix ID does not compute per-atom info*
   Self-explanatory.

*Dump modify fix ID does not compute per-atom vector*
   Self-explanatory.

*Dump modify fix ID vector is not large enough*
   Self-explanatory.

*Dump modify variable is not atom-style variable*
   Self-explanatory.

*Dump sort column is invalid*
   Self-explanatory.

*Dump xtc requires sorting by atom ID*
   Use the dump\_modify sort command to enable this.

*Dump\_modify format string is too short*
   There are more fields to be dumped in a line of output than
   your format string specifies.

*Dump\_modify region ID does not exist*
   Self-explanatory.

*Dumping an atom property that isn't allocated*
   The chosen atom style does not define the per-atom quantity being
   dumped.

*Dumping an atom quantity that isn't allocated*
   Only per-atom quantities that are defined for the atom style being
   used are allowed.

*Duplicate fields in read\_dump command*
   Self-explanatory.

*Duplicate particle in PeriDynamic bond - simulation box is too small*
   This is likely because your box length is shorter than 2 times
   the bond length.

*Electronic temperature dropped below zero*
   Something has gone wrong with the fix ttm electron temperature model.

*Empty brackets in variable*
   There is no variable syntax that uses empty brackets.  Check
   the variable doc page.

*Energy was not tallied on needed timestep*
   You are using a thermo keyword that requires potentials to
   have tallied energy, but they didn't on this timestep.  See the
   variable doc page for ideas on how to make this work.

*Epsilon or sigma reference not set by pair style in PPPMDisp*
   The pair style is not providing the needed epsilon or sigma values.

*Epsilon or sigma reference not set by pair style in ewald/n*
   The pair style is not providing the needed epsilon or sigma values.

*Expected floating point parameter in input script or data file*
   The quantity being read is an integer on non-numeric value.

*Expected floating point parameter in variable definition*
   The quantity being read is a non-numeric value.

*Expected integer parameter in input script or data file*
   The quantity being read is a floating point or non-numeric value.

*Expected integer parameter in variable definition*
   The quantity being read is a floating point or non-numeric value.

*Failed to allocate %ld bytes for array %s*
   Your LIGGGHTS(R)-PUBLIC simulation has run out of memory.  You need to run a
   smaller simulation or on more processors.

*Failed to reallocate %ld bytes for array %s*
   Your LIGGGHTS(R)-PUBLIC simulation has run out of memory.  You need to run a
   smaller simulation or on more processors.

*Fewer SRD bins than processors in some dimension*
   This is not allowed.  Make your SRD bin size smaller.

*File variable could not read value*
   Check the file assigned to the variable.

*Final box dimension due to fix deform is < 0.0*
   Self-explanatory.

*Fix GPU split must be positive for hybrid pair styles*
   Self-explanatory.

*Fix ID for compute atom/molecule does not exist*
   Self-explanatory.

*Fix ID for compute reduce does not exist*
   Self-explanatory.

*Fix ID for compute slice does not exist*
   Self-explanatory.

*Fix ID for fix ave/atom does not exist*
   Self-explanatory.

*Fix ID for fix ave/correlate does not exist*
   Self-explanatory.

*Fix ID for fix ave/histo does not exist*
   Self-explanatory.

*Fix ID for fix ave/spatial does not exist*
   Self-explanatory.

*Fix ID for fix ave/time does not exist*
   Self-explanatory.

*Fix ID for fix store/state does not exist*
   Self-explanatory

*Fix ID for read\_data does not exist*
   Self-explanatory.

*Fix ID must be alphanumeric or underscore characters*
   Self-explanatory.

*Fix SRD no-slip requires atom attribute torque*
   This is because the SRD collisions will impart torque to the solute
   particles.

*Fix SRD: bad bin assignment for SRD advection*
   Something has gone wrong in your SRD model; try using more
   conservative settings.

*Fix SRD: bad search bin assignment*
   Something has gone wrong in your SRD model; try using more
   conservative settings.

*Fix SRD: bad stencil bin for big particle*
   Something has gone wrong in your SRD model; try using more
   conservative settings.

*Fix SRD: too many big particles in bin*
   Reset the ATOMPERBIN parameter at the top of fix\_srd.cpp
   to a larger value, and re-compile the code.

*Fix SRD: too many walls in bin*
   This should not happen unless your system has been setup incorrectly.

*Fix adapt kspace style does not exist*
   Self-explanatory.

*Fix adapt pair style does not exist*
   Self-explanatory

*Fix adapt pair style param not supported*
   The pair style does not know about the parameter you specified.

*Fix adapt requires atom attribute charge*
   The atom style being used does not specify an atom charge.

*Fix adapt requires atom attribute diameter*
   The atom style being used does not specify an atom diameter.

*Fix adapt type pair range is not valid for pair hybrid sub-style*
   Self-explanatory.

*Fix append/atoms requires a lattice be defined*
   Use the lattice command for this purpose.

*Fix ave/atom compute array is accessed out-of-range*
   Self-explanatory.

*Fix ave/atom compute does not calculate a per-atom array*
   Self-explanatory.

*Fix ave/atom compute does not calculate a per-atom vector*
   A compute used by fix ave/atom must generate per-atom values.

*Fix ave/atom compute does not calculate per-atom values*
   A compute used by fix ave/atom must generate per-atom values.

*Fix ave/atom fix array is accessed out-of-range*
   Self-explanatory.

*Fix ave/atom fix does not calculate a per-atom array*
   Self-explanatory.

*Fix ave/atom fix does not calculate a per-atom vector*
   A fix used by fix ave/atom must generate per-atom values.

*Fix ave/atom fix does not calculate per-atom values*
   A fix used by fix ave/atom must generate per-atom values.

*Fix ave/atom missed timestep*
   You cannot reset the timestep to a value beyond where the fix
   expects to next perform averaging.

*Fix ave/atom variable is not atom-style variable*
   A variable used by fix ave/atom must generate per-atom values.

*Fix ave/correlate compute does not calculate a scalar*
   Self-explanatory.

*Fix ave/correlate compute does not calculate a vector*
   Self-explanatory.

*Fix ave/correlate compute vector is accessed out-of-range*
   The index for the vector is out of bounds.

*Fix ave/correlate fix does not calculate a scalar*
   Self-explanatory.

*Fix ave/correlate fix does not calculate a vector*
   Self-explanatory.

*Fix ave/correlate fix vector is accessed out-of-range*
   The index for the vector is out of bounds.

*Fix ave/correlate missed timestep*
   You cannot reset the timestep to a value beyond where the fix
   expects to next perform averaging.

*Fix ave/correlate variable is not equal-style variable*
   Self-explanatory.

*Fix ave/histo cannot input local values in scalar mode*
   Self-explanatory.

*Fix ave/histo cannot input per-atom values in scalar mode*
   Self-explanatory.

*Fix ave/histo compute array is accessed out-of-range*
   Self-explanatory.

*Fix ave/histo compute does not calculate a global array*
   Self-explanatory.

*Fix ave/histo compute does not calculate a global scalar*
   Self-explanatory.

*Fix ave/histo compute does not calculate a global vector*
   Self-explanatory.

*Fix ave/histo compute does not calculate a local array*
   Self-explanatory.

*Fix ave/histo compute does not calculate a local vector*
   Self-explanatory.

*Fix ave/histo compute does not calculate a per-atom array*
   Self-explanatory.

*Fix ave/histo compute does not calculate a per-atom vector*
   Self-explanatory.

*Fix ave/histo compute does not calculate local values*
   Self-explanatory.

*Fix ave/histo compute does not calculate per-atom values*
   Self-explanatory.

*Fix ave/histo compute vector is accessed out-of-range*
   Self-explanatory.

*Fix ave/histo fix array is accessed out-of-range*
   Self-explanatory.

*Fix ave/histo fix does not calculate a global array*
   Self-explanatory.

*Fix ave/histo fix does not calculate a global scalar*
   Self-explanatory.

*Fix ave/histo fix does not calculate a global vector*
   Self-explanatory.

*Fix ave/histo fix does not calculate a local array*
   Self-explanatory.

*Fix ave/histo fix does not calculate a local vector*
   Self-explanatory.

*Fix ave/histo fix does not calculate a per-atom array*
   Self-explanatory.

*Fix ave/histo fix does not calculate a per-atom vector*
   Self-explanatory.

*Fix ave/histo fix does not calculate local values*
   Self-explanatory.

*Fix ave/histo fix does not calculate per-atom values*
   Self-explanatory.

*Fix ave/histo fix vector is accessed out-of-range*
   Self-explanatory.

*Fix ave/histo input is invalid compute*
   Self-explanatory.

*Fix ave/histo input is invalid fix*
   Self-explanatory.

*Fix ave/histo input is invalid variable*
   Self-explanatory.

*Fix ave/histo inputs are not all global, peratom, or local*
   All inputs in a single fix ave/histo command must be of the
   same style.

*Fix ave/histo missed timestep*
   You cannot reset the timestep to a value beyond where the fix
   expects to next perform averaging.

*Fix ave/spatial compute does not calculate a per-atom array*
   Self-explanatory.

*Fix ave/spatial compute does not calculate a per-atom vector*
   A compute used by fix ave/spatial must generate per-atom values.

*Fix ave/spatial compute does not calculate per-atom values*
   A compute used by fix ave/spatial must generate per-atom values.

*Fix ave/spatial compute vector is accessed out-of-range*
   The index for the vector is out of bounds.

*Fix ave/spatial fix does not calculate a per-atom array*
   Self-explanatory.

*Fix ave/spatial fix does not calculate a per-atom vector*
   A fix used by fix ave/spatial must generate per-atom values.

*Fix ave/spatial fix does not calculate per-atom values*
   A fix used by fix ave/spatial must generate per-atom values.

*Fix ave/spatial fix vector is accessed out-of-range*
   The index for the vector is out of bounds.

*Fix ave/spatial for triclinic boxes requires units reduced*
   Self-explanatory.

*Fix ave/spatial missed timestep*
   You cannot reset the timestep to a value beyond where the fix
   expects to next perform averaging.

*Fix ave/spatial settings invalid with changing box*
   If the ave setting is "running" or "window" and the box size/shape
   changes during the simulation, then the units setting must be
   "reduced", else the number of bins may change.

*Fix ave/spatial variable is not atom-style variable*
   A variable used by fix ave/spatial must generate per-atom values.

*Fix ave/time cannot set output array intensive/extensive from these inputs*
   One of more of the vector inputs has individual elements which are
   flagged as intensive or extensive.  Such an input cannot be flagged as
   all intensive/extensive when turned into an array by fix ave/time.

*Fix ave/time cannot use variable with vector mode*
   Variables produce scalar values.

*Fix ave/time columns are inconsistent lengths*
   Self-explanatory.

*Fix ave/time compute array is accessed out-of-range*
   An index for the array is out of bounds.

*Fix ave/time compute does not calculate a scalar*
   Self-explantory.

*Fix ave/time compute does not calculate a vector*
   Self-explantory.

*Fix ave/time compute does not calculate an array*
   Self-explanatory.

*Fix ave/time compute vector is accessed out-of-range*
   The index for the vector is out of bounds.

*Fix ave/time fix array is accessed out-of-range*
   An index for the array is out of bounds.

*Fix ave/time fix does not calculate a scalar*
   Self-explanatory.

*Fix ave/time fix does not calculate a vector*
   Self-explanatory.

*Fix ave/time fix does not calculate an array*
   Self-explanatory.

*Fix ave/time fix vector is accessed out-of-range*
   The index for the vector is out of bounds.

*Fix ave/time missed timestep*
   You cannot reset the timestep to a value beyond where the fix
   expects to next perform averaging.

*Fix ave/time variable is not equal-style variable*
   Self-explanatory.

*Fix balance string is invalid*
   The string can only contain the characters "x", "y", or "z".

*Fix balance string is invalid for 2d simulation*
   The string cannot contain the letter "z".

*Fix bond/break requires special\_bonds = 0,1,1*
   This is a restriction of the current fix bond/break implementation.

*Fix bond/create cutoff is longer than pairwise cutoff*
   This is not allowed because bond creation is done using the
   pairwise neighbor list.

*Fix bond/create requires special\_bonds coul = 0,1,1*
   Self-explanatory.

*Fix bond/create requires special\_bonds lj = 0,1,1*
   Self-explanatory.

*Fix bond/swap cannot use dihedral or improper styles*
   These styles cannot be defined when using this fix.

*Fix bond/swap requires pair and bond styles*
   Self-explanatory.

*Fix bond/swap requires special\_bonds = 0,1,1*
   Self-explanatory.

*Fix box/relax generated negative box length*
   The pressure being applied is likely too large.  Try applying
   it incrementally, to build to the high pressure.

*Fix command before simulation box is defined*
   The fix command cannot be used before a read\_data, read\_restart, or
   create\_box command.

*Fix deform cannot use yz variable with xy*
   The yz setting cannot be a variable if xy deformation is also
   specified.  This is because LIGGGHTS(R)-PUBLIC cannot determine if the yz setting
   will induce a box flip which would be invalid if xy is also changing.

*Fix deform is changing yz too much with xy*
   When both yz and xy are changing, it induces changes in xz if the
   box must flip from one tilt extreme to another.  Thus it is not
   allowed for yz to grow so much that a flip is induced.

*Fix deform tilt factors require triclinic box*
   Cannot deform the tilt factors of a simulation box unless it
   is a triclinic (non-orthogonal) box.

*Fix deform volume setting is invalid*
   Cannot use volume style unless other dimensions are being controlled.

*Fix deposit region cannot be dynamic*
   Only static regions can be used with fix deposit.

*Fix deposit region does not support a bounding box*
   Not all regions represent bounded volumes.  You cannot use
   such a region with the fix deposit command.

*Fix efield requires atom attribute q*
   Self-explanatory.

*Fix evaporate molecule requires atom attribute molecule*
   The atom style being used does not define a molecule ID.

*Fix external callback function not set*
   This must be done by an external program in order to use this fix.

*Fix for fix ave/atom not computed at compatible time*
   Fixes generate their values on specific timesteps.  Fix ave/atom is
   requesting a value on a non-allowed timestep.

*Fix for fix ave/correlate not computed at compatible time*
   Fixes generate their values on specific timesteps.  Fix ave/correlate
   is requesting a value on a non-allowed timestep.

*Fix for fix ave/histo not computed at compatible time*
   Fixes generate their values on specific timesteps.  Fix ave/histo is
   requesting a value on a non-allowed timestep.

*Fix for fix ave/spatial not computed at compatible time*
   Fixes generate their values on specific timesteps.  Fix ave/spatial is
   requesting a value on a non-allowed timestep.

*Fix for fix ave/time not computed at compatible time*
   Fixes generate their values on specific timesteps.  Fix ave/time
   is requesting a value on a non-allowed timestep.

*Fix for fix store/state not computed at compatible time*
   Fixes generate their values on specific timesteps.  Fix store/state
   is requesting a value on a non-allowed timestep.

*Fix freeze requires atom attribute torque*
   The atom style defined does not have this attribute.

*Fix gcmc cannot exchange individual atoms belonging to a molecule*
   This is an error since you should not delete only one atom of a molecule.
   The user has specified atomic (non-molecular) gas exchanges, but an atom
   belonging to a molecule could be deleted.

*Fix gcmc could not find any atoms in the user-supplied template molecule*
   When using the molecule option with fix gcmc, the user must supply a
   template molecule in the usual LIGGGHTS(R)-PUBLIC data file with its molecule id
   specified in the fix gcmc command as the "type" of the exchanged gas.

*Fix gcmc incompatible with given pair\_style*
   Some pair\_styles do not provide single-atom energies, which are needed
   by fix gcmc.

*Fix gcmc incorrect number of atoms per molecule*
   The number of atoms in each gas molecule was not computed correctly.

*Fix gcmc molecule command requires that atoms have molecule attributes*
   Should not choose the GCMC molecule feature if no molecules are being
   simulated. The general molecule flag is off, but GCMC's molecule flag
   is on.

*Fix gcmc ran out of available molecule IDs*
   This is a code limitation where more than MAXSMALLINT (usually around
   two billion) molecules have been created. The code needs to be
   modified to either allow molecule ID recycling or use bigger ints for
   molecule IDs. A work-around is to run shorter simulations.

*Fix gcmc region cannot be dynamic*
   Only static regions can be used with fix gcmc.

*Fix gcmc region does not support a bounding box*
   Not all regions represent bounded volumes.  You cannot use
   such a region with the fix gcmc command.

*Fix gcmc region extends outside simulation box*
   Self-explanatory.

*Fix heat group has no atoms*
   Self-explanatory.

*Fix heat kinetic energy of an atom went negative*
   This will cause the velocity rescaling about to be performed by fix
   heat to be invalid.

*Fix heat kinetic energy went negative*
   This will cause the velocity rescaling about to be performed by fix
   heat to be invalid.

*Fix in variable not computed at compatible time*
   Fixes generate their values on specific timesteps.  The variable is
   requesting the values on a non-allowed timestep.

*Fix langevin angmom requires atom style ellipsoid*
   Self-explanatory.

*Fix langevin angmom requires extended particles*
   This fix option cannot be used with point paritlces.

*Fix langevin omega requires atom style sphere*
   Self-explanatory.

*Fix langevin omega requires extended particles*
   One of the particles has radius 0.0.

*Fix langevin period must be > 0.0*
   The time window for temperature relaxation must be > 0

*Fix langevin variable returned negative temperature*
   Self-explanatory.

*Fix momentum group has no atoms*
   Self-explanatory.

*Fix move cannot define z or vz variable for 2d problem*
   Self-explanatory.

*Fix move cannot rotate aroung non z-axis for 2d problem*
   Self-explanatory.

*Fix move cannot set linear z motion for 2d problem*
   Self-explanatory.

*Fix move cannot set wiggle z motion for 2d problem*
   Self-explanatory.

*Fix msst compute ID does not compute potential energy*
   Self-explanatory.

*Fix msst compute ID does not compute pressure*
   Self-explanatory.

*Fix msst compute ID does not compute temperature*
   Self-explanatory.

*Fix msst requires a periodic box*
   Self-explanatory.

*Fix msst tscale must satisfy 0 <= tscale < 1*
   Self-explanatory.

*Fix npt/nph has tilted box too far in one step - periodic cell is too far from equilibrium state*
   Self-explanatory.  The change in the box tilt is too extreme
   on a short timescale.

*Fix nve/asphere requires extended particles*
   This fix can only be used for particles with a shape setting.

*Fix nve/asphere/noforce requires atom style ellipsoid*
   Self-explanatory.

*Fix nve/asphere/noforce requires extended particles*
   One of the particles is not an ellipsoid.

*Fix nve/body requires atom style body*
   Self-explanatory.

*Fix nve/body requires bodies*
   This fix can only be used for particles that are bodies.

*Fix nve/line can only be used for 2d simulations*
   Self-explanatory.

*Fix nve/line requires atom style line*
   Self-explanatory.

*Fix nve/line requires line particles*
   Self-explanatory.

*Fix nve/sphere requires atom attribute mu*
   An atom style with this attribute is needed.

*Fix nve/sphere requires atom style sphere*
   Self-explanatory.

*Fix nve/sphere requires extended particles*
   This fix can only be used for particles of a finite size.

*Fix nve/tri can only be used for 3d simulations*
   Self-explanatory.

*Fix nve/tri requires atom style tri*
   Self-explanatory.

*Fix nve/tri requires tri particles*
   Self-explanatory.

*Fix nvt/nph/npt asphere requires extended particles*
   The shape setting for a particle in the fix group has shape = 0.0,
   which means it is a point particle.

*Fix nvt/nph/npt sphere requires atom style sphere*
   Self-explanatory.

*Fix nvt/npt/nph damping parameters must be > 0.0*
   Self-explanatory.

*Fix nvt/npt/nph dilate group ID does not exist*
   Self-explanatory.

*Fix nvt/sphere requires extended particles*
   This fix can only be used for particles of a finite size.

*Fix orient/fcc file open failed*
   The fix orient/fcc command could not open a specified file.

*Fix orient/fcc file read failed*
   The fix orient/fcc command could not read the needed parameters from a
   specified file.

*Fix orient/fcc found self twice*
   The neighbor lists used by fix orient/fcc are messed up.  If this
   error occurs, it is likely a bug, so send an email to the
   `developers <http://lammps.sandia.gov/authors.html>`_.

*Fix peri neigh does not exist*
   Somehow a fix that the pair style defines has been deleted.

*Fix pour region ID does not exist*
   Self-explanatory.

*Fix pour region cannot be dynamic*
   Only static regions can be used with fix pour.

*Fix pour region does not support a bounding box*
   Not all regions represent bounded volumes.  You cannot use
   such a region with the fix pour command.

*Fix pour requires atom attributes radius, rmass*
   The atom style defined does not have these attributes.

*Fix press/berendsen damping parameters must be > 0.0*
   Self-explanatory.

*Fix qeq/comb group has no atoms*
   Self-explanatory.

*Fix qeq/comb requires atom attribute q*
   An atom style with charge must be used to perform charge equilibration.

*Fix reax/bonds numbonds > nsbmax\_most*
   The limit of the number of bonds expected by the ReaxFF force field
   was exceeded.

*Fix recenter group has no atoms*
   Self-explanatory.

*Fix restrain requires an atom map, see atom\_modify*
   Self-explanatory.

*Fix rigid atom has non-zero image flag in a non-periodic dimension*
   Image flags for non-periodic dimensions should not be set.

*Fix rigid langevin period must be > 0.0*
   Self-explanatory.

*Fix rigid molecule requires atom attribute molecule*
   Self-explanatory.

*Fix rigid npt/nph dilate group ID does not exist*
   Self-explanatory.

*Fix rigid npt/nph does not yet allow triclinic box*
   Self-explanatory.

*Fix rigid npt/nph period must be > 0.0*
   Self-explanatory.

*Fix rigid nvt/npt/nph damping parameters must be > 0.0*
   Self-explanatory.

*Fix rigid xy torque cannot be on for 2d simulation*
   Self-explanatory.

*Fix rigid z force cannot be on for 2d simulation*
   Self-explanatory.

*Fix rigid/npt period must be > 0.0*
   Self-explanatory.

*Fix rigid/npt temperature order must be 3 or 5*
   Self-explanatory.

*Fix rigid/nvt period must be > 0.0*
   Self-explanatory.

*Fix rigid/nvt temperature order must be 3 or 5*
   Self-explanatory.

*Fix rigid/small atom has non-zero image flag in a non-periodic dimension*
   Image flags for non-periodic dimensions should not be set.

*Fix rigid/small langevin period must be > 0.0*
   Self-explanatory.

*Fix rigid/small requires atom attribute molecule*
   Self-explanatory.

*Fix rigid: Bad principal moments*
   The principal moments of inertia computed for a rigid body
   are not within the required tolerances.

*Fix shake cannot be used with minimization*
   Cannot use fix shake while doing an energy minimization since
   it turns off bonds that should contribute to the energy.

*Fix spring couple group ID does not exist*
   Self-explanatory.

*Fix srd lamda must be >= 0.6 of SRD grid size*
   This is a requirement for accuracy reasons.

*Fix srd requires SRD particles all have same mass*
   Self-explanatory.

*Fix srd requires ghost atoms store velocity*
   Use the communicate vel yes command to enable this.

*Fix srd requires newton pair on*
   Self-explanatory.

*Fix store/state compute array is accessed out-of-range*
   Self-explanatory.

*Fix store/state compute does not calculate a per-atom array*
   The compute calculates a per-atom vector.

*Fix store/state compute does not calculate a per-atom vector*
   The compute calculates a per-atom vector.

*Fix store/state compute does not calculate per-atom values*
   Computes that calculate global or local quantities cannot be used
   with fix store/state.

*Fix store/state fix array is accessed out-of-range*
   Self-explanatory.

*Fix store/state fix does not calculate a per-atom array*
   The fix calculates a per-atom vector.

*Fix store/state fix does not calculate a per-atom vector*
   The fix calculates a per-atom array.

*Fix store/state fix does not calculate per-atom values*
   Fixes that calculate global or local quantities cannot be used with
   fix store/state.

*Fix store/state for atom property that isn't allocated*
   Self-explanatory.

*Fix store/state variable is not atom-style variable*
   Only atom-style variables calculate per-atom quantities.

*Fix temp/berendsen period must be > 0.0*
   Self-explanatory.

*Fix temp/berendsen variable returned negative temperature*
   Self-explanatory.

*Fix temp/rescale variable returned negative temperature*
   Self-explanatory.

*Fix thermal/conductivity swap value must be positive*
   Self-explanatory.

*Fix tmd must come after integration fixes*
   Any fix tmd command must appear in the input script after all time
   integration fixes (nve, nvt, npt).  See the fix tmd documentation for
   details.

*Fix ttm electron temperatures must be > 0.0*
   Self-explanatory.

*Fix ttm electronic\_density must be > 0.0*
   Self-explanatory.

*Fix ttm electronic\_specific\_heat must be > 0.0*
   Self-explanatory.

*Fix ttm electronic\_thermal\_conductivity must be >= 0.0*
   Self-explanatory.

*Fix ttm gamma\_p must be > 0.0*
   Self-explanatory.

*Fix ttm gamma\_s must be >= 0.0*
   Self-explanatory.

*Fix ttm number of nodes must be > 0*
   Self-explanatory.

*Fix ttm v\_0 must be >= 0.0*
   Self-explanatory.

*Fix used in compute atom/molecule not computed at compatible time*
   The fix must produce per-atom quantities on timesteps that the compute
   needs them.

*Fix used in compute reduce not computed at compatible time*
   Fixes generate their values on specific timesteps.  Compute reduce is
   requesting a value on a non-allowed timestep.

*Fix used in compute slice not computed at compatible time*
   Fixes generate their values on specific timesteps.  Compute slice is
   requesting a value on a non-allowed timestep.

*Fix viscosity swap value must be positive*
   Self-explanatory.

*Fix viscosity vtarget value must be positive*
   Self-explanatory.

*Fix wall cutoff <= 0.0*
   Self-explanatory.

*Fix wall/colloid requires atom style sphere*
   Self-explanatory.

*Fix wall/colloid requires extended particles*
   One of the particles has radius 0.0.

*Fix wall/gran is incompatible with Pair style*
   Must use a granular pair style to define the parameters needed for
   this fix.

*Fix wall/gran requires atom style sphere*
   Self-explanatory.

*Fix wall/piston command only available at zlo*
   The face keyword must be zlo.

*Fix wall/region colloid requires atom style sphere*
   Self-explanatory.

*Fix wall/region colloid requires extended particles*
   One of the particles has radius 0.0.

*Fix wall/region cutoff <= 0.0*
   Self-explanatory.

*Fix\_modify pressure ID does not compute pressure*
   The compute ID assigned to the fix must compute pressure.

*Fix\_modify temperature ID does not compute temperature*
   The compute ID assigned to the fix must compute temperature.

*For triclinic deformation, specified target stress must be hydrostatic*
   Triclinic pressure control is allowed using the tri keyword, but
   non-hydrostatic pressure control can not be used in this case.

*Found no restart file matching pattern*
   When using a "\*" in the restart file name, no matching file was found.

*GPU library not compiled for this accelerator*
   Self-explanatory.

*GPU particle split must be set to 1 for this pair style.*
   For this pair style, you cannot run part of the force calculation on
   the host.  See the package command.

*Gmask function in equal-style variable formula*
   Gmask is per-atom operation.

*Gravity changed since fix pour was created*
   Gravity must be static and not dynamic for use with fix pour.

*Gravity must point in -y to use with fix pour in 2d*
   Gravity must be pointing "down" in a 2d box.

*Gravity must point in -z to use with fix pour in 3d*
   Gravity must be pointing "down" in a 3d box, i.e. theta = 180.0.

*Grmask function in equal-style variable formula*
   Grmask is per-atom operation.

*Group ID does not exist*
   A group ID used in the group command does not exist.

*Group ID in variable formula does not exist*
   Self-explanatory.

*Group command before simulation box is defined*
   The group command cannot be used before a read\_data, read\_restart, or
   create\_box command.

*Group region ID does not exist*
   A region ID used in the group command does not exist.

*If read\_dump purges it cannot replace or trim*
   These operations are not compatible.  See the read\_dump doc
   page for details.

*Illegal ... command*
   Self-explanatory.  Check the input script syntax and compare to the
   documentation for the command.  You can use -echo screen as a
   command-line option when running LIGGGHTS(R)-PUBLIC to see the offending line.

*Illegal COMB parameter*
   One or more of the coefficients defined in the potential file is
   invalid.

*Illegal Stillinger-Weber parameter*
   One or more of the coefficients defined in the potential file is
   invalid.

*Illegal Tersoff parameter*
   One or more of the coefficients defined in the potential file is
   invalid.

*Illegal fix gcmc gas mass <= 0*
   The computed mass of the designated gas molecule or atom type was less
   than or equal to zero.

*Illegal fix wall/piston velocity*
   The piston velocity must be positive.

*Illegal integrate style*
   Self-explanatory.

*Illegal number of angle table entries*
   There must be at least 2 table entries.

*Illegal number of bond table entries*
   There must be at least 2 table entries.

*Illegal number of pair table entries*
   There must be at least 2 table entries.

*Illegal simulation box*
   The lower bound of the simulation box is greater than the upper bound.

*Improper atom missing in delete\_bonds*
   The delete\_bonds command cannot find one or more atoms in a particular
   improper on a particular processor.  The pairwise cutoff is too short
   or the atoms are too far apart to make a valid improper.

*Improper atom missing in set command*
   The set command cannot find one or more atoms in a particular improper
   on a particular processor.  The pairwise cutoff is too short or the
   atoms are too far apart to make a valid improper.

*Improper atoms %d %d %d %d missing on proc %d at step %ld*
   One or more of 4 atoms needed to compute a particular improper are
   missing on this processor.  Typically this is because the pairwise
   cutoff is set too short or the improper has blown apart and an atom is
   too far away.

*Improper coeff for hybrid has invalid style*
   Improper style hybrid uses another improper style as one of its
   coefficients.  The improper style used in the improper\_coeff command
   or read from a restart file is not recognized.

*Improper coeffs are not set*
   No improper coefficients have been assigned in the data file or via
   the improper\_coeff command.

*Improper style hybrid cannot have hybrid as an argument*
   Self-explanatory.

*Improper style hybrid cannot have none as an argument*
   Self-explanatory.

*Improper style hybrid cannot use same improper style twice*
   Self-explanatory.

*Improper\_coeff command before improper\_style is defined*
   Coefficients cannot be set in the data file or via the improper\_coeff
   command until an improper\_style has been assigned.

*Improper\_coeff command before simulation box is defined*
   The improper\_coeff command cannot be used before a read\_data,
   read\_restart, or create\_box command.

*Improper\_coeff command when no impropers allowed*
   The chosen atom style does not allow for impropers to be defined.

*Improper\_style command when no impropers allowed*
   The chosen atom style does not allow for impropers to be defined.

*Impropers assigned incorrectly*
   Impropers read in from the data file were not assigned correctly to
   atoms.  This means there is something invalid about the topology
   definitions.

*Impropers defined but no improper types*
   The data file header lists improper but no improper types.

*Inconsistent iparam/jparam values in fix bond/create command*
   If itype and jtype are the same, then their maxbond and newtype
   settings must also be the same.

*Inconsistent line segment in data file*
   The end points of the line segment are not equal distances from the
   center point which is the atom coordinate.

*Inconsistent triangle in data file*
   The centroid of the triangle as defined by the corner points is not
   the atom coordinate.

*Incorrect # of floating-point values in Bodies section of data file*
   See doc page for body style.

*Incorrect # of integer values in Bodies section of data file*
   See doc page for body style.

*Incorrect args for angle coefficients*
   Self-explanatory.  Check the input script or data file.

*Incorrect args for bond coefficients*
   Self-explanatory.  Check the input script or data file.

*Incorrect args for dihedral coefficients*
   Self-explanatory.  Check the input script or data file.

*Incorrect args for improper coefficients*
   Self-explanatory.  Check the input script or data file.

*Incorrect args for pair coefficients*
   Self-explanatory.  Check the input script or data file.

*Incorrect args in pair\_style command*
   Self-explanatory.

*Incorrect atom format in data file*
   Number of values per atom line in the data file is not consistent with
   the atom style.

*Incorrect bonus data format in data file*
   See the read\_data doc page for a description of how various kinds of
   bonus data must be formatted for certain atom styles.

*Incorrect boundaries with slab Ewald*
   Must have periodic x,y dimensions and non-periodic z dimension to use
   2d slab option with Ewald.

*Incorrect boundaries with slab EwaldDisp*
   Must have periodic x,y dimensions and non-periodic z dimension to use
   2d slab option with Ewald.

*Incorrect boundaries with slab PPPM*
   Must have periodic x,y dimensions and non-periodic z dimension to use
   2d slab option with PPPM.

*Incorrect boundaries with slab PPPMDisp*
   Must have periodic x,y dimensions and non-periodic z dimension to use
   2d slab option with PPPM.

*Incorrect element names in ADP potential file*
   The element names in the ADP file do not match those requested.

*Incorrect element names in EAM potential file*
   The element names in the EAM file do not match those requested.

*Incorrect format in COMB potential file*
   Incorrect number of words per line in the potential file.

*Incorrect format in MEAM potential file*
   Incorrect number of words per line in the potential file.

*Incorrect format in NEB coordinate file*
   Self-explanatory.

*Incorrect format in Stillinger-Weber potential file*
   Incorrect number of words per line in the potential file.

*Incorrect format in TMD target file*
   Format of file read by fix tmd command is incorrect.

*Incorrect format in Tersoff potential file*
   Incorrect number of words per line in the potential file.

*Incorrect integer value in Bodies section of data file*
   See doc page for body style.

*Incorrect multiplicity arg for dihedral coefficients*
   Self-explanatory.  Check the input script or data file.

*Incorrect rigid body format in fix rigid file*
   The number of fields per line is not what expected.

*Incorrect sign arg for dihedral coefficients*
   Self-explanatory.  Check the input script or data file.

*Incorrect velocity format in data file*
   Each atom style defines a format for the Velocity section
   of the data file.  The read-in lines do not match.

*Incorrect weight arg for dihedral coefficients*
   Self-explanatory.  Check the input script or data file.

*Index between variable brackets must be positive*
   Self-explanatory.

*Indexed per-atom vector in variable formula without atom map*
   Accessing a value from an atom vector requires the ability to lookup
   an atom index, which is provided by an atom map.  An atom map does not
   exist (by default) for non-molecular problems.  Using the atom\_modify
   map command will force an atom map to be created.

*Initial temperatures not all set in fix ttm*
   Self-explantory.

*Input line quote not followed by whitespace*
   An end quote must be followed by whitespace.

*Insertion region extends outside simulation box*
   Region specified with fix pour command extends outside the global
   simulation box.

*Insufficient Jacobi rotations for POEMS body*
   Eigensolve for rigid body was not sufficiently accurate.

*Insufficient Jacobi rotations for body nparticle*
   Eigensolve for rigid body was not sufficiently accurate.

*Insufficient Jacobi rotations for rigid body*
   Eigensolve for rigid body was not sufficiently accurate.

*Insufficient Jacobi rotations for triangle*
   The calculation of the intertia tensor of the triangle failed.  This
   should not happen if it is a reasonably shaped triangle.

*Insufficient memory on accelerator*
   There is insufficient memory on one of the devices specified for the gpu
   package

*Internal error in atom\_style body*
   This error should not occur.  Contact the developers.

*Invalid -reorder N value*
   Self-explanatory.

*Invalid Boolean syntax in if command*
   Self-explanatory.

*Invalid REAX atom type*
   There is a mis-match between LIGGGHTS(R)-PUBLIC atom types and the elements
   listed in the ReaxFF force field file.

*Invalid angle style*
   The choice of angle style is unknown.

*Invalid angle table length*
   Length must be 2 or greater.

*Invalid angle type in Angles section of data file*
   Angle type must be positive integer and within range of specified angle
   types.

*Invalid angle type index for fix shake*
   Self-explanatory.

*Invalid args for non-hybrid pair coefficients*
   "NULL" is only supported in pair\_coeff calls when using pair hybrid

*Invalid atom ID in Angles section of data file*
   Atom IDs must be positive integers and within range of defined
   atoms.

*Invalid atom ID in Atoms section of data file*
   Atom IDs must be positive integers.

*Invalid atom ID in Bodies section of data file*
   Atom IDs must be positive integers and within range of defined
   atoms.

*Invalid atom ID in Bonds section of data file*
   Atom IDs must be positive integers and within range of defined
   atoms.

*Invalid atom ID in Bonus section of data file*
   Atom IDs must be positive integers and within range of defined
   atoms.

*Invalid atom ID in Dihedrals section of data file*
   Atom IDs must be positive integers and within range of defined
   atoms.

*Invalid atom ID in Impropers section of data file*
   Atom IDs must be positive integers and within range of defined
   atoms.

*Invalid atom ID in Velocities section of data file*
   Atom IDs must be positive integers and within range of defined
   atoms.

*Invalid atom mass for fix shake*
   Mass specified in fix shake command must be > 0.0.

*Invalid atom style*
   The choice of atom style is unknown.

*Invalid atom type in Atoms section of data file*
   Atom types must range from 1 to specified # of types.

*Invalid atom type in create\_atoms command*
   The create\_box command specified the range of valid atom types.
   An invalid type is being requested.

*Invalid atom type in fix bond/create command*
   Self-explanatory.

*Invalid atom type in fix gcmc command*
   The atom type specified in the GCMC command does not exist.

*Invalid atom type in neighbor exclusion list*
   Atom types must range from 1 to Ntypes inclusive.

*Invalid atom type index for fix shake*
   Atom types must range from 1 to Ntypes inclusive.

*Invalid atom types in pair\_write command*
   Atom types must range from 1 to Ntypes inclusive.

*Invalid atom vector in variable formula*
   The atom vector is not recognized.

*Invalid atom\_style body command*
   No body style argument was provided.

*Invalid atom\_style command*
   Self-explanatory.

*Invalid attribute in dump custom command*
   Self-explantory.

*Invalid attribute in dump local command*
   Self-explantory.

*Invalid attribute in dump modify command*
   Self-explantory.

*Invalid body nparticle command*
   Arguments in atom-style command are not correct.

*Invalid body style*
   The choice of body style is unknown.

*Invalid bond style*
   The choice of bond style is unknown.

*Invalid bond table length*
   Length must be 2 or greater.

*Invalid bond type in Bonds section of data file*
   Bond type must be positive integer and within range of specified bond
   types.

*Invalid bond type in fix bond/break command*
   Self-explanatory.

*Invalid bond type in fix bond/create command*
   Self-explanatory.

*Invalid bond type index for fix shake*
   Self-explanatory.  Check the fix shake command in the input script.

*Invalid coeffs for this dihedral style*
   Cannot set class 2 coeffs in data file for this dihedral style.

*Invalid color in dump\_modify command*
   The specified color name was not in the list of recognized colors.
   See the dump\_modify doc page.

*Invalid command-line argument*
   One or more command-line arguments is invalid.  Check the syntax of
   the command you are using to launch LIGGGHTS(R)-PUBLIC.

*Invalid compute ID in variable formula*
   The compute is not recognized.

*Invalid compute style*
   Self-explanatory.

*Invalid cutoff in communicate command*
   Specified cutoff must be >= 0.0.

*Invalid cutoffs in pair\_write command*
   Inner cutoff must be larger than 0.0 and less than outer cutoff.

*Invalid d1 or d2 value for pair colloid coeff*
   Neither d1 or d2 can be < 0.

*Invalid data file section: Angle Coeffs*
   Atom style does not allow angles.

*Invalid data file section: AngleAngle Coeffs*
   Atom style does not allow impropers.

*Invalid data file section: AngleAngleTorsion Coeffs*
   Atom style does not allow dihedrals.

*Invalid data file section: AngleTorsion Coeffs*
   Atom style does not allow dihedrals.

*Invalid data file section: Angles*
   Atom style does not allow angles.

*Invalid data file section: Bodies*
   Atom style does not allow bodies.

*Invalid data file section: Bond Coeffs*
   Atom style does not allow bonds.

*Invalid data file section: BondAngle Coeffs*
   Atom style does not allow angles.

*Invalid data file section: BondBond Coeffs*
   Atom style does not allow angles.

*Invalid data file section: BondBond13 Coeffs*
   Atom style does not allow dihedrals.

*Invalid data file section: Bonds*
   Atom style does not allow bonds.

*Invalid data file section: Dihedral Coeffs*
   Atom style does not allow dihedrals.

*Invalid data file section: Dihedrals*
   Atom style does not allow dihedrals.

*Invalid data file section: Ellipsoids*
   Atom style does not allow ellipsoids.

*Invalid data file section: EndBondTorsion Coeffs*
   Atom style does not allow dihedrals.

*Invalid data file section: Improper Coeffs*
   Atom style does not allow impropers.

*Invalid data file section: Impropers*
   Atom style does not allow impropers.

*Invalid data file section: Lines*
   Atom style does not allow lines.

*Invalid data file section: MiddleBondTorsion Coeffs*
   Atom style does not allow dihedrals.

*Invalid data file section: Triangles*
   Atom style does not allow triangles.

*Invalid delta\_conf in tad command*
   The value must be between 0 and 1 inclusive.

*Invalid density in Atoms section of data file*
   Density value cannot be <= 0.0.

*Invalid diameter in set command*
   Self-explanatory.

*Invalid dihedral style*
   The choice of dihedral style is unknown.

*Invalid dihedral type in Dihedrals section of data file*
   Dihedral type must be positive integer and within range of specified
   dihedral types.

*Invalid dipole length in set command*
   Self-explanatory.

*Invalid displace\_atoms rotate axis for 2d*
   Axis must be in z direction.

*Invalid dump dcd filename*
   Filenames used with the dump dcd style cannot be binary or compressed
   or cause multiple files to be written.

*Invalid dump frequency*
   Dump frequency must be 1 or greater.

*Invalid dump image element name*
   The specified element name was not in the standard list of elements.
   See the dump\_modify doc page.

*Invalid dump image filename*
   The file produced by dump image cannot be binary and must
   be for a single processor.

*Invalid dump image persp value*
   Persp value must be >= 0.0.

*Invalid dump image theta value*
   Theta must be between 0.0 and 180.0 inclusive.

*Invalid dump image zoom value*
   Zoom value must be > 0.0.

*Invalid dump reader style*
   Self-explanatory.

*Invalid dump style*
   The choice of dump style is unknown.

*Invalid dump xtc filename*
   Filenames used with the dump xtc style cannot be binary or compressed
   or cause multiple files to be written.

*Invalid dump xyz filename*
   Filenames used with the dump xyz style cannot be binary or cause files
   to be written by each processor.

*Invalid dump\_modify threshhold operator*
   Operator keyword used for threshold specification in not recognized.

*Invalid entry in -reorder file*
   Self-explanatory.

*Invalid fix ID in variable formula*
   The fix is not recognized.

*Invalid fix ave/time off column*
   Self-explantory.

*Invalid fix box/relax command for a 2d simulation*
   Fix box/relax styles involving the z dimension cannot be used in
   a 2d simulation.

*Invalid fix box/relax command pressure settings*
   If multiple dimensions are coupled, those dimensions must be specified.

*Invalid fix box/relax pressure settings*
   Settings for coupled dimensions must be the same.

*Invalid fix nvt/npt/nph command for a 2d simulation*
   Cannot control z dimension in a 2d model.

*Invalid fix nvt/npt/nph command pressure settings*
   If multiple dimensions are coupled, those dimensions must be
   specified.

*Invalid fix nvt/npt/nph pressure settings*
   Settings for coupled dimensions must be the same.

*Invalid fix press/berendsen for a 2d simulation*
   The z component of pressure cannot be controlled for a 2d model.

*Invalid fix press/berendsen pressure settings*
   Settings for coupled dimensions must be the same.

*Invalid fix rigid npt/nph command for a 2d simulation*
   Cannot control z dimension in a 2d model.

*Invalid fix rigid npt/nph command pressure settings*
   If multiple dimensions are coupled, those dimensions must be
   specified.

*Invalid fix rigid npt/nph pressure settings*
   Settings for coupled dimensions must be the same.

*Invalid fix style*
   The choice of fix style is unknown.

*Invalid flag in force field section of restart file*
   Unrecognized entry in restart file.

*Invalid flag in header section of restart file*
   Unrecognized entry in restart file.

*Invalid flag in type arrays section of restart file*
   Unrecognized entry in restart file.

*Invalid format in Bodies section of data file*
   The specified number of integer or floating point values does not
   appear.

*Invalid frequency in temper command*
   Nevery must be > 0.

*Invalid group ID in neigh\_modify command*
   A group ID used in the neigh\_modify command does not exist.

*Invalid group function in variable formula*
   Group function is not recognized.

*Invalid group in communicate command*
   Self-explanatory.

*Invalid image color range*
   The lo value in the range is larger than the hi value.

*Invalid image up vector*
   Up vector cannot be (0,0,0).

*Invalid immediate variable*
   Syntax of immediate value is incorrect.

*Invalid improper style*
   The choice of improper style is unknown.

*Invalid improper type in Impropers section of data file*
   Improper type must be positive integer and within range of specified
   improper types.

*Invalid index for non-body particles in compute body/local command*
   Only indices 1,2,3 can be used for non-body particles.

*Invalid index in compute body/local command*
   Self-explanatory.

*Invalid keyword in angle table parameters*
   Self-explanatory.

*Invalid keyword in bond table parameters*
   Self-explanatory.

*Invalid keyword in compute angle/local command*
   Self-explanatory.

*Invalid keyword in compute bond/local command*
   Self-explanatory.

*Invalid keyword in compute dihedral/local command*
   Self-explanatory.

*Invalid keyword in compute improper/local command*
   Self-explanatory.

*Invalid keyword in compute pair/local command*
   Self-explanatory.

*Invalid keyword in compute property/atom command*
   Self-explanatory.

*Invalid keyword in compute property/local command*
   Self-explanatory.

*Invalid keyword in compute property/molecule command*
   Self-explanatory.

*Invalid keyword in dump cfg command*
   Self-explanatory.

*Invalid keyword in pair table parameters*
   Keyword used in list of table parameters is not recognized.

*Invalid keyword in thermo\_style custom command*
   One or more specified keywords are not recognized.

*Invalid kspace style*
   The choice of kspace style is unknown.

*Invalid length in set command*
   Self-explanatory.

*Invalid mass in set command*
   Self-explanatory.

*Invalid mass line in data file*
   Self-explanatory.

*Invalid mass value*
   Self-explanatory.

*Invalid math function in variable formula*
   Self-explanatory.

*Invalid math/group/special function in variable formula*
   Self-explanatory.

*Invalid option in lattice command for non-custom style*
   Certain lattice keywords are not supported unless the
   lattice style is "custom".

*Invalid order of forces within respa levels*
   For respa, ordering of force computations within respa levels must
   obey certain rules.  E.g. bonds cannot be compute less frequently than
   angles, pairwise forces cannot be computed less frequently than
   kspace, etc.

*Invalid pair style*
   The choice of pair style is unknown.

*Invalid pair table cutoff*
   Cutoffs in pair\_coeff command are not valid with read-in pair table.

*Invalid pair table length*
   Length of read-in pair table is invalid

*Invalid partitions in processors part command*
   Valid partitions are numbered 1 to N and the sender and receiver
   cannot be the same partition.

*Invalid radius in Atoms section of data file*
   Radius must be >= 0.0.

*Invalid random number seed in fix ttm command*
   Random number seed must be a prime number greater 10000.

*Invalid random number seed in set command*
   Random number seed must be a prime number greater 10000.

*Invalid region style*
   The choice of region style is unknown.

*Invalid replace values in compute reduce*
   Self-explanatory.

*Invalid rigid body ID in fix rigid file*
   The ID does not match the number or an existing ID of rigid bodies
   that are defined by the fix rigid command.

*Invalid run command N value*
   The number of timesteps must fit in a 32-bit integer.  If you want to
   run for more steps than this, perform multiple shorter runs.

*Invalid run command start/stop value*
   Self-explanatory.

*Invalid run command upto value*
   Self-explanatory.

*Invalid seed for Marsaglia random # generator*
   The initial seed for this random number generator must be a prime
   number greater 10000 and smaller than 900 million.

*Invalid seed for Park random # generator*
   The initial seed for this random number generator must be a prime
   number greater 10000.

*Invalid shape in Ellipsoids section of data file*
   Self-explanatory.

*Invalid shape in Triangles section of data file*
   Two or more of the triangle corners are duplicate points.

*Invalid shape in set command*
   Self-explanatory.

*Invalid shear direction for fix wall/gran*
   Self-explanatory.

*Invalid special function in variable formula*
   Self-explanatory.

*Invalid style in pair\_write command*
   Self-explanatory.  Check the input script.

*Invalid syntax in variable formula*
   Self-explanatory.

*Invalid t\_event in prd command*
   Self-explanatory.

*Invalid t\_event in tad command*
   The value must be greater than 0.

*Invalid thermo keyword in variable formula*
   The keyword is not recognized.

*Invalid tmax in tad command*
   The value must be greater than 0.0.

*Invalid type for mass set*
   Mass command must set a type from 1-N where N is the number of atom
   types.

*Invalid value in set command*
   The value specified for the setting is invalid, likely because it is
   too small or too large.

*Invalid variable evaluation in variable formula*
   A variable used in a formula could not be evaluated.

*Invalid variable in next command*
   Self-explanatory.

*Invalid variable in special function next*
   Only file-style variables can be used with the next() function.

*Invalid variable name*
   Variable name used in an input script line is invalid.

*Invalid variable name in variable formula*
   Variable name is not recognized.

*Invalid variable style with next command*
   Variable styles *equal* and *world* cannot be used in a next
   command.

*Invalid wiggle direction for fix wall/gran*
   Self-explanatory.

*Invoked angle equil angle on angle style none*
   Self-explanatory.

*Invoked angle single on angle style none*
   Self-explanatory.

*Invoked bond equil distance on bond style none*
   Self-explanatory.

*Invoked bond single on bond style none*
   Self-explanatory.

*Invoked pair single on pair style none*
   A command (e.g. a dump) attempted to invoke the single() function on a
   pair style none, which is illegal.  You are probably attempting to
   compute per-atom quantities with an undefined pair style.

*KIM neighbor iterator exceeded range*
   This should not happen.  It likely indicates a bug
   in the KIM implementation of the interatomic potential
   where it is requesting neighbors incorrectly.

*KSpace accuracy must be > 0*
   The kspace accuracy designated in the input must be greater than zero.

*KSpace accuracy too large to estimate G vector*
   Reduce the accuracy request or specify gwald explicitly
   via the kspace\_modify command.

*KSpace accuracy too low*
   Requested accuracy must be less than 1.0.

*KSpace solver requires a pair style*
   No pair style is defined.

*KSpace style has not yet been set*
   Cannot use kspace\_modify command until a kspace style is set.

*KSpace style is incompatible with Pair style*
   Setting a kspace style requires that a pair style with a long-range
   Coulombic or dispersion component be used.

*Keyword %s in MEAM parameter file not recognized*
   Self-explanatory.

*Kspace style does not support compute group/group*
   Self-explanatory.

*Kspace style pppm/disp/tip4p requires newton on*
   Self-explanatory.

*Kspace style pppm/tip4p requires newton on*
   Self-explanatory.

*Kspace style requires atom attribute q*
   The atom style defined does not have these attributes.

*Kspace style with selected options requires atom attribute q*
   The atom style defined does not have these attributes.
   Change the atom style or switch of the coulomb solver.

*LIGGGHTS(R)-PUBLIC unit\_style lj not supported by KIM models*
   Self-explanatory. Check the input script or data file.

*LJ6 off not supported in pair\_style buck/long/coul/long*
   Self-exlanatory.

*Label wasn't found in input script*
   Self-explanatory.

*Lattice orient vectors are not orthogonal*
   The three specified lattice orientation vectors must be mutually
   orthogonal.

*Lattice orient vectors are not right-handed*
   The three specified lattice orientation vectors must create a
   right-handed coordinate system such that a1 cross a2 = a3.

*Lattice primitive vectors are collinear*
   The specified lattice primitive vectors do not for a unit cell with
   non-zero volume.

*Lattice settings are not compatible with 2d simulation*
   One or more of the specified lattice vectors has a non-zero z
   component.

*Lattice spacings are invalid*
   Each x,y,z spacing must be > 0.

*Lattice style incompatible with simulation dimension*
   2d simulation can use sq, sq2, or hex lattice.  3d simulation can use
   sc, bcc, or fcc lattice.

*Log of zero/negative value in variable formula*
   Self-explanatory.

*Lost atoms via balance: original %ld current %ld*
   This should not occur.  Report the problem to the developers.

*Lost atoms: original %ld current %ld*
   Lost atoms are checked for each time thermo output is done.  See the
   thermo\_modify lost command for options.  Lost atoms usually indicate
   bad dynamics, e.g. atoms have been blown far out of the simulation
   box, or moved futher than one processor's sub-domain away before
   reneighboring.

*MEAM library error %d*
   A call to the MEAM Fortran library returned an error.

*MPI\_LMP\_BIGINT and bigint in lmptype.h are not compatible*
   The size of the MPI datatype does not match the size of a bigint.

*MPI\_LMP\_TAGINT and tagint in lmptype.h are not compatible*
   The size of the MPI datatype does not match the size of a tagint.

*MSM grid is too large*
   The global MSM grid is larger than OFFSET in one or more dimensions.
   OFFSET is currently set to 16384.  You likely need to decrease the
   requested accuracy.

*MSM order must be 4, 6, 8, or 10*
   This is a limitation of the MSM implementation in LIGGGHTS(R)-PUBLIC:
   the MSM order can only be 4, 6, 8, or 10.

*Mass command before simulation box is defined*
   The mass command cannot be used before a read\_data, read\_restart, or
   create\_box command.

*Min\_style command before simulation box is defined*
   The min\_style command cannot be used before a read\_data, read\_restart,
   or create\_box command.

*Minimization could not find thermo\_pe compute*
   This compute is created by the thermo command.  It must have been
   explicitly deleted by a uncompute command.

*Minimize command before simulation box is defined*
   The minimize command cannot be used before a read\_data, read\_restart,
   or create\_box command.

*Mismatched brackets in variable*
   Self-explanatory.

*Mismatched compute in variable formula*
   A compute is referenced incorrectly or a compute that produces per-atom
   values is used in an equal-style variable formula.

*Mismatched fix in variable formula*
   A fix is referenced incorrectly or a fix that produces per-atom
   values is used in an equal-style variable formula.

*Mismatched variable in variable formula*
   A variable is referenced incorrectly or an atom-style variable that
   produces per-atom values is used in an equal-style variable
   formula.

*Modulo 0 in variable formula*
   Self-explanatory.

*Molecular data file has too many atoms*
   These kids of data files are currently limited to a number
   of atoms that fits in a 32-bit integer.

*Molecule count changed in compute atom/molecule*
   Number of molecules must remain constant over time.

*Molecule count changed in compute com/molecule*
   Number of molecules must remain constant over time.

*Molecule count changed in compute gyration/molecule*
   Number of molecules must remain constant over time.

*Molecule count changed in compute inertia/molecule*
   Number of molecules must remain constant over time.

*Molecule count changed in compute msd/molecule*
   Number of molecules must remain constant over time.

*Molecule count changed in compute property/molecule*
   Number of molecules must remain constant over time.

*More than one fix deform*
   Only one fix deform can be defined at a time.

*More than one fix freeze*
   Only one of these fixes can be defined, since the granular pair
   potentials access it.

*More than one fix shake*
   Only one fix shake can be defined.

*Must define angle\_style before Angle Coeffs*
   Must use an angle\_style command before reading a data file that
   defines Angle Coeffs.

*Must define angle\_style before BondAngle Coeffs*
   Must use an angle\_style command before reading a data file that
   defines Angle Coeffs.

*Must define angle\_style before BondBond Coeffs*
   Must use an angle\_style command before reading a data file that
   defines Angle Coeffs.

*Must define bond\_style before Bond Coeffs*
   Must use a bond\_style command before reading a data file that
   defines Bond Coeffs.

*Must define dihedral\_style before AngleAngleTorsion Coeffs*
   Must use a dihedral\_style command before reading a data file that
   defines AngleAngleTorsion Coeffs.

*Must define dihedral\_style before AngleTorsion Coeffs*
   Must use a dihedral\_style command before reading a data file that
   defines AngleTorsion Coeffs.

*Must define dihedral\_style before BondBond13 Coeffs*
   Must use a dihedral\_style command before reading a data file that
   defines BondBond13 Coeffs.

*Must define dihedral\_style before Dihedral Coeffs*
   Must use a dihedral\_style command before reading a data file that
   defines Dihedral Coeffs.

*Must define dihedral\_style before EndBondTorsion Coeffs*
   Must use a dihedral\_style command before reading a data file that
   defines EndBondTorsion Coeffs.

*Must define dihedral\_style before MiddleBondTorsion Coeffs*
   Must use a dihedral\_style command before reading a data file that
   defines MiddleBondTorsion Coeffs.

*Must define improper\_style before AngleAngle Coeffs*
   Must use an improper\_style command before reading a data file that
   defines AngleAngle Coeffs.

*Must define improper\_style before Improper Coeffs*
   Must use an improper\_style command before reading a data file that
   defines Improper Coeffs.

*Must define pair\_style before Pair Coeffs*
   Must use a pair\_style command before reading a data file that defines
   Pair Coeffs.

*Must have more than one processor partition to temper*
   Cannot use the temper command with only one processor partition.  Use
   the -partition command-line option.

*Must read Atoms before Angles*
   The Atoms section of a data file must come before an Angles section.

*Must read Atoms before Bodies*
   The Atoms section of a data file must come before a Bodies section.

*Must read Atoms before Bonds*
   The Atoms section of a data file must come before a Bonds section.

*Must read Atoms before Dihedrals*
   The Atoms section of a data file must come before a Dihedrals section.

*Must read Atoms before Ellipsoids*
   The Atoms section of a data file must come before a Ellipsoids
   section.

*Must read Atoms before Impropers*
   The Atoms section of a data file must come before an Impropers
   section.

*Must read Atoms before Lines*
   The Atoms section of a data file must come before a Lines section.

*Must read Atoms before Triangles*
   The Atoms section of a data file must come before a Triangles section.

*Must read Atoms before Velocities*
   The Atoms section of a data file must come before a Velocities
   section.

*Must set both respa inner and outer*
   Cannot use just the inner or outer option with respa without using the
   other.

*Must shrink-wrap piston boundary*
   The boundary style of the face where the piston is applied must be of
   type s (shrink-wrapped).

*Must specify a region in fix deposit*
   The region keyword must be specified with this fix.

*Must specify a region in fix pour*
   The region keyword must be specified with this fix.

*Must use -in switch with multiple partitions*
   A multi-partition simulation cannot read the input script from stdin.
   The -in command-line option must be used to specify a file.

*Must use a block or cylinder region with fix pour*
   Self-explanatory.

*Must use a block region with fix pour for 2d simulations*
   Self-explanatory.

*Must use a bond style with TIP4P potential*
   TIP4P potentials assume bond lengths in water are constrained
   by a fix shake command.

*Must use a molecular atom style with fix poems molecule*
   Self-explanatory.

*Must use a z-axis cylinder with fix pour*
   The axis of the cylinder region used with the fix pour command must
   be oriented along the z dimension.

*Must use an angle style with TIP4P potential*
   TIP4P potentials assume angles in water are constrained by a fix shake
   command.

*Must use atom style with molecule IDs with fix bond/swap*
   Self-explanatory.

*Must use pair\_style comb with fix qeq/comb*
   Self-explanatory.

*Must use variable energy with fix addforce*
   Must define an energy vartiable when applyting a dynamic
   force during minimization.

*NEB command before simulation box is defined*
   Self-explanatory.

*NEB requires damped dynamics minimizer*
   Use a different minimization style.

*NEB requires use of fix neb*
   Self-explanatory.

*NL ramp in wall/piston only implemented in zlo for now*
   The ramp keyword can only be used for piston applied to face zlo.

*Needed bonus data not in data file*
   Some atom styles require bonus data.  See the read\_data doc page for
   details.

*Needed topology not in data file*
   The header of the data file indicated that bonds or angles or
   dihedrals or impropers would be included, but they were not present.

*Neigh\_modify exclude molecule requires atom attribute molecule*
   Self-explanatory.

*Neigh\_modify include group != atom\_modify first group*
   Self-explanatory.

*Neighbor delay must be 0 or multiple of every setting*
   The delay and every parameters set via the neigh\_modify command are
   inconsistent.  If the delay setting is non-zero, then it must be a
   multiple of the every setting.

*Neighbor include group not allowed with ghost neighbors*
   This is a current restriction within LIGGGHTS(R)-PUBLIC.

*Neighbor list overflow, boost neigh\_modify one*
   There are too many neighbors of a single atom.  Use the neigh\_modify
   command to increase the max number of neighbors allowed for one atom.
   You may also want to boost the page size.

*Neighbor list overflow, boost neigh\_modify one or page*
   There are too many neighbors of a single atom.  Use the neigh\_modify
   command to increase the neighbor page size and the max number of
   neighbors allowed for one atom.

*Neighbor multi not yet enabled for ghost neighbors*
   This is a current restriction within LIGGGHTS(R)-PUBLIC.

*Neighbor multi not yet enabled for granular*
   Self-explanatory.

*Neighbor multi not yet enabled for rRESPA*
   Self-explanatory.

*Neighbor page size must be >= 10x the one atom setting*
   This is required to prevent wasting too much memory.

*New bond exceeded bonds per atom in fix bond/create*
   See the read\_data command for info on setting the "extra bond per
   atom" header value to allow for additional bonds to be formed.

*New bond exceeded special list size in fix bond/create*
   See the special\_bonds extra command for info on how to leave space in
   the special bonds list to allow for additional bonds to be formed.

*Newton bond change after simulation box is defined*
   The newton command cannot be used to change the newton bond value
   after a read\_data, read\_restart, or create\_box command.

*No Kspace style defined for compute group/group*
   Self-explanatory.

*No OpenMP support compiled in*
   An OpenMP flag is set, but LIGGGHTS(R)-PUBLIC was not built with
   OpenMP support.

*No angle style is defined for compute angle/local*
   Self-explanatory.

*No angles allowed with this atom style*
   Self-explanatory.  Check data file.

*No atoms in data file*
   The header of the data file indicated that atoms would be included,
   but they were not present.

*No basis atoms in lattice*
   Basis atoms must be defined for lattice style user.

*No bodies allowed with this atom style*
   Self-explanatory.  Check data file.

*No bond style is defined for compute bond/local*
   Self-explanatory.

*No bonds allowed with this atom style*
   Self-explanatory.  Check data file.

*No box information in dump. You have to use 'box no'*
   Self-explanatory.

*No dihedral style is defined for compute dihedral/local*
   Self-explanatory.

*No dihedrals allowed with this atom style*
   Self-explanatory.  Check data file.

*No dump custom arguments specified*
   The dump custom command requires that atom quantities be specified to
   output to dump file.

*No dump local arguments specified*
   Self-explanatory.

*No ellipsoids allowed with this atom style*
   Self-explanatory.  Check data file.

*No fix gravity defined for fix pour*
   Cannot add poured particles without gravity to move them.

*No improper style is defined for compute improper/local*
   Self-explanatory.

*No impropers allowed with this atom style*
   Self-explanatory.  Check data file.

*No lines allowed with this atom style*
   Self-explanatory.  Check data file.

*No matching element in ADP potential file*
   The ADP potential file does not contain elements that match the
   requested elements.

*No matching element in EAM potential file*
   The EAM potential file does not contain elements that match the
   requested elements.

*No overlap of box and region for create\_atoms*
   Self-explanatory.

*No pair hbond/dreiding coefficients set*
   Self-explanatory.

*No pair style defined for compute group/group*
   Cannot calculate group interactions without a pair style defined.

*No pair style is defined for compute pair/local*
   Self-explanatory.

*No pair style is defined for compute property/local*
   Self-explanatory.

*No rigid bodies defined*
   The fix specification did not end up defining any rigid bodies.

*No triangles allowed with this atom style*
   Self-explanatory.  Check data file.

*Non digit character between brackets in variable*
   Self-explantory.

*Non integer # of swaps in temper command*
   Swap frequency in temper command must evenly divide the total # of
   timesteps.

*Nprocs not a multiple of N for -reorder*
   Self-explanatory.

*Numeric index is out of bounds*
   A command with an argument that specifies an integer or range of
   integers is using a value that is less than 1 or greater than the
   maximum allowed limit.

*One or more atoms belong to multiple rigid bodies*
   Two or more rigid bodies defined by the fix rigid command cannot
   contain the same atom.

*One or zero atoms in rigid body*
   Any rigid body defined by the fix rigid command must contain 2 or more
   atoms.

*Only one cutoff allowed when requesting all long*
   Self-explanatory.

*Only zhi currently implemented for fix append/atoms*
   Self-explanatory.

*Out of range atoms - cannot compute MSM*
   One or more atoms are attempting to map their charge to a MSM grid point
   that is not owned by a processor.  This is likely for one of two
   reasons, both of them bad.  First, it may mean that an atom near the
   boundary of a processor's sub-domain has moved more than 1/2 the
   :doc:`neighbor skin distance <neighbor>` without neighbor lists being
   rebuilt and atoms being migrated to new processors.  This also means
   you may be missing pairwise interactions that need to be computed.
   The solution is to change the re-neighboring criteria via the
   `neigh\_modify <neigh_modify>`_ command.  The safest settings are "delay 0
   every 1 check yes".  Second, it may mean that an atom has moved far
   outside a processor's sub-domain or even the entire simulation box.
   This indicates bad physics, e.g. due to highly overlapping atoms, too
   large a timestep, etc.

*Out of range atoms - cannot compute PPPM*
   One or more atoms are attempting to map their charge to a PPPM grid
   point that is not owned by a processor.  This is likely for one of two
   reasons, both of them bad.  First, it may mean that an atom near the
   boundary of a processor's sub-domain has moved more than 1/2 the
   :doc:`neighbor skin distance <neighbor>` without neighbor lists being
   rebuilt and atoms being migrated to new processors.  This also means
   you may be missing pairwise interactions that need to be computed.
   The solution is to change the re-neighboring criteria via the
   `neigh\_modify <neigh_modify>`_ command.  The safest settings are "delay 0
   every 1 check yes".  Second, it may mean that an atom has moved far
   outside a processor's sub-domain or even the entire simulation box.
   This indicates bad physics, e.g. due to highly overlapping atoms, too
   large a timestep, etc.

*Out of range atoms - cannot compute PPPMDisp*
   One or more atoms are attempting to map their charge to a PPPM grid
   point that is not owned by a processor.  This is likely for one of two
   reasons, both of them bad.  First, it may mean that an atom near the
   boundary of a processor's sub-domain has moved more than 1/2 the
   :doc:`neighbor skin distance <neighbor>` without neighbor lists being
   rebuilt and atoms being migrated to new processors.  This also means
   you may be missing pairwise interactions that need to be computed.
   The solution is to change the re-neighboring criteria via the
   `neigh\_modify <neigh_modify>`_ command.  The safest settings are "delay 0
   every 1 check yes".  Second, it may mean that an atom has moved far
   outside a processor's sub-domain or even the entire simulation box.
   This indicates bad physics, e.g. due to highly overlapping atoms, too
   large a timestep, etc.

*Overlapping large/large in pair colloid*
   This potential is infinite when there is an overlap.

*Overlapping small/large in pair colloid*
   This potential is infinite when there is an overlap.

*POEMS fix must come before NPT/NPH fix*
   NPT/NPH fix must be defined in input script after all poems fixes,
   else the fix contribution to the pressure virial is incorrect.

*PPPM grid is too large*
   The global PPPM grid is larger than OFFSET in one or more dimensions.
   OFFSET is currently set to 4096.  You likely need to decrease the
   requested accuracy.

*PPPM grid stencil extends beyond nearest neighbor processor*
   This is not allowed if the kspace\_modify overlap setting is no.

*PPPM order < minimum allowed order*
   The default minimum order is 2.  This can be reset by the
   kspace\_modify minorder command.

*PPPM order cannot be < 2 or > than %d*
   This is a limitation of the PPPM implementation in LIGGGHTS(R)-PUBLIC.

*PPPMDisp Coulomb grid is too large*
   The global PPPM grid is larger than OFFSET in one or more dimensions.
   OFFSET is currently set to 4096.  You likely need to decrease the
   requested accuracy.

*PPPMDisp Dispersion grid is too large*
   The global dispersion grid is larger than OFFSET in one or more
   dimensions.  OFFSET is currently set to 4096.  You likely need to
   decrease the requested accuracy.

*PPPMDisp coulomb order cannot be greater than %d*
   This is a limitation of the PPPM implementation in LIGGGHTS(R)-PUBLIC.

*PRD command before simulation box is defined*
   The prd command cannot be used before a read\_data,
   read\_restart, or create\_box command.

*PRD nsteps must be multiple of t\_event*
   Self-explanatory.

*PRD t\_corr must be multiple of t\_event*
   Self-explanatory.

*Package command after simulation box is defined*
   The package command cannot be used afer a read\_data, read\_restart, or
   create\_box command.

*Package cuda command without USER-CUDA installed*
   The USER-CUDA package must be installed via "make yes-user-cuda"
   before LIGGGHTS(R)-PUBLIC is built.

*Pair body requires atom style body*
   Self-explanatory.

*Pair body requires body style nparticle*
   This pair style is specific to the nparticle body style.

*Pair brownian requires atom style sphere*
   Self-explanatory.

*Pair brownian requires extended particles*
   One of the particles has radius 0.0.

*Pair brownian requires monodisperse particles*
   All particles must be the same finite size.

*Pair brownian/poly requires atom style sphere*
   Self-explanatory.

*Pair brownian/poly requires extended particles*
   One of the particles has radius 0.0.

*Pair brownian/poly requires newton pair off*
   Self-explanatory.

*Pair coeff for hybrid has invalid style*
   Style in pair coeff must have been listed in pair\_style command.

*Pair colloid/poly requires atom style sphere*
   Self-explanatory.

*Pair coul/wolf requires atom attribute q*
   The atom style defined does not have this attribute.

*Pair cutoff < Respa interior cutoff*
   One or more pairwise cutoffs are too short to use with the specified
   rRESPA cutoffs.

*Pair dipole/cut requires atom attributes q, mu, torque*
   The atom style defined does not have these attributes.

*Pair dipole/cut/gpu requires atom attributes q, mu, torque*
   The atom style defined does not have this attribute.

*Pair distance < table inner cutoff*
   Two atoms are closer together than the pairwise table allows.

*Pair distance > table outer cutoff*
   Two atoms are further apart than the pairwise table allows.

*Pair dpd requires ghost atoms store velocity*
   Use the communicate vel yes command to enable this.

*Pair gayberne epsilon a,b,c coeffs are not all set*
   Each atom type involved in pair\_style gayberne must
   have these 3 coefficients set at least once.

*Pair gayberne requires atom style ellipsoid*
   Self-explanatory.

*Pair gayberne requires atoms with same type have same shape*
   Self-explanatory.

*Pair gayberne/gpu requires atom style ellipsoid*
   Self-explanatory.

*Pair gayberne/gpu requires atoms with same type have same shape*
   Self-explanatory.

*Pair granular requires atom style sphere*
   Self-explanatory.

*Pair granular requires ghost atoms store velocity*
   Use the communicate vel yes command to enable this.

*Pair granular with shear history requires newton pair off*
   This is a current restriction of the implementation of pair
   granular styles with history.

*Pair hybrid sub-style does not support single call*
   You are attempting to invoke a single() call on a pair style
   that doesn't support it.

*Pair hybrid sub-style is not used*
   No pair\_coeff command used a sub-style specified in the pair\_style
   command.

*Pair inner cutoff < Respa interior cutoff*
   One or more pairwise cutoffs are too short to use with the specified
   rRESPA cutoffs.

*Pair inner cutoff >= Pair outer cutoff*
   The specified cutoffs for the pair style are inconsistent.

*Pair line/lj requires atom style line*
   Self-explanatory.

*Pair lubricate requires atom style sphere*
   Self-explanatory.

*Pair lubricate requires ghost atoms store velocity*
   Use the communicate vel yes command to enable this.

*Pair lubricate requires monodisperse particles*
   All particles must be the same finite size.

*Pair lubricate/poly requires atom style sphere*
   Self-explanatory.

*Pair lubricate/poly requires extended particles*
   One of the particles has radius 0.0.

*Pair lubricate/poly requires ghost atoms store velocity*
   Use the communicate vel yes command to enable this.

*Pair lubricate/poly requires newton pair off*
   Self-explanatory.

*Pair lubricateU requires atom style sphere*
   Self-explanatory.

*Pair lubricateU requires ghost atoms store velocity*
   Use the communicate vel yes command to enable this.

*Pair lubricateU requires monodisperse particles*
   All particles must be the same finite size.

*Pair lubricateU/poly requires ghost atoms store velocity*
   Use the communicate vel yes command to enable this.

*Pair lubricateU/poly requires newton pair off*
   Self-explanatory.

*Pair peri lattice is not identical in x, y, and z*
   The lattice defined by the lattice command must be cubic.

*Pair peri requires a lattice be defined*
   Use the lattice command for this purpose.

*Pair peri requires an atom map, see atom\_modify*
   Even for atomic systems, an atom map is required to find Peridynamic
   bonds.  Use the atom\_modify command to define one.

*Pair resquared epsilon a,b,c coeffs are not all set*
   Self-explanatory.

*Pair resquared epsilon and sigma coeffs are not all set*
   Self-explanatory.

*Pair resquared requires atom style ellipsoid*
   Self-explanatory.

*Pair resquared requires atoms with same type have same shape*
   Self-explanatory.

*Pair resquared/gpu requires atom style ellipsoid*
   Self-explanatory.

*Pair resquared/gpu requires atoms with same type have same shape*
   Self-explanatory.

*Pair style AIREBO requires atom IDs*
   This is a requirement to use the AIREBO potential.

*Pair style AIREBO requires newton pair on*
   See the newton command.  This is a restriction to use the AIREBO
   potential.

*Pair style BOP requires atom IDs*
   This is a requirement to use the BOP potential.

*Pair style BOP requires newton pair on*
   See the newton command.  This is a restriction to use the BOP
   potential.

*Pair style COMB requires atom IDs*
   This is a requirement to use the AIREBO potential.

*Pair style COMB requires atom attribute q*
   Self-explanatory.

*Pair style COMB requires newton pair on*
   See the newton command.  This is a restriction to use the COMB
   potential.

*Pair style LCBOP requires atom IDs*
   This is a requirement to use the LCBOP potential.

*Pair style LCBOP requires newton pair on*
   See the newton command.  This is a restriction to use the LCBOP
   potential.

*Pair style MEAM requires newton pair on*
   See the newton command.  This is a restriction to use the MEAM
   potential.

*Pair style Stillinger-Weber requires atom IDs*
   This is a requirement to use the SW potential.

*Pair style Stillinger-Weber requires newton pair on*
   See the newton command.  This is a restriction to use the SW
   potential.

*Pair style Tersoff requires atom IDs*
   This is a requirement to use the Tersoff potential.

*Pair style Tersoff requires newton pair on*
   See the newton command.  This is a restriction to use the Tersoff
   potential.

*Pair style bop requires comm ghost cutoff at least 3x larger than %g*
   Use the communicate ghost command to set this.  See the pair bop
   doc page for more details.

*Pair style born/coul/long requires atom attribute q*
   An atom style that defines this attribute must be used.

*Pair style born/coul/long/gpu requires atom attribute q*
   The atom style defined does not have this attribute.

*Pair style born/coul/wolf requires atom attribute q*
   The atom style defined does not have this attribute.

*Pair style buck/coul/cut requires atom attribute q*
   The atom style defined does not have this attribute.

*Pair style buck/coul/long requires atom attribute q*
   The atom style defined does not have these attributes.

*Pair style buck/coul/long/gpu requires atom attribute q*
   The atom style defined does not have this attribute.

*Pair style buck/long/coul/long requires atom attribute q*
   The atom style defined does not have this attribute.

*Pair style coul/cut requires atom attribute q*
   The atom style defined does not have these attributes.

*Pair style coul/dsf requires atom attribute q*
   The atom style defined does not have this attribute.

*Pair style coul/dsf/gpu requires atom attribute q*
   The atom style defined does not have this attribute.

*Pair style coul/long/gpu requires atom attribute q*
   The atom style defined does not have these attributes.

*Pair style does not have extra field requested by compute pair/local*
   The pair style does not support the pN value requested by the compute
   pair/local command.

*Pair style does not support bond\_style quartic*
   The pair style does not have a single() function, so it can
   not be invoked by bond\_style quartic.

*Pair style does not support compute group/group*
   The pair\_style does not have a single() function, so it cannot be
   invokded by the compute group/group command.

*Pair style does not support compute pair/local*
   The pair style does not have a single() function, so it can
   not be invoked by compute pair/local.

*Pair style does not support compute property/local*
   The pair style does not have a single() function, so it can
   not be invoked by fix bond/swap.

*Pair style does not support fix bond/swap*
   The pair style does not have a single() function, so it can
   not be invoked by fix bond/swap.

*Pair style does not support pair\_write*
   The pair style does not have a single() function, so it can
   not be invoked by pair write.

*Pair style does not support rRESPA inner/middle/outer*
   You are attempting to use rRESPA options with a pair style that
   does not support them.

*Pair style granular with history requires atoms have IDs*
   Atoms in the simulation do not have IDs, so history effects
   cannot be tracked by the granular pair potential.

*Pair style hbond/dreiding requires an atom map, see atom\_modify*
   Self-explanatory.

*Pair style hbond/dreiding requires atom IDs*
   Self-explanatory.

*Pair style hbond/dreiding requires molecular system*
   Self-explanatory.

*Pair style hbond/dreiding requires newton pair on*
   See the newton command for details.

*Pair style hybrid cannot have hybrid as an argument*
   Self-explanatory.

*Pair style hybrid cannot have none as an argument*
   Self-explanatory.

*Pair style is incompatible with KSpace style*
   If a pair style with a long-range Coulombic component is selected,
   then a kspace style must also be used.

*Pair style lj/charmm/coul/charmm requires atom attribute q*
   The atom style defined does not have these attributes.

*Pair style lj/charmm/coul/long requires atom attribute q*
   The atom style defined does not have these attributes.

*Pair style lj/charmm/coul/long/gpu requires atom attribute q*
   The atom style defined does not have this attribute.

*Pair style lj/class2/coul/cut requires atom attribute q*
   The atom style defined does not have this attribute.

*Pair style lj/class2/coul/long requires atom attribute q*
   The atom style defined does not have this attribute.

*Pair style lj/class2/coul/long/gpu requires atom attribute q*
   The atom style defined does not have this attribute.

*Pair style lj/cut/coul/cut requires atom attribute q*
   The atom style defined does not have this attribute.

*Pair style lj/cut/coul/cut/gpu requires atom attribute q*
   The atom style defined does not have this attribute.

*Pair style lj/cut/coul/debye/gpu requires atom attribute q*
   The atom style defined does not have this attribute.

*Pair style lj/cut/coul/dsf requires atom attribute q*
   The atom style defined does not have these attributes.

*Pair style lj/cut/coul/dsf/gpu requires atom attribute q*
   The atom style defined does not have this attribute.

*Pair style lj/cut/coul/long requires atom attribute q*
   The atom style defined does not have this attribute.

*Pair style lj/cut/coul/long/gpu requires atom attribute q*
   The atom style defined does not have this attribute.

*Pair style lj/cut/tip4p/long requires atom IDs*
   There are no atom IDs defined in the system and the TIP4P potential
   requires them to find O,H atoms with a water molecule.

*Pair style lj/cut/tip4p/long requires atom attribute q*
   The atom style defined does not have these attributes.

*Pair style lj/cut/tip4p/long requires newton pair on*
   This is because the computation of constraint forces within a water
   molecule adds forces to atoms owned by other processors.

*Pair style lj/cut/coul/msm requires atom attribute q*
   The atom style defined does not have this attribute.

*Pair style lj/gromacs/coul/gromacs requires atom attribute q*
   An atom\_style with this attribute is needed.

*Pair style lj/long/coul/long requires atom attribute q*
   The atom style defined does not have this attribute.

*Pair style lj/long/tip4p/long requires atom IDs*
   There are no atom IDs defined in the system and the TIP4P potential
   requires them to find O,H atoms with a water molecule.

*Pair style lj/long/tip4p/long requires atom attribute q*
   The atom style defined does not have these attributes.

*Pair style lj/long/tip4p/long requires newton pair on*
   This is because the computation of constraint forces within a water
   molecule adds forces to atoms owned by other processors.

*Pair style peri requires atom style peri*
   Self-explanatory.

*Pair style reax requires atom IDs*
   This is a requirement to use the ReaxFF potential.

*Pair style reax requires newton pair on*
   This is a requirement to use the ReaxFF potential.

*Pair style requires a KSpace style*
   No kspace style is defined.

*Pair table cutoffs must all be equal to use with KSpace*
   When using pair style table with a long-range KSpace solver, the
   cutoffs for all atom type pairs must all be the same, since the
   long-range solver starts at that cutoff.

*Pair table parameters did not set N*
   List of pair table parameters must include N setting.

*Pair tersoff/zbl requires metal or real units*
   This is a current restriction of this pair potential.

*Pair tri/lj requires atom style tri*
   Self-explanatory.

*Pair yukawa/colloid requires atom style sphere*
   Self-explantory.

*Pair yukawa/colloid requires atoms with same type have same radius*
   Self-explantory.

*Pair yukawa/colloid/gpu requires atom style sphere*
   Self-explanatory.

*PairKIM only works with 3D problems.*
   This is a current restriction of this pair style.

*Pair\_coeff command before pair\_style is defined*
   Self-explanatory.

*Pair\_coeff command before simulation box is defined*
   The pair\_coeff command cannot be used before a read\_data,
   read\_restart, or create\_box command.

*Pair\_modify command before pair\_style is defined*
   Self-explanatory.

*Pair\_write command before pair\_style is defined*
   Self-explanatory.

*Particle on or inside fix wall surface*
   Particles must be "exterior" to the wall in order for energy/force to
   be calculated.

*Particle on or inside surface of region used in fix wall/region*
   Particles must be "exterior" to the region surface in order for
   energy/force to be calculated.

*Per-atom compute in equal-style variable formula*
   Equal-style variables cannot use per-atom quantities.

*Per-atom energy was not tallied on needed timestep*
   You are using a thermo keyword that requires potentials to
   have tallied energy, but they didn't on this timestep.  See the
   variable doc page for ideas on how to make this work.

*Per-atom fix in equal-style variable formula*
   Equal-style variables cannot use per-atom quantities.

*Per-atom virial was not tallied on needed timestep*
   You are using a thermo keyword that requires potentials to have
   tallied the virial, but they didn't on this timestep.  See the
   variable doc page for ideas on how to make this work.

*Per-processor system is too big*
   The number of owned atoms plus ghost atoms on a single
   processor must fit in 32-bit integer.

*Potential energy ID for fix neb does not exist*
   Self-explanatory.

*Potential energy ID for fix nvt/nph/npt does not exist*
   A compute for potential energy must be defined.

*Potential file has duplicate entry*
   The potential file for a SW or Tersoff potential has more than
   one entry for the same 3 ordered elements.

*Potential file is missing an entry*
   The potential file for a SW or Tersoff potential does not have a
   needed entry.

*Power by 0 in variable formula*
   Self-explanatory.

*Pressure ID for fix box/relax does not exist*
   The compute ID needed to compute pressure for the fix does not
   exist.

*Pressure ID for fix modify does not exist*
   Self-explanatory.

*Pressure ID for fix npt/nph does not exist*
   Self-explanatory.

*Pressure ID for fix press/berendsen does not exist*
   The compute ID needed to compute pressure for the fix does not
   exist.

*Pressure ID for fix rigid npt/nph does not exist*
   Self-explanatory.

*Pressure ID for thermo does not exist*
   The compute ID needed to compute pressure for thermodynamics does not
   exist.

*Pressure control can not be used with fix nvt*
   Self-explanatory.

*Pressure control can not be used with fix nvt/asphere*
   Self-explanatory.

*Pressure control can not be used with fix nvt/sllod*
   Self-explanatory.

*Pressure control can not be used with fix nvt/sphere*
   Self-explanatory.

*Pressure control must be used with fix nph*
   Self-explanatory.

*Pressure control must be used with fix nph/asphere*
   Self-explanatory.

*Pressure control must be used with fix nph/sphere*
   Self-explanatory.

*Pressure control must be used with fix nphug*
   A pressure control keyword (iso, aniso, tri, x, y, or z) must be
   provided.

*Pressure control must be used with fix npt*
   Self-explanatory.

*Pressure control must be used with fix npt/asphere*
   Self-explanatory.

*Pressure control must be used with fix npt/sphere*
   Self-explanatory.

*Processor count in z must be 1 for 2d simulation*
   Self-explanatory.

*Processor partitions are inconsistent*
   The total number of processors in all partitions must match the number
   of processors LIGGGHTS(R)-PUBLIC is running on.

*Processors command after simulation box is defined*
   The processors command cannot be used after a read\_data, read\_restart,
   or create\_box command.

*Processors custom grid file is inconsistent*
   The vales in the custom file are not consistent with the number of
   processors you are running on or the Px,Py,Pz settings of the
   processors command.  Or there was not a setting for every processor.

*Processors grid numa and map style are incompatible*
   Using numa for gstyle in the processors command requires using
   cart for the map option.

*Processors part option and grid style are incompatible*
   Cannot use gstyle numa or custom with the part option.

*Processors twogrid requires proc count be a multiple of core count*
   Self-explanatory.

*Pstart and Pstop must have the same value*
   Self-explanatory.

*R0 < 0 for fix spring command*
   Equilibrium spring length is invalid.

*Read\_dump command before simulation box is defined*
   The read\_dump command cannot be used before a read\_data, read\_restart,
   or create\_box command.

*Read\_dump field not found in dump file*
   Self-explanatory.

*Read\_dump triclinic status does not match simulation*
   Both the dump snapshot and the current LIGGGHTS(R)-PUBLIC simulation must
   be using either an orthogonal or triclinic box.

*Read\_dump x,y,z fields do not have consistent scaling*
   Self-explanatory.

*Reax\_defs.h setting for NATDEF is too small*
   Edit the setting in the ReaxFF library and re-compile the
   library and re-build LIGGGHTS(R)-PUBLIC.

*Reax\_defs.h setting for NNEIGHMAXDEF is too small*
   Edit the setting in the ReaxFF library and re-compile the
   library and re-build LIGGGHTS(R)-PUBLIC.

*Receiving partition in processors part command is already a receiver*
   Cannot specify a partition to be a receiver twice.

*Region ID for compute reduce/region does not exist*
   Self-explanatory.

*Region ID for compute temp/region does not exist*
   Self-explanatory.

*Region ID for dump custom does not exist*
   Self-explanatory.

*Region ID for fix addforce does not exist*
   Self-explanatory.

*Region ID for fix ave/spatial does not exist*
   Self-explanatory.

*Region ID for fix aveforce does not exist*
   Self-explanatory.

*Region ID for fix deposit does not exist*
   Self-explanatory.

*Region ID for fix evaporate does not exist*
   Self-explanatory.

*Region ID for fix gcmc does not exist*
   Self-explanatory.

*Region ID for fix heat does not exist*
   Self-explanatory.

*Region ID for fix setforce does not exist*
   Self-explanatory.

*Region ID for fix wall/region does not exist*
   Self-explanatory.

*Region ID in variable formula does not exist*
   Self-explanatory.

*Region cannot have 0 length rotation vector*
   Self-explanatory.

*Region intersect region ID does not exist*
   Self-explanatory.

*Region union or intersect cannot be dynamic*
   The sub-regions can be dynamic, but not the combined region.

*Region union region ID does not exist*
   One or more of the region IDs specified by the region union command
   does not exist.

*Replacing a fix, but new style != old style*
   A fix ID can be used a 2nd time, but only if the style matches the
   previous fix.  In this case it is assumed you with to reset a fix's
   parameters.  This error may mean you are mistakenly re-using a fix ID
   when you do not intend to.

*Replicate command before simulation box is defined*
   The replicate command cannot be used before a read\_data, read\_restart,
   or create\_box command.

*Replicate did not assign all atoms correctly*
   Atoms replicated by the replicate command were not assigned correctly
   to processors.  This is likely due to some atom coordinates being
   outside a non-periodic simulation box.

*Replicated molecular system atom IDs are too big*
   See the setting for the allowed atom ID size in the src/lmptype.h
   file.

*Replicated system is too big*
   See the setting for bigint in the src/lmptype.h file.

*Rerun command before simulation box is defined*
   The rerun command cannot be used before a read\_data, read\_restart, or
   create\_box command.

*Rerun dump file does not contain requested snapshot*
   Self-explanatory.

*Resetting timestep is not allowed with fix move*
   This is because fix move is moving atoms based on elapsed time.

*Respa inner cutoffs are invalid*
   The first cutoff must be <= the second cutoff.

*Respa levels must be >= 1*
   Self-explanatory.

*Respa middle cutoffs are invalid*
   The first cutoff must be <= the second cutoff.

*Restart variable returned a bad timestep*
   The variable must return a timestep greater than the current timestep.

*Restrain atoms %d %d %d %d missing on proc %d at step %ld*
   The 4 atoms in a restrain dihedral specified by the fix restrain
   command are not all accessible to a processor.  This probably means an
   atom has moved too far.

*Restrain atoms %d %d %d missing on proc %d at step %ld*
   The 3 atoms in a restrain angle specified by the fix restrain
   command are not all accessible to a processor.  This probably means an
   atom has moved too far.

*Restrain atoms %d %d missing on proc %d at step %ld*
   The 2 atoms in a restrain bond specified by the fix restrain
   command are not all accessible to a processor.  This probably means an
   atom has moved too far.

*Reuse of compute ID*
   A compute ID cannot be used twice.

*Reuse of dump ID*
   A dump ID cannot be used twice.

*Reuse of region ID*
   A region ID cannot be used twice.

*Rigid body atoms %d %d missing on proc %d at step %ld*
   This means that an atom cannot find the atom that owns the rigid body
   it is part of, or vice versa.  The solution is to use the communicate
   cutoff command to insure ghost atoms are acquired from far enough away
   to encompass the max distance printed when the fix rigid/small command
   was invoked.

*Rigid body has degenerate moment of inertia*
   Fix poems will only work with bodies (collections of atoms) that have
   non-zero principal moments of inertia.  This means they must be 3 or
   more non-collinear atoms, even with joint atoms removed.

*Rigid fix must come before NPT/NPH fix*
   NPT/NPH fix must be defined in input script after all rigid fixes,
   else the rigid fix contribution to the pressure virial is
   incorrect.

*Rmask function in equal-style variable formula*
   Rmask is per-atom operation.

*Run command before simulation box is defined*
   The run command cannot be used before a read\_data, read\_restart, or
   create\_box command.

*Run command start value is after start of run*
   Self-explanatory.

*Run command stop value is before end of run*
   Self-explanatory.

*Run\_style command before simulation box is defined*
   The run\_style command cannot be used before a read\_data,
   read\_restart, or create\_box command.

*SRD bin size for fix srd differs from user request*
   Fix SRD had to adjust the bin size to fit the simulation box.  See the
   cubic keyword if you want this message to be an error vs warning.

*SRD bins for fix srd are not cubic enough*
   The bin shape is not within tolerance of cubic.  See the cubic
   keyword if you want this message to be an error vs warning.

*SRD particle %d started inside big particle %d on step %ld bounce %d*
   See the inside keyword if you want this message to be an error vs
   warning.

*Same dimension twice in fix ave/spatial*
   Self-explanatory.

*Sending partition in processors part command is already a sender*
   Cannot specify a partition to be a sender twice.

*Set command before simulation box is defined*
   The set command cannot be used before a read\_data, read\_restart,
   or create\_box command.

*Set command with no atoms existing*
   No atoms are yet defined so the set command cannot be used.

*Set region ID does not exist*
   Region ID specified in set command does not exist.

*Shake angles have different bond types*
   All 3-atom angle-constrained SHAKE clusters specified by the fix shake
   command that are the same angle type, must also have the same bond
   types for the 2 bonds in the angle.

*Shake atoms %d %d %d %d missing on proc %d at step %ld*
   The 4 atoms in a single shake cluster specified by the fix shake
   command are not all accessible to a processor.  This probably means
   an atom has moved too far.

*Shake atoms %d %d %d missing on proc %d at step %ld*
   The 3 atoms in a single shake cluster specified by the fix shake
   command are not all accessible to a processor.  This probably means
   an atom has moved too far.

*Shake atoms %d %d missing on proc %d at step %ld*
   The 2 atoms in a single shake cluster specified by the fix shake
   command are not all accessible to a processor.  This probably means
   an atom has moved too far.

*Shake cluster of more than 4 atoms*
   A single cluster specified by the fix shake command can have no more
   than 4 atoms.

*Shake clusters are connected*
   A single cluster specified by the fix shake command must have a single
   central atom with up to 3 other atoms bonded to it.

*Shake determinant = 0.0*
   The determinant of the matrix being solved for a single cluster
   specified by the fix shake command is numerically invalid.

*Shake fix must come before NPT/NPH fix*
   NPT fix must be defined in input script after SHAKE fix, else the
   SHAKE fix contribution to the pressure virial is incorrect.

*Small, tag, big integers are not sized correctly*
   See description of these 3 data types in src/lmptype.h.

*Smallint setting in lmptype.h is invalid*
   It has to be the size of an integer.

*Smallint setting in lmptype.h is not compatible*
   Smallint stored in restart file is not consistent with LIGGGHTS(R)-PUBLIC version
   you are running.

*Specified processors != physical processors*
   The 3d grid of processors defined by the processors command does not
   match the number of processors LIGGGHTS(R)-PUBLIC is being run on.

*Specified target stress must be uniaxial or hydrostatic*
   Self-explanatory.

*Sqrt of negative value in variable formula*
   Self-explanatory.

*Substitution for illegal variable*
   Input script line contained a variable that could not be substituted
   for.

*System in data file is too big*
   See the setting for bigint in the src/lmptype.h file.

*System is not charge neutral, net charge = %g*
   The total charge on all atoms on the system is not 0.0, which
   is not valid for the long-range Coulombic solvers.

*TAD nsteps must be multiple of t\_event*
   Self-explanatory.

*TIP4P hydrogen has incorrect atom type*
   The TIP4P pairwise computation found an H atom whose type does not
   agree with the specified H type.

*TIP4P hydrogen is missing*
   The TIP4P pairwise computation failed to find the correct H atom
   within a water molecule.

*TMD target file did not list all group atoms*
   The target file for the fix tmd command did not list all atoms in the
   fix group.

*Tad command before simulation box is defined*
   Self-explanatory.

*Tagint setting in lmptype.h is invalid*
   Tagint must be as large or larger than smallint.

*Tagint setting in lmptype.h is not compatible*
   Smallint stored in restart file is not consistent with LIGGGHTS(R)-PUBLIC version
   you are running.

*Target temperature for fix nvt/npt/nph cannot be 0.0*
   Self-explanatory.

*Target temperature for fix rigid/npt cannot be 0.0*
   Self-explanatory.

*Target temperature for fix rigid/nvt cannot be 0.0*
   Self-explanatory.

*Temper command before simulation box is defined*
   The temper command cannot be used before a read\_data, read\_restart, or
   create\_box command.

*Temperature ID for fix bond/swap does not exist*
   Self-explanatory.

*Temperature ID for fix box/relax does not exist*
   Self-explanatory.

*Temperature ID for fix nvt/npt does not exist*
   Self-explanatory.

*Temperature ID for fix press/berendsen does not exist*
   Self-explanatory.

*Temperature ID for fix rigid nvt/npt/nph does not exist*
   Self-explanatory.

*Temperature ID for fix temp/berendsen does not exist*
   Self-explanatory.

*Temperature ID for fix temp/rescale does not exist*
   Self-explanatory.

*Temperature control can not be used with fix nph*
   Self-explanatory.

*Temperature control can not be used with fix nph/asphere*
   Self-explanatory.

*Temperature control can not be used with fix nph/sphere*
   Self-explanatory.

*Temperature control must be used with fix nphug*
   The temp keyword must be provided.

*Temperature control must be used with fix npt*
   Self-explanatory.

*Temperature control must be used with fix npt/asphere*
   Self-explanatory.

*Temperature control must be used with fix npt/sphere*
   Self-explanatory.

*Temperature control must be used with fix nvt*
   Self-explanatory.

*Temperature control must be used with fix nvt/asphere*
   Self-explanatory.

*Temperature control must be used with fix nvt/sllod*
   Self-explanatory.

*Temperature control must be used with fix nvt/sphere*
   Self-explanatory.

*Temperature for fix nvt/sllod does not have a bias*
   The specified compute must compute temperature with a bias.

*Tempering could not find thermo\_pe compute*
   This compute is created by the thermo command.  It must have been
   explicitly deleted by a uncompute command.

*Tempering fix ID is not defined*
   The fix ID specified by the temper command does not exist.

*Tempering temperature fix is not valid*
   The fix specified by the temper command is not one that controls
   temperature (nvt or langevin).

*Test\_descriptor\_string already allocated*
   This should not happen. It likely indicates a bug in the pair\_kim implementation.

*The package gpu command is required for gpu styles*
   Self-explanatory.

*Thermo and fix not computed at compatible times*
   Fixes generate values on specific timesteps.  The thermo output
   does not match these timesteps.

*Thermo compute array is accessed out-of-range*
   Self-explanatory.

*Thermo compute does not compute array*
   Self-explanatory.

*Thermo compute does not compute scalar*
   Self-explanatory.

*Thermo compute does not compute vector*
   Self-explanatory.

*Thermo compute vector is accessed out-of-range*
   Self-explanatory.

*Thermo custom variable cannot be indexed*
   Self-explanatory.

*Thermo custom variable is not equal-style variable*
   Only equal-style variables can be output with thermodynamics, not
   atom-style variables.

*Thermo every variable returned a bad timestep*
   The variable must return a timestep greater than the current timestep.

*Thermo fix array is accessed out-of-range*
   Self-explanatory.

*Thermo fix does not compute array*
   Self-explanatory.

*Thermo fix does not compute scalar*
   Self-explanatory.

*Thermo fix does not compute vector*
   Self-explanatory.

*Thermo fix vector is accessed out-of-range*
   Self-explanatory.

*Thermo keyword in variable requires lattice be defined*
   The xlat, ylat, zlat keywords refer to lattice properties.

*Thermo keyword in variable requires thermo to use/init pe*
   You are using a thermo keyword in a variable that requires
   potential energy to be calculated, but your thermo output
   does not use it.  Add it to your thermo output.

*Thermo keyword in variable requires thermo to use/init press*
   You are using a thermo keyword in a variable that requires pressure to
   be calculated, but your thermo output does not use it.  Add it to your
   thermo output.

*Thermo keyword in variable requires thermo to use/init temp*
   You are using a thermo keyword in a variable that requires temperature
   to be calculated, but your thermo output does not use it.  Add it to
   your thermo output.

*Thermo keyword requires lattice be defined*
   The xlat, ylat, zlat keywords refer to lattice properties.

*Thermo style does not use press*
   Cannot use thermo\_modify to set this parameter since the thermo\_style
   is not computing this quantity.

*Thermo style does not use temp*
   Cannot use thermo\_modify to set this parameter since the thermo\_style
   is not computing this quantity.

*Thermo\_modify int format does not contain d character*
   Self-explanatory.

*Thermo\_modify pressure ID does not compute pressure*
   The specified compute ID does not compute pressure.

*Thermo\_modify temperature ID does not compute temperature*
   The specified compute ID does not compute temperature.

*Thermo\_style command before simulation box is defined*
   The thermo\_style command cannot be used before a read\_data,
   read\_restart, or create\_box command.

*This variable thermo keyword cannot be used between runs*
   Keywords that refer to time (such as cpu, elapsed) do not
   make sense in between runs.

*Threshhold for an atom property that isn't allocated*
   A dump threshhold has been requested on a quantity that is
   not defined by the atom style used in this simulation.

*Timestep must be >= 0*
   Specified timestep is invalid.

*Too big a problem to use velocity create loop all*
   The system size must fit in a 32-bit integer to use this option.

*Too big a timestep*
   Specified timestep is too large.

*Too big a timestep for dump dcd*
   The timestep must fit in a 32-bit integer to use this dump style.

*Too big a timestep for dump xtc*
   The timestep must fit in a 32-bit integer to use this dump style.

*Too few bits for lookup table*
   Table size specified via pair\_modify command does not work with your
   machine's floating point representation.

*Too many MSM grid levels*
   The max number of MSM grid levels is hardwired to 10.

*Too many atom pairs for pair bop*
   The number of atomic pairs exceeds the expected number.  Check your
   atomic structure to ensure that it is realistic.

*Too many atom sorting bins*
   This is likely due to an immense simulation box that has blown up
   to a large size.

*Too many atom triplets for pair bop*
   The number of three atom groups for angle determinations exceeds the
   expected number.  Check your atomic structrure to ensure that it is
   realistic.

*Too many atoms for dump dcd*
   The system size must fit in a 32-bit integer to use this dump
   style.

*Too many atoms for dump xtc*
   The system size must fit in a 32-bit integer to use this dump
   style.

*Too many atoms to dump sort*
   Cannot sort when running with more than 2\^31 atoms.

*Too many exponent bits for lookup table*
   Table size specified via pair\_modify command does not work with your
   machine's floating point representation.

*Too many groups*
   The maximum number of atom groups (including the "all" group) is
   given by MAX\_GROUP in group.cpp and is 32.

*Too many iterations*
   You must use a number of iterations that fit in a 32-bit integer
   for minimization.

*Too many lines in one body in data file - boost MAXBODY*
   MAXBODY is a setting at the top of the src/read\_data.cpp file.
   Set it larger and re-compile the code.

*Too many local+ghost atoms for neighbor list*
   The number of nlocal + nghost atoms on a processor
   is limited by the size of a 32-bit integer with 2 bits
   removed for masking 1-2, 1-3, 1-4 neighbors.

*Too many mantissa bits for lookup table*
   Table size specified via pair\_modify command does not work with your
   machine's floating point representation.

*Too many masses for fix shake*
   The fix shake command cannot list more masses than there are atom
   types.

*Too many neighbor bins*
   This is likely due to an immense simulation box that has blown up
   to a large size.

*Too many timesteps*
   The cummulative timesteps must fit in a 64-bit integer.

*Too many timesteps for NEB*
   You must use a number of timesteps that fit in a 32-bit integer
   for NEB.

*Too many total atoms*
   See the setting for bigint in the src/lmptype.h file.

*Too many total bits for bitmapped lookup table*
   Table size specified via pair\_modify command is too large.  Note that
   a value of N generates a 2\^N size table.

*Too many touching neighbors - boost MAXTOUCH*
   A granular simulation has too many neighbors touching one atom.  The
   MAXTOUCH parameter in fix\_shear\_history.cpp must be set larger and
   LIGGGHTS(R)-PUBLIC must be re-built.

*Too much per-proc info for dump*
   Number of local atoms times number of columns must fit in a 32-bit
   integer for dump.

*Tree structure in joint connections*
   Fix poems cannot (yet) work with coupled bodies whose joints connect
   the bodies in a tree structure.

*Triclinic box skew is too large*
   The displacement in a skewed direction must be less than half the box
   length in that dimension.  E.g. the xy tilt must be between -half and
   +half of the x box length.  This constraint can be relaxed by using
   the box tilt command.

*Tried to convert a double to int, but input\_double > INT\_MAX*
   Self-explanatory.

*Two groups cannot be the same in fix spring couple*
   Self-explanatory.

*USER-CUDA mode requires CUDA variant of min style*
   CUDA mode is enabled, so the min style must include a cuda suffix.

*USER-CUDA mode requires CUDA variant of run style*
   CUDA mode is enabled, so the run style must include a cuda suffix.

*USER-CUDA package requires a cuda enabled atom\_style*
   Self-explanatory.

*Unable to initialize accelerator for use*
   There was a problem initializing an accelerator for the gpu package

*Unbalanced quotes in input line*
   No matching end double quote was found following a leading double
   quote.

*Unexpected end of -reorder file*
   Self-explanatory.

*Unexpected end of custom file*
   Self-explanatory.

*Unexpected end of data file*
   LIGGGHTS(R)-PUBLIC hit the end of the data file while attempting to read a
   section.  Something is wrong with the format of the data file.

*Unexpected end of dump file*
   A read operation from the file failed.

*Unexpected end of fix rigid file*
   A read operation from the file failed.

*Units command after simulation box is defined*
   The units command cannot be used after a read\_data, read\_restart, or
   create\_box command.

*Universe/uloop variable count < # of partitions*
   A universe or uloop style variable must specify a number of values >= to the
   number of processor partitions.

*Unknown command: %s*
   The command is not known to LIGGGHTS(R)-PUBLIC.  Check the input script.

*Unknown error in GPU library*
   Self-explanatory.

*Unknown identifier in data file: %s*
   A section of the data file cannot be read by LIGGGHTS(R)-PUBLIC.

*Unknown table style in angle style table*
   Self-explanatory.

*Unknown table style in bond style table*
   Self-explanatory.

*Unknown table style in pair\_style command*
   Style of table is invalid for use with pair\_style table command.

*Unknown unit\_style*
   Self-explanatory. Check the input script or data file.

*Unrecognized lattice type in MEAM file 1*
   The lattice type in an entry of the MEAM library file is not
   valid.

*Unrecognized lattice type in MEAM file 2*
   The lattice type in an entry of the MEAM parameter file is not
   valid.

*Unrecognized pair style in compute pair command*
   Self-explanatory.

*Unrecognized virial argument in pair\_style command*
   Only two options are supported: LAMMPSvirial and KIMvirial

*Unsupported mixing rule in kspace\_style ewald/disp*
   Only geometric mixing is supported.

*Unsupported mixing rule in kspace\_style pppm/disp for pair\_style %s*
   Only geometric mixing is supported.

*Unsupported order in kspace\_style ewald/disp*
   Only 1/r\^6 dispersion terms are supported.

*Unsupported order in kspace\_style pppm/disp pair\_style %s*
   Only 1/r\^6 dispersion terms are supported.

*Use of change\_box with undefined lattice*
   Must use lattice command with displace\_box command if units option is
   set to lattice.

*Use of compute temp/ramp with undefined lattice*
   Must use lattice command with compute temp/ramp command if units
   option is set to lattice.

*Use of displace\_atoms with undefined lattice*
   Must use lattice command with displace\_atoms command if units option
   is set to lattice.

*Use of fix append/atoms with undefined lattice*
   A lattice must be defined before using this fix.

*Use of fix ave/spatial with undefined lattice*
   A lattice must be defined to use fix ave/spatial with units = lattice.

*Use of fix deform with undefined lattice*
   A lattice must be defined to use fix deform with units = lattice.

*Use of fix deposit with undefined lattice*
   Must use lattice command with compute fix deposit command if units
   option is set to lattice.

*Use of fix dt/reset with undefined lattice*
   Must use lattice command with fix dt/reset command if units option is
   set to lattice.

*Use of fix indent with undefined lattice*
   The lattice command must be used to define a lattice before using the
   fix indent command.

*Use of fix move with undefined lattice*
   Must use lattice command with fix move command if units option is
   set to lattice.

*Use of fix recenter with undefined lattice*
   Must use lattice command with fix recenter command if units option is
   set to lattice.

*Use of fix wall with undefined lattice*
   Must use lattice command with fix wall command if units option is set
   to lattice.

*Use of fix wall/piston with undefined lattice*
   A lattice must be defined before using this fix.

*Use of region with undefined lattice*
   If units = lattice (the default) for the region command, then a
   lattice must first be defined via the lattice command.

*Use of velocity with undefined lattice*
   If units = lattice (the default) for the velocity set or velocity ramp
   command, then a lattice must first be defined via the lattice command.

*Using fix nvt/sllod with inconsistent fix deform remap option*
   Fix nvt/sllod requires that deforming atoms have a velocity profile
   provided by "remap v" as a fix deform option.

*Using fix nvt/sllod with no fix deform defined*
   Self-explanatory.

*Using fix srd with inconsistent fix deform remap option*
   When shearing the box in an SRD simulation, the remap v option for fix
   deform needs to be used.

*Using pair lubricate with inconsistent fix deform remap option*
   Must use remap v option with fix deform with this pair style.

*Using pair lubricate/poly with inconsistent fix deform remap option*
   If fix deform is used, the remap v option is required.

*Variable ID in variable formula does not exist*
   Self-explanatory.

*Variable evaluation before simulation box is defined*
   Cannot evaluate a compute or fix or atom-based value in a variable
   before the simulation has been setup.

*Variable evaluation in fix wall gave bad value*
   The returned value for epsilon or sigma < 0.0.

*Variable evaluation in region gave bad value*
   Variable returned a radius < 0.0.

*Variable for compute ti is invalid style*
   Self-explanatory.

*Variable for dump every is invalid style*
   Only equal-style variables can be used.

*Variable for dump image center is invalid style*
   Must be an equal-style variable.

*Variable for dump image persp is invalid style*
   Must be an equal-style variable.

*Variable for dump image phi is invalid style*
   Must be an equal-style variable.

*Variable for dump image theta is invalid style*
   Must be an equal-style variable.

*Variable for dump image zoom is invalid style*
   Must be an equal-style variable.

*Variable for fix adapt is invalid style*
   Only equal-style variables can be used.

*Variable for fix addforce is invalid style*
   Self-explanatory.

*Variable for fix aveforce is invalid style*
   Only equal-style variables can be used.

*Variable for fix deform is invalid style*
   The variable must be an equal-style variable.

*Variable for fix efield is invalid style*
   Only equal-style variables can be used.

*Variable for fix gravity is invalid style*
   Only equal-style variables can be used.

*Variable for fix heat is invalid style*
   Only equal-style or atom-style variables can be used.

*Variable for fix indent is invalid style*
   Only equal-style variables can be used.

*Variable for fix indent is not equal style*
   Only equal-style variables can be used.

*Variable for fix langevin is invalid style*
   It must be an equal-style variable.

*Variable for fix move is invalid style*
   Only equal-style variables can be used.

*Variable for fix setforce is invalid style*
   Only equal-style variables can be used.

*Variable for fix temp/berendsen is invalid style*
   Only equal-style variables can be used.

*Variable for fix temp/rescale is invalid style*
   Only equal-style variables can be used.

*Variable for fix wall is invalid style*
   Only equal-style variables can be used.

*Variable for fix wall/reflect is invalid style*
   Only equal-style variables can be used.

*Variable for fix wall/srd is invalid style*
   Only equal-style variables can be used.

*Variable for group is invalid style*
   Only atom-style variables can be used.

*Variable for region cylinder is invalid style*
   Only equal-style varaibles are allowed.

*Variable for region is invalid style*
   Only equal-style variables can be used.

*Variable for region is not equal style*
   Self-explanatory.

*Variable for region sphere is invalid style*
   Only equal-style varaibles are allowed.

*Variable for restart is invalid style*
   Only equal-style variables can be used.

*Variable for thermo every is invalid style*
   Only equal-style variables can be used.

*Variable for velocity set is invalid style*
   Only atom-style variables can be used.

*Variable formula compute array is accessed out-of-range*
   Self-explanatory.

*Variable formula compute vector is accessed out-of-range*
   Self-explanatory.

*Variable formula fix array is accessed out-of-range*
   Self-explanatory.

*Variable formula fix vector is accessed out-of-range*
   Self-explanatory.

*Variable has circular dependency*
   A circular dependency is when variable "a" in used by variable "b" and
   variable "b" is also used by varaible "a".  Circular dependencies with
   longer chains of dependence are also not allowed.

*Variable name for compute atom/molecule does not exist*
   Self-explanatory.

*Variable name for compute reduce does not exist*
   Self-explanatory.

*Variable name for compute ti does not exist*
   Self-explanatory.

*Variable name for dump every does not exist*
   Self-explanatory.

*Variable name for dump image center does not exist*
   Self-explanatory.

*Variable name for dump image persp does not exist*
   Self-explanatory.

*Variable name for dump image phi does not exist*
   Self-explanatory.

*Variable name for dump image theta does not exist*
   Self-explanatory.

*Variable name for dump image zoom does not exist*
   Self-explanatory.

*Variable name for fix adapt does not exist*
   Self-explanatory.

*Variable name for fix addforce does not exist*
   Self-explanatory.

*Variable name for fix ave/atom does not exist*
   Self-explanatory.

*Variable name for fix ave/correlate does not exist*
   Self-explanatory.

*Variable name for fix ave/histo does not exist*
   Self-explanatory.

*Variable name for fix ave/spatial does not exist*
   Self-explanatory.

*Variable name for fix ave/time does not exist*
   Self-explanatory.

*Variable name for fix aveforce does not exist*
   Self-explanatory.

*Variable name for fix deform does not exist*
   Self-explantory.

*Variable name for fix efield does not exist*
   Self-explanatory.

*Variable name for fix gravity does not exist*
   Self-explanatory.

*Variable name for fix heat does not exist*
   Self-explanatory.

*Variable name for fix indent does not exist*
   Self-explanatory.

*Variable name for fix langevin does not exist*
   Self-explanatory.

*Variable name for fix move does not exist*
   Self-explanatory.

*Variable name for fix setforce does not exist*
   Self-explanatory.

*Variable name for fix store/state does not exist*
   Self-explanatory.

*Variable name for fix temp/berendsen does not exist*
   Self-explanatory.

*Variable name for fix temp/rescale does not exist*
   Self-explanatory.

*Variable name for fix wall does not exist*
   Self-explanatory.

*Variable name for fix wall/reflect does not exist*
   Self-explanatory.

*Variable name for fix wall/srd does not exist*
   Self-explanatory.

*Variable name for group does not exist*
   Self-explanatory.

*Variable name for region cylinder does not exist*
   Self-explanatory.

*Variable name for region does not exist*
   Self-explanatory.

*Variable name for region sphere does not exist*
   Self-explanatory.

*Variable name for restart does not exist*
   Self-explanatory.

*Variable name for thermo every does not exist*
   Self-explanatory.

*Variable name for velocity set does not exist*
   Self-explanatory.

*Variable name must be alphanumeric or underscore characters*
   Self-explanatory.

*Velocity command before simulation box is defined*
   The velocity command cannot be used before a read\_data, read\_restart,
   or create\_box command.

*Velocity command with no atoms existing*
   A velocity command has been used, but no atoms yet exist.

*Velocity ramp in z for a 2d problem*
   Self-explanatory.

*Velocity temperature ID does not compute temperature*
   The compute ID given to the velocity command must compute
   temperature.

*Verlet/split requires 2 partitions*
   See the -partition command-line switch.

*Verlet/split requires Rspace partition layout be multiple of Kspace partition layout in each dim*
   This is controlled by the processors command.

*Verlet/split requires Rspace partition size be multiple of Kspace partition size*
   This is so there is an equal number of Rspace processors for every
   Kspace processor.

*Virial was not tallied on needed timestep*
   You are using a thermo keyword that requires potentials to
   have tallied the virial, but they didn't on this timestep.  See the
   variable doc page for ideas on how to make this work.

*Wall defined twice in fix wall command*
   Self-explanatory.

*Wall defined twice in fix wall/reflect command*
   Self-explanatory.

*Wall defined twice in fix wall/srd command*
   Self-explanatory.

*Water H epsilon must be 0.0 for pair style lj/cut/tip4p/long*
   This is because LIGGGHTS(R)-PUBLIC does not compute the Lennard-Jones interactions
   with these particles for efficiency reasons.

*Water H epsilon must be 0.0 for pair style lj/long/tip4p/long*
   This is because LIGGGHTS(R)-PUBLIC does not compute the Lennard-Jones interactions
   with these particles for efficiency reasons.

*World variable count doesn't match # of partitions*
   A world-style variable must specify a number of values equal to the
   number of processor partitions.

*Write\_restart command before simulation box is defined*
   The write\_restart command cannot be used before a read\_data,
   read\_restart, or create\_box command.

*Zero length rotation vector with displace\_atoms*
   Self-explanatory.

*Zero length rotation vector with fix move*
   Self-explanatory.

*Zero-length lattice orient vector*
   Self-explanatory.



.. _warn:

Warnings:
--------------------



*Adjusting Coulombic cutoff for MSM, new cutoff = %g*
   The adjust/cutoff command is turned on and the Coulombic cutoff has been
   adjusted to match the user-specified accuracy.

*Atom with molecule ID = 0 included in compute molecule group*
   The group used in a compute command that operates on moleclues
   includes atoms with no molecule ID.  This is probably not what you
   want.

*Bond/angle/dihedral extent > half of periodic box length*
   This is a restriction because LIGGGHTS(R)-PUBLIC can be confused about which image
   of an atom in the bonded interaction is the correct one to use.
   "Extent" in this context means the maximum end-to-end length of the
   bond/angle/dihedral.  LIGGGHTS(R)-PUBLIC computes this by taking the maximum bond
   length, multiplying by the number of bonds in the interaction (e.g. 3
   for a dihedral) and adding a small amount of stretch.

*Both groups in compute group/group have a net charge; the Kspace boundary correction to energy will be non-zero*
   Self-explantory.

*Broken bonds will not alter angles, dihedrals, or impropers*
   See the doc page for fix bond/break for more info on this
   restriction.

*Building an occasional neighobr list when atoms may have moved too far*
   This can cause LIGGGHTS(R)-PUBLIC to crash when the neighbor list is built.
   The solution is to check for building the regular neighbor lists
   more frequently.

*Cannot include log terms without 1/r terms; setting flagHI to 1*
   Self-explanatory.

*Cannot include log terms without 1/r terms; setting flagHI to 1.*
   Self-explanatory.

*Charges are set, but coulombic solver is not used*
   The atom style supports charge, but this KSpace style does not include
   long-range Coulombics.

*Compute cna/atom cutoff may be too large to find ghost atom neighbors*
   The neighbor cutoff used may not encompass enough ghost atoms
   to perform this operation correctly.

*Computing temperature of portions of rigid bodies*
   The group defined by the temperature compute does not encompass all
   the atoms in one or more rigid bodies, so the change in
   degrees-of-freedom for the atoms in those partial rigid bodies will
   not be accounted for.

*Created bonds will not create angles, dihedrals, or impropers*
   See the doc page for fix bond/create for more info on this
   restriction.

*Dihedral problem: %d %ld %d %d %d %d*
   Conformation of the 4 listed dihedral atoms is extreme; you may want
   to check your simulation geometry.

*Dump dcd/xtc timestamp may be wrong with fix dt/reset*
   If the fix changes the timestep, the dump dcd file will not
   reflect the change.

*Ewald/disp Newton solver failed, using old method to estimate g\_ewald*
   Self-explanatory.

*FENE bond too long: %ld %d %d %g*
   A FENE bond has stretched dangerously far.  It's interaction strength
   will be truncated to attempt to prevent the bond from blowing up.

*FENE bond too long: %ld %g*
   A FENE bond has stretched dangerously far.  It's interaction strength
   will be truncated to attempt to prevent the bond from blowing up.

*Fix SRD walls overlap but fix srd overlap not set*
   You likely want to set this in your input script.

*Fix bond/swap will ignore defined angles*
   See the doc page for fix bond/swap for more info on this
   restriction.

*Fix evaporate may delete atom with non-zero molecule ID*
   This is probably an error, since you should not delete only one atom
   of a molecule.

*Fix move does not update angular momentum*
   Atoms store this quantity, but fix move does not (yet) update it.

*Fix move does not update quaternions*
   Atoms store this quantity, but fix move does not (yet) update it.

*Fix recenter should come after all other integration fixes*
   Other fixes may change the position of the center-of-mass, so
   fix recenter should come last.

*Fix shake with rRESPA computes invalid pressures*
   This is a known bug in LIGGGHTS(R)-PUBLIC that has not yet been fixed.  If you use
   SHAKE with rRESPA and perform a constant volume simulation (e.g. using
   fix npt) this only affects the output pressure, not the dynamics of
   the simulation.  If you use SHAKE with rRESPA and perform a constant
   pressure simulation (e.g. using fix npt) then you will be
   equilibrating to the wrong volume.

*Fix srd SRD moves may trigger frequent reneighboring*
   This is because the SRD particles may move long distances.

*Fix srd grid size > 1/4 of big particle diameter*
   This may cause accuracy problems.

*Fix srd particle moved outside valid domain*
   This may indicate a problem with your simulation parameters.

*Fix srd particles may move > big particle diameter*
   This may cause accuracy problems.

*Fix srd viscosity < 0.0 due to low SRD density*
   This may cause accuracy problems.

*Fix thermal/conductivity comes before fix ave/spatial*
   The order of these 2 fixes in your input script is such that fix
   thermal/conductivity comes first.  If you are using fix ave/spatial to
   measure the temperature profile induced by fix viscosity, then this
   may cause a glitch in the profile since you are averaging immediately
   after swaps have occurred.  Flipping the order of the 2 fixes
   typically helps.

*Fix viscosity comes before fix ave/spatial*
   The order of these 2 fixes in your input script is such that
   fix viscosity comes first.  If you are using fix ave/spatial
   to measure the velocity profile induced by fix viscosity, then
   this may cause a glitch in the profile since you are averaging
   immediately after swaps have occurred.  Flipping the order
   of the 2 fixes typically helps.

*For better accuracy use 'pair\_modify table 0'*
   The user-specified force accuracy cannot be achieved unless the table
   feature is disabled by using 'pair\_modify table 0'.

*Geometric mixing assumed for 1/r\^6 coefficients*
   Self-explanatory.

*Group for fix\_modify temp != fix group*
   The fix\_modify command is specifying a temperature computation that
   computes a temperature on a different group of atoms than the fix
   itself operates on.  This is probably not what you want to do.

*Improper problem: %d %ld %d %d %d %d*
   Conformation of the 4 listed improper atoms is extreme; you may want
   to check your simulation geometry.

*Inconsistent image flags*
   The image flags for a pair on bonded atoms appear to be inconsistent.
   Inconsistent means that when the coordinates of the two atoms are
   unwrapped using the image flags, the two atoms are far apart.
   Specifically they are further apart than half a periodic box length.
   Or they are more than a box length apart in a non-periodic dimension.
   This is usually due to the initial data file not having correct image
   flags for the 2 atoms in a bond that straddles a periodic boundary.
   They should be different by 1 in that case.  This is a warning because
   inconsistent image flags will not cause problems for dynamics or most
   LIGGGHTS(R)-PUBLIC simulations.  However they can cause problems when such atoms
   are used with the fix rigid or replicate commands.

*KIM Model does not provide `energy'; Potential energy will be zero*
   Self-explanatory.

*KIM Model does not provide `forces'; Forces will be zero*
   Self-explanatory.

*KIM Model does not provide `particleEnergy'; energy per atom will be zero*
   Self-explanatory.

*KIM Model does not provide `particleVirial'; virial per atom will be zero*
   Self-explanatory.

*Kspace\_modify slab param < 2.0 may cause unphysical behavior*
   The kspace\_modify slab parameter should be larger to insure periodic
   grids padded with empty space do not overlap.

*Less insertions than requested*
   Less atom insertions occurred on this timestep due to the fix pour
   command than were scheduled.  This is probably because there were too
   many overlaps detected.

*Library error in lammps\_gather\_atoms*
   This library function cannot be used if atom IDs are not defined
   or are not consecutively numbered.

*Library error in lammps\_scatter\_atoms*
   This library function cannot be used if atom IDs are not defined or
   are not consecutively numbered, or if no atom map is defined.  See the
   atom\_modify command for details about atom maps.

*Lost atoms via change\_box: original %ld current %ld*
   The command options you have used caused atoms to be lost.

*Lost atoms via displace\_atoms: original %ld current %ld*
   The command options you have used caused atoms to be lost.

*Lost atoms: original %ld current %ld*
   Lost atoms are checked for each time thermo output is done.  See the
   thermo\_modify lost command for options.  Lost atoms usually indicate
   bad dynamics, e.g. atoms have been blown far out of the simulation
   box, or moved futher than one processor's sub-domain away before
   reneighboring.

*MSM mesh too small, increasing to 2 points in each direction*
   Self-explanatory.

*Mismatch between velocity and compute groups*
   The temperature computation used by the velocity command will not be
   on the same group of atoms that velocities are being set for.

*Mixing forced for LJ coefficients*
   Self-explanatory.

*Mixing forced for lj coefficients*
   Self-explanatory.

*More than one compute centro/atom*
   It is not efficient to use compute centro/atom more than once.

*More than one compute cluster/atom*
   It is not efficient to use compute cluster/atom  more than once.

*More than one compute cna/atom defined*
   It is not efficient to use compute cna/atom  more than once.

*More than one compute contact/atom*
   It is not efficient to use compute contact/atom more than once.

*More than one compute coord/atom*
   It is not efficient to use compute coord/atom more than once.

*More than one compute damage/atom*
   It is not efficient to use compute ke/atom more than once.

*More than one compute erotate/sphere/atom*
   It is not efficient to use compute erorate/sphere/atom more than once.

*More than one compute ke/atom*
   It is not efficient to use compute ke/atom more than once.

*More than one compute voronoi/atom command*
   It is not efficient to use compute voronoi/atom more than once.

*More than one fix poems*
   It is not efficient to use fix poems more than once.

*More than one fix rigid*
   It is not efficient to use fix rigid more than once.

*Neighbor exclusions used with KSpace solver may give inconsistent Coulombic energies*
   This is because excluding specific pair interactions also excludes
   them from long-range interactions which may not be the desired effect.
   The special\_bonds command handles this consistently by insuring
   excluded (or weighted) 1-2, 1-3, 1-4 interactions are treated
   consistently by both the short-range pair style and the long-range
   solver.  This is not done for exclusions of charged atom pairs via the
   neigh\_modify exclude command.

*New thermo\_style command, previous thermo\_modify settings will be lost*
   If a thermo\_style command is used after a thermo\_modify command, the
   settings changed by the thermo\_modify command will be reset to their
   default values.  This is because the thermo\_modify commmand acts on
   the currently defined thermo style, and a thermo\_style command creates
   a new style.

*No Kspace calculation with verlet/split*
   The 2nd partition performs a kspace calculation so the kspace\_style
   command must be used.

*No fixes defined, atoms won't move*
   If you are not using a fix like nve, nvt, npt then atom velocities and
   coordinates will not be updated during timestepping.

*No joints between rigid bodies, use fix rigid instead*
   The bodies defined by fix poems are not connected by joints.  POEMS
   will integrate the body motion, but it would be more efficient to use
   fix rigid.

*Not using real units with pair reax*
   This is most likely an error, unless you have created your own ReaxFF
   parameter file in a different set of units.

*Number of MSM mesh points increased to be a multiple of 2*
   MSM requires that the number of grid points in each direction be a multiple
   of two and the number of grid points in one or more directions have been
   adjusted to meet this requirement.

*OMP\_NUM\_THREADS environment is not set.*
   This environment variable must be set appropriately to use the
   USER-OMP pacakge.

*One or more atoms are time integrated more than once*
   This is probably an error since you typically do not want to
   advance the positions or velocities of an atom more than once
   per timestep.

*One or more compute molecules has atoms not in group*
   The group used in a compute command that operates on moleclues does
   not include all the atoms in some molecules.  This is probably not
   what you want.

*One or more respa levels compute no forces*
   This is computationally inefficient.

*Pair COMB charge %.10f with force %.10f hit max barrier*
   Something is possibly wrong with your model.

*Pair COMB charge %.10f with force %.10f hit min barrier*
   Something is possibly wrong with your model.

*Pair brownian needs newton pair on for momentum conservation*
   Self-explanatory.

*Pair dpd needs newton pair on for momentum conservation*
   Self-explanatory.

*Pair dsmc: num\_of\_collisions > number\_of\_A*
   Collision model in DSMC is breaking down.

*Pair dsmc: num\_of\_collisions > number\_of\_B*
   Collision model in DSMC is breaking down.

*Particle deposition was unsuccessful*
   The fix deposit command was not able to insert as many atoms as
   needed.  The requested volume fraction may be too high, or other atoms
   may be in the insertion region.

*Reducing PPPM order b/c stencil extends beyond nearest neighbor processor*
   This may lead to a larger grid than desired.  See the kspace\_modify overlap
   command to prevent changing of the PPPM order.

*Reducing PPPMDisp Coulomb order b/c stencil extends beyond neighbor processor.*
   This may lead to a larger grid than desired.  See the kspace\_modify overlap
   command to prevent changing of the PPPM order.

*Reducing PPPMDisp Dispersion order b/c stencil extends beyond neighbor processor*
   This may lead to a larger grid than desired.  See the kspace\_modify overlap
   command to prevent changing of the PPPM order.

*Replacing a fix, but new group != old group*
   The ID and style of a fix match for a fix you are changing with a fix
   command, but the new group you are specifying does not match the old
   group.

*Replicating in a non-periodic dimension*
   The parameters for a replicate command will cause a non-periodic
   dimension to be replicated; this may cause unwanted behavior.

*Resetting reneighboring criteria during PRD*
   A PRD simulation requires that neigh\_modify settings be delay = 0,
   every = 1, check = yes.  Since these settings were not in place,
   LIGGGHTS(R)-PUBLIC changed them and will restore them to their original values
   after the PRD simulation.

*Resetting reneighboring criteria during TAD*
   A TAD simulation requires that neigh\_modify settings be delay = 0,
   every = 1, check = yes.  Since these settings were not in place,
   LIGGGHTS(R)-PUBLIC changed them and will restore them to their original values
   after the PRD simulation.

*Resetting reneighboring criteria during minimization*
   Minimization requires that neigh\_modify settings be delay = 0, every =
   1, check = yes.  Since these settings were not in place, LIGGGHTS(R)-PUBLIC
   changed them and will restore them to their original values after the
   minimization.

*Restart file used different # of processors*
   The restart file was written out by a LIGGGHTS(R)-PUBLIC simulation running on a
   different number of processors.  Due to round-off, the trajectories of
   your restarted simulation may diverge a little more quickly than if
   you ran on the same # of processors.

*Restart file used different 3d processor grid*
   The restart file was written out by a LIGGGHTS(R)-PUBLIC simulation running on a
   different 3d grid of processors.  Due to round-off, the trajectories
   of your restarted simulation may diverge a little more quickly than if
   you ran on the same # of processors.

*Restart file used different boundary settings, using restart file values*
   Your input script cannot change these restart file settings.

*Restart file used different newton bond setting, using restart file value*
   The restart file value will override the setting in the input script.

*Restart file used different newton pair setting, using input script value*
   The input script value will override the setting in the restart file.

*Restart file version does not match LIGGGHTS(R)-PUBLIC version*
   This may cause problems when reading the restart file.

*Restrain problem: %d %ld %d %d %d %d*
   Conformation of the 4 listed dihedral atoms is extreme; you may want
   to check your simulation geometry.

*Running PRD with only one replica*
   This is allowed, but you will get no parallel speed-up.

*SRD bin shifting turned on due to small lamda*
   This is done to try to preserve accuracy.

*SRD bin size for fix srd differs from user request*
   Fix SRD had to adjust the bin size to fit the simulation box.  See the
   cubic keyword if you want this message to be an error vs warning.

*SRD bins for fix srd are not cubic enough*
   The bin shape is not within tolerance of cubic.  See the cubic
   keyword if you want this message to be an error vs warning.

*SRD particle %d started inside big particle %d on step %ld bounce %d*
   See the inside keyword if you want this message to be an error vs
   warning.

*Shake determinant < 0.0*
   The determinant of the quadratic equation being solved for a single
   cluster specified by the fix shake command is numerically suspect.  LIGGGHTS(R)-PUBLIC
   will set it to 0.0 and continue.

*Should not allow rigid bodies to bounce off relecting walls*
   LIGGGHTS(R)-PUBLIC allows this, but their dynamics are not computed correctly.

*System is not charge neutral, net charge = %g*
   The total charge on all atoms on the system is not 0.0, which
   is not valid for the long-range Coulombic solvers.

*Table inner cutoff >= outer cutoff*
   You specified an inner cutoff for a Coulombic table that is longer
   than the global cutoff.  Probably not what you wanted.

*Temperature for MSST is not for group all*
   User-assigned temperature to MSST fix does not compute temperature for
   all atoms.  Since MSST computes a global pressure, the kinetic energy
   contribution from the temperature is assumed to also be for all atoms.
   Thus the pressure used by MSST could be inaccurate.

*Temperature for NPT is not for group all*
   User-assigned temperature to NPT fix does not compute temperature for
   all atoms.  Since NPT computes a global pressure, the kinetic energy
   contribution from the temperature is assumed to also be for all atoms.
   Thus the pressure used by NPT could be inaccurate.

*Temperature for fix modify is not for group all*
   The temperature compute is being used with a pressure calculation
   which does operate on group all, so this may be inconsistent.

*Temperature for thermo pressure is not for group all*
   User-assigned temperature to thermo via the thermo\_modify command does
   not compute temperature for all atoms.  Since thermo computes a global
   pressure, the kinetic energy contribution from the temperature is
   assumed to also be for all atoms.  Thus the pressure printed by thermo
   could be inaccurate.

*Too many common neighbors in CNA %d times*
   More than the maximum # of neighbors was found multiple times.  This
   was unexpected.

*Too many inner timesteps in fix ttm*
   Self-explanatory.

*Too many neighbors in CNA for %d atoms*
   More than the maximum # of neighbors was found multiple times.  This
   was unexpected.

*Triclinic box skew is large*
   The displacement in a skewed direction is normally required to be less
   than half the box length in that dimension.  E.g. the xy tilt must be
   between -half and +half of the x box length.  You have relaxed the
   constraint using the box tilt command, but the warning means that a
   LIGGGHTS(R)-PUBLIC simulation may be inefficient as a result.

*Use special bonds = 0,1,1 with bond style fene*
   Most FENE models need this setting for the special\_bonds command.

*Use special bonds = 0,1,1 with bond style fene/expand*
   Most FENE models need this setting for the special\_bonds command.

*Using compute temp/deform with inconsistent fix deform remap option*
   Fix nvt/sllod assumes deforming atoms have a velocity profile provided
   by "remap v" or "remap none" as a fix deform option.

*Using compute temp/deform with no fix deform defined*
   This is probably an error, since it makes little sense to use
   compute temp/deform in this case.

*Using fix srd with box deformation but no SRD thermostat*
   The deformation will heat the SRD particles so this can
   be dangerous.

*Using largest cutoff for buck/long/coul/long*
   Self-exlanatory.

*Using largest cutoff for pair\_style lj/long/coul/long*
   Self-explanatory.

*Using largest cutoff for pair\_style lj/long/tip4p/long*
   Self-explanatory.

*Using pair tail corrections with nonperiodic system*
   This is probably a bogus thing to do, since tail corrections are
   computed by integrating the density of a periodic system out to
   infinity.




.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
