.. index:: fix particletemplate/superquadric

fix particletemplate/superquadric command
=========================================

Syntax
""""""


.. parsed-literal::

   fix ID group-ID particletemplate/superquadric seed keyword values ...

* ID, group-ID are documented in :doc:`fix <fix>` command
* particletemplate/superquadric = style name of this fix command
* seed = random number generator seed (prime number greater 10000)
* zero or more keyword/value pairs can be appended
* keyword = *atom\_type* or *density* or *volume\_limit* or *shape* or *blockiness*
  
  .. parsed-literal::
  
     *atom_type* value = atom type assigned to this particle template
     *density* values = constant param1
       param1 = constant density of particle material
     *volume_limit* value = lowest particle volume allowed in simulation
     *shape* values = constant a b c
       a, b, c = superquadric semi-axes
     *blockiness* values = constant n1 n2
       n1, n2 = superquadric blockiness parameters



Examples
""""""""


.. parsed-literal::

   fix pts1 all particletemplate/superquadric 123457 atom_type 1 density constant 2500 shape constant 0.0015 0.0025 0.0035 blockiness 4.0 4.0

Description
"""""""""""

Define a particle that is used as input for a 
:doc:`fix\_particledistribution\_discrete <fix_particledistribution_discrete>` command. 
You can choose the atom type, density and radius of the particles.

LIGGGHTS(R)-PUBLIC will throw an error if the particle volume is too small compared to machine precision.
If you are sure you know what you are doing you can override the default limit of 1e-12.

This model is used in the framework of SUPERQUADRIC simulations. For more information see also the :doc:`SUPERQUADRIC guide <superquadric_simulations>`.

**Restart, fix\_modify, output, run start/stop, minimize info:**

Information about the random state in this fix is written to :doc:`binary restart files <restart>` 
so you can restart a simulation with the same particles being chosen for insertion. None of the 
:doc:`fix\_modify <fix_modify>` options are relevant to this fix. No global scalar or vector or 
per-atom quantities are stored by this fix for access by various :ref:`output commands <4_15>`. 
No parameter of this fix can be used with the *start*\ /\ *stop* keywords of the :doc:`run <run>` command. 
This fix is not invoked during :doc:`energy minimization <minimize>`.

Restrictions
""""""""""""
 
This fix requires :doc:`atom\_style <atom_style>` superquadric. The *blockiness* values should be more or equal 2.

Related commands
""""""""""""""""

:doc:`fix\_particletemplate\_sphere <fix_particletemplate_sphere>`

Default
"""""""

a=1.0, b=1.0, c=1.0, n1=2.0, n2=2.0, atom\_type = 1, volume\_limit = 1e-12


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
