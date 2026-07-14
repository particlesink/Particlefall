.. index:: fix bond/create

fix bond/create command
=======================

Syntax
""""""


.. parsed-literal::

   fix ID group-ID bond/create Nevery itype jtype Rmin bondtype keyword values ...

* ID, group-ID are documented in :doc:`fix <fix>` command
* bond/create = style name of this fix command
* Nevery = attempt bond creation every this many steps
* itype,jtype = atoms of itype can bond to atoms of jtype
* Rmin = 2 atoms separated by less than Rmin can bond (distance units)
* bondtype = type of created bonds
* zero or more keyword/value pairs may be appended to args
* keyword = *iparam* or *jparam* or *prob*
  
  .. parsed-literal::
  
       *iparam* values = maxbond, newtype
         maxbond = max # of bonds of bondtype the itype atom can have
         newtype = change the itype atom to this type when maxbonds exist
       *jparam* values = maxbond, newtype
         maxbond = max # of bonds of bondtype the jtype atom can have
         newtype = change the jtype atom to this type when maxbonds exist
       *prob* values = fraction seed
         fraction = create a bond with this probability if otherwise eligible
         seed = random number seed (prime number greater 10000)



Examples
""""""""


.. parsed-literal::

   fix 5 all bond/create 10 1 2 0.8 1
   fix 5 all bond/create 1 3 3 0.8 1 prob 0.5 123457 iparam 2 3

