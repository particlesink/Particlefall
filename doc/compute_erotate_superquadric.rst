.. index:: compute erotate/superquadric

compute erotate/superquadric command
====================================

Syntax
""""""


.. parsed-literal::

   compute ID group-ID erotate/superquadric general_keyword general_values

* ID, group-ID are documented in :doc:`compute <compute>` command
* erotate/superquadric = style name of this compute command
* general\_keywords general\_values are documented in `compute <compute.html">`_

Examples
""""""""


.. parsed-literal::

   compute 1 all erotate/superquadric

Description
"""""""""""

Define a computation that calculates the rotational kinetic energy of
a group of superquadric particles. See the
:doc:`atom\_style <atom_style>` and :doc:`read\_data <read_data>` commands
for descriptions of these options.

For all 3 types of particles, the rotational kinetic energy is
computed as 1/2 I w\^2, where I is the inertia tensor for the
particle and w is its angular velocity, which is computed
from its angular momentum if needed.

**Output info:**

This compute calculates a global scalar (the KE).  This value can be
used by any command that uses a global scalar value from a compute as
input.  See :ref:`Section\_howto 15 <howto_8>` for an
overview of LIGGGHTS(R)-PUBLIC-SUPERQUADRIC output options.

The scalar value calculated by this compute is "extensive".  The
scalar value will be in energy :doc:`units <units>`.

Restrictions
""""""""""""


This compute requires :doc:`atom\_style <atom_style>` *superquadric*

**Related commands:** none

:doc:`compute erotate/sphere <compute_erotate_sphere>`

**Default:** none


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
