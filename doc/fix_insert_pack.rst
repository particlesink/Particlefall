.. index:: fix insert/pack

fix insert/pack command
=======================

Syntax
""""""


.. parsed-literal::

   fix ID group-ID insert/pack seed seed_value distributiontemplate dist-ID general_keywords general_values pack_keywords pack_values ...

* ID, group-ID are documented in :doc:`fix <fix>` command
* insert/pack = style name of this fix command
* seed = obligatory keyword
* seed\_value = random # seed (prime number greater than 10000)
* distributiontemplate = obligatory keyword
* dist-ID = ID of a :doc:`fix\_particledistribution\_discrete <fix_particledistribution_discrete>` to be used for particle insertion
* one or more general keyword/value pairs can be appended
* general\_keywords = *verbose* or *maxattempt* or *insert\_every* or *insert\_every\_time* or *interval* or *overlapcheck* or *all\_in* or *random\_distribute* or *compress\_tags* or *vel constant* or *vel uniform* or *vel gaussian* or *orientation* or *omega* or *set\_property* or *start* or *start\_time* or *t\_zero* or *stop\_time* or *t\_last* or *inclusion\_mesh* or *inclusion\_side* or *inclusion\_direction* or *inside* or *outside* or *fill\_stop\_above* or *fill\_resume\_below* or *fill\_axis* or *high*
  
  .. parsed-literal::
  
       *verbose* = yes or no
       *maxattempt* value = ma
         ma = max # of insertion attempts per atom (positive integer)
       *insert_every* value = once or ie
         once = value to signalise that insertion takes place only once (the step after the fix is issued)
         ie = every how many time-steps particles are inserted - insertion happens periodically (positive integer)
       *insert_every_time* value = once or te
         te = physical time between insertion events
       *interval* value = once or te
         te = alias for *insert_every_time*
       *start* value = ts
         ts = time-step at which insertion should start (positive integer larger than current time-step)
       *start_time* value = t0
         t0 = physical time at which insertion should start
       *t_zero* value = t0
         t0 = alias for *start_time*
       *stop_time* value = t1
         t1 = physical time after which no further insertion events are scheduled
       *t_last* value = t1
         t1 = alias for *stop_time*
       *overlapcheck* value = yes or no
       *all_in* value = yes or no
       *random_distribute* value = exact or uncorrelated
       *compress_tags* value = yes or no
       *vel constant* values = vx vy vz
         vx = x-velocity at insertion (velocity units)
         vy = y-velocity at insertion (velocity units)
         vz = z-velocity at insertion (velocity units)
       *vel uniform* values = vx vy vz vFluctx vFlucty vFluctz
         vx = mean x-velocity at insertion (velocity units)
         vy = mean y-velocity at insertion (velocity units)
         vz = mean z-velocity at insertion (velocity units)
         vFluctx = amplitude of uniform x-velocity fluctuation at insertion (velocity units)
         vFlucty = amplitude of uniform y-velocity fluctuation at insertion (velocity units)
         vFluctz = amplitude of uniform z-velocity fluctuation at insertion (velocity units)
       *vel gaussian* values = vx vy vz vFluctx vFlucty vFluctz
         vx = mean x-velocity at insertion (velocity units)
         vy = mean y-velocity at insertion (velocity units)
         vz = mean z-velocity at insertion (velocity units)
         vFluctx = standard deviation of Gaussian x-velocity fluctuation at insertion (velocity units)
         vFlucty = standard deviation of Gaussian y-velocity fluctuation at insertion (velocity units)
         vFluctz = standard deviation of Gaussian z-velocity fluctuation at insertion (velocity units)
       *orientation* values = random or template or constant q1 q2 q3 q4
         random = randomize rotational orientation
         template = use orientation from particle template
         constant = use constant quaternion for orientation
         q1 q2 q3 q4 = quaternion values for constant orientation
       *omega* values = constant omegax omegay omegaz
         constant = obligatory word
         omegax = x-component of angular velocity (1/time units)
         omegay = y-component of angular velocity (1/time units)
         omegaz = z-component of angular velocity (1/time units)
       *set_property* values = property-varname val
         property-varname = variable name of a :doc:`fix property/atom <fix_property>` holding a scalar value for each particle
         val = value to initialize the property with upon insertion
       *inclusion_mesh* value = mesh-ID
         mesh-ID = ID of a :doc:`fix mesh/surface or fix mesh/surface/planar <fix_mesh_surface>` used as an inside/outside classifier
       *inclusion_side* value = inside or outside
         inside or outside = keep only candidate points with odd/even crossing parity relative to the inclusion mesh
       *inclusion_direction* value = x or y or z or xy or xz or yz or xyz
         enabled directions = axis-aligned ray tests that must each match the requested inclusion side
       *inside* values = mesh-ID ix iy iz
         mesh-ID = ID of a :doc:`fix mesh/surface or fix mesh/surface/planar <fix_mesh_surface>` used as an inside classifier
         ix, iy, iz = 0/1 flags selecting which axis tests are active
       *outside* values = mesh-ID ix iy iz
         mesh-ID = ID of a :doc:`fix mesh/surface or fix mesh/surface/planar <fix_mesh_surface>` used as an outside classifier
         ix, iy, iz = 0/1 flags selecting which axis tests are active
       *fill_stop_above* value = hstop
         hstop = pause insertion whenever the detected fill level rises above this coordinate
       *fill_resume_below* value = hresume
         hresume = resume insertion only after the detected fill level drops below this coordinate
       *fill_axis* value = x or y or z
         x or y or z = coordinate direction used to measure hopper fill level
       *high* value = hstop
         hstop = shorthand for *fill_stop_above* with *fill_resume_below* set to the same value

