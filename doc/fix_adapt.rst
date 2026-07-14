.. index:: fix adapt

fix adapt command
=================

Syntax
""""""


.. parsed-literal::

   fix ID group-ID adapt N attribute args ... keyword value ...

* ID, group-ID are documented in :doc:`fix <fix>` command
* adapt = style name of this fix command
* N = adapt simulation settings every this many timesteps
* one or more attribute/arg pairs may be appended
* attribute = *pair* or *atom*
  
  .. parsed-literal::
  
       *pair* args = pstyle pparam I J v_name
         pstyle = pair style name, e.g. soft
         pparam = parameter to adapt over time
         I,J = type pair(s) to set parameter for
         v_name = variable with name that calculates value of pparam
       *atom* args = aparam v_name
         aparam = parameter to adapt over time
         v_name = variable with name that calculates value of aparam

* zero or more keyword/value pairs may be appended
* keyword = *scale* or *reset*
  
  .. parsed-literal::
  
       *scale* value = *no* or *yes*
         *no* = the variable value is the new setting
         *yes* = the variable value multiplies the original setting
       *reset* value = *no* or *yes*
         *no* = values will remain altered at the end of a run
         *yes* = reset altered values to their original values at the end of a run



Examples
""""""""


.. parsed-literal::

   fix 1 all adapt 1 pair soft a 1 1 v_prefactor
   fix 1 all adapt 1 pair soft a 2\* 3 v_prefactor
   fix 1 all adapt 10 atom diameter v_size