Description
"""""""""""

Create bonds between pairs of atoms as a simulation runs according to
specified criteria.  This can be used to model cross-linking of
polymers, the formation of a percolation network, etc.  In this
context, a bond means an interaction between a pair of atoms computed
by the :doc:`bond\_style <bond_style>` command.  Once the bond is created
it will be permanently in place.

A check for possible new bonds is performed every *Nevery* timesteps.
If two atoms I,J are within a distance *Rmin* of each other, if I is
of atom type *itype*\ , if J is of atom type *jtype*\ , if both I and J
are in the specified fix group, if a bond does not already exist
between I and J, and if both I and J meet their respective *maxbond*
requirement (explained below), then I,J is labeled as a "possible"
bond pair.

If several atoms are close to an atom, it may have multiple possible
bond partners.  Every atom checks its list of possible bond partners
and labels the closest such partner as its "sole" bond partner.  After
this is done, if atom I has atom J as its sole partner, and atom J has
atom I as its sole partner, then the I,J bond is "eligible" to be
formed.

Note that these rules mean an atom will only be part of at most one
created bond on a given timestep.  It also means that if atom I
chooses atom J as its sole partner, but atom J chooses atom K is its
sole partner (due to Rjk < Rij), then this means atom I will not form
a bond on this timestep, even if it has other possible bond partners.

It is permissible to have *itype* = *jtype*\ .  *Rmin* must be <= the
pairwise cutoff distance between *itype* and *jtype* atoms, as defined
by the :doc:`pair\_style <pair_style>` command.

The *iparam* and *jparam* keywords can be used to limit the bonding
functionality of the participating atoms.  Each atom keeps track of
how many bonds of *bondtype* it already has.  If atom I of
itype already has *maxbond* bonds (as set by the *iparam*
keyword), then it will not form any more.  Likewise for atom J.  If
*maxbond* is set to 0, then there is no limit on the number of bonds
that can be formed with that atom.

The *newtype* value for *iparam* and *jparam* can be used to change
the atom type of atom I or J when it reaches *maxbond* number of bonds
of type *bondtype*\ .  This means it can now interact in a pairwise
fashion with other atoms in a different way by specifying different
:doc:`pair\_coeff <pair_coeff>` coefficients.  If you do not wish the
atom type to change, simply specify *newtype* as *itype* or *jtype*\ .

The *prob* keyword can also effect whether an eligible bond is
actually created.  The *fraction* setting must be a value between 0.0
and 1.0.  A uniform random number between 0.0 and 1.0 is generated and
the eligible bond is only created if the random number < fraction.

Any bond that is created is assigned a bond type of *bondtype*\ .  Data
structures within Packfall that store bond topology are updated to
reflect the new bond.  This can also affect subsequent computation of
pairwise interactions involving the atoms in the bond.  See the
Restriction section below for additional information.

.. warning::

   To create a new bond, the internal Packfall data
   structures that store this information must have space for it.  When
   Packfall is initialized from a data file, the list of bonds is scanned
   and the maximum number of bonds per atom is tallied.  If some atom
   will acquire more bonds than this limit as this fix operates, then the
   "extra bonds per atom" parameter in the data file header must be set
   to allow for it.  See the :doc:`read\_data <read_data>` command for more
   details.  Note that if this parameter needs to be set, it means a data
   file must be used to initialize the system, even if it initially has
   no bonds.  A data file with no atoms can be used if you wish to add
   unbonded atoms via the :doc:`create atoms <create_atoms>` command,
   e.g. for a percolation simulation.

.. warning::

   Packfall also maintains a data structure that stores a
   list of 1st, 2nd, and 3rd neighbors of each atom (in the bond topology
   of the system) for use in weighting pairwise interactions for bonded
   atoms.  Adding a bond adds a single entry to this list.  The "extra"
   keyword of the :doc:`special\_bonds <special_bonds>` command should be
   used to leave space for new bonds if the maximum number of entries for
   any atom will be exceeded as this fix operates.  See the
   :doc:`special\_bonds <special_bonds>` command for details.

Note that even if your simulation starts with no bonds, you must
define a :doc:`bond\_style <bond_style>` and use the
:doc:`bond\_coeff <bond_coeff>` command to specify coefficients for the
*bondtype*\ .  Similarly, if new atom types are specified by the
*iparam* or *jparam* keywords, they must be within the range of atom
types allowed by the simulation and pairwise coefficients must be
specified for the new types.

Computationally, each timestep this fix operates, it loops over
neighbor lists and computes distances between pairs of atoms in the
list.  It also communicates between neighboring processors to
coordinate which bonds are created.  Thus it roughly doubles the cost
of a timestep.  Thus you should be cautious about invoking this fix
too frequently.

You can dump out snapshots of the current bond topology via the :doc:`dump local <dump>` command.

.. warning::

   Creating a bond typically alters the energy of a
   system.  You should be careful not to choose bond creation criteria
   that induce a dramatic change in energy.  For example, if you define a
   very stiff harmonic bond and create it when 2 atoms are separated by a
   distance far from the equilibribum bond length, then the 2 atoms will
   oscillate dramatically when the bond is formed.  More generally, you
   may need to thermostat your system to compensate for energy changes
   resulting from created bonds.


----------


**Restart, fix\_modify, output, run start/stop, minimize info:**

No information about this fix is written to :doc:`binary restart files <restart>`.  None of the :doc:`fix\_modify <fix_modify>` options
are relevant to this fix.

This fix computes two statistics which it stores in a global vector of
length 2, which can be accessed by various :ref:`output commands <howto_8>`.  The vector values calculated
by this fix are "intensive".

These are the 2 quantities:

* (1) # of bonds created on the most recent creation timestep
* (2) cummulative # of bonds created

No parameter of this fix can be used with the *start/stop* keywords of
the :doc:`run <run>` command.  This fix is not invoked during :doc:`energy minimization <minimize>`.

Restrictions
""""""""""""


This fix is part of the MC package.  It is only enabled if Packfall was
built with that package.  See the :ref:`Making Packfall <start_3>` section for more info.

Currently, there are 2 restrictions for using this fix.  We may relax
these in the future if there are new models that would be enabled by
it.

When a bond is created, you might wish to induce new angle and
dihedral interactions that include that bond.  However, Packfall does
not create these angles and dihedrals, even if your simulation defines
an :doc:`angle\_style <angle_style>` or
:doc:`dihedral\_style <dihedral_style>`.

This fix requires that the pairwise weightings defined by the
:doc:`special\_bonds <special_bonds>` command be 0,1,1 for 1-2, 1-3, and
1-4 neighbors within the bond topology.  This effectively means that
the pairwise interaction between atoms I and J will be turned off when
a bond between them is created.  It also means that the pairwise
interaction of I with J's other bond partners will be unaffected by
the new bond.

Related commands
""""""""""""""""

:doc:`fix bond/break <fix_bond_break>`, :doc:`fix bond/swap <fix_bond_swap>`, :doc:`dump local <dump>`,
:doc:`special\_bonds <special_bonds>`

Default
"""""""

The option defaults are iparam = (0,itype), jparam = (0,jtype), and
prob = 1.0.


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
