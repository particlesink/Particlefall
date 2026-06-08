.. index:: compute atom/molecule

compute atom/molecule command
=============================

Syntax
""""""


.. parsed-literal::

   compute ID group-ID atom/molecule general_keywords general_values input1 input2 ...

* ID, group-ID are documented in :doc:`compute <compute>` command
* atom/molecule = style name of this compute command
* general\_keywords general\_values are documented in `compute <compute.html">`_
* one or more inputs can be listed
* input = c\_ID, c\_ID[N], f\_ID, f\_ID[N], v\_name
  
  .. parsed-literal::
  
       c_ID = per-atom vector calculated by a compute with ID
       c_ID[I] = Ith column of per-atom array calculated by a compute with ID
       f_ID = per-atom vector calculated by a fix with ID
       f_ID[I] = Ith column of per-atom array calculated by a fix with ID
       v_name = per-atom vector calculated by an atom-style variable with name



Examples
""""""""


.. parsed-literal::

   compute 1 all atom/molecule c_ke c_pe
   compute 1 top atom/molecule v_myFormula c_stress\ **3**

Description
"""""""""""

Define a calculation that sums per-atom values on a per-molecule
basis, one per listed input.  The inputs can :doc:`computes <compute>`,
:doc:`fixes <fix>`, or :doc:`variables <variable>` that generate per-atom
quantities.  Note that attributes stored by atoms, such as mass or
force, can also be summed on a per-molecule basis, by accessing these
quantities via the :doc:`compute property/atom <compute_property_atom>`
command.

Each listed input is operated on independently.  Only atoms within the
specified group contribute to the per-molecule sum.  Note that compute
or fix inputs define their own group which may affect the quantities
they return.  For example, if a compute is used as an input which
generates a per-atom vector, it will generate values of 0.0 for atoms
that are not in the group specified for that compute.

The ordering of per-molecule quantities produced by this compute is
consistent with the ordering produced by other compute commands that
generate per-molecule datums.  Conceptually, them molecule IDs will be
in ascending order for any molecule with one or more of its atoms in
the specified group.

If an input begins with "c\_", a compute ID must follow which has been
previously defined in the input script and which generates per-atom
quantities.  See the individual :doc:`compute <compute>` doc page for
details.  If no bracketed integer is appended, the vector calculated
by the compute is used.  If a bracketed integer is appended, the Ith
column of the array calculated by the compute is used.  Users can also
write code for their own compute styles and :doc:`add them to LIGGGHTS(R)-PUBLIC <Section_modify>`.

If an input begins with "f\_", a fix ID must follow which has been
previously defined in the input script and which generates per-atom
quantities.  See the individual :doc:`fix <fix>` doc page for details.
Note that some fixes only produce their values on certain timesteps,
which must be compatible with when compute atom/molecule references
the values, else an error results.  If no bracketed integer is
appended, the vector calculated by the fix is used.  If a bracketed
integer is appended, the Ith column of the array calculated by the fix
is used.  Users can also write code for their own fix style and :doc:`add them to LIGGGHTS(R)-PUBLIC <Section_modify>`.

If an input begins with "v\_", a variable name must follow which has
been previously defined in the input script.  It must be an
:doc:`atom-style variable <variable>`.  Atom-style variables can
reference thermodynamic keywords and various per-atom attributes, or
invoke other computes, fixes, or variables when they are evaluated, so
this is a very general means of generating per-atom quantities to sum
on a per-molecule basis.


----------


**Output info:**

This compute calculates a global vector or global array depending on
the number of input values.  The length of the vector or number of
rows in the array is the number of molecules.  If a single input is
specified, a global vector is produced.  If two or more inputs are
specified, a global array is produced where the number of columns =
the number of inputs.  The vector or array can be accessed by any
command that uses global values from a compute as input.  See :ref:`this section <howto_8>` for an overview of LIGGGHTS(R)-PUBLIC output
options.

All the vector or array values calculated by this compute are
"extensive".

The vector or array values will be in whatever :doc:`units <units>` the
input quantities are in.

Restrictions
""""""""""""
 none

Related commands
""""""""""""""""

:doc:`compute <compute>`, :doc:`fix <fix>`, :doc:`variable <variable>`

**Default:** none


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
