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
* keyword = *atom\_type* or *density* or *volume\_limit* or *shape* or *shapex* or *shapey* or *shapez* or *size* or *blockiness* or *roundness* or *relative* or *region* or *region\_variable*
  
  .. parsed-literal::
  
     *atom_type* value = atom type assigned to this particle template
     *density* values = constant param1
       param1 = constant density of particle material
     *volume_limit* value = lowest particle volume allowed in simulation
     *shape* values = random_style params
       random_style = constant or uniform or gaussian
       params = a b c for constant
       params = ax ay az sigma for gaussian
       params = axmin axmax aymin aymax azmin azmax for uniform
     *shapex*, *shapey*, *shapez* values = random_style params
       random_style = constant or uniform or gaussian
       params = a for constant
       params = mean sigma for gaussian
       params = min max for uniform
     *size* values = random_style params
       random_style = constant or uniform or gaussian
       params = size for constant
       params = mean sigma for gaussian
       params = min max for uniform
     *blockiness* values = constant n1 n2
       n1, n2 = superquadric blockiness parameters
     *roundness* values = constant n1 n2
       deprecated alias for *blockiness*
     *relative* value = yes or no
       no = treat dimensions as absolute length units
       yes = mark dimensions as relative to a reference size in commands that support it
     *region* value = region-ID
       restrict template randomization to a region for commands that use it
     *region_variable* value = fix-ID
       region variable fix used by commands that support region-variable templates



Examples
""""""""


.. parsed-literal::

   fix pts1 all particletemplate/superquadric 123457 atom_type 1 density constant 2500 shape constant 0.0015 0.0025 0.0035 blockiness constant 4.0 4.0
   fix pts2 all particletemplate/superquadric 123458 atom_type 1 density constant 2500 shapex constant 0.0015 shapey constant 0.0015 shapez gaussian 0.004 0.0005 blockiness constant 8.0 2.0
   fix pts3 all particletemplate/superquadric 123459 atom_type 1 density constant 2500 size uniform 0.002 0.004 blockiness constant 2.0 2.0

Description
"""""""""""

Define a particle that is used as input for a 
:doc:`fix\_particledistribution\_discrete <fix_particledistribution_discrete>` command. 
You can choose the atom type, density, superquadric dimensions, and
blockiness parameters of the particles.

Superquadrics are defined by three semi-axes *a*, *b*, *c* and two
blockiness parameters *n1* and *n2*. The ellipsoidal limit is obtained with
*n1 = n2 = 2*. Larger values create more box-like particles with flatter
faces and sharper edge transitions.

The *density* keyword currently supports only the *constant* style.
The size-related keywords support the following styles:

* *shape* sets all three semi-axes at once and supports *constant*,
  *uniform*, and *gaussian*.
* *shapex*\ , *shapey*\ , and *shapez* allow each semi-axis to be specified
  independently and each supports *constant*, *uniform*, and *gaussian*.
* *size* is a shorthand for equiaxed particles and sets
  *a = b = c = size/2*. It supports *constant*, *uniform*, and *gaussian*.

Keyword *shape* and keyword *size* are mutually exclusive within the same
template.

The *blockiness* keyword currently supports only the *constant* style.
The deprecated alias *roundness* is still accepted and maps to the same
parameters.

Keyword *relative* lets the user set a flag to let other commands know if the
dimensions specified by this command are absolute (length units) or relative
(in % of a reference size). As with the spherical template, this should only
be changed from the default value when explicitly required by another command.

Particle orientation is not set by this template fix. For inserted
superquadrics, initial orientation is controlled by the insertion fix, e.g.
:doc:`fix\_insert\_pack <fix_insert_pack>` with
``orientation random``, ``orientation template``, or
``orientation constant q1 q2 q3 q4``.

Packfall will throw an error if the particle volume is too small compared to machine precision.
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
 
This fix requires :doc:`atom\_style <atom_style>` superquadric.
The *blockiness* values must be greater than or equal to 2.

Related commands
""""""""""""""""

:doc:`fix\_particletemplate\_sphere <fix_particletemplate_sphere>`,
:doc:`fix\_particledistribution\_discrete <fix_particledistribution_discrete>`,
:doc:`fix\_insert\_pack <fix_insert_pack>`,
:doc:`fix\_insert\_rate\_region <fix_insert_rate_region>`,
:doc:`fix\_insert\_stream <fix_insert_stream>`

Default
"""""""

a = 1.0, b = 1.0, c = 1.0, n1 = 2.0, n2 = 2.0, atom\_type = 1,
volume\_limit = 1e-12, relative = no


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
