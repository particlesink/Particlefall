gran surface superquadric model
===============================

Syntax
""""""


.. parsed-literal::

   surface superquadric [other model_type/model_name pairs as described :doc:`here <pair_gran>` ] keyword values

* zero or more keyword/value pairs may be appended to the end (after all models are specified)

.. parsed-literal::

     *curvatureLimitFactor* values = more or equal *0*
     *gaussianCurvature* values = *yes* or *no* 
     *meanCurvature* values = *yes* or *no*

Description
"""""""""""

This is the surface model for smooth convex superquadric particles. For contact detection and force calculation algorithms see Podlozhnyuk et al (2017). Calculation of particle radius R1 and R2 in :doc:`Hertz <gran_model_hertz>` or :doc:`Hooke <gran_model_hooke>` model is based on mean or gaussian curvature radius at a contact point. The effective radius R\* is limited by the factor of *curvatureLimitFactor* with respect to R\* calculated using R1 and R2 as volume equivalent radii. If *curvatureLimitFactor=0* particle radii R1 and R2 are volume equivalent radii that don't depend on the contact point.

This model is used in the framework of SUPERQUADRIC simulations. For more information see also the :doc:`SUPERQUADRIC guide <superquadric_simulations>`.

Restrictions
""""""""""""
 requires :doc:`atom\_style <atom_style>` *superquadric*\ .

*meanCurvature* and *gaussianCurvature* cannot be simultaneously *yes*

**Coarse-graining information:**

Using :doc:`coarsegraining <coarsegraining>` in
combination with this command might lead to
statistically different dynamics and system state.
To the best knowledge of the developers, the 
cross-influence between this command and coarse-graining
is unknown.

Default
"""""""

*curvatureLimitFactor* = 0

*gaussianCurvature* = 'no'

*meanCurvature* = 'no'

**(Podlozhnyuk et al.)** A.Podlozhnyuk, S.Pirker, C.Kloss, J.Comp.Part.Mech, 4 (1), p 1-18 (2017).


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
