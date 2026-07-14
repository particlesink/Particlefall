.. index:: compute ke/multisphere

compute ke/multisphere command
==============================

Syntax
""""""


.. parsed-literal::

   compute ID group-ID ke/multisphere general_keyword general_values

* ID, group-ID are documented in :doc:`compute <compute>` command
* ke = style name of this compute command
* general\_keywords general\_values are documented in `compute <compute.html">`_

Examples
""""""""


.. parsed-literal::

   compute 1 all ke/multisphere

Description
"""""""""""

Define a computation that calculates the translational kinetic energy
of a collection of multisphere bodies.

The kinetic energy of each multisphere body is computed as 1/2 M Vcm\^2,
where M is the total mass of the multisphere body, and Vcm is its
center-of-mass velocity.

This compute automatically connects to the :doc:`fix multisphere <fix_multisphere>`
commands which defines the multisphere bodies.  The group specified in the
compute command is ignored.  The kinetic energy of all the multisphere
bodies defined by the fix multisphere command in included in the
calculation.

**Output info:**

This compute calculates a global scalar (the summed KE of all the
multisphere bodies).  This value can be used by any command that uses a
global scalar value from a compute as input.  See :ref:`Section\_howto 15 <howto_8>` for an overview of Packfall output
options.

The scalar value calculated by this compute is "extensive".  The
scalar value will be in energy :doc:`units <units>`.

Restrictions
""""""""""""
 none

Related commands
""""""""""""""""

:doc:`compute erotate/multisphere <compute_erotate_multisphere>`

**Default:** none


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
