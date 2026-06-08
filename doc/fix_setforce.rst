.. index:: fix setforce

fix setforce command
====================

Syntax
""""""


.. parsed-literal::

   fix ID group-ID setforce fx fy fz keyword value ...

* ID, group-ID are documented in :doc:`fix <fix>` command
* setforce = style name of this fix command
* fx,fy,fz = force component values
* any of fx,fy,fz can be a variable (see below)
* zero or more keyword/value pairs may be appended to args
* keyword = *region*
  
  .. parsed-literal::
  
       *region* value = region-ID
         region-ID = ID of region atoms must be in to have added force



Examples
""""""""


.. parsed-literal::

   fix freeze indenter setforce 0.0 0.0 0.0
   fix 2 edge setforce NULL 0.0 0.0
   fix 2 edge setforce NULL 0.0 v_oscillate

Description
"""""""""""

Set each component of force on each atom in the group to the specified
values fx,fy,fz.  This erases all previously computed forces on the
atom, though additional fixes could add new forces.  This command can
be used to freeze certain atoms in the simulation by zeroing their
force, either for running dynamics or performing an energy
minimization.  For dynamics, this assumes their initial velocity is
also zero.

Any of the fx,fy,fz values can be specified as NULL which means do not
alter the force component in that dimension.

Any of the 3 quantities defining the force components can be specified
as an equal-style or atom-style :doc:`variable <variable>`, namely *fx*\ ,
*fy*\ , *fz*\ .  If the value is a variable, it should be specified as
v\_name, where name is the variable name.  In this case, the variable
will be evaluated each timestep, and its value used to determine the
force component.

Equal-style variables can specify formulas with various mathematical
functions, and include :doc:`thermo\_style <thermo_style>` command
keywords for the simulation box parameters and timestep and elapsed
time.  Thus it is easy to specify a time-dependent force field.

Atom-style variables can specify the same formulas as equal-style
variables but can also include per-atom values, such as atom
coordinates.  Thus it is easy to specify a spatially-dependent force
field with optional time-dependence as well.

If the *region* keyword is used, the atom must also be in the
specified geometric :doc:`region <region>` in order to have force added
to it.





**Restart, fix\_modify, output, run start/stop, minimize info:**

No information about this fix is written to :doc:`binary restart files <restart>`.  None of the :doc:`fix\_modify <fix_modify>` options
are relevant to this fix.

This fix computes a global 3-vector of forces, which can be accessed
by various :ref:`output commands <howto_8>`.  This is the
total force on the group of atoms before the forces on individual
atoms are changed by the fix.  The vector values calculated by this
fix are "extensive".

No parameter of this fix can be used with the *start/stop* keywords of
the :doc:`run <run>` command.

The forces due to this fix are imposed during an energy minimization,
invoked by the :doc:`minimize <minimize>` command, but you cannot set
forces to any value besides zero when performing a minimization.  Use
the :doc:`fix addforce <fix_addforce>` command if you want to apply a
non-zero force to atoms during a minimization.

Restrictions
""""""""""""
 none

Related commands
""""""""""""""""

:doc:`fix addforce <fix_addforce>`, :doc:`fix aveforce <fix_aveforce>`

**Default:** none


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
