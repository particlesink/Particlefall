Packages
========

This section gives a quick overview of the add-on packages that extend
LIGGGHTS(R)-PUBLIC functionality.

LIGGGHTS(R)-PUBLIC includes optional packages, which are groups of files that
enable a specific set of features.  For example, force fields for
molecular systems or granular systems are in packages.  You can see
the list of all packages by typing "make package" from within the src
directory of the LIGGGHTS(R)-PUBLIC distribution.

See :ref:`Section\_start 3 <start_3>` of the manual for
details on how to include/exclude specific packages as part of the
LIGGGHTS(R)-PUBLIC build process, and for more details about the differences
between standard packages and user packages in LIGGGHTS(R)-PUBLIC.

Below, the packages currently availabe in LIGGGHTS(R)-PUBLIC are listed.  For
standard packages, just a one-line description is given:

+----------+-------------------------------+-----------------------+----------------------------------------------------+---------+-----------+
| Package  | Description                   | Author(s)             | Doc page                                           | Example | Library   |
+----------+-------------------------------+-----------------------+----------------------------------------------------+---------+-----------+
| MOLECULE | molecular system force fields | -                     | -                                                  | -       | -         |
+----------+-------------------------------+-----------------------+----------------------------------------------------+---------+-----------+
| POEMS    | coupled rigid body motion     | Rudra Mukherjee (JPL) | :doc:`fix poems <fix_poems>`                       | rigid   | lib/poems |
+----------+-------------------------------+-----------------------+----------------------------------------------------+---------+-----------+
| VORONOI  | Voronoi tesselations          | Daniel Schwen (LANL)  | :doc:`compute voronoi/atom <compute_voronoi_atom>` | -       | Voro++    |
+----------+-------------------------------+-----------------------+----------------------------------------------------+---------+-----------+
|          |                               |                       |                                                    |         |           |
+----------+-------------------------------+-----------------------+----------------------------------------------------+---------+-----------+

The "Doc page" column links to either a portion of the
:doc:`Section\_howto <Section_howto>` of the manual, or an input script
command implemented as part of the package.

The "Example" column is a sub-directory in the examples directory of
the distribution which has an input script that uses the package.
E.g. "peptide" refers to the examples/peptide directory.

The "Library" column lists an external library which must be built
first and which LIGGGHTS(R)-PUBLIC links to when it is built.  If it is listed as
lib/package, then the code for the library is under the lib directory
of the LIGGGHTS(R)-PUBLIC distribution.  See the lib/package/README file for info
on how to build the library.  If it is not listed as lib/package, then
it is a third-party library not included in the LIGGGHTS(R)-PUBLIC distribution.
See the src/package/README or src/package/Makefile.lammps file for
info on where to download the library.  :ref:`Section start <start_3_3>` of the manual also gives details
on how to build LIGGGHTS(R)-PUBLIC with both kinds of auxiliary libraries.






.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
