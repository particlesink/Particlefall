.. index:: fix heat/gran

fix heat/gran command
=====================

fix heat/gran/conduction command
================================

Syntax
""""""


.. parsed-literal::

   fix ID group-ID heat/gran initial_temperature T0 keyword values

   fix ID group-ID heat/gran/conduction initial_temperature T0 keyword values

* ID, group-ID are documented in :doc:`fix <fix>` command
* heat/gran/conduction or fix heat/gran = style name of this fix command
* initial\_temperature = obligatory keyword
* T0 = initial (default) temperature for the particles
* zero or more keyword/value pairs may be appended
* keyword = *contact\_area* or *area\_correction* or *store\_contact\_data*

.. parsed-literal::

     *contact_area* values = *overlap* or *constant areavalue* or *projection*
     *area_correction* values = *yes* or *no*
     *store_contact_data* values = *yes* or *no*

Examples
""""""""


.. parsed-literal::

   fix 3 hg heat/gran/conduction initial_temperature 273.15

**Packfall Availability:**

This command is specific to Packfall and is not available in upstream
LAMMPS.

Description
"""""""""""

Calculates heat conduction between particles in contact and temperature 
update (see :ref:`(Chaudhuri) <Chaudhuri>`) according to

.. image:: Eqs/heat_gran.png
   :align: center

It is assumed that the temperature within the particles is uniform.
To make particles adiabatic (so they do not change the temperature),
do not include them in the fix group. However, heat transfer is calculated
between particles in the group and particles not in the group (but
temperature update is not performed for particles not in the group).
Thermal conductivity and specific thermal capacity must be defined for
each atom type used in the simulation by means of
:doc:`fix property/global <fix_property>` commands:


.. parsed-literal::

   fix id all property/global thermalConductivity peratomtype value_1 value_2 ...
   (value_i=value for thermal conductivity of atom type i)

   fix id all property/global thermalCapacity peratomtype value_1 value_2 ...
   (value_i=value for thermal capacity of atom type i)

To set the temperature for a group of particles, you can use the set command
with keyword *property/atom* and values *Temp T*\ . *T* is the
temperature value you want the particles to have. To set heat sources
(or sinks) for a group of particles, you can also use the set command
with the set keyword: *property/atom* and the set values: *heatSource h*
where *h* is the heat source value you want the particles to have (in Energy/time
units). A negative value means it is a heat sink. Examples would be:


.. parsed-literal::

   set region halfbed property/peratom Temp 800.
   set region srcreg property/peratom heatSource 0.5


----------


Contact area calculation:

Using keyword *contact\_area*, you can choose from 3 modes of calulating
the contact area for particle-particle heat transfer: If *overlap* is used,
the contact area is calculated from the area of the sphere-sphere intersection
If *constant* is used, a constant user-defined overlap area is assumed.
If *projection* is used, the overlap area is assumed to be equal to rmin\*rmin\*Pi,
where rmin is the radius of the smaller of the two particles in contact.

Area correction:

For *contact\_area*  = overlap, an area correction can additionally
be performed using keyword *area\_correction* to account for the fact that
the Young's modulus might have been decreased in order to speed-up the
simulation, thus artificially increasing the overlap. In this case, you have
to specify the original Young's modulus of each material by means of a
:doc:`fix property/global <fix_property>` command:


.. parsed-literal::

   fix id all property/global youngsModulusOriginal peratomtype value_1 value_2 ...
   (value_i=value for original Young's modulus of atom type i)

This area correction is performed by scaling the contact area with
(Y\*/Y\*,orig)\^a, where Y\* and Y\*,orig are calculated as defined in
:doc:`pair\_style gran <pair_gran>` .
The scaling factor is given as e.g. a=1 for a Hooke and a=2/3 for a Hertz
interaction.

**Coarse-graining information:**

Using :doc:`coarsegraining <coarsegraining>` in
combination with this command should lead to
statistically equivalent dynamics and system state.

.. note::

   :doc:`Coarsegraining <coarsegraining>` may or 
   may not be available in Packfall.

**Output info:**

You can visualize the heat sources by accessing f\_heatSource[0], and the
heatFluxes by f\_heatFlux[0] . With f\_directionalHeatFlux[0], f\_directionalHeatFlux[1]
and f\_directionalHeatFlux[2] you can access the conductive heat fluxes in x,y,z directions.
The conductive heat fluxes are calculated per-contact and
half the value is stored in each atom participating in the contact.
With f\_Temp[0] you can access the per-particle
temperature. You can also access the total thermal energy of the fix group
(useful for the thermo command) with f\_id .

In case the *store\_contact\_data* option is set to *yes*\ , additional output of
per-atom properties is available. The property ids are nContactsConduction,
contactAreaConduction and wallHeatFlux which represent the number of contacts a
particle has, the combined contact area with other particles and the wall heat
flux, respectively. These ids can be used as usual to output them in a dump
command or to calculate atom properties.

.. note::

   *store\_contact\_data yes* is required any heat conduction properties are
   used in the steady state detection through :doc:`fix ave/euler/custom <fix_ave_euler_custom>`.

**Restart, fix\_modify, run start/stop, minimize info:**

The particle temperature and heat source is written is written to
:doc:`binary restart files <restart>` so simulations can continue properly.
None of the :doc:`fix\_modify <fix_modify>` options are relevant to this fix.

This fix computes a scalar which can be accessed by various :ref:`output commands <4_15>`.  This scalar is the total thermal energy
of the fix group

No parameter of this fix can be used with the *start/stop* keywords of
the :doc:`run <run>` command.  This fix is not invoked during :doc:`energy minimization <minimize>`.

Restrictions
""""""""""""


none

Related commands
""""""""""""""""

:doc:`compute temp <compute_temp>`, :doc:`compute temp/region <compute_temp_region>`

Default
"""""""

*contact\_area* = overlap, *area\_correction* = no

**Literature:**

.. _Chaudhuri:



**(Chaudhuri)** Chaudhuri et al, Chemical Engineering Science, 61, p 6348 (2006).


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
