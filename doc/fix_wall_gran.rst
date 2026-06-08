.. index:: fix wall/gran 

fix wall/gran  command
======================

Syntax
""""""


.. parsed-literal::

   fix ID group-ID style model_type model_name wallstyle wallstyleargs general_keywords general_values model_keywords model_values  ...

* ID, group-ID are documented in :doc:`fix <fix>` command
* style = *wall/gran*
* zero or more model\_type/model\_name pairs may be appended. They must be appended in the following order
  
  .. parsed-literal::
  
       *model* values = described :doc:`here <Section_gran_models>`
       *tangential* values = described :doc:`here <Section_gran_models>`
       *cohesion* values = described :doc:`here <Section_gran_models>`
       *rolling_friction* values = described :doc:`here <Section_gran_models>`
       *surface* values = described :doc:`here <Section_gran_models>`

* wallstyle = *mesh* or *primitive*
* wallstyle args for wallstyle *mesh* = *n\_meshes* and *meshes*
  
  .. parsed-literal::
  
       *n_meshes* value = nm
         nm = # of meshes (see :doc:`fix mesh/surface <fix_mesh_surface>`) to use for the wall (positive integer)
       *meshes* values = meshlist
         meshlist =  id(s) of the mesh(es) (see :doc:`fix mesh/surface <fix_mesh_surface>`) to be used. These must be defined before

* wallstyle args for wallstyle *primitive* = *type* or *xplane* or *yplane* or *zplane* or *xcylinder* or *ycylinder* or *zcylinder*
  
  .. parsed-literal::
  
       *type* args = tp
         tp = atom_type (material type) of the wall
       *xplane* or *yplane* or *zplane* args = pos
         pos = position plane (distance units)
       *xcylinder* or *ycylinder* or *zcylinder* args = radius c1 c2
         radius = cylinder radius (distance units)
         c1,c2 = coordinates of center axis in other 2 dims (distance units)

* zero or more general\_keyword/value pairs may be appended
* general\_keyword =  *shear* or *store\_force* or *store\_force\_contact* or *store\_force\_contact\_stress*
  
  .. parsed-literal::
  
       *shear* values = dim vshear
         dim = *x* or *y* or *z*
         vshear = magnitude of shear velocity (velocity units)
       *temperature* value = T0
         T0 = Temperature of the wall (temperature units)
       *contact_area* values = 'overlap' or 'constant [value]' or 'projection'
       *store_force* value = 'yes' or 'no'
         yes, no = determines if the wall force exerted on the particles is stored in a :doc:`fix property/atom <fix_property>` with id force_(ID), where (ID) is the id of the fix wall/gran command.
       *store_force_contact* value = 'yes' or 'no'
         yes, no = determines if the force for each particle-wall contact is stored in a :doc:`fix property/atom <fix_property>` with id contactforces_(ID), where (ID) is the id of the fix wall/gran command.
       *store_force_contact_stress* value = 'yes' or 'no'
         yes, no = determines if the force and contact point for each particle-wall contact is stored in a :doc:`fix property/atom <fix_property>` with id contactforces_stress_(ID), where (ID) is the id of the fix wall/gran command.

* following the general\_keyword/value pairs, zero or more model\_keyword/model\_value pairs may be appended in arbitrary order
  model\_keyword/model\_value pairs = described for each model separately :doc:`here <Section_gran_models>`

Examples
""""""""


.. parsed-literal::

   fix zwalls all wall/gran model hertz tangential history primitive type 1 zplane 0.15
   fix zwalls all wall/gran model hertz tangential history surface superquadric primitive type 1 zplane 0.15 
   fix meshwalls all wall/gran model hertz tangential history mesh n_meshes 2 meshes cad1 cad2

