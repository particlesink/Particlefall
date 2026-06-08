.. index:: fix particledistribution/discrete

fix particledistribution/discrete command
=========================================

fix particledistribution/discrete/massbased command
===================================================

fix particledistribution/discrete/numberbased command
=====================================================

Syntax
""""""


.. parsed-literal::

   fix ID group-ID particledistribution/discrete seed ntemp t_id t_weight ...

* ID, group-ID are documented in :doc:`fix <fix>` command
* particledistribution/discrete or particledistribution/discrete/massbased or particledistribution/discrete/numberbased = style name of this fix command
* seed = random number generator seed (prime number greater 10000)
* ntemp = number of particle templates to be used in this command
* zero or more *t\_id*/*t\_weight* pairs are appended, number of pairs must match ntemp
  
  .. parsed-literal::
  
       *t_id* = ID of a fix of type particletemplate/sphere
       *t_weight* = mass % or number % for this template in the distribution



Examples
""""""""


.. parsed-literal::

   fix pdd1 all particledistribution/discrete 123457  1 pts1 1.0
   fix pdd1 all particledistribution/discrete 17903  2 pts1 0.3 pts2 0.7

Description
"""""""""""

Define a discrete particle distribution that defines a discrete particle distribution 
to be inserted by a :doc:`fix insert/stream <fix_insert_stream>`, 
:doc:`fix insert/pack <fix_insert_pack>`, :doc:`fix insert/rate/region <fix_insert_rate_region>`
or a related command. It takes several templates of type 
:doc:`fix\_particletemplate\_sphere <fix_particletemplate_sphere>` as input, which define 
the properties of the single particles (such as radius, density that are part of the 
distribution. The pairs of IDs and weights for the templates (*t\_id* / *t\_weight*)
define the distribution. Please note that the number of pairs must match *ntemp*\ , but
can be arbitrtarily large, so that any type of particle size distribution can be
discretized.

For style *particledistribution/discrete* or *particledistribution/discrete/massbased*
the weight of each template within the distribution is interpreted as
mass-%, for style *particledistribution/discrete/numberbased* the weight is interpreted
as number-%. Note that the sum of all weights must be equal to 1.0, 
if this is not the case the user is warned and the distribution is normalized automatically.
Note  that large particles are inserted first, so that a higher volume fraction can be 
achieved. If not all desired insertions could be performed, it is likely that the 
distribution is not accurately reproduced.

**Restart, fix\_modify, output, run start/stop, minimize info:**

Information about the random state in this fix is written to 
:doc:`binary restart files <restart>` so you can restart a simulation with the same 
particles being chosen for insertion. None of the :doc:`fix\_modify <fix_modify>` options 
are relevant to this fix. No global scalar or vector or per-atom quantities are stored 
by this fix for access by various :ref:`output commands <4_15>`. 
No parameter of this fix can be used with the *start*\ /\ *stop* keywords of the 
:doc:`run <run>` command. This fix is not invoked during :doc:`energy minimization <minimize>`.

Restrictions
""""""""""""
 none

Related commands
""""""""""""""""

:doc:`fix insert/stream <fix_insert_stream>`, 
:doc:`fix insert/pack <fix_insert_pack>`, 
:doc:`fix insert/rate/region <fix_insert_rate_region>`

**Default:** none


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
