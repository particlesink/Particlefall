gran model hooke/stiffness model
================================

Syntax
""""""


.. parsed-literal::

   model hooke/stiffness [other model_type/model_name pairs as described :doc:`here <pair_gran>` ] keyword values

* zero or more keyword/value pairs may be appended to the end (after all models are specified)

.. parsed-literal::

     *absolute_damping* values = 'on' or 'off'
       on = activates tangential damping
       off = no tangential damping
     *limitForce* values = 'on' or 'off'
       on = ensures that the normal force is never attractive (an artefact that can occur at the end of a collision).
       off = standard implementation that might lead to attractive forces.
     *tangential_damping* values = 'on' or 'off'
       on = activates tangential damping
       off = no tangential damping

Description
"""""""""""

This granular model uses the following formula for the frictional force between two granular
particles, when the distance r between two particles of radii Ri and Rj is less than their
contact distance d = Ri + Rj. There is no force between the particles when r > d.

For the case of *absolute\_damping* = 'off' (which is default), the specified damping
coefficient is multiplied by the effective mass. The forces are implemented as

.. image:: Eqs/pair_gran_html_m566a9984.png
   :align: center

For the case of *absolute\_damping* = 'on', this multiplication is omitted and the forces become

.. image:: Eqs/pair_gran_html_m76cd8bd9.png
   :align: center

In the first term is the normal force between the two particles and the second term is the
tangential force. The normal force has 2 terms, a spring force and a damping force. The
tangential force also has 2 terms: a shear force and a damping force. The shear force is
a "history" effect that accounts for the tangential displacement ("tangential overlap")
between the particles for the duration of the time they are in contact.
This term is controlled by the :doc:`tangential model <Section_gran_models>` in action
Keyword *tangential\_damping* can be used to eliminate the second part of the force in
tangential direction. The way how the Coulomb friction limit acts is also controlled
by the :doc:`tangential model <Section_gran_models>` chosen by the user.

The quantities in the equations are as follows:

* delta\_n = d - r = overlap distance of 2 particles
* k\_n = elastic constant for normal contact
* k\_t = elastic constant for tangential contact
* gamma\_n = viscoelastic damping constant for normal contact
* gamma\_t = viscoelastic damping constant for tangential contact
* delta\_t = tangential displacement vector between 2 spherical particles which is truncated to satisfy a frictional yield criterion
* rmu = coefficient of rolling friction
* contactradius = contact radius, equal to particle radius - 0.5 \* delta\_n
* v\_n = normal component of the relative velocity of the 2 particles
* v\_t = tangential component of the relative velocity of the 2 particles
* w\_r = relative rotational velocity of the 2 particles


To define those k\_n\_specified, k\_t\_specified, gamma\_n\_specified, and gamma\_t\_specified coefficients
(material properties), it is mandatory to use multiple :doc:`fix property/global <fix_property>` commands:


.. parsed-literal::

   fix id all property/global kn peratomtypepair n_atomtypes value_11 value_12 .. value_21 value_22 .. .
       (value_ij=value for k_n between atom type i and j; n_atomtypes is the number of atom types you want to use in your simulation)
   fix id all property/global kt peratomtypepair n_atomtypes value_11 value_12 .. value_21 value_22 .. .
       (value_ij=value for k_t between atom type i and j; n_atomtypes is the number of atom types you want to use in your simulation)
   fix id all property/global gamman peratomtypepair n_atomtypes value_11 value_12 .. value_21 value_22 .. .
       (value_ij=value for gamma_n between atom type i and j; n_atomtypes is the number of atom types you want to use in your simulation)
   fix id all property/global gammat peratomtypepair n_atomtypes value_11 value_12 .. value_21 value_22 .. .
       (value_ij=value for gamma_t between atom type i and j; n_atomtypes is the number of atom types you want to use in your simulation)

If the absolute damping implementation is used (*absolute\_damping* = 'on'), the damping coefficients
 must be named gamman\_abs and gammat\_abs instead of gamman, gammat as follows:


.. parsed-literal::

   fix id all property/global gamman_abs peratomtypepair n_atomtypes value_11 value_12 .. value_21 value_22 .. .
       (value_ij=value for gamma_n between atom type i and j; n_atomtypes is the number of atom types you want to use in your simulation)
   fix id all property/global gammat_abs peratomtypepair n_atomtypes value_11 value_12 .. value_21 value_22 .. .
       (value_ij=value for gamma_t between atom type i and j; n_atomtypes is the number of atom types you want to use in your simulation)

.. warning::

   You have to use atom styles beginning from 1, e.g. 1,2,3,...

**Force Limiting:**

Note, that not using limitForce might lead to attractive forces between particles and walls, especially in case the 
coefficient of restitution is small. Be sure you include this key word for the pair style and the wall model if you 
like to avoid this.

Restrictions
""""""""""""


If using SI units, youngsModulus must be > 5e6
If using CGS units, youngsModulus must be > 5e5
When using the limitForce, the specified coefficient of restitution is only approximate. This might become 
problematic for low coefficients of resitution as showin in Schwager and Poschel.

**Coarse-graining information:**

Using :doc:`coarsegraining <coarsegraining>` in
combination with this command might lead to
different dynamics or system state and thus to 
inconsistancies.

.. note::

   :doc:`Coarsegraining <coarsegraining>` may or 
   may not be available in LIGGGHTS(R)-PUBLIC.

Default
"""""""

*tangential\_damping* = 'on'
*limitForce* = 'off'

**(Di Renzo)** Alberto Di Renzo, Francesco Paolo Di Maio, CES, 59 (3), p 525-541 (2004).

**(Ai)** Jun Ai, Jian-Fei Chen, J. Michael Rotter, Jin Y. Ooi, Powder Technology, 206 (3), p 269-282 (2011).

**(Brilliantov)** Brilliantov, Spahn, Hertzsch, Poschel, Phys Rev E, 53, p 5382-5392 (1996).

**(Schwager)** Schwager, Poschel, Gran Matt, 9, p 465-469 (2007).

**(Silbert)** Silbert, Ertas, Grest, Halsey, Levine, Plimpton, Phys Rev E, 64, p 051302 (2001).

**(Zhang)** Zhang and Makse, Phys Rev E, 72, p 011301 (2005).


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
