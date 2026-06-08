.. index:: bond\_style harmonic

bond\_style harmonic command
============================

Syntax
""""""


.. parsed-literal::

   bond_style harmonic

Examples
""""""""


.. parsed-literal::

   bond_style harmonic
   bond_coeff 5 80.0 1.2

Description
"""""""""""

The *harmonic* bond style uses the potential

.. image:: Eqs/bond_harmonic.jpg
   :align: center

where r0 is the equilibrium bond distance.  Note that the usual 1/2
factor is included in K.

The following coefficients must be defined for each bond type via the
:doc:`bond\_coeff <bond_coeff>` command as in the example above, or in
the data file or restart files read by the :doc:`read\_data <read_data>`
or :doc:`read\_restart <read_restart>` commands:

* K (energy/distance\^2)
* r0 (distance)

Restrictions
""""""""""""


This bond style can only be used if LIGGGHTS(R)-PUBLIC was built with the
MOLECULAR package (which it is by default).  See the :ref:`Making LIGGGHTS(R)-PUBLIC <start_3>` section for more info on packages.

Related commands
""""""""""""""""

:doc:`bond\_coeff <bond_coeff>`, :doc:`delete\_bonds <delete_bonds>`

**Default:** none


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
