.. index:: pair\_style sph/artVisc/tensCorr

pair\_style sph/artVisc/tensCorr command
========================================

Syntax
""""""


.. parsed-literal::

   pair_style sph/artVisc/tensCorr kernelstyle args keyword values ...

* sph/artVisc/tensCorr = name of this pair\_style command
* kernelstyle = *cubicspline* or *wendland*
* args = list of arguments for a particular style


.. parsed-literal::

     *cubicspline* or *wendland* args = h
       h = smoothing length

* zero or more keyword/value pairs may be appended to args
* keyword = *artVisc* or *tensCorr*


.. parsed-literal::

     *artVisc* values = alpha beta eta
       alpha = free parameter to control shear viscosity
       beta = free parameter to control bulk viscosity
       eta = coefficient to avoid singularities
     *tensCorr* values = epsilon deltap
       epsilon = free parameter
       deltap = initial particle distribution

Examples
""""""""


.. parsed-literal::

   pair_style sph/artVisc/tensCorr wendland 0.001 artVisc 1e-4 0 1e-8
   pair_style sph/artVisc/tensCorr cubicspline 0.001 artVisc 1e-4 0 1e-8 tensCorr 0.2 1e-2

Description
"""""""""""

The *sph/artVisc/tensCorr* style uses the smoothed particle hydrodynamics (SPH) method according to Monaghan :ref:`(1992) <Monaghan1992>`. The acting force is calculated from the acceleration as stated in the equation:

.. image:: Eqs/pair_sph_artvisc_tenscorr_eq1.jpg
   :align: center

Whereas the indices a and b stand for particles, P<sub>j</sub> stands for pressure and &#961;<sub>j</sub> for the density. W<sub>ab</sub> represents the kernel, which is defined by the kernelstyle.

For kernelstyle cubicspline a piecewise defined, 3-order kernel is used:

.. image:: Eqs/pair_sph_artvisc_tenscorr_eq2.jpg
   :align: center

The wendland kernel (Wendland,1995) is defined as

.. image:: Eqs/pair_sph_artvisc_tenscorr_eq3.jpg
   :align: center

The smoothing length h is the most important parameter for SPH-calculations. It depends on initial particle spacing, initial density &#961;<sub>0</sub> and mass per particle m<sub>j</sub>. In case that the smoothing length is about 1.2 times the initial particle spacing and it is a 3-dimensional cubic lattice (therefore the summation is over 57 particles), you can use the following equation :ref:`(Liu and Liu, 2003, p. 211-213) <LiuLiu2003>`:

.. image:: Eqs/pair_sph_artvisc_tenscorr_eq4.jpg
   :align: center

The atom style *sph/var* uses the input argument h as initial smoothing length for all particles. In case the atom style *sph* (per-type smoothing length) is used an additional per-type property *sl* must be defined, e.g.,


.. parsed-literal::

   fix           m2 all property/global sl peratomtype 0.0012

For further details on the basics of the SPH-method we recommend the papers from Monaghan :ref:`(1992) <Monaghan1992>`, :ref:`(1994) <Monaghan1994>`, etc.

Optionally, this pairstyle can take into account the artificial viscosity proposed my Monaghan (1985), if the artVisc keyword is appended. In this case, &#928;<sub>ab</sub> is added to the bracket term in the above acceleration equation, where &#928;<sub>ab</sub> is given by

.. image:: Eqs/pair_sph_artvisc_tenscorr_eq5.jpg
   :align: center

and

.. image:: Eqs/pair_sph_artvisc_tenscorr_eq6.jpg
   :align: center

This expression produces a shear and bulk viscosity. The quadratic term enables simulation of high Mach number shocks. The parameter &#951;<sup>2</sup> prevents singularities. A good choice is normally &#951;<sup>2</sup>=0.01h<sup>2</sup>.
The choice of &#945; and &#946; should not be critical, although there are some aspects which you should take into account:

"In the present case, with negligible changes in the density [weakly compressible SPH], the viscosity is almost entirely shear viscosity with a viscosity coefficient approximately &#945;hc." :ref:`(Monaghan, 1994) <Monaghan1994>`

Bar-parameters like c<sub>ab</sub> and &#961;<sub>ab</sub> are mean values of particle a and b.
NOTE: &#961;<sub>ab</sub> is calculated, and for the calculation of c<sub>ab</sub> the per-type property *speedOfSound* has to be defined, e.g.,


.. parsed-literal::

   fix 		      m1 all property/global speedOfSound peratomtype 20.

By appending the keyword *tensCorr* you enable the tensile correction algorithm :ref:`(Monaghan, 2000) <Monaghan2000>` which improves results in combination with negative pressures (e.g. EOS like Tait's equation). This method adds R\*(f<sub>ab</sub>)<sup>n</sup> to the bracket term, where the factor R is related to the pressure and can be calculated by R=R<sub>a</sub> + R<sub>b</sub>. In case of negative pressures (P<sub>a</sub> < 0) we use the rule

.. image:: Eqs/pair_sph_artvisc_tenscorr_eq7.jpg
   :align: center

otherwise R<sub>a</sub> is zero. Typical values of *epsilon* are about 0.2.
fab is calculated by

.. image:: Eqs/pair_sph_artvisc_tenscorr_eq8.jpg
   :align: center

where &Delta;p denotes the initial particle spacing.
NOTE: In a next version this calculation should be improved too.


----------


**Mixing, shift, table, tail correction, restart, rRESPA info**\ :

The :doc:`pair\_modify <pair_modify>` mix, shift, table, and tail options
are not relevant for sph pair styles.

These pair styles write their information to :doc:`binary restart files <restart>`, so a pair\_style command does not need to be
specified in an input script that reads a restart file.

These pair styles can only be used via the *pair* keyword of the
:doc:`run\_style respa <run_style>` command.  They do not support the
*inner*\ , *middle*\ , *outer* keywords.


----------


Restrictions
""""""""""""


...

Related commands
""""""""""""""""

:doc:`pair\_coeff <pair_coeff>`

**Default:** none


----------


.. _LiuLiu2003:



**(Liu and Liu, 2003)** "Smoothed Particle Hydrodynamics: A Meshfree Particle Method", G. R. Liu and M. B. Liu, World Scientific, p. 449 (2003).

.. _Monaghan1992:



**(Monaghan, 1992)** "Smoothed Particle Hydrodynamics", J. J. Monaghan, Annu. Rev. Astron. Astrophys., 30, p. 543-574 (1992).

.. _Monaghan1994:



**(Monaghan, 1994)** J. J. Monaghan, Journal of Computational Physics, 110, p. 399-406 (1994).

.. _Monaghan2000:



**(Monaghan, 2000)** J. J. Monaghan, Journal of Computational Physics, 159, p. 290-311 (2000).


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
