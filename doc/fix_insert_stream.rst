.. index:: fix insert/stream

fix insert/stream command
=========================

Syntax
""""""


.. parsed-literal::

   fix ID group-ID insert/stream seed seed_value distributiontemplate dist-ID general_keywords general_values stream_keywords stream_values ...

* ID, group-ID are documented in :doc:`fix <fix>` command
* insert/stream = style name of this fix command
* seed = obligatory keyword
* seed\_value = random # seed (prime number greater 1000)
* distributiontemplate = obligatory keyword
* dist-ID = ID of a :doc:`fix\_particledistribution\_discrete <fix_particledistribution_discrete>` to be used for particle insertion
* one or more general keyword/value pairs can be appended
* general\_keywords = *verbose* or *maxattampt* or *nparticles* or *mass* or *particlerate* or *massrate* or *insert\_every* or *insert\_every\_time* or *interval* or *overlapcheck* or *all\_in* or *random\_distribute* or\ *vel constant* or *vel uniform* or *vel gaussian* or *orientation* or *omega* or *set\_property* or *start* or *start\_time* or *t\_zero* or *stop\_time* or *t\_last*
  
  .. parsed-literal::
  
       *verbose* = yes or no
       *maxattempt* value = ma
         ma = max # of insertion attempts per atom (positive integer)
       *nparticles* values = np or INF
         np =  number of particles to insert (positive integer)
         INF =  insert as many particles as possible
       *mass* values = mp
         mp = mass of particles to be inserted (positive float)
         INF =  insert as many particles as possible
       *particlerate* values = pr
         pr = particle insertion rate (particles/time units)
       *massrate* values = mr
         mr = mass insertion rate (mass/time units)
       *insert_every* value = ie
         ie = every how many time-steps particles are inserted - insertion happens periodically (positive integer)
       *insert_every_time* value = te
         te = physical time between insertion events
       *interval* value = te
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

* following the general keyword/value section, one or more stream keyword/value pairs can be appended for the fix insert/stream command
* stream\_keywords = *duration* or *parallel* or *insertion\_face* or *extrude\_length*
  
  .. parsed-literal::
  
       *insertion_face* value = mesh-ID
         mesh-ID = ID of the :doc:`fix mesh/surface or fix mesh/surface/planar <fix_mesh_surface>` to use as starting face for particle generation
       *extrude_length* values = L
         L =  length for extruding the insertion face in normal direction so to generate in insertion volume
       *parallel* values = yes or no
         yes, no = pre-calculate location of overlap of processor subdomains and extrusion volume to some extent
       *duration* values = du
         du =  duration of insertion in time-steps



Examples
""""""""

fix ins all insert/stream seed 123457 distributiontemplate pdd1 nparticles 5000 vel constant 0. -0.5 -2. particlerate 1000 overlapcheck yes insertion\_face ins\_mesh extrude\_length 0.6

