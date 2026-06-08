.. index:: fix sph/density/corr

fix sph/density/corr command
============================

Syntax
""""""


.. parsed-literal::

   fix ID group-ID sph/density/corr style args

* ID, group-ID are documented in :doc:`fix <fix>` command
* sph/density/corr = style name of this fix command
* style = *shepard*
* args = list of arguments for a particular style


.. parsed-literal::

     *shepard* args = every nSteps
       nSteps = determes number of timesteps

Examples
""""""""


.. parsed-literal::

   fix corr all sph/density/corr shepard every 30

Description
"""""""""""

In general the pressure field in SPH exhibits large oscillations. One approach to overcome this problem is to perform a filter over the density.

The filterstyle *shepard* is one of the most simple and quick correction. Every *nSteps* timesteps the following rule is applied:

.. image:: Eqs/fix_sph_density_corr_eq1.jpg
   :align: center


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
 none

Related commands
""""""""""""""""

:doc:`pair\_style sph/artVisc/tensCorr <pair_sph_artvisc_tenscorr>`, :doc:`fix sph/pressure <fix_sph_pressure>`, :doc:`fix sph/density/continuity <fix_sph_density_continuity>`

**Default:** none


----------


.. _LiuLiu2003:



**(Liu and Liu, 2003)** "Smoothed Particle Hydrodynamics: A Meshfree Particle Method", G. R. Liu and M. B. Liu, World Scientific, p. 449 (2003).


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
