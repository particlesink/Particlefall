.. index:: atom\_style

atom\_style command
===================

Syntax
""""""


.. parsed-literal::

   atom_style style args

* style = *bond* or *charge* or         *ellipsoid* or *full* or *line* or 	*molecular* or *tri* or *hybrid* or         *sphere* or *granular* or *bond/gran* or         *superquadric* or *convexhull* or *sph*


.. parsed-literal::

     args = none for any style except *body* and *hybrid*
     *body* args = bstyle bstyle-args
       bstyle = style of body particles
       bstyle-args = additional arguments specific to the bstyle
                     see the :doc:`body <body>` doc page for details
     *hybrid* args = list of one or more sub-styles, each with their args

Examples
""""""""


.. parsed-literal::

   atom_style bond
   atom_style sphere
   atom_style superquadric
   atom_style hybrid sphere bond

Description
"""""""""""

Define what style of atoms to use in a simulation.  This determines
what attributes are associated with the atoms.  This command must be
used before a simulation is setup via a :doc:`read\_data <read_data>`,
:doc:`read\_restart <read_restart>`, or :doc:`create\_box <create_box>`
command.

Once a style is assigned, it cannot be changed, so use a style general
enough to encompass all attributes.  E.g. with style *bond*\ , angular
terms cannot be used or added later to the model.  It is OK to use a
style more general than needed, though it may be slightly inefficient.

The choice of style affects what quantities are stored by each atom,
what quantities are communicated between processors to enable forces
to be computed, and what quantities are listed in the data file read
by the :doc:`read\_data <read_data>` command.

These are the additional attributes of each style and the typical
kinds of physical systems they are used to model.  All styles store
coordinates, velocities, atom IDs and types.  See the
:doc:`read\_data <read_data>`, :doc:`create\_atoms <create_atoms>`, and
:doc:`set <set>` commands for info on how to set these various
quantities.

+----------------------+----------------------------------------------------------------------+----------------------------+
| *bond*               | bonds                                                                | bead-spring polymers       |
+----------------------+----------------------------------------------------------------------+----------------------------+
| *bond/gran*          | number of bonds and bond information                                 | granular bond models       |
+----------------------+----------------------------------------------------------------------+----------------------------+
| *charge*             | charge                                                               | atomic system with charges |
+----------------------+----------------------------------------------------------------------+----------------------------+
| *convexhull*         | mass, angular velocity, quaternion                                   | granular models            |
+----------------------+----------------------------------------------------------------------+----------------------------+
| *ellipsoid*          | shape, quaternion, angular momentum                                  | aspherical particles       |
+----------------------+----------------------------------------------------------------------+----------------------------+
| *line*               | end points, angular velocity                                         | rigid bodies               |
+----------------------+----------------------------------------------------------------------+----------------------------+
| *molecular*          | bonds, angles, dihedrals, impropers                                  | uncharged molecules        |
+----------------------+----------------------------------------------------------------------+----------------------------+
| *sph*                | q(pressure), density                                                 | SPH particles              |
+----------------------+----------------------------------------------------------------------+----------------------------+
| *sphere or granular* | diameter, mass, angular velocity                                     | granular models            |
+----------------------+----------------------------------------------------------------------+----------------------------+
| *superquadric*       | semi-axes, blockiness parameters, mass, angular velocity, quaternion | granular models            |
+----------------------+----------------------------------------------------------------------+----------------------------+
| *tri*                | corner points, angular momentum                                      | rigid bodies  AWPMD        |
+----------------------+----------------------------------------------------------------------+----------------------------+

.. warning::

   It is possible to add some attributes, such as a
   molecule ID, to atom styles that do not have them via the :doc:`fix property/atom <fix_property_atom>` command.  This command also
   allows new custom attributes consisting of extra integer or
   floating-point values to be added to atoms.  See the :doc:`fix property/atom <fix_property_atom>` doc page for examples of cases
   where this is useful and details on how to initialize, access, and
   output the custom values.

All of the styles assign mass to particles on a per-type basis, using
the :doc:`mass <mass>` command, except for *sphere or granular*
styles.  They assign mass to individual particles on a per-particle
basis.

For the *sphere* style, the particles are spheres and each stores a
per-particle diameter and mass.  If the diameter > 0.0, the particle
is a finite-size sphere.  If the diameter = 0.0, it is a point
particle. This is typically used for granular models. Instead of
*sphere*\ , keyword *granular* can be used.

For the *bond/gran* style, the number of granular bonds per atom is
stored, and the information associated to it: the type of each bond,
the ID of the bonded partner atom and the so-called bond history.
The bond history is similar to the contact history for granular
interaction, it stores the internal state of the bond. What exactly
is stored in this internal state is defined by the granular
:doc:`bond style <bond_gran>` used. There are 2 parameters: The number
of bond types, and the maximum number of bonds that each atom can
have. For each bond type, the parameters have to be specified via the
:doc:`bond\_coeff <bond_coeff>` command (see example :doc:`here <bond_gran>` )
Note that *bond/gran* is an experimental code which is may not be
available in your release of LIGGGHTS. An example for the sytnax is given below:


.. parsed-literal::

   atom_style bond/gran n_bondtypes 1 bonds_per_atom 6

For the *ellipsoid* style, the particles are ellipsoids and each
stores a flag which indicates whether it is a finite-size ellipsoid or
a point particle.  If it is an ellipsoid, it also stores a shape
vector with the 3 diamters of the ellipsoid and a quaternion 4-vector
with its orientation.

For the *line* style, the particles are idealized line segments and
each stores a per-particle mass and length and orientation (i.e. the
end points of the line segment).

For the *tri* style, the particles are planar triangles and each
stores a per-particle mass and size and orientation (i.e. the corner
points of the triangle).


----------


Typically, simulations require only a single (non-hybrid) atom style.
If some atoms in the simulation do not have all the properties defined
by a particular style, use the simplest style that defines all the
needed properties by any atom.  For example, if some atoms in a
simulation are charged, but others are not, use the *charge* style.
If some atoms have bonds, but others do not, use the *bond* style.

The only scenario where the *hybrid* style is needed is if there is no
single style which defines all needed properties of all atoms.  For
example, if you want dipolar particles which will rotate due to
torque, you would need to use "atom\_style hybrid sphere dipole".  When
a hybrid style is used, atoms store and communicate the union of all
quantities implied by the individual styles.

LIGGGHTS(R)-PUBLIC can be extended with new atom styles as well as new body
styles; see :doc:`this section <Section_modify>`.

Restrictions
""""""""""""


This command cannot be used after the simulation box is defined by a
:doc:`read\_data <read_data>` or :doc:`create\_box <create_box>` command.

The *bond*\ , *molecular* styles are part of the
MOLECULAR package. 

Related commands
""""""""""""""""

:doc:`read\_data <read_data>`, :doc:`pair\_style <pair_style>`

**Default:** none


.. _ld: Manual.html
.. _lc: Section_commands.html#comm
