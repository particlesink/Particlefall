.. index:: compute erotate/sphere

compute erotate/sphere command
==============================

Syntax
""""""


.. parsed-literal::

   compute ID group-ID erotate/sphere general_keywords general_values halfstep

* ID, group-ID are documented in :doc:`compute <compute>` command
* erotate/sphere = style name of this compute command
* general\_keywords general\_values are documented in `compute <compute.html">`_
* halfstep = optional keyword to compute energy at half step


Examples
""""""""


.. parsed-literal::

   compute 1 all erotate/sphere

Description
"""""""""""

Define a computation that calculates the rotational kinetic energy of
a group of spherical particles.

The rotational energy is computed as 1/2 I w\^2, where I is the moment of inertia
for a sphere and w is the particle's angular velocity.  By default the angular
velocity is taken at the full time step (*w\^i*). If the *halfstep* keyword is
present in the definition of this compute, then the angular velocity is taken at
the next half time step (*w\^(i+1/2)*).

.. warning::

   For :doc:`2d models <dimension>`, particles are treated
   as spheres, not disks, meaning their moment of inertia will be the
   same as in 3d.

**Output info:**

This compute calculates a global scalar (the KE).  This value can be
used by any command that uses a global scalar value from a compute as
input.  See :ref:`Section\_howto 15 <howto_8>` for an
overview of Packfall output options.

The scalar value calculated by this compute is "extensive".  The
scalar value will be in energy :doc:`units <units>`.

Restrictions
""""""""""""


This compute requires that atoms store a radius and angular velocity
(omega) as defined by the :doc:`atom\_style sphere <atom_style>` command.

All particles in the group must be finite-size spheres or point
particles.  They cannot be aspherical.  Point particles will not
contribute to the rotational energy.

Related commands
""""""""""""""""

:doc:`compute erotate/asphere <compute_erotate_asphere>`

**Default:** none


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
