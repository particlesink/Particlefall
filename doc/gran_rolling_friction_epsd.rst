gran rolling\_friction epsd model
=================================

Syntax
""""""


.. parsed-literal::

   rolling_friction epsd [other model_type/model_name pairs as described :doc:`here <pair_gran>` ] keyword values

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

Here k\_r denotes the rolling stiffness that depends on the stiffness of the normal spring (from the normal contact law), the effective radius and the coefficient of rolling friction (mu\_r). Following **(Ai)** the prefactor of 2.25 is valid for 3D simulations. The :doc:`EPSD3 model <gran_rolling_friction_epsd3>` allows to modify the prefactor. dtheta\_r is the incremental relative
rotation between the particles. The spring torque is limited by the full mobilisation
torque M\_rm that is determined by the normal force F\_n and the coefficient of rolling
friction (mu\_r) (compare the :doc:`CDT model <gran_rolling_friction_cdt>`).

The viscous damping torque M\_rd is implemented as

.. image:: Eqs/pair_gran_hooke_eqEpsd3.gif
   :align: center

where in the current implementation the damping is disabled in case of full mobilisation
(f = 0). The damping coefficient C\_r may be expressed as:

.. image:: Eqs/pair_gran_hooke_eqEpsd4.gif
   :align: center

Here I\_i/j is the moment of inertia and m\_i/j is the mass of the particles i and j,
respectively.

The coefficient of rolling friction (mu\_r) must be defined as


.. parsed-literal::

   fix id all property/global coefficientRollingFriction peratomtypepair n_atomtypes value_11 value_12 .. value_21 value_22 .. .
       (value_ij=value for the coefficient of rolling friction between atom type i and j; n_atomtypes is the number of atom types you want to use in your simulation)

This coefficient mu\_r is equal to the mu\_r as defined in the :doc:`CDT model <gran_rolling_friction_cdt>`.
In addition to mu\_r, eta\_r is the required material property that must be defined as


.. parsed-literal::

   fix id all property/global coefficientRollingViscousDamping peratomtypepair n_atomtypes value_11 value_12 .. value_21 value_22 .. .
       (value_ij=value for the coefficient of rolling friction between atom type i and j; n_atomtypes is the number of atom types you want to use in your simulation)

.. warning::

   You have to use atom styles beginning from 1, e.g. 1,2,3,...

**Torque information:**

By default the relative, normal rotation (torsion) is subtracted and does not contribute to the resulting torque. By setting the *torsionTorque* keyword to 'on', the full relative rotation contributes to the rolling friciton torque.

**Coarse-graining information:**

Using :doc:`coarsegraining <coarsegraining>` in
combination with this command might lead to
different dynamics or system state and thus to 
inconsistencies.

.. note::

   :doc:`Coarsegraining <coarsegraining>` may or 
   may not be available in LIGGGHTS(R)-PUBLIC.

Default
"""""""

*torsionTorque* = 'off'

**(Ai)** Jun Ai, Jian-Fei Chen, J. Michael Rotter, Jin Y. Ooi, Powder Technology, 206 (3), p 269-282 (2011).


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
