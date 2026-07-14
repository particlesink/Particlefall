.. index:: fix particletemplate/multisphere

fix particletemplate/multisphere command
========================================

Syntax
""""""


.. parsed-literal::

   fix ID group-ID particletemplate/multisphere seed keyword values nspheres nspheresvalue  ntry ntryvalue spheres values_spheres type mt opt_keyword opt_values

* ID, group-ID are documented in :doc:`fix <fix>` command
* particletemplate/multisphere = style name of this fix command
* seed = random number generator seed (prime number greater 10000)
* zero or more keyword/value pairs can be appended
* keyword, values are documented in :doc:`fix particletemplate/sphere <fix_particletemplate_sphere>` command
* nspheres = obligatory keyword
* nspheresvalue = number of spheres in the template (integer value)
* ntry = obligatory keyword
* ntryvalue: number of tries for Monte Carlo approach
* spheres = obligatory keyword
* values\_spheres = one out of the following options
  
  .. parsed-literal::
  
         option 1 = *file* *filename*
         option 2 = *file* *filename* *scale* *scalefactor*
         option 3 = *x1 y1 z1 r1 x2 y2 r2...* where x/y/z are sphere positions and r are the radii

* type = obligatory keyword
* mt = multisphere type of the template
* opt\_keyword = *mass* or *inertia\_tensor* or *use\_volume* or *use\_density* or *fflag* or *tflag*
  
  .. parsed-literal::
  
     *mass* value = mass assigned to this particle template
     *inertia_tensor* values = Ixx Ixy Ixz Iyy Iyz Izz
       Ixx Ixy Ixz Iyy Iyz Izz = 6 independant components of the inertia tensor
     *use_volume* = particle density calculated from mass and volume (only if keyword 'mass' is used')
     *use_density* = particle volume calculated from mass and density (only if keyword 'mass' is used')
     *fflag* values = fflagx fflagy fflagz
       fflagx = on or off
       fflagy = on or off
       fflagz = on or off
     *tflag* values = tflagx tflagy tflagz
       tflagx = on or off
       tflagy = on or off
       tflagz = on or off



Examples
""""""""


.. parsed-literal::

   fix pts1 all particletemplate/multisphere 123457 atom_type 1 density constant 2500 nspheres 50 ntry 1000000 spheres file data/stone1.multisphere scale 0.001 type 1
   fix pts2 all particletemplate/multisphere 17903 atom_type 1 density constant 2500 nspheres 50 ntry 1000000 spheres 0. 0. 0. 0.005   0.005 0. 0. 0.005   0.01 0. 0. 0.005 scale 0.001 type 1

Description
"""""""""""

Define a multisphere particle template that is used as input for a
:doc:`fix\_particledistribution\_discrete <fix_particledistribution_discrete>`
command. The particle parameters (positions, radii) are either defined
manually as arguments to this command or via a text file, via the keyword
*file*\ . The format of this text file is


.. parsed-literal::

   x1 y1 z1 r1
   x2 y2 z2 r2
   ...

and the number of lines in this file has to be equal to n\_spheres as
defined in this command. Comments can be made in this file via the '#'
character. Optionally, when a file is used for defining the multi-sphere
template, keyword *scale* can be used to define a *scalefactor* to scale
up or down the particle positions and radii.

After the spheres are read, a Monte Carlo procedure is used to assess
everything that is needed for the motion integration: mass, center of mass,
and the inertia tensor including its eigensystem. The values calculated
are written to the logfile.

As an alternative, the body's mass and inertia tensor can be specified
directly via keywords *mass* and *inertia\_tensor*. Note that you can
use these keywords only together, i.e. defining only *mass* but not
*inertia\_tensor* will throw an error. Also note that only 2 out of the
3 variables density, mass and volume are independant. Thus, you are
offered two options when *mass* and *inertia\_tensor* are used:
(a) if keyword *use\_volume* is specified, Packfall will use the
specified *mass* and volume\_mc (the volume of the particle template
calculated by the Monte Carlo procedure), and calculate the density from
these two variables.
(b) if keyword *use\_density* is used, Packfall will use the specified
*mass* and the specified *density* (see doc of
:doc:`fix particletemplate/sphere <fix_particletemplate_sphere>` command),
and the volume of the clump is then calculated from these two variables.
Note you have to use either *use\_volume* or *use\_density* in case
*mass* and *inertia\_tensor* are used.

The multisphere type or shape type as defined via the *type* keyword must
be unique integer given to each fix particletemplate/multisphere
command by the user (starting with 1), the list of all multisphere types
in the simulation must be consecutive. At the moment, the multisphere type
is not used, but will be used to implement orientation-dependant drag
for CFD-DEM simulations in the future.

The additional keywords *fflag* and *tflag* can be used to deactivate
selected translational and rotational degrees of freedom of the bodies.
For example *fflag* = on on off and *tflag* = off on on would mean that
bodies will not move translationally in z-direction and will not rotate
around the x-axis.

.. warning::

   As opposed to the number-based distributions, this fix uses the more common
   distribution based on mass-% for the radius distribution (as does
   :doc:`fix\_particledistribution\_discrete <fix_particledistribution_discrete>`).

**Restart, fix\_modify, output, run start/stop, minimize info:**

Information about the random state in this fix is written to
:doc:`binary restart files <restart>` so you can restart a simulation with
the same particles being chosen for insertion. None of the
:doc:`fix\_modify <fix_modify>` options are relevant to this fix. No global
scalar or vector or per-atom quantities are stored by this fix for access
by various :ref:`output commands <4_15>`. No parameter of this
fix can be used with the *start*\ /\ *stop* keywords of the :doc:`run <run>`
command. This fix is not invoked during :doc:`energy minimization <minimize>`.

Restrictions
""""""""""""
 none

Related commands
""""""""""""""""

:doc:`fix\_particletemplate\_sphere <fix_particletemplate_sphere>`

Default
"""""""

radius = 1.0, density = 1.0, atom\_type = 1, fflag = tflag = on on on


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
