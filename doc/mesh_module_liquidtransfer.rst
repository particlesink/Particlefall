mesh module liquidtransfer
==========================

Syntax
""""""


.. parsed-literal::

   liquidtransfer liquidtransfer_keywords liquidtransfer_values

* two liquidtransfer\_keyword/value pairs need to be appended
* heattransfer\_keyword = *wall\_thickness* or *initial\_liquid\_content*
  
  .. parsed-literal::
  
       *wall_thickness* value = wall_thickness (in length units)
         wall_thickness = virtual wall thickness
       *initial_liquid_content* value = LC_initial
         LC_initial = initial liquid content (in vol%)



Examples
""""""""

fix plate all mesh/surface/liquidtransfer file meshes/plate.stl type 1 scale 1.0 wall\_thickness 0.01 initial\_liquid\_content 0.05

Description
"""""""""""

This module allows the use of a liquid bridge cohesion model that exchanges liquid with the wall.
Additionally, it also allows the use of the :doc:`fix\_addliquid\_wall <fix_addliquid_wall>`
which acts as an external liquid source for a certain part of the mesh.

The initial liquid content is given by the *initial\_liquid\_content* parameter and is then
evolved when bridges break. The *wall\_thickness* parameter asigns a virtual thickness to
the wall and is responsible for the volume of liquid a wall can store. The other
parameters for the cohesion model are taken from the global definitions.

.. warning::

   When using this module and periodic boundary conditions the mesh
   needs to fulfill the following properties:
   (i) a triangle is not allowed to be in contact with itself through periodic
   boundaries,
   (ii) if two triangles are in contact inside the domain, they are not
   allowed to be in contact through periodic boundaries.
   Note that these restrictions are not validated by Packfall and must be
   ensured by the user.

**Restart, fix\_modify, output, run start/stop, minimize info:**

This module writes information to :doc:`binary restart files <restart>` to be
able to continue correctly after restart.  None of the :doc:`fix\_modify <fix_modify>` 
options are relevant to this module.

This mesh module stores per-mesh-element properties ("liquid": liquid vol%)
which can be accessed via :doc:`dump mesh/vtk <dump_mesh_vtk>`

No parameter of this mesh module can be used with the *start/stop* keywords of
the :doc:`run <run>` command.  This mesh module is not invoked during :doc:`energy minimization <minimize>`.

Restrictions
""""""""""""


None.

Related commands
""""""""""""""""

:doc:`fix mesh/surface <fix_mesh_surface>`
:doc:`fix wall/gran <fix_wall_gran>`
:doc:`fix mesh/surface/heattransfer <fix_mesh_surface_heattransfer>`

Default
"""""""

none


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
