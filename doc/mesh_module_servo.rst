mesh module servo
=================

Syntax
""""""


.. parsed-literal::

   servo servo_keywords servo_values

* zero or more servo\_keyword/value pairs may be appended
* servo keywords = *com* (obligatory) or *dim* (obligatory) or *ctrlPV* (obligatory) or *vel\_max* (obligatory) or *kp* or *ki* or *kd*
  *com* values = x, y, z
  
  .. parsed-literal::
  
         x, y, z = coordinates of the center of mass of the body (distance units)
     *ctrlPV* values = force or torque
         force = use force as controll process value, i.e. control force
         torque = use torque as controll process value, i.e. control torque
     *axis* args = x y z
         x y z = vector direction to apply the controlled mesh motion
         x or y or z can be a variable (see below)
     *target_val* values = val
         val = target value for the controller (force units or torque units, depending on *ctrlPV*\ )
     *vel_max* values = v
         v = maximum velocity magnitude for servo wall (velocity units)
     *kp* values = k
         k = proportional constant for PID controller
     *ki* values = k
         k = integral constant for PID controller
     *kd* values = k
         k = differential constant for PID controller
     *mode* values = auto
         auto = use alternative controller algorithm
     *ratio* values = dr
         dr = constant for the alternative controller approach (\ *mode* = auto)



Examples
""""""""


.. parsed-literal::

   fix servo all mesh/surface/stress/servo file plate.stl type 1 com 0. 0. 0. ctrlPV force axis 0. 0. 1. target_val 10 vel_max 1. kp 5.
   fix servo all mesh/surface/stress/servo file stirrer.stl type 1 com 0. 0. 0. ctrlPV torque axis 0. 0. 1. target_val 10 vel_max 1. ratio 0.01 mode auto

Description
"""""""""""

This mesh module assumes the mesh being a servo wall that compacts a particle packing until either a
total force (for *ctrlPV* = force)  or a total torque (for *ctrlPV* = torque) is acting on the mesh.
The target value is defined via keyword *target\_val*. The servo can act in any dimension
(as specified by the *axis* keyword). Only the direction of the axis is important; it's
length is ignored.
A negative value for *target\_val* leads to a wall motion towards negative *axis*\ -direction and vice versa. The user has to specify
the center of mass (keyword *com*\ ) and the maximum velocity allowed for the servo wall by keyword *vel\_max*.
Note that *vel\_max* < :doc:`skin <neighbor>` /(2\* :doc:`timestep <timestep>` ) is required.

The controller itself is a proportional-integral-derivative (PID) controller which is controlled by
3 constants *kp*\ , *ki*\ , *kd*\ :

output = kp \* error + ki \* errorsum + kd \* errorchange

where 'error' is the current deviation of the controll process value to the target value,
'errorsum' is the time integration (sum) of the error values and 'errorchange' its derivative. The controller
also includes an "anti-wind-up scheme" which prohibits accumulation of erroneous controller output caused
by the integral part due to unavoidable long-lasting deviations.

By using the keyword *mode* = auto an alternative controller approach is applied. It is a pure proportional controller with gain scheduling. In the absence of neighbour particles the servo wall may move with maximum velocity (defined by *vel\_max*). Otherwise, the maximum wall velocity is defined by *ratio* \* min(radius) / dt. Approaching *target\_val* the maximum velocity decreases to 0.1 \* *ratio* \* min(radius) / dt.

**Restart, fix\_modify, output, run start/stop, minimize info:**

This mesh module stores a global vector with 3 components for access by various :ref:`output commands <4_15>`.
The 3 components output the wall position.
Furthermore, this mesh module writes the state of the servo wall to binary restart files so
that a simulation can continue correctly.
This mesh module supports :doc:`fix\_modify <fix_modify>` with option *servo/integrate* = 'start' or 'stop'
to start or stop the servo wall integration inbetween two runs.
This module also supports :doc:`fix\_modify <fix_modify>` with option *servo/target\_val* = val to
change the target value inbetween two runs.
This module also supports `fix\_modify <fix_modify_html>`_ with option *servo/vel\_max* = val to change the the target value inbetween two runs.
This module also supports :doc:`fix\_modify <fix_modify>` with option *servo/ctrlParam* = kp ki kd
to change the controller params inbetween two runs.

Restrictions
""""""""""""


This mesh module requires the usage of :doc:`mesh module stress <mesh_module_stress>` and that module needs to
preceede the servo module.

When using this mesh module, along with scaling or rotate the body, all the servo\_keyword/value pairs have to represent
the state after scaling/rotation.
Mesh elements may not be deleted in case due to leaving the simulation box for a fixed boundary.
In this case, an error is generated. See :doc:`boundary <boundary>` command for details.
This mesh module can not be used in conjunction with another fix that manipulates mesh geometry,
such as a :doc:`fix move/mesh <fix_move_mesh>` or the :doc:`mesh module 6dof <mesh_module_6dof>`

Related commands
""""""""""""""""

:doc:`Mesh module stress <mesh_module_stress>`, :doc:`fix wall/gran <fix_wall_gran>`, :doc:`fix mesh/surface <fix_mesh_surface>`

Default
"""""""

kp = 1e-2, ki = 0, kd = 0


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
