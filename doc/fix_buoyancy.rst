.. index:: fix buoyancy

fix buoyancy command
====================

Syntax
""""""


.. parsed-literal::

   fix ID group-ID buoyancy keyword value ...

* ID, group-ID are documented in :doc:`fix <fix>` command
* buoyancy = style name of this fix command
* zero or more keyword/value pairs may be appended to args; the *dim* keyword must be used
* keyword = *density* or *dim* or *level* or *region*
  
  .. parsed-literal::
  
       *density* value = density-value
         density-value = fluid density in mass/length\^3 units
       *dim* value = x or y or z
         x, y, z = define the water level as x=const, y=const, z=const plane
       *level* value = lev
         lev = water level in length units, along the axis defined by dim
       *region* value = region-ID
         region-ID = ID of region atoms must be in to have added buoyancy force



Examples
""""""""


.. parsed-literal::

   fix bu all buoyancy level 0.06 dim z density 1000

Description
"""""""""""

Add a buoyancy force for each atom in the group. The water level is
assumed to be a x=const, y=const or z=const plane, where the axis
is defined via the *dim* keyword and const is defined via the *level*
keyword. E.g. *dim* = x and *level* = 0.1 would define the water level
as x = 0.1.

The buoyancy force is equivalent to the weight of the displaced fluid.
Thus, if the particle is fully submerged, the buoyancy force is equal
to particle volume \* fluid density. If the particle is not submerged,
the buoyancy force is 0. If the particle is partially submerged, the
fix will calculate the force based on the submerged particle volume.
The fluid density is defined by keyword *density*\ .

As buoyancy is triggered by static pressure difference in a fluid usually
caused by gravity, this fix requires to use a :doc:`fix gravity <fix_gravity>`.
The gravity vector has to be specified in the same axis as in this fix
(via the *dim* keyword).

If the *region* keyword is used, the atom must also be in the
specified geometric :doc:`region <region>` in order to have buoyancy force
added to it.


----------


**Restart, fix\_modify, output, run start/stop, minimize info:**

No information about this fix is written to :doc:`binary restart files <restart>`. No :doc:`fix\_modify <fix_modify>` option applies
to this fix. This fix computes a a global 3-vector of the total
buoyancy force, which can be accessed by various :ref:`output commands <howto_8>`.
No parameter of this fix can be used with the *start/stop* keywords of
the :doc:`run <run>` command.

The forces due to this fix are imposed during an energy minimization,
invoked by the :doc:`minimize <minimize>` command.

Restrictions
""""""""""""


This fix requires to use a :doc:`fix gravity <fix_gravity>`. The gravity
vector has to be specified in the same axis as in this fix (via the
*dim* keyword).

Related commands
""""""""""""""""

:doc:`fix setforce <fix_setforce>`, :doc:`fix aveforce <fix_aveforce>`,
:doc:`fix addforce <fix_addforce>`

Default
"""""""

*level* = 0, *density* = 0


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
