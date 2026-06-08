.. index:: fix massflow/mesh

fix massflow/mesh command
=========================

Syntax
""""""


.. parsed-literal::

   fix id group massflow/mesh mesh mesh-ID vec_side vx vy vz keyword value ...

* ID, group-ID are documented in :doc:`fix <fix>` command
* massflow/mesh = style name of this fix command
* mesh = obligatory keyword
* mesh-ID = ID of a :doc:`fix mesh/surface <fix_mesh_surface>` command
* vec\_side = obligatory keyword
* vx, vy, vz = vector components defining the "outside" of the mesh
* zero or more keyword/value pairs may be appended to args
* keywords = *count* or *point\_at\_outlet* or *append* or *file* or *screen* or *delete\_atoms*
  
  .. parsed-literal::
  
       *count* value = *once* or *multiple*
         once = count particles only once
         multiple = allow particles to be counted multiple times
       *point_at_outlet*  pointX pointY pointZ
         pointX pointY pointZ = coordinates of point on the outlet side of the surface
       *inside_out*
         use this in connection with *point_at_outlet* to flip direction particle counting
       *file* value = filename
       *append* value = filename
        filename = name of the file to print diameter, position and velocity values of the particles
       *screen* value = *yes* or *no*
       *writeTime*
         include this keyword to write the time to the out files
       *delete_atoms* value = *yes* or *no*
         yes = to remove the particles that pass through the mesh surface



Examples
""""""""


.. parsed-literal::

   fix mass all massflow/mesh mesh inface vec_side 0. 0. -1.

   fix mass all massflow/mesh mesh inface count once point_at_outlet 0. 0. 0.

Description
"""""""""""

Fix massflow/mesh tracks how many particles penetrate through a mesh surface,
as defined by a :doc:`fix mesh/surface <fix_mesh_surface>` command.
It counts the total number of particles and the associated mass. Only particles
part of *group* are eligible for counting.

Particles are counted if they cross from the inner side of the mesh to the outer
side of the mesh. The outer side can be defined either by using the keyword *vec\_side* or 
by specifying a point at the outlet side of the mesh (keyword *point\_at\_outlet*). Note
that the vector defined by *vec\_side* does not necessarily have to be
perpendicular to the mesh face.

The following restrictions apply in case *vec\_side* is specified: (i) the
:doc:`fix mesh/surface <fix_mesh_surface>` has to be planar, and (ii) the vector
defined by *vec\_side* may not lie inside the mesh plane.

The following restriction applies in case *point\_at\_outlet* is used: the *count*
value has to be set to once.

The keyword *point\_at\_outlet* is especially useful in case a cylindrically-shaped
surface is used. The *point\_at\_outlet* value should be on the cylinder axis in
this case. If you like to track particles moving away from the cylinder axis,
specify the *point\_at\_outlet* on the axis, and use the keyword
*inside\_out* to flip the direction.

When *count* = once, then each particle is only counted once, for *count* = multiple
a particle contributes to number and mass count each time it crosses the mesh face.
This can happen e.g. in the case of periodic boundary conditions or in re-circulating
flow conditions.

The diameter, position and velocity of the particles can be written into a file using the *file* keyword,
by specifying a filename.

.. warning::

   When using fix massflow/mesh with multispherical particles,
   the evaluation of the mass passing the mesh is done for each sphere within the
   multisphere particle. Each sphere counts as a fraction of a particle, i.e. 
   floating point number might evolve for the number of particles passing the
   mesh if not all spheres belonging to one multisphere particle pass the mesh.
   The weight for each sphere is calculated taking into account the possible overlap 
   between spheres in a multisphere particle.

If the *screen* keyword is used, output by this fix to the screen and
logfile can be turned on or off as desired.

If the *delete\_atoms* keyword is used then the particles passing through the mesh
surface are deleted at the next re-neighboring step.

**Restart, fix\_modify, output, run start/stop, minimize info:**

Information about this fix is written to :doc:`binary restart files <restart>` .

This fix computes a per-atom vector (the marker) which can be accessed by
various :ref:`output commands <howto_8>`. The per-atom vector
(i.e., the marker) can be accessed by dumps by using "f\_massflow\_ID", .
This fix also computes a global vector of length 6. This vector can be
accessed via "f\_ID", where ID is the fix id. The first vector component
is equal to the total mass which has crossed the mesh surface, the second vector
component indicates the particle count. The third vector component
is equal to the total mass which has crossed the mesh surface since the last output
divived by the time since the last output (i.e., the mass flow rate), the fourth vector
component indicates the particle count since the last output divived by the time
since the last output (i.e., the number rate of particles). The fifth and sixth vector
components are the deleted mass and the number of deleted particles. This vector
can also be accessed by various :ref:`output commands <howto_8>`.

Restrictions
""""""""""""


none

Related commands
""""""""""""""""

:doc:`compute nparticles/tracer/region <compute_nparticles_tracer_region>`

Default
"""""""

*count* = multiple, *inside\_out*  =false, *delete\_atoms* = false


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
