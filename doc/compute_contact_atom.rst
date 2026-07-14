.. index:: compute contact/atom

compute contact/atom command
============================

compute contact/atom/gran command
=================================

Syntax
""""""


.. parsed-literal::

   compute ID group-ID contact/atom general_keywords general_values
   compute ID group-ID contact/atom/gran general_keywords general_values

* ID, group-ID are documented in :doc:`compute <compute>` command
* contact/atom or contact/atom/gran = style name of this compute command
* general\_keywords general\_values are documented in `compute <compute.html">`_

Examples
""""""""


.. parsed-literal::

   compute 1 all contact/atom
   compute 1 all contact/atom/gran

Description
"""""""""""

Define a computation that calculates the number of contacts for each
atom in a group.

The contact number is defined for finite-size spherical particles as
the number of neighbor atoms which are close to the central particle,
meaning that their distance of separation is less than or equal to the
sum of the radii of the two particles plus the skin distance. In case
the *contact/atom/gran* style is chosen, a contact is defined as the
overlap of two particles (distance < sum of radii) or if a contactflag
in the history is set (e.g. due to a bond contact)

The value of the contact number will be 0.0 for atoms not in the
specified compute group.

This compute is one of the three different ways to compute a coordination
number. The following table gives an overview over the different options:

+---------------------------------------------------------+---------------------------------------------+-----------------------+
| **style**                                               | **contact counted condition**               | **formula**           |
+---------------------------------------------------------+---------------------------------------------+-----------------------+
| :doc:`compute contact/atom <compute_contact_atom>`      | particles touch each other                  | *r* < *r\_i* + *r\_j* |
+---------------------------------------------------------+---------------------------------------------+-----------------------+
| :doc:`compute contact/atom/gran <compute_contact_atom>` | particles interact with each other          | *f\_ij* > 0           |
+---------------------------------------------------------+---------------------------------------------+-----------------------+
| :doc:`compute coord/atom <compute_coord_atom>`          | particles are in the vicinity of each other | *r* < *cutoff*        |
+---------------------------------------------------------+---------------------------------------------+-----------------------+

**Output info:**

This compute calculates a per-atom vector, whose values can be
accessed by any command that uses per-atom values from a compute as
input.  See :ref:`Section\_howto 15 <howto_8>` for an
overview of Packfall output options.

The per-atom vector values will be a number >= 0.0, as explained
above.

Restrictions
""""""""""""


This compute requires that atoms store a radius as defined by the
:doc:`atom\_style sphere <atom_style>` command.

This compute might not yield correct results for multisphere simulations as
it considers all spherical particles separately.

Related commands
""""""""""""""""

:doc:`compute coord/atom <compute_coord_atom>`

**Default:** none


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
