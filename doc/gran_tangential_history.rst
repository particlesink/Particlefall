gran tangential history model
=============================

Syntax
""""""


.. parsed-literal::

   tangential history [other model_type/model_name pairs as described :doc:`here <pair_gran>` ] keyword values

* zero or more keyword/value pairs may be appended to the end (after all models are specified)

.. parsed-literal::

     *heating_tangential_history* values = 'on' or 'off'
       on = model contributes to surface heating in the frame of :doc:`surface sphere/heatable <gran_surface_sphere_heatable>`
       off = model does not contributes to surface heating

Description
"""""""""""

This granular model is based on the general description of granular
force interaction as described in :doc:`pair gran <pair_gran>`.

The spring part of the tangential force (F\_t = k\_t \* u\_t) is a "history" effect
that accounts for the tangential displacement ("tangential overlap") between the
particles for the duration of the time they are in contact.  The tangential
overlap (u\_t) is calculated by integrating the releative tangential velocity at
the contact point over time.

If this model is chosen, then this "tangential overlap" spring force is
actually calculated / taken into account.

The coefficient of friction cof is the upper limit of the tangential force through
the Coulomb criterion Ft\_spring = cof\*Fn, where Ft\_spring and Fn are the tangential 
spring and total normal force components. Thus in the Hookean case, the tangential force
between 2 particles grows according to a tangential spring and dash-pot model until
Ft\_spring/Fn = cof and is then held at Ft\_spring = Fn\*cof until the particles lose contact.
In the Hertzian case and other non-linear cases, a similar analogy holds, though the 
spring is no longer linear.

The damping contribution is only added in time-steps where there is no slip,
i.e. the Coulomb criterion is not met.

This model contributes to surface heating in the frame of 
:doc:`surface sphere/heatable <gran_surface_sphere_heatable>` if the 
appropriate flag is activated (only available in the PREMIUM version).

**Coarse-graining information:**

Using :doc:`coarsegraining <coarsegraining>` in
combination with this command might lead to
different dynamics or system state and thus to 
inconsistancies. However, the influence of this
model on the global dynamics or system state might 
be small so in some cases the results may be valid. 
This has to be reviewed by a specialist on a 
case-by-case basis.

.. note::

   :doc:`Coarsegraining <coarsegraining>` may or 
   may not be available in LIGGGHTS(R)-PUBLIC.

Default
"""""""

*heating\_tangential\_history* = 'off'


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
