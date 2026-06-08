.. index:: fix sph/pressure

fix sph/pressure command
========================

Syntax
""""""


.. parsed-literal::

   fix ID group-ID sph/pressure style args

* ID, group-ID are documented in :doc:`fix <fix>` command
* sph/pressure = style name of this fix command
* style = *absolut* or *relativ* or *Tait*
* args = list of arguments for a particular style


.. parsed-literal::

     *absolut* args = B
       B = coefficient
     *relativ* args = B density0
       B = coefficient
       density0 = reference density
     *Tait* args = B density0 gamma
       B = coefficient
       density0 = reference density
       gamma = isentropic exponent

Examples
""""""""


.. parsed-literal::

   fix pressure all sph/pressure absolut
   fix pressure all sph/pressure Tait 60000. 1000. 7.

Description
"""""""""""

The equation of state (EOS) for the SPH calculation is the link between the density field and the pressure field. A lot of different equations can be found in the literature.

The *absolut* style was the first implemented EOS. The calculation rule is

.. image:: Eqs/fix_sph_pressure_abs.jpg
   :align: center

where &#961;<sub>a</sub> is the density of particle a and B is the pressure prefactor.

The *relativ* style uses the rule

.. image:: Eqs/fix_sph_pressure_rel.jpg
   :align: center

as EOS. B denotes the pressure prefactor and &#961;<sub>0</sub> is the reference density.

In case of *Tait* style the rule

.. image:: Eqs/fix_sph_pressure_tait_eq1.jpg
   :align: center

is applied. B denotes the pressure prefactor which is calculated by

.. image:: Eqs/fix_sph_pressure_tait_eq2.jpg
   :align: center

where c<sub>0</sub> is the speed of sound of the material. &#961;<sub>0</sub> is the reference density and &gamma; is the isentropic exponent defined as cp/cv.

.. note::

   Monaghan has found that the speed of sound could be artificially reduced. (Monaghan, 1994) Therefore, we can choose a greater time step. He argues that the minimum sound speed should be about ten times greater than the maximum expected flow speed. (&Delta;&#961; &lt; 1%)


----------


**Restart, fix\_modify, output, run start/stop, minimize info:**

No informatino about this fix is written to :doc:`binary restart files <restart>`.

None of the :doc:`fix\_modify <fix_modify>` options are relevant to this fix.

No global scalar or vector or per\_atom quantities are stored by this fix for access by various :ref:`output commands <4_15>`.

No parameter of this fix can be used with the *start/stop* keywords of the :doc:`run <run>` command.

This fix is not invoked during :doc:`energy minimization <minimize>`.


----------


Restrictions
""""""""""""


One fix sph/density/summation (only dev-version) or sph/denstiy/continuity has to exist.

Related commands
""""""""""""""""

:doc:`pair\_style sph/artVisc/tensCorr <pair_sph_artvisc_tenscorr>`, :doc:`fix sph/density/continuity <fix_sph_density_continuity>`

**Default:** none


----------


.. _LiuLiu2003:



**(Liu and Liu, 2003)** "Smoothed Particle Hydrodynamics: A Meshfree Particle Method", G. R. Liu and M. B. Liu, World Scientific, p. 449 (2003).


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
