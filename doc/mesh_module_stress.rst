mesh module stress
==================

Syntax
""""""


.. parsed-literal::

   stress stress_keywords stress_values

* zero or more stress\_keyword/value pairs may be appended
* stress\_keyword = *reference\_point*, *stress* or *wear*
  
  .. parsed-literal::
  
       *reference_point* values = rx ry rz
         rx, ry, rz = coordinates of reference point
       *stress* value = on or off
       *wear* value = finnie or off



Examples
""""""""


.. parsed-literal::

   fix cad all mesh/surface/stress file mesh.stl type 1 wear finnie

Description
"""""""""""

This mesh module computes the average normal and shear stresses that the particles in the
fix group exert on each triangle of the mesh is evaluated (which costs a bit of performance).
Also, the total force and torque on the particle is calculated (see output info).
The per-element average normal stress can be dumped into VTK format using :doc:`dump mesh/vtk <dump>`.

With the optional *stress* keyword, stress tracking can be turned off if desired. The
reference point for calculating the body torque can be controlled via the *reference\_point*
keyword. The optional *wear* keyword can activates a simple qualitative wear model
(\ *finnie*\ ) - for details on the model, see the seperate /doc/finnie-wear.pdf. The wear coefficient
is divided by the mesh element area, therefore it is independent of mesh size.
The finnie constant k in Eqn. (4.23) has to be specified as follows:


.. parsed-literal::

   fix id all property/global k_finnie peratomtypepair n_atomtypes value_11 value_12 .. value_21 value_22 .. .

       (value_ij=value for the finnie constant between atom type i and j; n_atomtypes is the number of atom types you want to use in your simulation)

This mesh module is a prerequisite for the following mesh modules

* :doc:`6dof <mesh_module_6dof>`
* :doc:`binning <mesh_module_binning>`
* :doc:`contact <mesh_module_contact>`
* :doc:`deform <mesh_module_deform>`
* :doc:`servo <mesh_module_servo>`
* :doc:`stress <mesh_module_stress>`


**Restart, fix\_modify, output, run start/stop, minimize info:**

This mesh module stores a global vector with 9 components for access by various
:ref:`output commands <4_15>`. The first 3 components are equal to the
total force on the mesh, the next 3 components store the total torque on the body
exerted by the particles. Finally, the last 3 components are the coordinates (moved, scaled, rotated) of the reference point. Other info see :doc:`fix mesh <fix_mesh>`.

Related commands
""""""""""""""""

:doc:`fix mesh/surface <fix_mesh_surface>`
:doc:`fix wall/gran <fix_wall_gran>`

Default
"""""""

reference\_point = 0. 0. 0.
stress = on
wear = off


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
