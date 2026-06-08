.. index:: fix sph/density/summation

fix sph/density/summation command
=================================

Syntax
""""""


.. parsed-literal::

   fix ID group-ID sph/density/summation

* ID, group-ID are documented in :doc:`fix <fix>` command
* sph/density/summation = style name of this fix command

Examples
""""""""


.. parsed-literal::

   fix density all sph/density/summation

Description
"""""""""""

Calculates the density field with the classic SPH-summation approach. The governing equation is given by:

.. image:: Eqs/fix_sph_density_summation.jpg
   :align: center

rho<sub>a</sub> is the density of particle a, m is the mass and W<sub>ab</sub> denotes the interpolating kernel for the particle-particle distance r<sub>a</sub> - r<sub>b</sub>. The summation is over all particles b other than particle a.

.. note::

   In the current version boundary or image particles are not implemented. Therefore, the density calculation in the vicinity to a wall will be wrong.


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


There can be only one fix sph/density/... (except :doc:`fix sph/density/corr <fix_sph_density_corr>`)

Related commands
""""""""""""""""

:doc:`pair\_style sph/artVisc/tensCorr <pair_sph_artvisc_tenscorr>`, :doc:`fix sph/pressure <fix_sph_pressure>`, :doc:`fix sph/density/continuity <fix_sph_density_continuity>`

**Default:** none


----------


.. _LiuLiu2003:



**(Liu and Liu, 2003)** "Smoothed Particle Hydrodynamics: A Meshfree Particle Method", G. R. Liu and M. B. Liu, World Scientific, p. 449 (2003).

.. _Monaghan1992:



**(Monaghan, 1992)** "Smoothed Particle Hydrodynamics", J. J. Monaghan, Annu. Rev. Astron. Astrophys., 30, p. 543-574 (1992).


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
