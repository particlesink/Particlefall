.. index:: delete\_bonds

delete\_bonds command
=====================

Syntax
""""""


.. parsed-literal::

   delete_bonds group-ID style args keyword ...

* group-ID = group ID
* style = *multi* or *atom* or *bond* or *stats*
  
  .. parsed-literal::
  
       *multi* args = none
       *atom* args = an atom type
       *bond* args = a bond type
       *stats* args = none

* zero or more keywords may be appended
* keyword = *any* or *undo* or *remove* or *special*


Examples
""""""""


.. parsed-literal::

   delete_bonds frozen multi remove
   delete_bonds all atom 4 special
   delete_bonds all stats

Description
"""""""""""

Turn off (or on) molecular topology interactions, i.e. bonds.
This command is useful for deleting
interactions that have been previously turned off by bond-breaking
potentials.  It is also useful for turning off topology interactions
between frozen or rigid atoms.  Pairwise interactions can be turned
off via the :doc:`neigh\_modify exclude <neigh_modify>` command.

For all styles, by default, an interaction is only turned off (or on)
if all the atoms involved are in the specified group.  See the *any*
keyword to change the behavior.

Style *atom* is the same as style *multi* except that in addition, one
or more of the atoms involved in the bond, interaction must also be of the specified atom type.

For style *bond*\ , only bonds are candidates for turn-off, and the bond
must also be of the specified type.

For style *bond*\ , you can set the type to 0 to delete bonds that have
been previously broken by a bond-breaking potential (which sets the
bond type to 0 when a bond is broken); e.g. see the :doc:`bond\_style quartic <bond_style>` command.

For style *stats* no interactions are turned off (or on); the status
of all interactions in the specified group is simply reported.  This
is useful for diagnostic purposes if bonds have been turned off by a
bond-breaking potential during a previous run.

The default behavior of the delete\_bonds command is to turn off
interactions by toggling their type to a negative value, but not to
permanently remove the interaction.  E.g. a bond\_type of 2 is set to
-2.  The neighbor list creation routines will not include such an
interaction in their interaction lists.  The default is also to not
alter the list of 1-2, 1-3, 1-4 neighbors computed by the
:doc:`special\_bonds <special_bonds>` command and used to weight pairwise
force and energy calculations.  This means that pairwise computations
will proceed as if the bond were still turned on.

Several keywords can be appended to the argument list to alter the
default behaviors.

The *any* keyword changes the requirement that all atoms in the bond
must be in the specified group in order to turn-off the
interaction.  Instead, if any of the atoms in the interaction are in
the specified group, it will be turned off (or on if the *undo*
keyword is used).

The *undo* keyword inverts the delete\_bonds command so that the
specified bonds are turned on if they are currently
turned off.

The *remove* keyword is invoked at the end of the delete\_bonds
operation.  It causes turned-off bonds to be removed
from each atom's data structure and then adjusts the global bond
counts accordingly.  Removal is a permanent change;
removed bonds cannot be turned back on via the *undo* keyword.
Removal does not alter the pairwise 1-2, 1-3, 1-4 weighting list.

The *special* keyword is invoked at the end of the delete\_bonds
operation, after (optional) removal.  It re-computes the pairwise 1-2,
1-3, 1-4 weighting list.  The weighting list computation treats
turned-off bonds the same as turned-on.  Thus, turned-off bonds must
be removed if you wish to change the weighting list.

Note that the choice of *remove* and *special* options affects how
1-2, 1-3, 1-4 pairwise interactions will be computed across bonds that
have been modified by the delete\_bonds command.

Restrictions
""""""""""""


This command requires inter-processor communication to coordinate the
deleting of bonds.  This means that your system must be ready to
perform a simulation before using this command (force fields setup,
atom masses set, etc).

Related commands
""""""""""""""""

:doc:`neigh\_modify <neigh_modify>` exclude,
:doc:`special\_bonds <special_bonds>`

**Default:** none


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
