.. index:: compute ke

compute ke command
==================

Syntax
""""""


.. parsed-literal::

   compute ID group-ID ke general_keyword general_values halfstep

* ID, group-ID are documented in :doc:`compute <compute>` command
* ke = style name of this compute command
* general\_keywords general\_values are documented in `compute <compute.html">`_
* halfstep = optional keyword to save kinetic energy at half step


Examples
""""""""


.. parsed-literal::

   compute 1 all ke

Description
"""""""""""

Define a computation that calculates the translational kinetic energy
of a group of particles.

The kinetic energy of each particle is computed as 1/2 m v\^2, where m and v are
the mass and velocity of the particle. By default the velocity is taken at the
full time step (*v\^i*). If the *halfstep* keyword is present in the definition
of this compute, then the velocity is taken at the next half time step
(*v\^(i+1/2)*).

There is a subtle difference between the quantity calculated by this
compute and the kinetic energy calculated by the *ke*
keyword used in thermodynamic output, as specified by the
:doc:`thermo\_style <thermo_style>` command.  For this compute, kinetic
energy is "translational" kinetic energy, calculated by the simple
formula above.  For thermodynamic output, the *ke* keyword infers
kinetic energy from the temperature of the system with 1/2 Kb T of
energy for each degree of freedom.  For the default temperature
computation via the :doc:`compute temp <compute_temp>` command, these
are the same.  But different computes that calculate temperature can
subtract out different non-thermal components of velocity and/or
include different degrees of freedom (translational, rotational, etc).

**Output info:**

This compute calculates a global scalar (the summed KE).  This value
can be used by any command that uses a global scalar value from a
compute as input.  See :ref:`Section\_howto 15 <howto_8>`
for an overview of Packfall output options.

The scalar value calculated by this compute is "extensive".  The
scalar value will be in energy :doc:`units <units>`.

Restrictions
""""""""""""
 none

Related commands
""""""""""""""""

:doc:`compute erotate/sphere <compute_erotate_sphere>`

**Default:** none


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