Description
"""""""""""

Bound the simulation domain of a granular system with a frictional wall.
All particles in the group interact with the wall when they are close enough
to touch it. The equation for the force between the wall and particles
touching it is the same as the corresponding equation on the
:doc:`pair\_style granular <pair_gran>` doc page, in the limit of one of the
 two particles going to infinite radius and mass (flat wall).

You must choose the models matching the pair style used, otherwise
an error is created. As with :doc:`pair\_style granular <pair_gran>`, you have
to define the mechanical properties for each material you are using in the
simulation with fix property commands. See :doc:`pair\_style gran <pair_gran>`
for more details and :doc:`the model doc page <Section_gran_models>` for
details.

For wallstyle *mesh*\ , fix\_id1, fix\_id2 etc. must be IDs of valid fixes of
type :doc:`fix mesh/surface <fix_mesh_surface>`, defining the granular mesh
to be used for the wall. Triangle-particle neighbor lists are built to efficiently
track particle-triangle contacts. Particle-tri neighbor list build is triggered
if any particle moves more than half the skin distance or (in case of moving mesh) if the mesh
itself moves more than half the skin distance since the last build. A warning
is generated if a dangerous particle-tri neigh list build is detected
(e.g. if particles are inserted too close to a wall, see section 'Restrictions').
For style *mesh*\ , the atom\_type (material type) is inherited from the atom style provided
in the :doc:`fix mesh/surface <fix_mesh_surface>` command.

For *wallstyle primitive*\ , the atom\_type (material type) has to be provided via
keyword *type*\ . Primitve walls can be *xplane* or *yplane* or *zplane* or *cylindrical*\ .
The 3 planar options specify a single wall in a dimension.
Wall positions are given by values for lo and hi. For an *xcylinder*\ , *ycylinder* or
*zcylinder*\ , the radius and the cylinder axis in the other two dims is specified.


----------


Optionally, primitive walls can be moving, if the shear keyword is appended.

For the *shear* keyword, the wall moves continuously in the specified dimension
with velocity vshear. The dimension must be tangential to walls with a planar
wallstyle, e.g. in the y or z directions for an *xplane* wall. For *zcylinder*
walls, a dimension of z means the cylinder is moving in the z-direction along
it's axis. A dimension of x or y means the cylinder is spinning around the
z-axis, either in the clockwise direction for vshear > 0 or counter-clockwise
for vshear < 0. In this case, vshear is the tangential velocity of the wall
at whatever radius has been defined. The same applies to *xcylinder* and
*ycylinder* accordingly.

.. note::

   The keywords wiggle or shear can NOT be used for *wallstyle mesh*\ .
   For a moving a granular wall with *wallstyle mesh*\ , use the more flexible
   command :doc:`fix move/mesh <fix_move_mesh>`, or use the keywords *velocity*
   or *angular\_velocity* in :doc:`fix mesh/surface <fix_mesh_surface>`.

The keyword *temperature* is used to assign a constant temperature to the wall.
This setting gets effective in conjunction with heat conduction via
:doc:`fix heat/gran/conduction <fix_heat_gran_conduction>`. For *wallstyle mesh*\ , the value
for the temperature given in this command is ignored and the temperature
value is specified per mesh via :doc:`fix mesh/surface <fix_mesh_surface>`.
Contact area calculation can be controlled by keyword *contact\_area* in the
same manner as for :doc:`fix heat/gran/conduction <fix_heat_gran_conduction>`.

By specifying *store\_force* = 'yes', you can instruct the command to
store the wall force exerted on the particles in a
:doc:`fix property/atom <fix_property>` with id force\_(ID), where (ID)
is the id of the fix wall/gran command.

*store\_force\_contact* does the same for contact forces between particle and wall.

*store\_force\_contact\_stress* does the same for contact forces and contact point 
between particle and wall. In general this keyword will be set automatically if a 
:doc:`continuum/weighted <fix_continuum_weighted>` fix is used. It needs to be set 
explicitly only if a new wall is created after the such a fix is specified.
This functionality may not be available in your version of LIGGGHTS.

The effect of keyword *rolling\_friction*, *cohesion*\ , *tangential\_damping*,
*viscous* and *absolute\_damping* is explanted in :doc:`pair gran <pair_gran>`

**Restart, fix\_modify, output, run start/stop, minimize info:**

If applicable, contact history is written to :doc:`binary restart files <restart>`
so simulations can continue properly.
None of the :doc:`fix\_modify <fix_modify>` options are relevant to this fix.
No global scalar or vector or are stored by this fix.
If *store\_force* = 'yes' is specified, the per-atom wall force can be
accessed by the various output commands via f\_force(ID)\ **1**\ , f\_force(ID)\ **2**\ ,
f\_force(ID)\ **3**\ . (ID) is the id of the fix wall/gran command.
No parameter of this fix can be used with the start/stop keywords of the
:doc:`run <run>` command. This fix is not invoked during :doc:`energy minimization <minimize>` .

Restrictions
""""""""""""


There can be only one fix wall/gran command with style *mesh*\ . Note that
this is not really a restriction because you can include multiple fixes
of type :doc:`fix mesh/surface <fix_mesh_surface>` in the fix wall/gran command.

*store\_force\_contact* is available in selected versions only; it is not available
in the PUBLIC version.

When using style style *mesh*\ , you have to use the style *bin* for
the :doc:`neighbor command <neighbor>`.

Style *mesh* can not be used in conjunction with triclinic simulation boxes.

When using style *mesh*\ ' in combination with a particle insertion
command, you always have to keep a minimum distance between the wall and the
insertion region that is equal to maximum particle radius + half the skin
defined in the :doc:`neighbor command <neighbor>`. Otherwise, newly inserted
particles interpenetrate the walls before a triangle neighbor list is built
the first time.

The keyword shear can NOT be used for style *mesh*\ .
For moving granular wall with style mesh, use :doc:`fix move/mesh <fix_move_mesh>`.

Any dimension (xyz) that has a planar granular wall must be non-periodic.

Related commands
""""""""""""""""

:doc:`fix mesh/surface <fix_mesh_surface>`, :doc:`fix\_move\_mesh <fix_move_mesh>`, :doc:`pair\_style granular <pair_gran>`
Models for use with this command are described :doc:`here <Section_gran_models>`

Default
"""""""

*model* = 'hertz'
*tangential* = 'history'
*rolling\_friction* = 'off'
*cohesion* = 'off'
*surface* = 'default'


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
