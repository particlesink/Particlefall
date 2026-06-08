.. index:: fix property/atom/tracer

fix property/atom/tracer command
================================

Syntax
""""""


.. parsed-literal::

   fix id group property/atom/tracer region_mark region-ID mark_step s keyword value ...

* ID, group-ID are documented in :doc:`fix <fix>` command
* property/atom/tracer = style name of this fix command
* region\_mark = obligatory keyword
* region-ID = ID of region atoms must be in to be marked
* mark\_step = obligatory keyword
* s = step when atoms are marked (or started to be marked, depending on marker\_style)
* zero or more keyword/value pairs may be appended to args
* keyword = *marker\_style* or *check\_mark\_every*
  
  .. parsed-literal::
  
       *marker_style* value = *dirac* or *heaviside*
         dirac = use a dirac impulse at time step s to mark the particles
         heaviside = use a dirac impulse staring at time step s to mark the particles
       *check_mark_every* value = n
         n = check every that many time-step if atom are in region to be marked



Examples
""""""""


.. parsed-literal::

   fix tracer all property/atom/tracer region_mark mark mark_step 10000 marker_style dirac check_mark_every 10

Description
"""""""""""

Fix property/atom/tracer marks particles using either a Dirac delta
impulse (default) or a Heaviside impulse, as specified by the
*marker\_style* keyword. Particles are marked if they are inside the region
specified by the *region\_mark* keyword. Using the Dirac impulse means that all the
particles which are in the region at the time-step specified by the
*mark\_step* keyword are marked. Using the Heaviside impulse means that all
the particles which pass by the specified region after the specified time-step
are marked.

Keyword *check\_mark\_every* can be used to control how often the region
is checked. Typically, this is useful when the *heaviside* option is
used, because you may not want to check each particle each time-step.
However, be careful not to choose this value too large, in this case
you could skip particles passing through the region.

It is useful to combine this command with a
:doc:`compute nparticles/tracer/region <compute_nparticles_tracer_region>`
command to compute residence time distributions.

.. warning::

   Using :doc:`compute nparticles/tracer/region <compute_nparticles_tracer_region>`
   can change the tracer value (keyword **reset\_marker**.)

**Restart, fix\_modify, output, run start/stop, minimize info:**

Information about this fix is written to :doc:`binary restart files <restart>` .

This fix computes a per-atom scalar (the marker) which can be accessed by
various :ref:`output commands <howto_8>`. This scalar is equal to 1
if a particle has been marked and 0 otherwise.  This fix also computes a global
scalar indicating how many particles were marked since the last time the global
scalar was computed. This scalar can also be accessed by various :ref:`output commands <howto_8>`.

Restrictions
""""""""""""


Currently, this feature does not support multi-sphere particles.

Related commands
""""""""""""""""

:doc:`compute nparticles/tracer/region <compute_nparticles_tracer_region>`

Default
"""""""

*marker\_style* = dirac, check\_mark\_every = 10


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
