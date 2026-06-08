.. index:: fix sph/density/continuity

fix sph/density/continuity command
==================================

Syntax
""""""


.. parsed-literal::

   fix ID group-ID sph/density/continuity

* ID, group-ID are documented in :doc:`fix <fix>` command
* sph/density/continuity = style name of this fix command

Examples
""""""""


.. parsed-literal::

   fix density all sph/density/continuity

Description
"""""""""""

Based on the continuity equation in the form

.. image:: Eqs/fix_sph_density_continuity_eq1.jpg
   :align: center

this fix calculates the density of each particle by the rule

.. image:: Eqs/fix_sph_density_continuity_eq2.jpg
   :align: center

where the summation is over all particles b other than particle a, m is the mass, v is the velocity, W<sub>ab</sub> is the interpolating kernel (documented in :doc:`pair\_style sph/artVisc/tensCorr <pair_sph_artvisc_tenscorr>`) and &nabla;<sub>a</sub> is the gradient of W<sub>ab</sub>.


----------


**Restart, fix\_modify, output, run start/stop, minimize info:**

No information about this fix is written to :doc:`binary restart files <restart>`.

None of the :doc:`fix\_modify <fix_modify>` options are relevant to this fix.

No global scalar or vector or per\_atom quantities are stored by this fix for access by various :ref:`output commands <4_15>`.

No parameter of this fix can be used with the *start/stop* keywords of the :doc:`run <run>` command.

This fix is not invoked during :doc:`energy minimization <minimize>`.


----------


Restrictions
""""""""""""


There can be only one fix sph/density.

Related commands
""""""""""""""""

:doc:`pair\_style sph/artVisc/tensCorr <pair_sph_artvisc_tenscorr>`, :doc:`fix sph/pressure <fix_sph_pressure>`, :doc:`fix sph/density/corr <fix_sph_density_corr>`

**Default:** none


----------


.. _LiuLiu2003:



**(Liu and Liu, 2003)** "Smoothed Particle Hydrodynamics: A Meshfree Particle Method", G. R. Liu and M. B. Liu, World Scientific, p. 449 (2003).

.. _Monaghan1992:



**(Monaghan, 1992)** "Smoothed Particle Hydrodynamics", J. J. Monaghan, Annu. Rev. Astron. Astrophys., 30, p. 543-574 (1992).


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
