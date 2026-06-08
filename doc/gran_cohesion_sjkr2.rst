gran cohesion sjkr2 model
=========================

Syntax
""""""


.. parsed-literal::

   cohesion sjkr2

* zero or more keyword/value pairs may be appended to the end (after all models are specified)

.. parsed-literal::

     *tangential_reduce* values = 'on' or 'off'
       on = tangential model does not see normal force computed by this model
       off = tangential model does see normal force computed by this model

Description
"""""""""""

This model can be used as part of :doc:`pair gran <pair_gran>` and
:doc:`fix wall/gran <fix_wall_gran>`

The modified simplified JKR - Johnson-Kendall-Roberts (SJKR2) model adds an additional normal force
contribution. If two particle are in contact, it adds an additional normal force tending
to maintain the contact, which writes

F = k A,

where A is the particle contact area and k is the cohesion energy density in J/m3.
For *sjkr2*\ , the sphere-sphere contact area is calculated as

A = 2\*Pi \* delta\_n \* (2R\*)

If you are using the SJKR2 model, you must also define the cohesion energy density:


.. parsed-literal::

   fix id all property/global cohesionEnergyDensity peratomtypepair n_atomtypes value_11 value_12 .. value_21 value_22 .. .
       (value_ij=value for the cohesion energy density (in Energy/Length3 units) between atom type i and j; n_atomtypes is the number of atom types you want to use in your simulation)

.. warning::

   The cohesion model has been derived for the Hertzian Style, it may note be
   appropriate for the Hookean styles.

.. warning::

   You have to use atom styles beginning from 1, e.g. 1,2,3,...

The optional keyword *tangential\_reduce* defines if the tangential force model should
"see" the additional normal force excerted by this model. If it is 'off' (which is default)
then the tangential force model will be able to transmit a larger amount of tangential force
If *tangential\_reduce* = 'on' then the tangential model will not take the normal force
from this model into account, typically leading to a lower value of tangential force
(via the Coulomb friction limit)

Restrictions
""""""""""""


The cohesion model has been derived for the Hertzian Style, it may note be
appropriate for the Hookean styles.

It is not available for :doc:`atom\_style <atom_style>` superquadric

**Coarse-graining information:**

Using :doc:`coarsegraining <coarsegraining>` in
combination with this command might lead to
statistically different dynamics and system state.
To the best knowledge of the developers, the 
cross-influence between this command and coarse-graining
is unknown.

.. note::

   :doc:`Coarsegraining <coarsegraining>` may or 
   may not be available in LIGGGHTS(R)-PUBLIC.

Default
"""""""

*tangential\_reduce* = 'off'


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
