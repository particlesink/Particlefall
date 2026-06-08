LIGGGHTS(R)-PUBLIC 2.X vs. LIGGGHTS(R)-PUBLIC - syntax changes
##############################################################

Introduction:
*************

This is a short outline of the most important changes in LIGGGHTS(R)-PUBLIC 2.X
compared to LIGGGHTS(R)-PUBLIC 1.5.3 regarding the syntax of major commands. The
motivation for these changes was to make the script language more readable and to
improve extendability (in the sense of object oriented programming) with respect
to modelling approaches.

Commands covered by this tutorial:
**********************************

* atom\_style sphere (formerly atom\_style granular)
* dump mesh/stl (formerly dump stl)
* dump mesh/vtk (formerly dump mesh/gran/VTK)
* fix heat/gran
* fix mesh/surface (formerly fix mesh/gran)
* fix mesh/surface/stress (formerly fix mesh/gran/stressanalysis)
* fix move/mesh (formerly fix move/mesh/gran)
* fix wall/gran/\*
* pair\_style gran/\*


Changes in syntax for each command:
***********************************

Changes are indicated as follows


.. parsed-literal::

   OLD: old_syntax
   NEW: new_syntax

where *old\_syntax* refers to the syntax used in LIGGGHTS(R)-PUBLIC 1.5.3 and before,
and *new\_syntax* refers to the syntax used in LIGGGHTS(R)-PUBLIC 2.X


----------


**atom\_style sphere (formerly atom\_style granular):**


.. parsed-literal::

   OLD: atom_style granular
   NEW: atom_style sphere

.. note::

   For compatibility reasons, the old syntax can still be used in LIGGGHTS(R)-PUBLIC 2.0

For details, see :doc:`atom\_style sphere <atom_style>`.


----------


**dump mesh/stl:**


.. parsed-literal::

   OLD: dmpstl all stl 300 post/dump\*.stl
   NEW: dmpstl all mesh/stl 300 post/dump\*.stl

.. note::

   For compatibility reasons, the old syntax can still be used in LIGGGHTS(R)-PUBLIC 2.0


----------


**dump mesh/vtk:**


.. parsed-literal::

   OLD: dmpstl all mesh/gran/VTK 300 post/dump\*.stl id
   NEW: dmpstl all mesh/vtk 300 post/dump\*.stl id

.. note::

   For compatibility reasons, the old syntax can still be used in LIGGGHTS(R)-PUBLIC 2.0


----------


**fix heat/gran:**


.. parsed-literal::

   OLD: fix ID group-ID heat/gran 273.15
   NEW: fix ID group-ID heat/gran initial_temperature 273.15

   OLD: fix ID group-ID heat/gran 273.15 + activate area correction via fix property/global
   NEW: fix ID group-ID heat/gran initial_temperature 273.15 area_correction yes


----------


**fix mesh/surface (formerly fix mesh/gran):**


.. parsed-literal::

   OLD: fix ID group-ID mesh/gran mesh.stl 1 1.0 0. 0. 0. 0. 0. 0.
   NEW: fix ID group-ID mesh/surface file mesh.stl type 1

   OLD: fix ID group-ID mesh/gran mesh.stl 1 0.001 0. 0. 0. -90. 0. 0.
   NEW: fix ID group-ID mesh/surface file mesh.stl type 1 scale 0.001 rotate axis 1. 0. 0. angle -90.

   OLD: fix ID group-ID mesh/gran mesh.stl 1 1.0 1. 2. 3. 0. 0. 0.
   NEW: fix ID group-ID mesh/surface file mesh.stl type 1 move 1. 2. 3

   OLD: fix ID group-ID mesh/gran mesh.stl 1 1.0 0. 0. 0. 0. 0. 0. conveyor 5. 0. 0.
   NEW: fix ID group-ID mesh/surface file mesh.stl type 1 surface_vel 5. 0. 0.

   OLD: fix ID group-ID mesh/gran mesh.stl 1 1.0 0. 0. 0. 0. 0. 0. rotate 0. 0. 0. 1. 0. 0. 5.
   NEW: fix ID group-ID mesh/surface file mesh.stl type 1 surface_ang_vel origin 0. 0. 0. axis  1. 0. 0. omega 5.

For details, see :doc:`fix mesh/surface <fix_mesh_surface>`.


----------


**fix mesh/surface/stress (formerly fix mesh/gran/stressanalysis):**


