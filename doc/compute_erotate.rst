.. index:: compute erotate

compute erotate command
=======================

Syntax
""""""


.. parsed-literal::

   compute ID group-ID erotate general_keywords general_values

* ID, group-ID are documented in :doc:`compute <compute>` command
* erotate = style name of this compute command
* general\_keywords general\_values are documented in `compute <compute.html">`_


Examples
""""""""


.. parsed-literal::

   compute 1 all erotate

Description
"""""""""""

Define a computation that calculates the rotational kinetic energy of all
particles. This compute sums the rotational kinetic energy of spherical,
superquadric and multisphere particles.

The detailed way of how these energies of computed can be found in the
respective child computes.

**Output info:**

This compute calculates a global scalar (the rotational kinetic energy).  This
value can be used by any command that uses a global scalar value from a compute
as input.  See :ref:`Section\_howto 15 <howto_8>` for an overview of
LIGGGHTS(R)-PUBLIC output options.

The scalar value calculated by this compute is "extensive".  The
scalar value will be in energy :doc:`units <units>`.

Restrictions
""""""""""""


Those of the child computes.

Related commands
""""""""""""""""

:doc:`compute erotate/sphere <compute_erotate_sphere>`
:doc:`compute erotate/mulitsphere <compute_erotate_multisphere>`
:doc:`compute erotate/superquadric <compute_erotate_superquadric>`

**Default:** none


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