Description
"""""""""""

Insert particles into a granular run either once or every few timesteps
within a specified region until either np particles have been inserted
or the desired particle mass (mp) has been reached.

The *verbose* keyword controls whether statistics about particle
insertion is output to the screen each time particles are inserted.

Each timestep particles are inserted, they are placed randomly inside
the insertion volume so as to mimic a stream of poured particles. The
insertion volume is generated by extruding the insertion face as
specified via *insertion\_face* in the direction of the face normal.
The sign of this face normal is automatically flipped so that it
is opposite to the normal component of the insertion velocity.

To specify the number of particles to be inserted, you must use either
the *nparticles* or the *mass* keyword (but not both). In the latter
case, the number of particles to be inserted is calculated from the
mass expectancy given by the particle distribution. The *start* keyword
can be used to set the time-step at which the insertion should start.
The aliases *start_time* and *t_zero* provide the same control in physical
time units. Likewise, *insert_every_time* and *interval* let you specify
the insertion spacing in physical time units, and *stop_time* / *t_last*
stop scheduling further insertion events after a physical cutoff time.

Likewise, you can use the *particlerate* or the *massrate* keyword (but not both)
to control the insertion rate. Particles are not inserted continuously,
but in packets (for efficiency reasons). Particles are inserted again
after enough time has elapsed that the previously inserted particles
have left the insertion volume.

One of the two keywords *insert\_every* and *extrude\_length* must be
provided by the user (but not both).

In case *insert\_every* is defined, this sets the frequency of the particle
insertion directly, i.e. the number of time-steps between two insertions.
The number of particles to be inserted at each insertion event is calculated
from the insertion rate and *insert\_every*.

If *extrude\_length* is specified, the amount of extrusion is fixed and
the insertion frequency is calculated from *extrude\_length* and the insertion
velocity normal to the insertion face.

When defining *insert\_every*, you have the possibility to define the duration
of each insertion via the *duration* keyword. *duration* < *insert\_every*
will generate a "pulsed" stream as opposed to a continuous stream.
Example: Setting *insert\_every* = 1000 and *duration* = 600 will produce
a stream that pours material for 600 time-steps, will pause for 400 time-steps,
pour for another 600 time-steps etc.

As mentioned above, particles are inserted again after enough time has
elapsed that the previously inserted particles have left the insertion
volume. Until the time these particles reach the insertion face, no
other forces affect the particles (pair forces, gravity etc.). Fix
insert/stream internally issues a special integrator to take care
of this. This procedure guarantees that the specified velocity,
omega etc. values are perfectly met at the specified insertion face.

The larger the volume, the more particles that can be inserted at one
insertion step. Insertions will continue until the desired # of particles
has been inserted.

.. note::

   The insertion face must be a planar face, and the insertion
   velocity projected on the face normal must be non-zero.

.. note::

   Keywords *insert\_every* and *extrude\_length* may not be used
   together

.. note::

   Keywords *duration* and *extrude\_length* cannot be used together.

This command must use the distributiontemplate keyword to refer to a
:doc:`fix\_particledistribution\_discrete <fix_particledistribution_discrete>` (defined by dist-fix-ID) that defines the properties of the
inserted particles.

Inserted particles are assigned the atom type specified by the
particledistribution defined via the
:doc:`fix\_particledistribution\_discrete <fix_particledistribution_discrete>` and
are assigned to 4 groups: the default group "all" and the group
specified in the fix insert command, as well as the groups specified
in the :doc:`fix\_particledistribution\_discrete <fix_particledistribution_discrete>`
and :doc:`fix\_particletemplate\_sphere <fix_particletemplate_sphere>`
command (all of which can also be "all").

The keyword *overlapcheck* controls if overlap is checked for at insertion,
both within the inserted particle package and with other existing particles.
If this option is turned off, insertion will scale very well in parallel,
otherwise not. Be aware that in case of no overlap check, highly overlapping
configurations will be produced, so you will have to relax these configurations.

If overlapcheck if performed, the number of insertion attempts per
particle can be specified via the *maxattempt* keyword. Each timestep
particles are inserted, the command will make up to a total of M tries
to insert the new particles without overlaps, where
M = # of inserted particles \* ma.  If unsuccessful at completing all
insertions, a warning will be printed.

The *all\_in* flag determines if the particle is completely contained
in the insertion region (*all\_in = yes*) or only the particle center
(*all\_in = no*).Using *all\_in* = yes requires you to use an insertion
face of style :doc:`fix mesh/surface/planar <fix_mesh_surface>`

.. note::

   You also have to use :doc:`fix mesh/surface/planar <fix_mesh_surface>`
   if there is a :doc:`run <run>` command between the definition of the
   insertion face and the fix insert/stream command. Otherwise, a
   :doc:`fix mesh/surface/planar <fix_mesh_surface>` will do.

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

If keyword *parallel* is set to 'yes', LIGGGHTS(R)-PUBLIC tries to pre-calculate
more accurately the overlap of process subdomains and extrusion volume.
For cases where the insertion volume is highly divided between different
processes, this can lead to a speed-up of insertion as random number generation
is more efficient. For cases where the extrusion volume is divided
among few processes this will impose a small computation overhead.

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
The insertion velocity must be non-zero.

The *set\_property* option can be used to initialize scalar per-particle
properties such as temperatures, which are stored in a 
a :doc:`fix property/atom <fix_property>`.

The setting of *compress\_tags* will trigger a periodic re-tagging of
atom ids. This is usefull in case the simulation domain is used to
model a periodic in and outflow of particles. If this switch is set,
the global ids of the particles will remain in a certain range,
and no "holes" in the arrays holding the global atom ids exist.
IMPORTANT NOTE: This functionality may confuse external tools which 
perform post-processing based on atom IDs!

**Restart, fix\_modify, output, run start/stop, minimize info:**

Information about this fix is written to :doc:`binary restart files <restart>`.  This means you can restart a simulation simulation
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


Keywords *duration* and *extrude\_length* can not be used together.
The insertion face cannot move.
Related commands
""""""""""""""""

:doc:`fix\_insert\_pack <fix_insert_pack>`, 
:doc:`fix\_insert\_rate\_region <fix_insert_rate_region>`,

Default
"""""""

The defaults are maxattempt = 50, all\_in = no, overlapcheck = yes
vel = 0.0 0.0 0.0, omega = 0.0 0.0 0.0, start = next time-step,
duration = insert\_every, random\_distribute = exact, parallel = no,
*compress\_tags* = no


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
