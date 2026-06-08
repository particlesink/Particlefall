.. index:: fix particletemplate/sphere

fix particletemplate/sphere command
===================================

Syntax
""""""


.. parsed-literal::

   fix ID group-ID particletemplate/sphere seed keyword values ...

* ID, group-ID are documented in :doc:`fix <fix>` command
* particletemplate/sphere = style name of this fix command
* seed = random number generator seed (prime number greater 10000)
* zero or more keyword/value pairs can be appended
* keyword = *atom\_type* or *density* or *volume\_limit* or *radius* or *relative*
  
  .. parsed-literal::
  
     *atom_type* value = atom type assigned to this particle template
     *density* values = random_style param1 (param2)
       random_style = 'constant' 
       param1 = density for 'constant',
         param2 = omitted for 'constant'
     *volume_limit* value = lowest particle volume allowed in simulation
     *radius* values = random_style params
       random_style = 'constant' or 'uniform' or 'normal' or 'gaussian' or 'lognormal'
       params = radius for 'constant'
       params = min max for 'uniform'
       params = mean sigma min max for 'normal' or 'gaussian'
       params = mu sigma min max for 'lognormal'
     *relative* value = yes or no
       no = set a flag for other commands that radius given here is abosulute (in length units)
       yes = set a flag for other commands that radius given here is relative (in % of a reference radius)



Examples
""""""""


.. parsed-literal::

   fix pts1 all particletemplate/sphere 123457 atom_type 1 density constant 2500 radius constant 0.0015
   fix pts2 all particletemplate/sphere 123458 atom_type 1 density constant 2500 radius uniform 0.001 0.002
   fix pts3 all particletemplate/sphere 123459 atom_type 1 density constant 2500 radius normal 0.002 0.00025 0.00125 0.00275
   fix pts4 all particletemplate/sphere 123460 atom_type 1 density constant 2500 radius lognormal -6.2 0.08 0.0015 0.0022

Description
"""""""""""

Define a particle that is used as input for a 
:doc:`fix\_particledistribution\_discrete <fix_particledistribution_discrete>` command. 
You can choose the atom type, density and radius of the particles. Density currently
supports the *constant* style. Radius supports *constant*, *uniform*, *normal*,
*gaussian* and *lognormal* styles.

The *uniform* style uses the specified minimum and maximum radius directly. The
*normal* and *gaussian* styles use the specified mean and standard deviation and
require explicit minimum and maximum cutoffs to keep the distribution tails bounded.
The *lognormal* style uses the underlying normal-distribution parameters *mu* and
*sigma* of ln(radius), plus explicit minimum and maximum cutoffs in length units.

Keyword *relative* lets the user set a flag to let other commands know if the radii specified
by this command are absolute (length units) or relative (in % of a reference radius). 
IMPORTANT NOTE: This setting should not be changed away from the default value (no) unless
explicity required by another command!

LIGGGHTS(R)-PUBLIC will throw an error if the particle volume is too small compared to machine precision.
If you are sure you know what you are doing you can override the default limit of 1e-12.

**Restart, fix\_modify, output, run start/stop, minimize info:**

Information about the random state in this fix is written to :doc:`binary restart files <restart>` 
so you can restart a simulation with the same particles being chosen for insertion. None of the 
:doc:`fix\_modify <fix_modify>` options are relevant to this fix. No global scalar or vector or 
per-atom quantities are stored by this fix for access by various :ref:`output commands <4_15>`. 
No parameter of this fix can be used with the *start*\ /\ *stop* keywords of the :doc:`run <run>` command. 
This fix is not invoked during :doc:`energy minimization <minimize>`.

Restrictions
""""""""""""
 none

Related commands
""""""""""""""""

:doc:`fix\_particletemplate\_sphere <fix_particletemplate_sphere>`

Default
"""""""

radius = not set, density = not set, atom\_type = 1, volume\_limit = 1e-12, relative = no


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
