.. index:: fix multisphere

fix multisphere command
=======================

Syntax
""""""


.. parsed-literal::

   fix ID group-ID multisphere

* ID, group-ID are documented in :doc:`fix <fix>` command
* multisphere = style name of this fix command
* zero or more keyword/value pairs may be appended
* keyword = *allow\_group\_and\_set* or *allow\_heatsource* or *CAddRhoFluid*
  
  .. parsed-literal::
  
       *allow_group_and_set* value = 'yes' or 'no'
         yes, no = determines if group and set command are allowed in the current simulation.
       *allow_heatsource* value = 'yes' or 'no'
         yes, no = determines if heatsources are allowed in the current simulation.
       *CAddRhoFluid* value = CAdd RhoFluid
         CAdd = Coefficient for additional mass term. (Usually 0.5)
         RhoFluid = Fluid density



Examples
""""""""


.. parsed-literal::

   fix ms all multisphere

Description
"""""""""""

Treat one or more sets of atoms as independent rigid bodies.  This
means that each timestep the total force and torque on each rigid body
is computed as the sum of the forces and torques on its constituent
particles and the coordinates, velocities, and orientations of the
atoms in each body are updated so that the body moves and rotates as a
single entity. The integration that is performed by this command is
euqivalent to :doc:`fix\_rigid <fix_rigid>`. However, the following
implementation details are different:

(1) Body data held by this fix is distributed across all processes, yielding
better parallel scalabilty.

(2) Bodies (particle clumps) can be added/inserted via
:doc:`fix\_insert\_stream <fix_insert_stream>`,
:doc:`fix\_insert\_rate\_region <fix_insert_rate_region>` or
:doc:`fix\_insert\_pack <fix_insert_pack>` and are automatically added
to this fix.

(3) A body is removed from the simulation domain if any of it's particles
is removed from the simulation according to the :doc:`boundary <boundary>`
settings or by any command that deleted particles (e.g.
:doc:`delete\_atoms <delete_atoms>`).

(4) By using this fix, gravity (:doc:`fix\_gravity <fix_gravity>`)
will be handled correctly for for overlapping particle clumps.

(5) This fix internally performs a :doc:`neigh\_modify <neigh_modify>` exclude
command so that particles belonging to the same rigid body are excluded
from the neighborlist build.

.. warning::

   You should not update the atoms in rigid bodies via
   other time-integration fixes (e.g. nve, nvt, npt), or you will be
   integrating their motion more than once each timestep.

The option *CAddRhoFluid* enables additional mass terms for the integration of
multisphere particles. These model the influence of displaced fluid on the 
particle motion.


----------


**Restart, fix\_modify, output, run start/stop, minimize info:**

Information about this fix is written to :doc:`binary restart files <restart>`.  This means you can restart a simulation
using multisphere particles.

:doc:`fix\_modify <fix_modify>` can be used to manipulate the fflag and tflag switches
 of multispheres. They can be initially defined in the 
:doc:`fix\_particletemplate\_multisphere <fix_particletemplate_multisphere>` command.
The fix\_modify manipulates all multispheres, not only the initially defined f/tflags from the template.


.. parsed-literal::

   E.g. fix_modify ms fflag on on off tflag on on off

No global scalar or vector or per-atom quantities are stored by
this fix for access by various :ref:`output commands <4_15>`.  No parameter of this fix can be
used with the *start/stop* keywords of the :doc:`run <run>` command.
This fix is not invoked during :doc:`energy minimization <minimize>`.

Restrictions
""""""""""""


Fix multisphere does not work together with :doc:`fix deform <fix_deform>`

Only one fix multisphere at a time is allowed.
Heat transfer simulations are not possible when using this fix
(e.g. :doc:`fix heat/gran/conduction <fix_heat_gran_conduction>`)

.. warning::

   All fixes or computes gathering statistical output
   (such as e.g. :doc:`compute com <compute_com>` or :doc:`fix ave/time <fix_ave_time>`
   operate on a per-sphere rather than on a per-body basis.

Currently, using fix multisphere requires :doc:`newton <newton>` = off and
:doc:`dimension <dimension>` = 3.

Related commands
""""""""""""""""

:doc:`fix\_rigid <fix_rigid>`
:doc:`fix\_particletemplate\_multisphere <fix_particletemplate_multisphere>`
:doc:`neigh\_modify <neigh_modify>`

Default
"""""""

*allow\_group\_and\_set* = 'no'
*allow\_heatsource* = 'no'


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
