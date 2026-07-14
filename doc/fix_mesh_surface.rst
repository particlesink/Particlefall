.. index:: fix mesh/surface

fix mesh/surface command
========================

fix mesh/surface/planar command
===============================

Syntax
""""""


.. parsed-literal::

   fix ID group-ID mesh/surface data_keyword data_value premesh_keywords premesh_values mesh_keywords mesh_values surface_keywords surface_values
   fix ID group-ID mesh/surface/module_list data_keyword data_value premesh_keywords premesh_values mesh_keywords mesh_values surface_keywords surface_values module_keywords module_values
   fix ID group-ID mesh/surface/planar data_keyword data_value premesh_keywords premesh_values mesh_keywords mesh_values surface_keywords surface_values

* ID, is documented in :doc:`fix <fix>` command.
* mesh/surface or mesh/surface/planar = style name of this fix command
* module\_list = list of mesh modules separated by /
* one data\_keyword/data\_value pair needs to be appended
* data\_keyword = *file*\ , *generate* or *fix*
  
  .. parsed-literal::
  
       *file* value = name of STL or VTK file containing the triangle mesh data
       *generate* value = mesh generator
         cube size mask
         box xsize ysize zsize mask
         cylinder radius zsize nsegments mask
         pipe inner_radius outer_radius zsize nsegments mask
         disk radius nsegments
         plane size
       *fix* value = id of a fix that can generate meshes (e.g. this one using the *extrude_planar* keyword)

* zero or more premesh\_keywords/premesh\_value pairs may be appended
* premesh\_keyword = *type* or *precision* or *heal* or *min\_feature\_length*  or *element\_exclusion\_list* or *verbose*
  
  .. parsed-literal::
  
       *type* value = atom type (material type) of the wall imported from the STL file
       *region* value = ID of :doc:`region <region>` to filter elements which are imported
       *precision* value = length mesh nodes this far away at maximum will be recognized as identical (length units)
       *heal* value = auto_remove_duplicates or no
       *min_feature_length* value = exclude element if belongs to an entity where no element is larger than this (length units)
       *element_exclusion_list* values = mode element_exlusion_file
         mode = read or write
         element_exlusion_file = name of file containing the elements to be excluded
       *verbose* value = yes or no

* zero or more mesh\_keywords/mesh\_value pairs may be appended
  
  .. parsed-literal::
  
     mesh_keyword = *scale* or *move* or *rotate* or *temperature* or *mass_temperature* or *extrude_planar*:l
       *scale* value = factor
         factor = factor to scale the mesh in x-, y-, and z-direction (double value)
       *move* values = mx my mz
         mx my mz = move the mesh by this extent in x-, y-, and z-direction (length units)
       *rotate* values = axis ax ay az angle ang
         axis = obligatory keyword
         ax, ay, az = axis vector for rotation (length units)
         angle = obligatory keyword
         ang = rotation angle (degrees)
       *temperature* value = T0
         T0 = Temperature of the wall (temperature units)
       *mass_temperature* value = mt0
         mt0 = mass (mass units) assumed for temperature update in the frame of :doc:`surface sphere/heatable <gran_surface_sphere_heatable>`
       *extrude_planar* value = length
         length = length (length units) that a planar mesh is extruded in anti-normal direction

* zero or more surface\_keywords/surface\_value pairs may be appended
* surface\_keyword = *surface\_vel* or *surface\_ang\_vel* or *curvature* or *curvature\_tolerant*
  
  .. parsed-literal::
  
       *surface_vel* values = vx vy vz
         vx vy vz = conveyor belt surface velocity  (velocity units)
       *surface_ang_vel* values = origin ox oy oz axis ax ay az omega om
         origin = mandatory keyword
         ox oy oz = origin of rotation (length units)
         axis = mandatory keyword
         ax ay az = axis vector for rotation (length units)
         omega = mandatory keyword
         om = rotational velocity around specifyied axis (rad/time units)
       *curvature* value = c
         c = maximum angle between mesh faces belonging to the same surface (in degree) 
       *curvature_tolerant* value = ct
         ct = yes or no

* zero or more module\_keywords/module\_value pairs may be appended if a mesh module is used
  
  .. parsed-literal::
  
       see the respective mesh module parameter list for details



Examples
""""""""


.. parsed-literal::

   fix cad all mesh/surface file mesh.stl type 1
   fix cad1 all mesh/surface/stress/deform file meshes/plate.stl type 1 scale 1.0 wear finnie
   fix extrude all mesh/surface file plane.stl type 1 extrude_planar 0.1
   fix side_walls all mesh/surface fix extrude type 1
   fix cad all mesh/surface file mesh.stl type 1
   fix cubeMesh all mesh/surface/stress generate cube 0.1 63 type 1
   fix pipeMesh all mesh/surface/stress generate pipe 0.05 0.06 0.1 10 58 type 1

