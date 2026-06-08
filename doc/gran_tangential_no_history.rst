gran tangential no\_history model
=================================

Syntax
""""""


.. parsed-literal::

   tangential no_history [other model_type/model_name pairs as described :doc:`here <pair_gran>` ] keyword values

* zero or more keyword/value pairs may be appended to the end (after all models are specified)

.. parsed-literal::

     *disableTangentialWhenBonded* values = 'on' or 'off'
       on = if the :doc:`cohesion bond <gran_cohesion_bond>` model is used, then the tangential force is only added if the two particles are not bonded
       of = the normal force is always added if two particles overlap

     *computeDissipatedEnergy* values = 'on' or 'off'
       on = the tangential model saves the dissipated energy for each contact for the
       use in :doc:`fix calculate/dissipated_energy <fix_calculate_dissipated_energy>`
       of = no values are saved

**LIGGGHTS(R)-PUBLIC vs. LAMMPS Info:**

This part of :doc:`pair gran <pair_gran>` and
:doc:`fix wall/gran <fix_wall_gran>`
is not availabe in LAMMPS.

Description
"""""""""""

This granular model is based on the general description of granular
force interaction as described in :doc:`pair gran <pair_gran>`.

If this model is chose, then this "tangential overlap" spring force is
NOT calculated / taken into account, i.e. k\_t = 0.

The coefficient of friction cof is the upper limit of the tangential force through
the Coulomb criterion Ft = cof\*Fn, where Ft and Fn are the tangential spring and normal
force components in the formulas above.

When the :doc:`cohesion model bond <gran_cohesion_bond>` is used the
*disableTangentialWhenBonded* keyword can be used. If this parameter is set to 'on' then
the tangential model will only compute its contribution if the two neighboring particles
do not have an active bond.

**Coarse-graining information:**

Using :doc:`coarsegraining <coarsegraining>` in
combination with this command should lead to
statistically equivalent dynamics in the system.

.. note::

   :doc:`Coarsegraining <coarsegraining>` may or 
   may not be available in LIGGGHTS(R)-PUBLIC.


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
