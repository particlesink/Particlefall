.. index:: fix check/timestep/gran

fix check/timestep/gran command
===============================

Syntax
""""""

* fix ID group-ID check/timestep/gran nevery fraction\_r fraction\_h  keywords vales:pre
  ID, group-ID are documented in :doc:`fix <fix>` command
* check/timestep/gran = style name of this fix command
* nevery = evaluate time-step size accuracy every this many time-steps
* fraction\_r = warn if time-step size exceeds this fraction of the Rayleigh time
* fraction\_h = warn if time-step size exceeds this fraction of the Hertz time
* zero or more keyword/value pairs may be appended
* keyword = *warn* or *error* or *vmax*
  
  .. parsed-literal::
  
       *warn* value = yes or no
       *error* value = yes or no
       *vmax* value = v_max
         v_max = maximum particle velocity to be used as minimum for evaluation Hertz criterion



Examples
""""""""


.. parsed-literal::

   fix ts_check all check/timestep/gran 1000 0.1 0.1

Description
"""""""""""

Periodically calculate estimations of the Rayleigh- and Hertz time
dt\_r and dt\_h for a granular system every 'nevery' time-steps. The user
can specify two quantities *fraction\_r* and *fraction\_h*. A warning
message is printed if the time-step size as specified via the
:doc:`timestep <timestep>` command exceeds either of dt\_r \* fraction\_r
or dt\_h \* fraction\_h.

The former quantity is 

dt\_r = PI\*r\*sqrt(rho/G)/(0.1631\*nu+0.8766),

where rho is particle density, G is the shear modulus and nu is Poisson's
ratio. The latter quantity is expressed by

dt\_h = 2.87\*(m\_eff\^2/(r\_eff\*Y\_eff\^2\*v\_max))\^0.2.

The effective mass m\_eff, the effective radius r\_eff and the effective Young's
modulus Y\_eff are as defined in :doc:`pair gran <pair_gran>`. v\_max is the
maximum relative velocity, taking mesh movement into account. Please note
that the Hertz criterion will also be used if you use a different granular
pair style (e.g. Hooke). If keyword *vmax* is used, a user-defined maximum
velocity is used as a minimum in the formula above, i.e. the maximum of
v\_max of the particles in the simulation and v\_max specified by the user
is used.

Additionally, this command checks the ratio of skin to the distance that
particles can travel relative to each other in one time-step. This value
should be >1, otherwise some interactions may be missed or overlap energy
may be generated artificially. This command will warn you if this is the case.

These criteria are checked every 'nevery' time-steps. Rayleigh time dt\_r is
calculated for each particle in the simulation, and the minimum value is
taken for further calculations. Hertz time dt\_h is estimated by testing a
collision of each particle with itself using v\_max as the assumed collision
velocity.

Keyword *warn* can be used to turn off the warning message. Keyword *error*
can be used to have Packfall issue an error message and abort the simulation
if any of the criteria is violated.

**Restart, fix\_modify, output, run start/stop, minimize info:**

No information about this fix is written to :doc:`binary restart files <restart>`.  None of the :doc:`fix\_modify <fix_modify>` options
are relevant to this fix.  This fix computes a 3-vector, for access by
various :ref:`output commands <4_15>`.  The vector consists
of the time-step size expressed as fraction of the Rayleigh and Hertz
time-step sizes and the ratio of skin to the distance particles can
travel relative to each other in one time-step, respectively. No
parameter of this fix can be used with the *start/stop* keywords of
the :doc:`run <run>` command. This fix is not invoked during
:doc:`energy minimization <minimize>`.

Restrictions
""""""""""""
 none

**Related commands:** none

**Default:** 
warn = yes, error = no, v\_max = 0


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
