.. index:: compute erotate/multisphere

compute erotate/multisphere command
===================================

Syntax
""""""


.. parsed-literal::

   compute ID group-ID erotate/multisphere general_keyword general_values

* ID, group-ID are documented in :doc:`compute <compute>` command
* erotate/multisphere = style name of this compute command
* general\_keywords general\_values are documented in `compute <compute.html">`_

Examples
""""""""


.. parsed-literal::

   compute 1 all erotate/multisphere

Description
"""""""""""

Define a computation that calculates the rotational kinetic energy of
a collection of multisphere bodies.

The rotational energy of each multisphere body is computed as 1/2 I Wbody\^2,
where I is the inertia tensor for the multisphere body, and Wbody is its
angular velocity vector.  Both I and Wbody are in the frame of
reference of the multisphere body, i.e. I is diagonalized.

This compute automatically connects to the :doc:`fix multisphere <fix_multisphere>`
commands which defines the multisphere bodies.  The group specified in the
compute command is ignored.  The rotational energy of all the multisphere
bodies defined by the fix multisphere command in included in the
calculation.

**Output info:**

This compute calculates a global scalar (the summed rotational energy
of all the multisphere bodies).  This value can be used by any command that
uses a global scalar value from a compute as input.  See
:ref:`Section\_howto 15 <howto_8>` for an overview of
Packfall output options.

The scalar value calculated by this compute is "extensive".  The
scalar value will be in energy :doc:`units <units>`.

Restrictions
""""""""""""
 none

Related commands
""""""""""""""""

:doc:`compute ke/multisphere <compute_erotate_ke_multisphere>`

**Default:** none


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