Description
"""""""""""

This fix allows the import of triangular surface mesh wall geometry for granular simulations from
ASCII STL files or legacy ASCII VTK files. Style *mesh/surface* is a general surface mesh, and
*mesh/surface/planar* represents a planar mesh. *mesh/surface/planar* requires the mesh to
consist of only 1 planar face.

The *region* keyword can be used to filter the elements which are imported. An element is only
imported if all of its vertices is inside the specified :doc:`region <region>`. Please note that 
the geometric extent of this :doc:`region <region>` refer to the unscaled, unmoved, and unrotated
original state of the geometry.

In general, you can apply scaling, offset and rotation to the imported mesh via
keywords *scale*\ , *move*\ , *rotate*\ . Operations are applied in the order as they
are specified. The rotation via *rotate* is performed around the rotation axis
that goes through the origin (0,0,0) and in the direction of *axis*\ .

If a simulation box dimension is periodic, a surface mesh may extend partly
outside the corresponding box limits. During mesh ownership assignment and
parallel communication, Packfall remaps such geometry through the periodic
boundary so that walls that straddle a periodic seam can be used directly.
This is primarily intended for *mesh/surface* walls used together with
:doc:`fix wall/gran <fix_wall_gran>`.

The group-ID defines which particles will "see" the mesh, in case it is used as a granular wall.

One fix represents one wall with a specific material, where the material is identified
via keyword *type*\ . If multiple meshes with different materials are desired, the respective
meshes must be imported with different fix mesh/surface commands.

With the *temperature* keyword, you can define a constant temperature for a mesh in
conjunction with heat conduction via :doc:`fix heat/gran <fix_heat_gran>`. Note that the
actual calculation of the heat transfer happens only if you use the mesh in conjunction
with a granular wall, see :doc:`fix wall/gran <fix_wall_gran>`.

The *extrude\_planar* option allows to extrude a planar mesh in direction of the
anti-normal by a specified length values. After a *run 0* command this fix will
hold the generated triangles which can be used in another *mesh/surface* fix
that uses the *fix* instead of *file* keyword.

With the optional *surface\_vel* keyword, you can specify the imported mesh as conveyor belt.
The velocity direction for each mesh face is given by the projection of the conveyor
belt velocity parallel to the mesh face, the velocity magnitude for each mesh face is
equal to the conveyor belt velocity. This ensures the model makes sense also in case
the mesh is curved. Likewise, the optional rotation model activated via keyword
*surface\_ang\_vel* mimics rotational motion of the mesh (e.g. for modeling a shear cell)

The *generate* keyword always generates axis-aligned meshes where the center of mass is at (0, 0, 0).
The *plane* and the *disk* meshes lie on the XY plane.
The mask value allows selected sides to be included in the mesh. It is a bitmask, meaning the bit values must
be bitwise or-ed together, which can be an addition in this case.
The mask bits for the different types:

cube & box:


.. parsed-literal::

     1 - TOP ( z+ )
     2 - BOTTOM ( z- )
     4 - FRONT ( y- )
     8 - BACK ( y+ )
     16 - LEFT ( x- )
     32 - RIGHT ( x+ )

cylinder:


.. parsed-literal::

     1 - TOP ( z+ )
     2 - BOTTOM ( z- )
     4 - SIDE

pipe:


.. parsed-literal::

     1 - INNER_TOP ( inner_radius, z+ )
     2 - OUTER_TOP ( outer_radius, z+ )
     4 - INNER_BOTTOM ( inner_radius, z- )
     8 - OUTER_BOTTOM ( outer_radius, z- )
     16 - INNER_SIDE ( inner_radius )
     32 - OUTER_SIDE ( outer_radius )

For example: A full cube has a mask of 63, an opened pipe 58.

**Mesh modules:**

The "mesh/surface" fix allows for several mesh modules to be appended. A
complete list of the available mesh modules can be found in :doc:`Section mesh modules <Section_mesh_modules>`.

Note that some of these modules are dependent on each other and some are mutually exclusive.
The order of the mesh modules is important and the arguments for each mesh module need to
be appended in the same order as the modules.

.. warning::

   When using the deform, heattransfer or liquidtransfer module and
   periodic boundary conditions the mesh needs to fulfill the following properties:
   (i) a triangle is not allowed to be in contact with itself through periodic
   boundaries,
   (ii) if two triangles are in contact inside the domain, they are not
   allowed to be in contact through periodic boundaries.
   Note that these restrictions are not validated and must be
   ensured by the user.

.. warning::

   Periodic overhang support for meshes is currently a first-pass feature.
   Geometry may extend past a periodic box face, but the mesh should still be
   defined as a single consistent wall and should not rely on intentional
   self-contact through the periodic seam. If a mesh is exactly split across a
   periodic seam, topology-sensitive operations may still be more robust if the
   mesh is pre-shifted into a single preferred image before import.

**Quality checks / error and warning messages:**

The code checks a couple of quality criteria upon loading a mesh. It tries
to give you as much information about the issue as possible.

Warning messages:

* There should be no angle < 0.5 degrees in any element (soft limit for angles)
* All nodes should be within the simulation box

If any of the above rules is not fulfilled, a warning is generated. Keyword
*verbose* controls if details about the warning are written to the screen.

For periodic dimensions, nodes that extend beyond a periodic face are accepted
and remapped for mesh ownership and communication. In non-periodic dimensions,
mesh elements are still required to remain inside the simulation domain.

Error messages:

* The *curvature* must not be larger than any angle in any mesh element
* There must be no element with an angle <  0.0181185 degrees in any element (hard limit for angles)
* The number of neighbor elements must be <= 5 for any element
* Mesh elements must not be duplicate
* Coplanar mesh elements that share an edge must not overlap
* Mesh elements must not leave the simulation domain (based on the center of the element)

If any of the obove rules is not fulfilled, an error is generated and
the code halts. Error messages are always *verbose*\ .
If the code halts due to the last error, you might think about (a)
changing the mesh import parameters (\ *scale*\ , *move*\ , *rotate*\ ), (b)
changing the mesh dynamics if a :doc:`fix move/mesh <fix_move_mesh>` is
applied or using :doc:`boundary m m m <boundary>`

There are a couple of features that help you read / repair a mesh which 
can not be read correctly in the first place:

* *precision*
* *curvature*
* *heal*
* *element\_exclusion\_list*
* *min\_feature\_length*
* *curvature\_tolerant*

The *precision* keyword specifies how far away mesh nodes can be at maximum to
be recognized as identical. This is important for building the topology of the mesh
and identify elements as neighbors. Normally, this should only be changed if the mesh file
you are working with is suffering from precsion / round-off issues.

The *curvature* keyword lets you specify up to which angle between two triangles the
triangles should be treated as belonging to the same surface (e.g. useful for bends).
This angle is used to decide if (a) contact history is copied from one triangle to
the other as the contact point proceeds and (b) if edge and corner interaction is
calculated.

If the code stalls because of duplicate elements, you can try setting
*heal* to auto\_remove\_duplicates. The code will then try to heal the geometry
by removing duplicate elements.

With the optional *element\_exclusion\_list* keyword is used in mode 'write', the code 
will write a list of elements which

* have more than the allowed 5 face neighbors per surface
* have an angle below 0.0181185 degrees
* are duplicate

element to a file. The 'read' mode can then use this file and will skip the elements in 
the list. However, you can also manually write such a file to exclude elements you do not 
want to have included

.. warning::

   The *element\_exclusion\_list write* model works in serial only. However,
   this is not a real restriction, since you can generate the exclusion lists in serial,
   but then read them to import the mesh into a parallel simulation

.. warning::

   If you use the 'heal' or 'element\_exclusion\_list' keywords, you should 
   check the changes to the geometry, e.g. by using a :doc:`dump mesh/stl <dump>` command.

With the *min\_feature\_length* option, you can exlude elements which do not belong
to an entity which has any element larger than *min\_feature\_length*. In this context,
'entity' is defined by the 'neighborhood' of the element.
*min\_feature\_length* also writes to the exclusion list, and can thus only be used along 
with the 'element\_exclusion\_list' feature.

The *curvature\_tolerant* keyword can simply turn off the check that
the *curvature* must not be larger than any angle in any mesh element.
This is typically not recommended, but can be used as a last resort measure.

**Restart, fix\_modify, output, run start/stop, minimize info:**

This fix writes the STL data to binary :doc:`restart files <restart>` to be able to
correctly resume the simulation in case the mesh is moved. None of the
:doc:`fix\_modify <fix_modify>` options are relevant to this fix directly. However,
some mesh modules do have fix\_modify options, please refer to the respective module
for further information.

No global scalar or vector or per-atom quantities are stored by this fix for access by
various :ref:`output commands <4_15>`. No parameter of this fix
can be used with the *start/stop* keywords of the :doc:`run <run>` command.
This fix is not invoked during :doc:`energy minimization <minimize>`.

Restrictions
""""""""""""


The code reads ASCII/binary STL and ASCII VTK files.
In the current implementation, each processor allocates memory for the whole
geometry, which may lead to memory issues for very large geometries .
It is not supported to use both the moving mesh and the conveyor belt feature.

Related commands
""""""""""""""""

:doc:`fix wall/gran <fix_wall_gran>`

**Default:** curvature = 0.256235 degrees, precision = 1e-8, verbose = no, heal = no


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
