LIGGGHTS(R)-PUBLIC 3.X vs. LIGGGHTS(R)-PUBLIC 2.X - syntax changes
##################################################################

Introduction:
*************

This is a short outline of the most important changes in LIGGGHTS(R)-PUBLIC 3.X
compared to LIGGGHTS(R)-PUBLIC 2.X regarding the syntax of major commands. The
motivation for these changes was to make the script language more readable and to
improve extendability (in the sense of object oriented programming) with respect
to modelling approaches.

Commands covered by this tutorial:
**********************************

* fix wall/gran/\*
* pair\_style gran/\*


Changes in syntax for each command:
***********************************

Changes are indicated as follows


.. parsed-literal::

   OLD: old_syntax
   NEW: new_syntax

where *old\_syntax* refers to the syntax used in LIGGGHTS(R)-PUBLIC 2.X and before,
and *new\_syntax* refers to the syntax used in LIGGGHTS(R)-PUBLIC 3.X.


----------


**fix wall/gran/\*:**


.. parsed-literal::

   OLD: fix ID group-ID wall/gran/\* *WALL-OPTIONS*
   NEW: fix ID group-ID wall/gran *MODEL-SELECTION* *WALL-OPTIONS* *MODEL-SETTINGS*

MODEL-SELECTION
===============


.. parsed-literal::

   *MODEL-SELECTION* = model *M* [tangential *T*\ ] [cohesion *C*\ ] [rolling_friction *R*\ ]

   *M* = hooke \| hooke/stiffness \| hooke/hysteresis \| hertz \| hertz/stiffness

   *T* = **no_history** \| history

   *C* = **off** \| sjkr \| sjkr2 \| hamaker

   *R* = **off** \| cdt \| epsd

MODEL-SETTINGS
==============


.. parsed-literal::

   *MODEL-SELECTION* = [tangential_damping (\ **on**\ \|off)] [absolute_damping (on\|\ **off**\ )] [viscous (on\|\ **off**\ )]

Examples
========


.. parsed-literal::

   OLD: fix ID group-ID wall/gran/hertz/history mesh n_meshes 2 meshes cad1 cad2
   NEW: fix ID group-ID wall/gran model hertz tangential history mesh n_meshes 2 meshes cad1 cad2

   OLD: fix ID1 group-ID wall/gran/hooke/history type 1 xplane -0.5
   NEW: fix ID1 group-ID wall/gran model hooke tangential history type 1 xplane -0.5

   OLD: fix ID1 group-ID wall/gran/hertz/history primitive type 1 xplane -0.5 rolling_friction cdt
   NEW: fix ID1 group-ID wall/gran model hertz tangential history rolling_friction cdt primitive type 1 xplane -0.5

   OLD: fix ID group-ID wall/gran/hertz/history primitive type 1 xplane -0.5 0.5 cohesion sjkr
   NEW: fix ID group-ID wall/gran model hertz tangential history cohesion sjkr primitive type 1 xplane -0.5 0.5

   OLD: fix ID group-ID wall/gran/hertz/history primitive type 1 zcylinder 0.05 0. 0. tangential_damping off
   NEW: fix ID group-ID wall/gran model hertz tangential history primitive type 1 zcylinder 0.05 0. 0. tangential_damping off


----------


**pair\_style gran/\*:**


.. parsed-literal::

   OLD: pair_style gran/\* **MODEL-SETTINGS**
   NEW: pair_style gran  **MODEL-SELECTION** **MODEL-SETTINGS**

MODEL-SELECTION
===============

Same as in fix wall/gran/\*

MODEL-SETTINGS
==============

Same as in fix wall/gran/\*

Examples
========


.. parsed-literal::

   OLD: pair_style gran/hertz/history
   NEW: pair_style gran model hertz tangential history

   OLD: pair_style gran/hertz/history rolling_friction cdt
   NEW: pair_style gran model hertz tangential history rolling_friction cdt

   OLD: pair_style gran/hertz/history cohesion sjkr
   NEW: pair_style gran model hertz tangential history cohesion sjkr

   OLD: pair_style gran/hertz/history tangential_damping off
   OLD: pair_style gran model hertz tangential history tangential_damping off


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
