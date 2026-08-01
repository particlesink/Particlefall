Introduction
============

This section provides a brief overview of Packfall, its scope, and its
license.


.. contents:: 
   :local:
   :depth: 1
   




.. _intro\_1:

What is Packfall
--------------------------

Packfall is an open-source Discrete Element Method (DEM) particle
simulation code.

Packfall is designed to be extended with new capabilities, such as new
force fields, atom types, boundary
conditions, or diagnostics.  See :doc:`Section\_modify <Section_modify>`
for more details.

Packfall is distributed under the terms of the `GNU Public License
<gnu_>`_.

.. _gnu: http://www.gnu.org/copyleft/gpl.html



History
-------

Packfall is derived from earlier LIGGGHTS-PUBLIC and LAMMPS code. The
project keeps inherited simulation infrastructure where it is useful,
while evolving the code and documentation in directions needed for
Packfall.


.. _intro\_2:

Packfall features
---------------------------

In the most general sense, Packfall integrates Newton's equations of
motion for collections of atoms, molecules, or macroscopic particles
that interact via short- or long-range forces with a variety of
initial and/or boundary conditions.  For computational efficiency
Packfall uses neighbor lists to keep track of nearby particles.  The
lists are optimized for systems with particles that are repulsive at
short distances, so that the local density of particles never becomes
too large.  On parallel machines, Packfall uses spatial-decomposition
techniques to partition the simulation domain into small 3d
sub-domains, one of which is assigned to each processor.  Processors
communicate and store "ghost" atom information for atoms that border
their sub-domain.

General features
----------------

* runs on a single processor or in parallel
* distributed-memory message-passing parallelism (MPI)
* spatial-decomposition of simulation domain for parallelism
* open-source distribution
* highly portable C++
* optional libraries used: MPI
* easy to extend with new features and functionality
* runs from an input script
* syntax for defining and using variables and formulas
* syntax for looping over runs and breaking out of loops
* run one or multiple simulations simultaneously (in parallel) from one script
* output to the widely used, open source VTK data format
* build as a library, invoke Packfall through the library interface or the provided Python wrapper
* couple with other codes: Packfall calls other code, other code calls Packfall, umbrella code calls both
* a strong ecosystem of fellow simulation engines for co-simulation, efficiently and tightly coupled via MPI

Model features
--------------

* import and handling of complex geometries: STL walls and VTK tet volume meshes
* moving mesh feature with a variety of motion schemes and a model for conveyor belts
* force and wear analysis on meshes as well as stress-controlled walls
* a variety of particle-particle contact implementations, including models for tangential history, non-sphericity, and cohesion
* interface to easily extend contact implementations
* heat conduction between particles
* particle insertion based on pre-defined volumes, meshes and particle streams from faces as well as particle growth and shrinkage
* flexible definition of particle distributions
* (WIP) smoothed Particle Hydrodynamics (SPH) fluid models

----------


.. _intro\_3:

Open source distribution
------------------------

Packfall comes with no warranty of any kind and is distributed under the
terms of the `GNU Public License <gnu_>`_ (GPL). The full license text is
included in the ``LICENSE`` file in the Packfall distribution.

In practical terms, this means you may use, modify, and redistribute
Packfall under the GPL. If you redistribute modified versions or source
that incorporates Packfall code, the GPL obligations continue to apply.

If you find an error, omission, or bug in this manual or in the code,
please report it through the project channels you are using for Packfall
development.

----------


.. _intro\_4:

Acknowledgments and citations
--------------------------------------------

Packfall inherits substantial architecture from earlier LIGGGHTS-PUBLIC
and LAMMPS code. The LAMMPS development effort was supported by the
`US Department of Energy <doe_>`_ (DOE), including its CRADA, LDRD,
ASCI, and Genomes-to-Life programs and its `OASCR <oascr_>`_ and
`OBER <ober_>`_ offices. Additional support came from the DOE
Genomics:GTL program (`www.doegenomestolife.org <gtl_>`_) under the
`project <ourgtl_>`_, "Carbon Sequestration in Synechococcus Sp.: From
Molecular Machines to Hierarchical Modeling".

.. _doe: http://www.doe.gov



.. _gtl: http://www.doegenomestolife.org



.. _ourgtl: http://www.genomes2life.org



.. _oascr: http://www.sc.doe.gov/ascr/home.html



.. _ober: http://www.er.doe.gov/production/ober/ober\_top.html



If Packfall contributes to published work, please cite Packfall 
and, where relevant, the upstream method papers describing the 
algorithms used. For example, the following paper describes the 
basic parallel algorithms used in the underlying code base. `this paper <citelig_>`_.

.. _citelig: http://www.cfdem.com/citing-liggghtsr




.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
