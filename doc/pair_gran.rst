.. index:: pair\_style gran

pair\_style gran command
========================

pair\_style bubble command
==========================

pair\_style gran\_bubble bubble command
=======================================

Syntax
""""""


.. parsed-literal::

   pair_style gran model_type model_name model_keyword model_value

   pair_style bubble model_type model_name model_keyword model_value

   pair_style gran_bubble model_type model_name model_keyword model_value

* zero or more model\_type/model\_name pairs may be appended. They must be appended in the following order (!)

.. parsed-literal::

     *model* values = described :doc:`here <Section_gran_models>`
     *tangential* values = described :doc:`here <Section_gran_models>`
     *cohesion* values = described :doc:`here <Section_gran_models>`
     *rolling_friction* values = described :doc:`here <Section_gran_models>`
     *surface* values = described :doc:`here <Section_gran_models>`

* following the model\_type/model\_name pairs, zero or more model\_keyword/model\_valuezero pairs may be appended in arbitrary order

.. parsed-literal::

     model_type/model_name pairs = described for each model separately :doc:`here <Section_gran_models>`



Examples
""""""""


.. parsed-literal::

   pair_style gran model hooke tangential history
   pair_style gran model hertz tangential history rolling_friction cdt
   pair_style gran model hertz tangential no_history cohesion sjkr

**General description:**

The *gran* styles imposes a force between two neighboring particles. Typically, there is
a force when the distance r between two particles of radii Ri and Rj is less than their
contact distance dist = Ri + Rj, and no force otherwise. Some models, such as cohesion
models, may impose a force also when the particle surfaces do not touch. This is documented
for those models specifically.

The general form of a granular interaction can be written as:

.. image:: Eqs/pair_gran_html_60b8ced2.png
   :align: center

The quantities in the equations are as follows:

* delta\_n = d - r = overlap distance of 2 particles
* k\_n = elastic constant for normal contact
* k\_t = elastic constant for tangential contact
* gamma\_n = viscoelastic damping constant for normal contact
* gamma\_t = viscoelastic damping constant for tangential contact
* delta\_t = tangential displacement vector between 2 spherical particles


In the first term is the normal force between the two particles and the second term is the
tangential force. The normal force has 2 terms, a contact force and a damping force. The
tangential force also has 2 terms: a shear force and a damping force. The shear force is
a "history" effect that accounts for the tangential displacement ("tangential overlap")
between the particles for the duration of the time they are in contact.

The concrete implementation for k\_n, k\_t, gamma\_n, gamma\_t and the shear history
depend on the concrete models as chosen by the user. They are described on separate
doc pages :doc:`here <Section_gran_models>`

Also, other models may add additional forces or torques on the particles, such as
cohesive or rolling friction forces. These are also described on separate
doc pages :doc:`here <Section_gran_models>`

The styles *bubble* and *gran\_bubble* are aliases for *gran*\ , which can e.g. be used
for the modelling of systems with different phases using :doc:`pair hybrid <pair_hybrid>`, where a
different set of interaction laws is used for each phase. An example would be


.. parsed-literal::

   pair_style hybrid gran        model hertz     tangential history &
                     bubble      model hertz     tangential off &
                     gran_bubble model hertz     tangential off

.. warning::

   The order of model keywords is important, you have to stick
   to the order as outlined in the "Syntax" section of this doc page.

**General comments:**

For granular styles there are no additional coefficients to set for each pair of atom types
via the :doc:`pair\_coeff <pair_coeff>` command. All settings are global and are made via the
pair\_style command. However you must still use the :doc:`pair\_coeff <pair_coeff>` for all pairs
of granular atom types. For example the command


.. parsed-literal::

   pair_coeff \* \*

should be used if all atoms in the simulation interact via a granular potential
(i.e. one of the pair styles above is used). If a granular potential is used as a sub-style
of :doc:`pair\_style hybrid <pair_hybrid>`, then specific atom types can be used in the pair\_coeff
command to determine which atoms interact via a granular potential.

**Mixing, shift, table, tail correction, restart, rRESPA info:**

The :doc:`pair\_modify <pair_modify>` mix, shift, table, and tail options are not
relevant for granular pair styles.

These pair styles write their information to :doc:`binary restart files <restart>`,
so a pair\_style command does not need to be specified in an input script that reads a restart file.

.. warning::

   The material properties are not written to restart files! Thus, if you
   restart a simulation, you have to re-define them (by using the fixes mentioned above).

These pair styles can only be used via the pair keyword of the :doc:`run\_style respa <run_style>`
command. They do not support the inner, middle, outer keywords.

Restrictions
""""""""""""


These pair styles require that atoms store torque and angular velocity (omega) as
defined by the :doc:`atom\_style <atom_style>`. They also require a per-particle radius is
stored. The *sphere* or *granular* atom style does all of this.

This pair style requires you to use the :doc:`communicate vel yes <communicate>` option so that
velocites are stored by ghost atoms.

Only unit system that are self-consistent (si, cgs, lj) can be used with this pair style.

Related commands
""""""""""""""""

:doc:`pair\_coeff <pair_coeff>`
Models for use with this command are described :doc:`here <Section_gran_models>`

Default
"""""""

*model* = 'hertz'
*tangential* = 'off'
*rolling\_friction* = 'off'
*cohesion* = 'off'
*surface* = 'default'


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
