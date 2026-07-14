gran rolling\_friction epsd3 model
==================================

Syntax
""""""


.. parsed-literal::

   rolling_friction epsd3 [other model_type/model_name pairs as described :doc:`here <pair_gran>` ] keyword values

* zero or more keyword/value pairs may be appended to the end (after all models are specified)

.. parsed-literal::

     *torsionTorque* values = 'on' or 'off'
       on = also the normal, relative rotation (torsion) contributes to the resulting torque
       off = only the tangential, relative rotation contributes to the resulting torque

Description
"""""""""""

This model can be used as part of :doc:`pair gran <pair_gran>` and
:doc:`fix wall/gran <fix_wall_gran>`

The elastic-plastic spring-dashpot (EPSD) model (see Ai et al.) adds an additional
torque contribution, equal to

.. image:: Eqs/pair_gran_hooke_eqEpsd1.gif
   :align: center

where the torque due to the spring M\_rk is calculated as

.. image:: Eqs/pair_gran_hooke_eqEpsd2.gif
   :align: center

Here k\_r denotes the rolling stiffness and dtheta\_r is the incremental relative
rotation between the particles. The spring torque is limited by the full mobilisation
torque M\_rm that is determined by the normal force F\_n and the coefficient of rolling
friction (rmu) (compare the :doc:`CDT model <gran_rolling_friction_cdt>`).

The rolling stiffness k\_r is computed using the "coeffRollingStiffness" prefactor, 
that needs to be defined by the user (see below). k\_r is computed from 
k\_r = coeffRollingStiffness\*k\_n\*rmu\*rmu\*reff\*reff.

The viscous damping torque M\_rd is implemented as

.. image:: Eqs/pair_gran_hooke_eqEpsd3.gif
   :align: center

where in the current implementation the damping is disabled in case of full mobilisation
(f = 0). The damping coefficient C\_r may be expressed as:

.. image:: Eqs/pair_gran_hooke_eqEpsd4.gif
   :align: center

Here I\_i/j is the moment of inertia and m\_i/j is the mass of the particles i and j,
respectively.

The coefficient of rolling friction (rmu) must be defined as


.. parsed-literal::

   fix id all property/global coefficientRollingFriction peratomtypepair n_atomtypes value_11 value_12 .. value_21 value_22 .. .
       (value_ij=value for the coefficient of rolling friction between atom type i and j; n_atomtypes is the number of atom types you want to use in your simulation)

This coefficient rmu is equal to the rmu as defined in the :doc:`CDT model <gran_rolling_friction_cdt>`.
In addition to rmu, eta\_r is the required material property that must be defined as


.. parsed-literal::

   fix id all property/global coefficientRollingViscousDamping peratomtypepair n_atomtypes value_11 value_12 .. value_21 value_22 .. .
       (value_ij=value for the coefficient of rolling friction between atom type i and j; n_atomtypes is the number of atom types you want to use in your simulation)

The "coeffRollingStiffness" prefactor needs to be defined by the user as


.. parsed-literal::

   fix     id all property/global coeffRollingStiffness scalar value

Please see Ai et al., 2011 for discussion.

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
