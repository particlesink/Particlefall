.. index:: fix property/global

fix property/global command
===========================

fix property/atom command
=========================

Syntax
""""""


.. parsed-literal::

   fix id group property/atom variablename style restartvalue comm_ghost_value comm_reverse_ghost_value defaultvalue(s)...
   fix id group property/global variablename style stylearg defaultvalue(s)...

* ID, group-ID are documented in :doc:`fix <fix>` command
* property/global or property/atom = style name of this fix command
* variablename = a valid C++ string
* restartvalues = 'yes' or 'no'
* comm\_ghost\_value = 'yes' or 'no'
* comm\_reverse\_ghost\_value = 'yes' or 'no'


fix property/global:

* style = scalar or vector or atomtype or matrix or atomtypepair
  
  .. parsed-literal::
  
         stylearg for scalar/vector: none
         stylearg for matrix/atomtypepair: nCols



fix property/atom:

* style = scalar or vector
* restartvalue = yes or no
* communicate\_ghost\_value = yes or no
* communicate\_reverse\_ghost\_value = yes or no


Examples
""""""""


.. parsed-literal::

   fix m3 all property/global coefficientRestitution peratomtypepair 1 0.3
   fix m5 all property/global characteristicVelocity scalar 2.
   fix uf all property/atom uf vector yes no no  0. 0. 0.

Description
"""""""""""

**Fix property/atom** reserves per-atom properties to be accessed by the user or other fixes.
Style *scalar* reserves one value per atom, style *vector* multiple values per atoms, where
the number of *defaultvalues* (that are assigned to the atoms at creation) determines the
length of the vector. The group of atoms the fix is applied to is always "all", irrespective
of which group is used for the fix command . If you want to assign different values for
different groups, you can use the :doc:`set <set>` command with keyword 'property/atom'.
Keyword *restartvalues* determines whether information about the values stored by this fix
is written to binary restart files.
Keyword *communicate\_ghost\_value* determines whether information about the values stored by this fix
can be communicated to ghost particles (forward communication). The exact location during a time-step
when this happens depends on the model that uses this fix.
Keyword *communicate\_reverse\_ghost\_value* determines whether information about the values stored by this fix
can be communicated from ghost particles to owned particles (reverse communication). The exact location
during a time-step when this happens depends on the model that uses this fix.

**Fix property/global** reserves global properties to be accessed by the user or other
fixes or pair styles. The number of defaultvalues determines the length of the vector /
the number of matrix components . For style *vector* or *atomtype*\ , the user provides
the number of vector components . For style *matrix* or *atomtypepair*\ , the user provides
the number of matrix columns (\ *nCols*\ ) .

Example: *nCols*\ = 2 and *defaultvalues* = 1 2 3 4 5 6 would be mapped into a matrix like

[ 1 2 ]

[ 3 4 ]

[ 5 6 ]

Note that the number of default values must thus be a multiple of *nCols*\ .
Note that *vector* and *atomtype* do the same thing, *atomtype* is just provided to make input scripts more readable .
Note that *matrix* and *atomtypepair* both refer to a matrix of global values. However, a matrix defined via *atomtypepair* is required to be symmetric.

Note that the group of atoms the fix is applied to is ignored (as the fix is not applied to atoms, but defines values of global scope).

**Restart, fix\_modify, output, run start/stop, minimize info:**

Information about this fix is written to :doc:`binary restart files <restart>` if you set *restartvalue* to 'yes'.

Restrictions
""""""""""""
 none

Related commands
""""""""""""""""

:doc:`set <set>`, :doc:`pair\_gran <pair_gran>`

**Default:** none


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