Description
"""""""""""

Change or adapt one or more specific simulation attributes or settings
over time as a simulation runs.  Pair potential and K-space and atom
attributes which can be varied by this fix are discussed below.  Many
other fixes can also be used to time-vary simulation parameters,
e.g. the "fix deform" command will change the simulation box
size/shape and the "fix move" command will change atom positions and
velocities in a prescribed manner.  Also note that many commands allow
variables as arguments for specific parameters, if described in that
manner on their doc pages.  An equal-style variable can calculate a
time-dependent quantity, so this is another way to vary a simulation
parameter over time.

If *N* is specified as 0, the specified attributes are only changed
once, before the simulation begins.  This is all that is needed if the
associated variables are not time-dependent.  If *N* > 0, then changes
are made every *N* steps during the simulation, presumably with a
variable that is time-dependent.

Depending on the value of the *reset* keyword, attributes changed by
this fix will or will not be reset back to their original values at
the end of a simulation.  Even if *reset* is specified as *yes*\ , a
restart file written during a simulation will contain the modified
settings.

If the *scale* keyword is set to *no*\ , then the value the parameter is
set to will be whatever the variable generates.  If the *scale*
keyword is set to *yes*\ , then the value of the altered parameter will
be the initial value of that parameter multiplied by whatever the
variable generates.  I.e. the variable is now a "scale factor" applied
in (presumably) a time-varying fashion to the parameter.  Internally,
the parameters themselves are actually altered; make sure you use the
*reset yes* option if you want the parameters to be restored to their
initial values after the run.


----------


The *pair* keyword enables various parameters of potentials defined by
the :doc:`pair\_style <pair_style>` command to be changed, if the pair
style supports it.  Note that the :doc:`pair\_style <pair_style>` and
:doc:`pair\_coeff <pair_coeff>` commands must be used in the usual manner
to specify these parameters initially; the fix adapt command simply
overrides the parameters.

The *pstyle* argument is the name of the pair style.  If :doc:`pair\_style hybrid or hybrid/overlay <pair_hybrid>` is used, *pstyle* should be
a sub-style name.  For example, *pstyle* could be specified as "soft"
or "lubricate".  The *pparam* argument is the name of the parameter to
change.  This is the current list of pair styles and parameters that
can be varied by this fix.  See the doc pages for individual pair
styles and their energy formulas for the meaning of these parameters:

+-------------------------+---+------------+
| :doc:`soft <pair_soft>` | a | type pairs |
+-------------------------+---+------------+

.. warning::

   It is easy to add new potentials and their parameters
   to this list.  All it typically takes is adding an extract() method to
   the pair\_\*.cpp file associated with the potential.

Some parameters are global settings for the pair style, e.g. the
viscosity setting "mu" for :doc:`pair\_style lubricate <pair_lubricate>`.
Other parameters apply to atom type pairs within the pair style,
e.g. the prefactor "a" for :doc:`pair\_style soft <pair_soft>`.

If a type pair parameter is specified, the *I* and *J* settings should
be specified to indicate which type pairs to apply it to.  If a global
parameter is specified, the *I* and *J* settings still need to be
specified, but are ignored.

Similar to the :doc:`pair\_coeff command <pair_coeff>`, I and J can be
specified in one of two ways.  Explicit numeric values can be used for
each, as in the 1st example above.  I <= J is required.  Packfall sets
the coefficients for the symmetric J,I interaction to the same values.

A wild-card asterisk can be used in place of or in conjunction with
the I,J arguments to set the coefficients for multiple pairs of atom
types.  This takes the form "\*" or "\*n" or "n\*" or "m\*n".  If N = the
number of atom types, then an asterisk with no numeric values means
all types from 1 to N.  A leading asterisk means all types from 1 to n
(inclusive).  A trailing asterisk means all types from n to N
(inclusive).  A middle asterisk means all types from m to n
(inclusive).  Note that only type pairs with I <= J are considered; if
asterisks imply type pairs where J < I, they are ignored.

IMPROTANT NOTE: If :doc:`pair\_style hybrid or hybrid/overlay <pair_hybrid>` is being used, then the *pstyle* will
be a sub-style name.  You must specify I,J arguments that correspond
to type pair values defined (via the :doc:`pair\_coeff <pair_coeff>`
command) for that sub-style.

The *v\_name* argument for keyword *pair* is the name of an
:doc:`equal-style variable <variable>` which will be evaluated each time
this fix is invoked to set the parameter to a new value.  It should be
specified as v\_name, where name is the variable name.  Equal-style
variables can specify formulas with various mathematical functions,
and include :doc:`thermo\_style <thermo_style>` command keywords for the
simulation box parameters and timestep and elapsed time.  Thus it is
easy to specify parameters that change as a function of time or span
consecutive runs in a continuous fashion.  For the latter, see the
*start* and *stop* keywords of the :doc:`run <run>` command and the
*elaplong* keyword of :doc:`thermo\_style custom <thermo_style>` for
details.

For example, these commands would change the prefactor coefficient of
the :doc:`pair\_style soft <pair_soft>` potential from 10.0 to 30.0 in a
linear fashion over the course of a simulation:


.. parsed-literal::

   variable prefactor equal ramp(10,30)
   fix 1 all adapt 1 pair soft a \* \* v_prefactor


----------


The *atom* keyword enables various atom properties to be changed.  The
*aparam* argument is the name of the parameter to change.  This is the
current list of atom parameters that can be varied by this fix:

* charge = charge on particle
* diameter = diameter of particle

The *v\_name* argument of the *atom* keyword is the name of an
:doc:`equal-style variable <variable>` which will be evaluated each time
this fix is invoked to set the parameter to a new value.  It should be
specified as v\_name, where name is the variable name.  See the
discussion above describing the formulas associated with equal-style
variables.  The new value is assigned to the corresponding attribute
for all atoms in the fix group.

If the atom parameter is *diameter* and per-atom density and per-atom
mass are defined for particles (e.g. :doc:`atom\_style granular <atom_style>`), then the mass of each particle is also
changed when the diameter changes (density is assumed to stay
constant).

For example, these commands would shrink the diameter of all granular
particles in the "center" group from 1.0 to 0.1 in a linear fashion
over the course of a 1000-step simulation:


.. parsed-literal::

   variable size equal ramp(1.0,0.1)
   fix 1 center adapt 10 atom diameter v_size


----------


**Restart, fix\_modify, output, run start/stop, minimize info:**

No information about this fix is written to :doc:`binary restart files <restart>`.  None of the :doc:`fix\_modify <fix_modify>` options
are relevant to this fix.  No global or per-atom quantities are stored
by this fix for access by various :ref:`output commands <howto_8>`.  No parameter of this fix can
be used with the *start/stop* keywords of the :doc:`run <run>` command.
This fix is not invoked during :doc:`energy minimization <minimize>`.

Restrictions
""""""""""""
 none

Related commands
""""""""""""""""

:doc:`compute ti <compute_ti>`

Default
"""""""

The option defaults are scale = no, reset = no.


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
