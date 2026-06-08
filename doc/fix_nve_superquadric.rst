.. index:: fix nve/superquadric

fix nve/superquadric command
============================

Syntax
""""""


.. parsed-literal::

   fix ID group-ID nve/superquadric keyword value

* ID, group-ID are documented in :doc:`fix <fix>` command
* nve/superquadric = style name of this fix command
* zero or one keyword/value pair may be appended to args; the *dim* keyword must be used
* keyword = *integration\_scheme*
  
  .. parsed-literal::
  
       *integration_scheme* values = 0, 1 or 2
         0 = based on Richardson iterations
         1 = symplectic algorithm (recommended)
         2 = leap-frog predictor-corrector
         3 = `Woodem <http://woodem.org/theory/leapfrog.html#orientation>`_ scheme



Examples
""""""""


.. parsed-literal::

   fix 1 all nve/superquadric integration_scheme 1

Description
"""""""""""

Perform constant NVE integration to update position, velocity,
orientation, and angular velocity for superquadric particles in the
group each timestep.  V is volume; E is energy.  This creates a system
trajectory consistent with the microcanonical ensemble.

This fix differs from the :doc:`fix nve/sphere <fix_nve_sphere>` command, which
assumes point particles and only updates their position and velocity.

This model is used in the framework of SUPERQUADRIC simulations. For more information see also the :doc:`SUPERQUADRIC guide <superquadric_simulations>`.

**Restart, fix\_modify, output, run start/stop, minimize info:**

No information about this fix is written to :doc:`binary restart files <restart>`.  None of the :doc:`fix\_modify <fix_modify>` options
are relevant to this fix.  No global or per-atom quantities are stored
by this fix for access by various :ref:`output commands <howto_8>`.  No parameter of this fix can
be used with the *start/stop* keywords of the :doc:`run <run>` command.
This fix is not invoked during :doc:`energy minimization <minimize>`.

Restrictions
""""""""""""


This fix requires :doc:`atom\_style <atom_style>` superquadric.

All particles in the group must be finite-size.  They cannot be point
particles, but they can be aspherical/spherical as defined by their
shape attribute.

Related commands
""""""""""""""""

:doc:`fix nve <fix_nve>`, :doc:`fix nve/sphere <fix_nve_sphere>`

Default
"""""""
*integration\_scheme* = 1

**( `Miller <http://scitation.aip.org/content/aip/journal/jcp/116/20/10.1063/1.1473654>`_ )** Miller. et al., Journal of Chemical Physics, 116 (20), p 8649-8659 (2002)

**( `Walton <http://www.grainflow.com/index_files/Rotary_Drum_Simulation_DOE-NSF-1993.pdf>`_ )** Walton and Braun, DOE/NSF Workshop on Flow of Particulates and Fluids, p 1-18 (1993)


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
