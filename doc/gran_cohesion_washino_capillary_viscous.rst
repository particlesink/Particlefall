gran cohesion washino/capillary/viscous model
=============================================

Syntax
""""""


.. parsed-literal::

   cohesion washino/capillary/viscous [other model_type/model_name pairs as described :doc:`here <pair_gran>` ] keyword values

* zero or more keyword/value pairs may be appended to the end (after all models are specified)

.. parsed-literal::

     *limitLiquidContent* values = 'on' or 'off'
       on = enables the model parameter *maxLiquidContent*
       off = standard implementation without a limiter for the per particle liquid content
     *modifyLbVolume* values = 'on' or 'off'
       on = enables the model parameter *lbVolumeFraction*
       off = standard implementation with lbVolumeFraction = 0.05
     *tangential_reduce* values = 'on' or 'off'
       on = tangential model does not see normal force computed by this model
       off = tangential model does see normal force computed by this model

Description
"""""""""""

This model can be used as part of :doc:`pair gran <pair_gran>`
It adds a liquid bridge force, caused by a surface liquid film on the particles,
to a pair of particles, which consists
of a capilary and a viscous part. Furthermore, it solves for the
transfer of surface liquid from one particle to the other as the bridge breaks up.
The model uses a parameter, *maxSeparationDistanceRatio*\ , to apply a cut-off
to the liquid bridge force, i.e. *radius*\ \*\ *maxSeparationDistanceRatio* is
the effective contact radius of the particles.

The model parameter *maxLiquidContent* allows to limit the maximum per particle 
liquid content (enabled by keyword *limitLiquidContent*\ ). In case this feature is
enabled, the maxSeparationDistanceRation will be overwritten by a value that
allows the maximum volume to be achieved.

The model parameter *lbVolumeFraction* defines the amout of liquid that formes the liquid bridge with a neighbouring particle. (enabled by the keyword *modifyLbVolume*\ )


----------


Bridge formation and break-up, surface liquid transfer:

*Vbond*\ , the volume of surface liquid involved in the bridge, is given by


.. parsed-literal::

   Vbond = lbVolumeFraction \* (surfaceLiquidVolI+surfaceLiquidVolJ)

where *surfaceLiquidVolI/J* is the surface liquid volume attached to particle i/j.
This model assumes that both formation distance and rupture distance
are given as follows by ref`(Lian) <Lian>` 


.. parsed-literal::

   dist0 = (1+contactAngleEff/2)\*Vbond\^(1/3)

When the bridge breaks, it is assumed that the surface liquid volume
distributes evenly to the two particles.


----------


Capilary force:

The capilary force is given by :ref:`(Rabinovitch) <Rabinovitch>` as


.. parsed-literal::

   Fcapilary = 2\*pi\*rEff\*surfaceTension\* (cos(contactAngleEff) / (1. + dist/(2.\*dSpSp)) + sin(alpha)\*sin(alpha+contactAngleEff))

where


.. parsed-literal::

   prefactor = -1+sqrt(1+2\*volBond/(pi\*rEff\*dist\^2))
   dSpSp = 0.5\*dist\*prefactor
   alpha = sqrt(dist/rEff\*prefactor)
   contactAngleEff = 0.5\*(contactAngleI+contactAngleJ)

*dist* is the distance between the particles' surfaces, *surfaceTension* is the surface tension
of the fluid, *contactAngleI/J* are the contact angles for particle i/j and the fluid.


----------


Viscous force:

the normal and tangential parts of the viscous force are calculated as given by ref`(Nase) <Nase>` 


.. parsed-literal::

   rEff = radi\*radj/(radi+radj)
   stokesPreFactor = 6\*pi\*fluidViscosity\*rEff
   FviscN = stokesPreFactor\*vn\*rEff/dist
   FviscT = stokesPreFactor\*vt\*(8/15\*log(rEff/dist)+0.9588)

where *vn* and *vt* are the normal and tangential relative velocities of the
particles at the contact point, *fluidViscosity* is the viscosity of the fluid
and *ri* and *rj* are the particle radii. An additional parameter,
*minSeparationDistanceRatio*\ , is used to prevent the value of the viscous force from
becoming too large, i.e. *radius*\ \*\ *minSeparationDistanceRatio* is assumed to be
the minimum separation distance.


----------


Computation of liquid transport and effect of liquid content on other particle properties:

Per default, this model automatically instatiates a scalar transport equation that solved for
the surface liquid content of each particles, expressed in volume % of solid volume
(4/3 pi \* radius \^3). The surface liquid volume is assumed to be small, i.e. it is assumed
to have no effect on the particle mass, diameter and density.

The user can override the default behavior by explicity specifing a fix that solves
for the surface liquid transport between particles. Such fixes are
:doc:`fix liquidtransport/porous <fix_liquidtransport_porous>` or
:doc:`fix liquidtransport/sponge <fix_liquidtransport_sponge>`


----------


Initialization:


.. parsed-literal::

   If you are using the this model model, you must define the following properties:
   fix id all property/global minSeparationDistanceRatio scalar value
       (value=value for the minimum separation distance, recommended as 1.01)
   fix id all property/global maxSeparationDistanceRatio scalar value
       (value=value for the maximum separation distance, recommended as 1.1)
   fix id all property/global surfaceLiquidContentInitial scalar value
       (value=value for the initial surface liquid volume in % of the solid volume)
   fix id all property/global surfaceTension scalar value
       (value=value for the surface tension of liquid (SI units N/m))
   fix id all property/global fluidViscosity scalar value
       (value=value for the fluidViscosity (SI units Pas))
   fix id all property/global contactAngle peratomtype value_1 value_2 ...
       (value_i=value for contact angle of atom type i and fluid)
   fix id all property/global maxLiquidContent peratomtype value_1 value_2 ...
       (value_i=value for maximum liquid content of an atom of type i in % of particle volume)

The optional keyword *tangential\_reduce* defines if the tangential force model should
"see" the additional normal force excerted by this model. If it is 'off' (which is default)
then the tangential force model will be able to transmit a larger amount of tangential force
If *tangential\_reduce* = 'on' then the tangential model will not take the normal force
from this model into account, typically leading to a lower value of tangential force
(via the Coulomb friction limit)

**Output:**

This gran model stores a couple of per-particle properties, for access
by various :ref:`output commands <4_15>`.

You can access the property surfaceLiquidContent by f\_surfaceLiquidContent[0]
(units % of solid particle volume), liquidFlux (units % of solid particle volume/time)
by accessing f\_liquidFlux[0] and liquidSource (units % of solid particle volume/time)
by accessing f\_liquidSource[0]. The latter can be used to manually set a
surface liquid source via the :doc:`set <set>` command.

Currently, there is a restriction that these properties can only be accessed
after a :doc:`run 0 <run>` command.

Restrictions
""""""""""""


This model can ONLY be used as part of :doc:`pair gran <pair_gran>`, not as part
of a :doc:`fix wall/gran <fix_wall_gran>`.

The cohesion model is not available for :doc:`atom\_style <atom_style>` superquadric

**Coarse-graining information:**

Using :doc:`coarsegraining <coarsegraining>` in
combination with this command might lead to
different dynamics or system state and thus to 
inconsistancies.

.. note::

   :doc:`Coarsegraining <coarsegraining>` may or 
   may not be available in Packfall.

**References:**

.. _Lian:



**(Lian)** Lian, Thornton, Adams, Journal of Colloid and Interface Science, p134, 161 (1993).

.. _Nase:



**(Nase)** S. T. Nase, W. L. Vargas, A. A. Abatan, and J. J. Mc-carthy, Powder Technol 116, 214 (2001).

.. _Rabinovitch:



**(Rabinovitch)** Rabinovitch, Esayanur, Moudil, Langmuir, p10992, 21 (2005)

Default
"""""""

*tangential\_reduce* = 'off', lbVolumeFraction = 0.05


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
