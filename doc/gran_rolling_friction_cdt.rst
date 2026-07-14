gran rolling\_friction cdt model
================================

Syntax
""""""


.. parsed-literal::

   rolling_friction cdt [other model_type/model_name pairs as described :doc:`here <pair_gran>` ] keyword values

* zero or more keyword/value pairs may be appended to the end (after all models are specified)

.. parsed-literal::

     *torsionTorque* values = 'on' or 'off'
       on = also the normal, relative rotation (torsion) contributes to the resulting torque
       off = only the tangential, relative rotation contributes to the resulting torque

Description
"""""""""""

This model can be used as part of :doc:`pair gran <pair_gran>` and
:doc:`fix wall/gran <fix_wall_gran>`

The constant directional torque (CDT) model adds an additional torque
contribution, equal to

torque\_rf = rmu\*k\_n\*delta\_n\*w\_r\_shear/mag(w\_r\_shear)\*(R\*).

w\_r\_shear is the projection of w\_r into the shear plane, where w\_r = w1-w2

If the rolling friction model is activated, the coefficient of rolling friction (rmu) must be defined as


.. parsed-literal::

   fix id all property/global coefficientRollingFriction peratomtypepair n_atomtypes value_11 value_12 .. value_21 value_22 .. .
       (value_ij=value for the coefficient of rolling friction between atom type i and j; n_atomtypes is the number of atom types you want to use in your simulation)

.. warning::

   You have to use atom styles beginning from 1, e.g. 1,2,3,...

**Torque information:**

By default the relative, normal rotation (torsion) is subtracted and does not contribute to the resulting torque. By setting the *torsionTorque* keyword to 'on', the full relative rotation contributes to the rolling friciton torque.

**Coarse-graining information:**

Using :doc:`coarsegraining <coarsegraining>` in
combination with this command might lead to
different dynamics or system state and thus to 
inconsistancies.

.. note::

   :doc:`Coarsegraining <coarsegraining>` may or 
   may not be available in Packfall.

Default
"""""""

*torsionTorque* = 'off'

**(Ai)** Jun Ai, Jian-Fei Chen, J. Michael Rotter, Jin Y. Ooi, Powder Technology, 206 (3), p 269-282 (2011).


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
