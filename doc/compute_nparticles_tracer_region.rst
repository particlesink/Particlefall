.. index:: compute nparticles/tracer/region

compute nparticles/tracer/region command
========================================

Syntax
""""""


.. parsed-literal::

   compute ID group-ID nparticles/tracer/region general_keyword general_values

* ID, group-ID are documented in :doc:`compute <compute>` command
* nparticles/tracer/region = style name of this compute command
* general\_keywords general\_values are documented in `compute <compute.html">`_
* region\_count = obligatory keyword
* region-ID = ID of region atoms must be in to be counted
* tracer = obligatory keyword
* tracer-ID = ID of a fix of type :doc:`fix property/atom/tracer <fix_property_atom_tracer>`
* zero or more keyword/value pairs may be appended to args
* keyword = *periodic* or *reset\_marker*

.. parsed-literal::

     *periodic* value = dim image
       dim = *x* or *y* or *z*
       image = image that a particle has to be in to be counted (any integer number or *all*\ )
     *reset_marker* value = *yes* or *no*
       yes = un-mark particles after counting them
       no = do not un-mark particles after counting them



Examples
""""""""


.. parsed-literal::

   compute nparticles all nparticles/tracer/region region_count count tracer tr periodic z -1

Description
"""""""""""

Define a computation that calculates the number and mass of marked and un-marked particles
that are in the region speficied via the *region\_count* keyword. Particles have
to be in the group "group-ID" to be counted.

Note that only particles marked by a :doc:`fix property/atom/tracer <fix_property_atom_tracer>`
or :doc:`fix property/atom/tracer/stream <fix_property_atom_tracer_stream>`
command are counted - therefore, a valid ID of such a fix has to be
provided via the *tracer* keyword.

The *reset\_marker* keyword controls if particles are un-marked (default)
after they have been counted once by this command.

.. warning::

   If multiple compute nparticles/tracer/region
   commands are operating on the same :doc:`fix property/atom/tracer <fix_property_atom_tracer>`
   commands, and the first compute resets the marker value, the second
   compute will not count them.

With the *periodic* keyword, you can restrict counting/unmarking to
particles which are in a specified image in a periodic simulation.
For example, using


.. parsed-literal::

   periodic z +2

means that particles are only counted if they are in z-image #2.
By default, all particles are counted/unmarked regardless in which
periodic image they are.

.. warning::

   Currently, this command only supports one periodic
   boundary restriction via the *periodic* keyword. If keyword *periodic*
   is used multiple times, the last setting will be applied.

**Output info:**

This this compute calculates a global vector containing the following information
(the number in brackets corresponds to the vector id):

* **(1)** total number of (marked + un-marked) particles in region
* **(2)** number of marked particles in region
* **(3)** total mass of (marked + un-marked) particles in region
* **(4)** mass of marked particles in region
See :ref:`this section <howto_8>` for an overview of LIGGGHTS(R)-PUBLIC output
options.

Restrictions
""""""""""""


Currently, only one periodic restriction via the *periodic* keyword
can be used.

Related commands
""""""""""""""""

:doc:`fix property/atom/tracer <fix_property_atom_tracer>`

Default
"""""""
*reset\_marker* = yes, *periodic* is off per default


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
