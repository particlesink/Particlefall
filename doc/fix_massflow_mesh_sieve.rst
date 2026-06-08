.. index:: fix massflow/mesh/sieve

fix massflow/mesh/sieve command
===============================

Syntax
""""""


.. parsed-literal::

   fix id group massflow/mesh/sieve mesh mesh-ID point_at_outlet pointX pointY pointZ keyword value ...

* ID, group-ID are documented in :doc:`fix <fix>` command
* massflow/mesh/sieve = style name of this fix command
* mesh = obligatory keyword
* mesh-ID = ID of a :doc:`fix mesh/surface <fix_mesh_surface>` command
* point\_at\_outlet = obligatory keyword
* pointX pointY pointZ = vector components defining the "outside" point of the mesh
* zero or more keyword/value pairs may be appended to args
* keywords = same as for :doc:`fix massflow/mesh <fix_massflow_mesh>`, in addition: *sieveSize* and *sieveSpacing* and  *sieveStiffness*\ , and *sieveDampings*\ , or *sieveMultiSphereCanPass*
  
  .. parsed-literal::
  
       *sieveSize* value = *x in meters*
         this is the diameter of the sieve opening. Used to compute the probability of sieve passage.
       *sieveSpacing* value = *x in meters*
         this is the spacing between sieve openings. Used to compute the probability of sieve passage.
       *sieveStiffness* value = *x in Newton per meter*
         value to compute the linear spring part of the normal repulsive force. WARNING: use a reasonable value to avoid popcorn!
       *sieveDamping* value = *x in Newton per meter/s*
         value to compute the dashpot part of the normal repulsive force. ARNING: use a reasonable value to avoid popcorn!
       *sieveMultiSphereCanPass*
         add this keyword to also calculate the probability of sieve passage for multisphere particles. If not set, it is impossible for multisphere particles to pass the sieve.



**Example:**


.. parsed-literal::

   fix     massFlow1 all massflow/mesh/sieve mesh massFlow1M point_at_outlet 0 0 -1000 count once delete_atoms no file massFlow1.dat sieveSize 2e-3 sieveSpacing 2.5e-3 sieveStiffness 1e5 sieveDamping 1e3

Description
"""""""""""

This fix is an extension of the fix "massflow/mesh", so the reader may wish to consult 
also the documentation of this fix for further details.

Fix massflow/mesh/sieve models a sieve, and counts the particles that cross the sieve 
(same as in massflow/mesh). The sieve is modeled with circular holes in a the mesh (the holes 
have a certain spacing), and a passage probability is computed based on the particle diameter. 
Thus, a probability is computed based on the relative area that would result into particle passage 
divided by the total cross-sectional area of the sieve, assuming particles move perfectly normal 
to the mesh. This probability is evaluated at the first time step of every collision event, and 
compared with a uniformly-distributed random number. Thus, sliding contacts, or multiple collision 
may not by modelled accurately.

Restrictions
""""""""""""


Currently, this feature does not allow deleting multi-sphere particles when crossing the sieve. 
Also, it is strongly recommended to NOT activate *sieveMultiSphereCanPass*\ , since this feature 
is not tested in high detail. For example, it may occur that multisphere particles may get stuck 
when passing the sieve.

Related commands
""""""""""""""""

:doc:`compute nparticles/tracer/region <compute_nparticles_tracer_region>`
:doc:`fix massflow/mesh <fix_massflow_mesh>`

Default
"""""""

*sieveDamping* = 0; *sieveMultiSphereCanPass* = false; all other parameters need to be set by the user.


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
