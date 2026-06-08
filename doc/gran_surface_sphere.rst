gran surface sphere model
=========================

Syntax
""""""


.. parsed-literal::

   surface sphere

Description
"""""""""""

This is the default surface model for a particle, it 
prepresents a smooth (non-rough) sphere.
It is the default surface model for all LIGGGHTS(R)-PUBLIC
simulations. In case of a multi-sphere simulation,
this surface model is applied to all spheres within
one rigid body.

**Coarse-graining information:**

Using :doc:`coarsegraining <coarsegraining>` in
combination with this command should lead to
statistically equivalent dynamics and system state.

.. note::

   :doc:`Coarsegraining <coarsegraining>` may or 
   may not be available in LIGGGHTS(R)-PUBLIC.

Restrictions
""""""""""""
 none


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
