.. index:: fix multicontact/halfspace

fix multicontact/halfspace command
==================================

Syntax
""""""


.. parsed-literal::

   fix ID group multicontact/halfspace geometric_prefactor gp_value

* ID, group are documented in :doc:`fix <fix>` command
* multicontact/halfspace = style name of this fix command
* geometric\_prefactor/gp\_value = optional keyword-value pair
  
  .. parsed-literal::
  
       *geometric_prefactor* gp_value = gamma
         gamma is an empirical factor accounting for the geometry



Examples
""""""""


.. parsed-literal::

   fix mc all multicontact/halfspace
   fix mc all multicontact/halfspace geometric_prefactor 1.8

Description
"""""""""""

Implements the multicontact model by :ref:`(Brodu et al.) <Brodu>` . This model computes a per-contact deformation for each particle based on the other contacts this particles has. A particle *i* with contact *ij* has the following new radius (when computing the contact laws with particle *j*\ ):


.. parsed-literal::

   r_i + sum_k delta_{ij->ik}

where *r\_i* is the default radius of particle *i*\ , *sum\_k* is the sum over all particles in contact with *i* (\ *k*\ !=\ *j*\ ). The delta value is given by


.. parsed-literal::

   delta_{ij->ik} = - gamma (1 + nu) F_ik / (2 pi Y d_{ik->ij}) \* [(\ **n**\ _ik . **u**\ _{ik->ij})(\ **n**\ _ij . **u**\ _{ik->ij}) + (3 - 4 nu) **n**\ _ik . **n**\ _ij - (1 - 2 nu) (\ **n**\ _ik + **u**\ _{ik->ij}) . **n**\ _ij) / (1 + **n**\ _ik **u**\ _{ik->ij}) ]

where

* gamma = geometric prefactor
* nu = Poisson ratio
* F\_ik = absolute value of normal force acting at contact *ik*
* Y = Youngs modulus
* d\_{ik->ij} = distance from contact *ik* to contact *ij*
* **u**\ \_{ik->ij} = unit vector pointing from contact *ik* to contact *ij*
* **n**\ \_ik = normal vector of contact *ik*
* **n**\ \_ij = normal vector of contact *ij*



----------


**Restart, fix\_modify, output, run start/stop, minimize info:**

No information about this fix is written to :doc:`binary restart files <restart>`.

:doc:`fix\_modify <fix_modify>` cannot be used on the parameter of this fix.

Restrictions
""""""""""""


Requires the use of the :doc:`gran surface model multicontact <gran_surface_multicontact>`

Related commands
""""""""""""""""

:doc:`gran surface multicontact <gran_surface_multicontact>`

**Default:** none

**References:**

.. _Brodu:



**(Brodu)** Brodu, Dijksman, Behringer, Multiple-contact discrete-element model for simulating dense granular media, Phys. Rev. E (2015).


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
