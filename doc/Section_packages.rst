Packages
========

This section gives a quick overview of the add-on packages that extend
Packfall functionality.

Packfall includes optional packages, which are groups of files that
enable a specific set of features.  For example, force fields for
molecular systems or granular systems are in packages.  You can see
the list of all packages by typing "make package" from within the src
directory of the Packfall distribution.

See :ref:`Section\_start 3 <start_3>` of the manual for
details on how to include/exclude specific packages as part of the
Packfall build process, and for more details about the differences
between standard packages and user packages in Packfall.

Below, the packages currently availabe in Packfall are listed.  For
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
first and which Packfall links to when it is built.  If it is listed as
lib/package, then the code for the library is under the lib directory
of the Packfall distribution.  In the current Packfall tree, `lib/poems`
is the only retained in-tree auxiliary library.  See the corresponding
`lib/package/README` file for build details.  If a library is not listed
as `lib/package`, then it is a third-party dependency not included in the
Packfall distribution.  See the `src/package/README` or
the corresponding package makefile for info on where to download the
library.  :ref:`Section start <start_3_3>` of the manual also gives details
on how to build Packfall with both kinds of auxiliary libraries.






.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