* following the general keyword/value section, one or more pack keyword/value pairs can be appended for the fix insert/pack command
* pack\_keywords = *region* or *volumefraction\_region* or *particles\_in\_region* or *mass\_in\_region* or *check\_dist\_from\_subdomain\_border* or *ntry\_mc*
  
  .. parsed-literal::
  
     pack_keywords = where exactly one out of *volumefraction_region* or *particles_in_region* or *mass_in_region* has to be defined:l
       *region* value = region-ID
         region-ID = ID of the :doc:`region <region>` where the particles will be generated 
       *volumefraction_region* values = vol
         vol =  desired volume fraction for the region (positive float, 0 < vol < 1)
       *particles_in_region* values = np
         np =  desired number of particles in the region (positive integer)
       *mass_in_region* values = m
         m =  desired mass in the region (positive float, m > 0)
       *check_dist_from_subdomain_border* values = yes or no
       *ntry_mc* values = n
         n = number of Monte-Carlo steps for calculating the region's volume  (positive integer)



Examples
""""""""


.. parsed-literal::

   fix ins all insert/pack seed 123457 distributiontemplate pdd1 insert_every once overlapcheck yes volumefraction_region 0.3 region mysphere ntry_mc 10000

Description
"""""""""""

Insert particles into a granular run either once or every few timesteps
within the specified region, as defined via the *region* keyword.

The *verbose* keyword controls whether statistics about particle
insertion is output to the screen each time particles are inserted.

This command must use the distributiontemplate keyword to refer to a
:doc:`fix\_particledistribution\_discrete <fix_particledistribution_discrete>` (defined by dist-fix-ID) that defines the properties of the
inserted particles.

At each insertion step, fix insert/pack tries inserts as many particles as
needed to reach a defined target, which can be either a region volume
fraction (keyword *volumefraction\_region*), the total number of particles
in the region (keyword *particles\_in\_region*), or the total particle mass
in the region (keyword *mass\_in\_region*). Exactly one out of the keywords
*volumefraction\_region*, *particles\_in\_region*, *mass\_in\_region* must
be defined.

The frequency of the particle insertion can be controlled by the
keyword *insert\_every*, which defines  the number of time-steps between
two insertions. Alternatively, by specifying *insert\_every once*,
particles are inserted only once.

The *start* keyword can be used to set the time-step at which the insertion
should start. The aliases *start_time* and *t_zero* provide the same
control in physical time units. Likewise, *insert_every_time* and *interval*
let you specify the insertion spacing in physical time units, and
*stop_time* / *t_last* stop scheduling further insertion events after a
physical cutoff time.

Inserted particles are assigned the atom type specified by the
particledistribution defined via the :doc:`fix\_particledistribution\_discrete <fix_particledistribution_discrete>` and
are assigned to 4 groups: the default group "all" and the group
specified in the fix insert command, as well as the groups specified
in the :doc:`fix\_particledistribution\_discrete <fix_particledistribution_discrete>` and :doc:`fix\_particletemplate\_sphere <fix_particletemplate_sphere>`
command (all of which can also be "all").

The keyword *overlapcheck* controls if overlap is checked for at insertion,
both within the inserted particle package and with other existing particles.
If this option is turned off, insertion will scale very well in parallel,
otherwise not. Be aware that in case of no overlap check, highly overlapping
configurations will be produced, so you will have to relax these configurations.

With keyword *check\_dist\_from\_subdomain\_border*, the user can specify if
in case of *overlapcheck* = yes particles are inserted only at least one
bounding radius away from processor domain borders to avoid possible
overlaps with particles inserted on another process.

If overlapcheck if performed, the number of insertion attempts per
particle can be specified via the *maxattempt* keyword. Each timestep
particles are inserted, the command will make up to a total of M tries
to insert the new particles without overlaps, where
M = # of inserted particles \* ma.  If unsuccessful at completing all
insertions, a warning will be printed.

The *all\_in* flag determines if the particle is completely contained
in the insertion region (*all\_in yes*) or only the particle center
(*all\_in no*). Currently *all\_in yes* is not yet supported for all
types of insertion.

Keyword *random\_distribute* controls how the number of particles to
be inserted is distributed among parallel processors and among the
particle templates in the particle distribution. For style *exact*\ ,
the number of particles to be inserted each step is matched exactly.
For style *uncorrelated*\ , the number of particles to be inserted
for each particle template will be rounded in an uncorrelated way,
so the total number of inserted particles may vary for each insertion
step. However, statistically both ways should produce the same
result. Style *uncorrelated* may be faster in parallel since it
does not need global MPI operations.
Please note that if the # of particles to be inserted is calculated
e.g. from a particle mass to be inserted, the number of particles
to be inserted each insertion step will vary by 1, irrespective of
the *random\_distribute* settings. This is because in this case the
# of particles to insert in each step will be a floating point number,
and applying a simple floor/ceil rounding operation would lead to a
statistical bias.

If keyword *compress\_tags* is set to 'yes', then atom IDs are re-assigned
after each insertion procedure. The default is 'no'. This is typically
only recommended if some models internally store arrays which have the
length defined via the max ID of any atom.

.. warning::

   Setting *compress\_tags* to 'yes' will result in all contact
   history to be lost at that point in time. External post-processing tools
   will give wrong results because they typically track atoms via their IDs.

The initial velocity and rotational velocity can be controlled via the
*vel* and *omega* keywords. *vel constant* simply patches a constant
velocity to the inserted particles, *vel uniform* sets uniformly distributed
velocities with mean and amplitude. *vel gaussian* sets Gaussian
distributed particle velocities with mean and std. deviation.

The *set\_property* option can be used to initialize scalar per-particle
properties such as temperatures, which are stored in a 
a :doc:`fix property/atom <fix_property>`.

**Description for fix insert/pack:**

This command must use the *region* keyword to define an insertion
volume. The specified region must have been previously defined with a
:doc:`region <region>` command. Dynamic regions are not supported as insertion region.
Each timestep particles are inserted, they are placed randomly inside the
insertion volume.

If *inclusion\_mesh* is specified, each candidate insertion point is also
classified by a prototype ray-crossing inside/outside test against the given
mesh. *inclusion\_side inside* keeps only points classified as inside the
mesh, while *inclusion\_side outside* keeps only points classified as
outside. *inclusion\_direction* selects which axis-aligned crossing tests
are evaluated; for example, *xy* checks x and y crossings but ignores z.
All enabled directions must agree with the requested side for the point to
be accepted. As a shorthand, *inside mesh-ID ix iy iz* and
*outside mesh-ID ix iy iz* configure the same filter using 0/1 axis flags in
the style of older hopper-filling workflows.

In practical simulations, *mesh-ID* will often be the same
:doc:`fix mesh/surface <fix_mesh_surface>` that is also referenced by a
:doc:`fix wall/gran <fix_wall_gran>` command, so the hopper or vessel mesh
acts as both the real contact geometry and the insertion classifier.

For *insert/pack* and :doc:`fix insert/rate/region <fix_insert_rate_region>`,
the same inside classifier can also act as a hopper refill controller.
*fill_stop_above* pauses insertion if any particle already classified as
inside the hopper has its top extent above the requested coordinate along
*fill_axis*. *fill_resume_below* adds hysteresis so insertion resumes only
after the measured level drops below a lower threshold. The shorthand
*high* sets both thresholds to the same value. This controller requires an
*inside* classifier, not an *outside* classifier.

The *volumefraction* option specifies what volume fraction of the insertion
volume will be filled with particles. The higher the value, the more
particles are inserted each timestep. Since inserted particles should not
overlap, the maximum volume fraction should be no higher than about 0.6.

To determine the volume of the insertion region, a Monte Carlo approach
might be used for some cases where the volume is difficult to calculate
or where the volume calculation is simply not implemented by the region.
The *ntry\_mc* keyword is used to control the number of MC tries that
are used for the volume calculation.

**Restart, fix\_modify, output, run start/stop, minimize info:**

Information about this fix is written to :doc:`binary restart files <restart>`.  This means you can restart a simulation
while inserting particles, when the restart file was written during the
insertion operation.

None of the :doc:`fix\_modify <fix_modify>` options are relevant to this
fix. A global vector is stored by this fix for access by various :ref:`output commands <4_15>`. The first component of the vector is the
number of particles already inserted, the second component is the mass
of particles already inserted. No parameter of this fix can be
used with the *start/stop* keywords of the :doc:`run <run>` command.
This fix is not invoked during :doc:`energy minimization <minimize>`.

Restrictions
""""""""""""


The *overlapcheck* = 'yes' option performs an inherently serial operation
and will thus not scale well in parallel. For this reason, if you
want to generate large systems, you are advised to turn *overlapcheck*
off and let the packing relax afterwards to generate a valid packing.

Option *all\_in* = 'yes' will not work if the :doc:`region <region>` used
is a tet mesh region.

Keywords *duration* and *extrude\_length* can not be used together.

Currently *all\_in yes* is not yet supported for all types of insertion.

Dynamic regions are not supported as insertion region.

The *inclusion\_mesh* prototype is intended for closed,
non-self-intersecting meshes. Points exactly on the surface may give
ambiguous classifications. In parallel runs, the crossing count is reduced
globally across MPI ranks, which is more robust for decomposed meshes but
adds communication overhead to candidate evaluation.

Related commands
""""""""""""""""

:doc:`fix\_insert\_stream <fix_insert_stream>`, 
:doc:`fix\_insert\_rate\_region <fix_insert_rate_region>`,
:doc:`region <region>`

Default
"""""""

The defaults are maxattempt = 50, all\_in = no, overlapcheck = yes
vel = 0.0 0.0 0.0, omega = 0.0 0.0 0.0, start = next time-step,
duration = insert\_every, ntry\_mc = 100000, random\_distribute = exact,
*compress\_tags* = no, *check\_dist\_from\_subdomain\_border* = yes


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
