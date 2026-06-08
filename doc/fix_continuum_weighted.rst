.. index:: fix continuum/weighted

fix continuum/weighted command
==============================

Syntax
""""""

fix ID group-ID continuum/weighted keyword value

* ID, group-ID are documented in :doc:`fix <fix>` command
* continuum/weighted = style name of this fix command
* zero or one keyword/value pairs may be appended
  
  .. parsed-literal::
  
     keyword = {kernel_radius, kernel_type, compute}
       *kernel_radius* value = radius
         radius = Radius of the smoothing kernel
       *kernel_type* value = type
         type = Type of kernel {Top_Hat, Gaussian, Wendland}
       *compute* value = compute-type
         compute-type = Which tensor(s) to compute {stress, strain, stress_strain}



Examples
""""""""


.. parsed-literal::

   fix 1 all continuum/weighted kernel_radius 0.01 compute stress
   fix 1 all continuum/weighted kernel_radius 0.2 kernel_type Wendland compute stress_strain

Description
"""""""""""

If the *compute* keyword is set to either stress or stress\_strain this fix calculates the complete stress tensor at
each particle according to :ref:`Goldhirsch <Goldhirsch>`. The formula is given by:


.. parsed-literal::

     sigma_{i,ab} = -1/2 sum_{j,k} f_{jk,a} r_{jk,b} integral_0\^1 phi(\ **r**\ _i - **r**\ _j + s **r**\ _{jk}) ds
                  - sum_j m_j v'_{ij,a} v'_{ij,b} phi(r_i - r_j)
                  + sigma_wall_{i,ab}

where


.. parsed-literal::

     **v**\ '_{ij} = **v**\ _j - **V**\ _i
     **V**\ _i = **p**\ _i / rho_i
     **p**\ _i = sum_j m_j **v**\ _j phi(r_i - r_j)
     rho_i = sum_j m_j phi(r_i - r_j)
     phi(\ **r**\ ) = H(radius - \|r\|) / Omega(radius)
     Omega(radius) = 4/3 pi radius\^3

and


.. parsed-literal::

     **v** is the velocity vector
     **f**\ _{ij} force acting from j onto i
     **r**\ _{ij} vector from center of j to center of i
     m_i mass of particle i
     H is the Heavyside function

In case solid boundaries are present the last term is given according to :ref:`Weinhart et al. <Weinhart>` by


.. parsed-literal::

       sigma_wall_{i,ab} = - sum_{j,k} f_{jk,a} a_{jk,b} integral_0\^1 phi(\ **r**\ _i - **r**\ _j + s **a**\ _{jk}) ds

where


.. parsed-literal::

       **a**\ _{jk} = **r**\ _j - c_{jk}

and **c**\ \_{jk} is the contact point of particle *j* with wall *k* and the sum runs over all particles *j* and walls *k*\ .

If the *compute* keyword is set to either strain or stress\_strain this fix calculates the incremental strain tensor at
each particle according to :ref:`Zhang et al. <Zhang>` The formula is given by


.. parsed-literal::

     epsilon_\ *i,ab* = 1/(2 rho_i) sum_\ *j,k* m_j m_k phi(\ **r**\ _{ij}) dt (\ **v**\ _{jk,a} grad_phi(\ **r**\ _{ik},b) + **v**\ _{jk,b} grad_phi(\ **r**\ _{ik},a))

where most of the variables are given as above and additionally


.. parsed-literal::

       **v**\ _{ij,a} is the a-th component of the velocity difference between i and j
       grad_phi(\ **r**\ _{ij},a) is the a-th component of the gradient of phi with respect to **r**\ _i
       dt is the time-step size

The following three kernel types are implemented at the moment:

* *Top\_Hat* - Top hat kernel
  
  .. parsed-literal::
  
             w(r) = a_t \* 1 if q < 1 (q = r / kernel_radius)
                  = 0 otherwise

* *Gaussian* - Gaussian kernel
  
  .. parsed-literal::
  
             w(r) = a_g \* exp(-q\^2) (q = 3 r / kernel_radius)

* *Wendland* - Quinitic radial polynomial
  
  .. parsed-literal::
  
             w(r) = a_w \* (1-q/2)\^4 (1+2q) (q = 2 r / kernel_radius)



Note that all kernels are equal to zero if *r > kernel\_radius* (this implies a cut-off for the Gaussian). The constants *a* (different for each kernel) are chosen such that the integral of *w* over the ball of radius *kernel\_radius* is equal to one. In case of the top hat kernel *a\_t* is equal to the volume of this sphere.


----------


**Restart, fix\_modify, output, run start/stop, minimize info:**

No information about this fix is written to :doc:`binary restart files <restart>`.  None of the :doc:`fix\_modify <fix_modify>` options
are relevant to this fix.

The values can be dumped by using the *f\_stressTensor\_[i]* and/or *f\_strainTensor\_[i]* (0 <= i <= 8) values in :doc:`dump <dump>` commands

No parameter of this fix can be used with the *start/stop* keywords of
the :doc:`run <run>` command.  This fix is not invoked during :doc:`energy minimization <minimize>`.

Restrictions
""""""""""""


Strain computation does not work with the default *TOP\_HAT* kernel as its derivative is zero.

In order to ensure that all particles in the kernel radius are considered make use of the :doc:`neigh\_modify <neigh_modify>`
command. In particular the *contact\_distance\_factor* which should be set such that


.. parsed-literal::

     2\*min(radius)\*contact_distance_factor >= kernel_radius

Related commands
""""""""""""""""

:doc:`compute <compute>`, :doc:`compute stress/atom <compute_stress_atom>`,
:doc:`fix ave/atom <fix_ave_atom>`, :doc:`fix ave/histo <fix_ave_histo>`,
:doc:`fix ave/time <fix_ave_time>`, :doc:`fix ave/spatial <fix_ave_spatial>`,
:doc:`fix ave/euler <fix_ave_euler>`

**Default:** none

**References:**

.. _Goldhirsch:



**(Goldhirsch)** Goldhirsch; Stress, stress asymmetry and couple stress: from discrete particles to continuous fields, Granular Matter (2010)

.. _Weinhart:



**(Weinhart)** Weinhart, Thornton, Luding, Bokhove; From discrete particles to continuum fields near a boundary (2012)

.. _Zhang:



**(Zhang)** Zhang, Behringer, Goldhirsch; Coarse-Graining of a Physical Granular System, Progress of Theoretical Physics Supplement (2010)


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
