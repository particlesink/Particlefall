.. index:: thermo

thermo command
==============

Syntax
""""""


.. parsed-literal::

   thermo N

* N = output thermodynamics every N timesteps
* N can be a variable (see below)
* N can also be specified as a physical time with a trailing *sec* or *s* suffix (prototype)

Examples
""""""""


.. parsed-literal::

   thermo 100
   thermo 0.25 sec

Description
"""""""""""

Compute and print thermodynamic info (e.g. temperature, energy,
pressure) on timesteps that are a multiple of N and at the beginning
and end of a simulation.  A value of 0 will only print thermodynamics
at the beginning and end.

As a prototype, a physical time can also be specified directly.  For
example, with a fixed timestep of 1.0e-5 seconds, these two commands
are equivalent:


.. parsed-literal::

   thermo 100000
   thermo 1.0 sec

This allows input scripts to express thermo cadence in physical time
without first converting it to a timestep count via a variable.

The content and format of what is printed is controlled by the
:doc:`thermo\_style <thermo_style>` and
:doc:`thermo\_modify <thermo_modify>` commands.

Instead of a numeric value, N can be specifed as an :doc:`equal-style variable <variable>`, which should be specified as v\_name, where
name is the variable name.  In this case, the variable is evaluated at
the beginning of a run to determine the next timestep at which
thermodynamic info will be written out.  On that timestep, the
variable will be evaluated again to determine the next timestep, etc.
Thus the variable should return timestep values.  See the stagger()
and logfreq() and stride() math functions for :doc:`equal-style variables <variable>`, as examples of useful functions to use in
this context.  Other similar math functions could easily be added as
options for :doc:`equal-style variables <variable>`.

For example, the following commands will output thermodynamic info at
timesteps 0,10,20,30,100,200,300,1000,2000,etc:


.. parsed-literal::

   variable	s equal logfreq(10,3,10)
   thermo	        v_s

Restrictions
""""""""""""
 none

Related commands
""""""""""""""""

:doc:`thermo\_style <thermo_style>`, :doc:`thermo\_modify <thermo_modify>`

Default
"""""""


.. parsed-literal::

   thermo 0


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
