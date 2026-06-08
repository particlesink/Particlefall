gran surface multicontact
=========================

Syntax
""""""


.. parsed-literal::

   surface multicontact

Description
"""""""""""

This is the surface model for a spherical particle that can have multiple contacts. 
It represents a smooth (non-rough) sphere and its task is to save certain variables 
(surface position and normal force) for the 
:doc:`fix multicontact/halfspace <fix_multicontact_halfspace>` which is mandatory with this style.

Restrictions
""""""""""""
 none

**Coarse-graining information:**

Using :doc:`coarsegraining <coarsegraining>` in
combination with this command might lead to
statistically different dynamics and system state.
To the best knowledge of the developers, the 
cross-influence between this command and coarse-graining
is unknown.

Related commands
""""""""""""""""

:doc:`fix multicontact/halfspace <fix_multicontact_halfspace>`


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