.. parsed-literal::

   OLD: fix ID group-ID mesh/gran/stressanalysis mesh.stl 1 1.0 0. 0. 0. 0. 0. 0. finnie yes
   NEW: fix ID group-ID fix mesh/surface/stress file mesh.stl type 1 wear finnie

For details, see :doc:`fix mesh/surface/stress <fix_mesh_surface_stress>`.


----------


**fix move/mesh (formerly fix move/mesh/gran):**


.. parsed-literal::

   OLD: fix ID group-ID move/mesh/gran wiggle -0.1 0. 0. 0.02 cad1 1
   NEW: fix ID group-ID move/mesh mesh cad1 wiggle amplitude -0.1 0. 0. period 0.02

   OLD: fix ID group-ID move/mesh/gran rotate 0. 0. 0. 0. 0. 1. 0.05 cad1 1
   NEW: fix ID group-ID move/mesh mesh cad1 rotate origin 0. 0. 0. axis  0. 0. 1. period 0.05

   OLD: fix ID group-ID move/mesh/gran linear 20. 20. 0. cad1 1
   NEW: fix ID group-ID move/mesh mesh cad1 linear 20. 20. 0.

NOTES:

* The trailing "1" for the *old\_syntax* was no longer used in LIGGGHTS(R)-PUBLIC 1.5.3
* For compatibility reasons, the command name *fix move/mesh/gran* can be
  used in LIGGGHTS(R)-PUBLIC 2.0. However, the syntax has to follow the LIGGGHTS(R)-PUBLIC 2.0 syntax.
For details, see :doc:`fix move/mesh <fix_move_mesh>`.


----------


**fix wall/gran/\*:**


.. parsed-literal::

   OLD: fix ID group-ID wall/gran/hertz/history 1 0 mesh/gran 2 cad1 cad2
   NEW: fix ID group-ID wall/gran/hertz/history mesh n_meshes 2 meshes cad1 cad2

   OLD: fix ID group-ID wall/gran/hertz/history 1 0 xplane -0.5 0.5 1
   NEW: fix ID1 group-ID wall/gran/hertz/history type 1 xplane -0.5
        fix ID2 group-ID wall/gran/hertz/history type 1 xplane 0.5

   OLD: fix ID group-ID wall/gran/hertz/history 3 0 xplane -0.5 0.5 1
   NEW: fix ID1 group-ID wall/gran/hertz/history primitive type 1 xplane -0.5 rolling_friction cdt
        fix ID2 group-ID wall/gran/hertz/history primitive type 1 xplane  0.5 rolling_friction cdt

   OLD: fix ID group-ID wall/gran/hertz/history 1 1 xplane -0.5 0.5 1
   NEW: fix ID group-ID wall/gran/hertz/history primitive type 1 xplane -0.5 0.5 cohesion sjkr

   OLD: fix ID group-ID wall/gran/hertz/history 0 0 zcylinder 0.05 1
   NEW: fix ID group-ID wall/gran/hertz/history primitive type 1 zcylinder 0.05 0. 0. tangential_damping off

NOTES:

* Same applies for hooke/history, hooke, hooke/history/simple, hertz/history/simple
* *sjkr* stands for 'simplified JKR (Johnson-Kendall-Roberts)' model, and *cdt* for
  'constant directional torque' model
* styles *xplane*\ , *yplane*\ , *zplane* take only one arg now (the wall position), if
  you want two walls you have to use two fix commands
* in addition to the existing style *zcylinder*\ , there is now *xcylinder* and
  *ycylinder* as well
* *xcylinder* *ycylinder* and *zcylinder* take 3 args: the cylinder radius, and
  the location of the axis in the other two dimensions
For details, see :doc:`fix wall/gran <fix_wall_gran>`.


----------


**pair\_style gran/\*:**


.. parsed-literal::

   OLD: pair_style gran/hertz/history 1 0
   NEW: pair_style gran/hertz/history

   OLD: pair_style gran/hertz/history 3 0
   NEW: pair_style gran/hertz/history rolling_friction cdt

   OLD: pair_style gran/hertz/history 1 1
   NEW: pair_style gran/hertz/history cohesion sjkr

   OLD: pair_style gran/hertz/history 0 0
   NEW: pair_style gran/hertz/history tangential_damping off

NOTES:

* Same applies for hooke/history, hooke, hooke/history/simple, hertz/history/simple
* *sjkr* stands for "simplified JKR (Johnson-Kendall-Roberts)" model, and *cdt* for "constant directional torque" model
For details, see :doc:`pair\_style gran <pair_gran>`.


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
