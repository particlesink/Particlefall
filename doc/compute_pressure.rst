.. index:: compute pressure

compute pressure command
========================

Syntax
""""""


.. parsed-literal::

   compute ID group-ID pressure general_keyword general_values temp-ID keyword ...

* ID, group-ID are documented in :doc:`compute <compute>` command
* pressure = style name of this compute command
* general\_keywords general\_values are documented in `compute <compute.html">`_
* temp-ID = ID of compute that calculates temperature
* zero or more keywords may be appended
* keyword = *ke* or *pair* or *bond* or *angle* or *dihedral* or *improper* or *kspace* or *fix* or *virial*

Examples
""""""""


.. parsed-literal::

   compute 1 all pressure myTemp
   compute 1 all pressure thermo_temp pair bond

Description
"""""""""""

Define a computation that calculates the pressure of the entire system
of atoms.  The specified group must be "all".  See the :doc:`compute stress/atom <compute_stress_atom>` command if you want per-atom
pressure (stress).  These per-atom values could be summed for a group
of atoms via the :doc:`compute reduce <compute_reduce>` command.

The pressure is computed by the formula

.. image:: Eqs/pressure.jpg
   :align: center

where N is the number of atoms in the system (see discussion of DOF
below), Kb is the Boltzmann constant, T is the temperature, d is the
dimensionality of the system (2 or 3 for 2d/3d), V is the system
volume (or area in 2d), and the second term is the virial, computed
within LIGGGHTS(R)-PUBLIC for all pairwise as well as 2-body, 3-body, and 4-body,
and long-range interactions.  :doc:`Fixes <fix>` that impose constraints
(e.g. the :doc:`fix shake <fix_shake>` command) also contribute to the
virial term.

A symmetric pressure tensor, stored as a 6-element vector, is also
calculated by this compute.  The 6 components of the vector are
ordered xx, yy, zz, xy, xz, yz.  The equation for the I,J components
(where I and J = x,y,z) is similar to the above formula, except that
the first term uses components of the kinetic energy tensor and the
second term uses components of the virial tensor:

.. image:: Eqs/pressure_tensor.jpg
   :align: center

If no extra keywords are listed, the entire equations above are
calculated which include a kinetic energy (temperature) term and the
virial as the sum of pair, bond, angle, dihedral, improper, kspace
(long-range), and fix contributions to the force on each atom.  If any
extra keywords are listed, then only those components are summed to
compute temperature or ke and/or the virial.  The *virial* keyword
means include all terms except the kinetic energy *ke*\ .

The temperature and kinetic energy tensor is not calculated by this
compute, but rather by the temperature compute specified with the
command.  Normally this compute should calculate the temperature of
all atoms for consistency with the virial term, but any compute style
that calculates temperature can be used, e.g. one that excludes frozen
atoms or other degrees of freedom.

Note that the N in the first formula above is really
degrees-of-freedom divided by d = dimensionality, where the DOF value
is calcluated by the temperature compute.  See the various :doc:`compute temperature <compute>` styles for details.

A compute of this style with the ID of "thermo\_press" is created when
LIGGGHTS(R)-PUBLIC starts up, as if this command were in the input script:


.. parsed-literal::

   compute thermo_press all pressure thermo_temp

where "thermo\_temp" is the ID of a similarly defined compute of style
"temp".  See the "thermo\_style" command for more details.


----------


**Output info:**

This compute calculates a global scalar (the pressure) and a global
vector of length 6 (pressure tensor), which can be accessed by indices
1-6.  These values can be used by any command that uses global scalar
or vector values from a compute as input.  See :ref:`this section <howto_8>` for an overview of LIGGGHTS(R)-PUBLIC output
options.

The scalar and vector values calculated by this compute are
"intensive".  The scalar and vector values will be in pressure
:doc:`units <units>`.

Restrictions
""""""""""""
 none

Related commands
""""""""""""""""

:doc:`compute temp <compute_temp>`, :doc:`compute stress/atom <compute_stress_atom>`,
:doc:`thermo\_style <thermo_style>`,

**Default:** none


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
