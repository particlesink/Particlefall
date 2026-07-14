.. index:: dump local/gran/vtk

dump local/gran/vtk command
===========================

Syntax
""""""


.. parsed-literal::

   dump ID group-ID style N file ID

* ID = user-assigned name for the dump
* group-ID = ID of the group of atoms to be dumped
* style = *local/gran/vtk*
* N = dump every this many timesteps
* file = name of file to write dump info to
  
  .. parsed-literal::
  
     ID = ID of a :doc:`compute pair/gran/local or wall/gran/local <compute_pair_gran_local>`



Examples
""""""""


.. parsed-literal::

   compute cpgl all pair/gran/local pos vel force
   dump dmp2 all local/gran/vtk 100 post/forcechain\*.vtp cpgl
   dump dmp3 all local/gran/vtk 200 post/forcechain\*.pvtu cpgl

Description
"""""""""""

Dump a snapshot of granular or granular bond quantities to one or more files every N
timesteps. *ID* refers to a :doc:`compute pair/gran/local or wall/gran/local <compute_pair_gran_local>`
which is providing the data to be dumped.

The timesteps on which dump output is written can be controlled by a variable;
see the :doc:`dump\_modify every <dump_modify_vtk>` command for details.  Only
information for atoms in the specified group is dumped.  The :doc:`dump\_modify thresh and region <dump_modify_vtk>` commands can also alter what atoms are
included; see details below. As described below, the special character "\*" and
the suffix in the filename determine the kind of output.

.. note::

   In Paraview, the "Tube" filter can be used for viz of the VTK line data.

**Description for dumping bond data:**

Optionally, for dumping bond quantities, this command requires a :doc:`cohesion bond <gran_cohesion_bond>` 
granular pair style interaction to be active and a compute pair/gran/local/bond instead of
a a :doc:`compute pair/gran/local or wall/gran/local <compute_pair_gran_local>` command, such as


.. parsed-literal::

   compute bonds all pair/gran/local/bond

For the bond dump, the values that are output to VTK are hard-coded as follows:

+---------------------+----------------------------------------------------------------------------------------+-------------------+
| *x1*                | position of particle i in bond                                                         | 3 values (vector) |
+---------------------+----------------------------------------------------------------------------------------+-------------------+
| *x2*                | position of particle j in bond                                                         | 3 values (vector) |
+---------------------+----------------------------------------------------------------------------------------+-------------------+
| *id1*               | ID of particle i                                                                       | 1 value (scalar)  |
+---------------------+----------------------------------------------------------------------------------------+-------------------+
| *id2*               | ID of particle 2                                                                       | 1 value (scalar)  |
+---------------------+----------------------------------------------------------------------------------------+-------------------+
| *force*             | total force between bonded particles, including bond and pair contributions            | 3 values (vector) |
+---------------------+----------------------------------------------------------------------------------------+-------------------+
| *torque*            | total torque between bonded particles, including bond and pair contributions           | 3 values (vector) |
+---------------------+----------------------------------------------------------------------------------------+-------------------+
| *force\_normal*     | normal total force between bonded particles, including bond and pair contributions     | 3 values (vector) |
+---------------------+----------------------------------------------------------------------------------------+-------------------+
| *force\_tangential* | tangential total force between bonded particles, including bond and pair contributions | 3 values (vector) |
+---------------------+----------------------------------------------------------------------------------------+-------------------+

.. warning::

   Only data of particles which have an actual active bond
   via :doc:`cohesion bond <gran_cohesion_bond>` are included in the output.
   However, *F*\ , *T*\ , etc. are the total forces and torques, including other
   contributions such as Hertz normal forces etc.

.. warning::

   Because periodic boundary conditions are enforced only
   on timesteps when neighbor lists are rebuilt, the coordinates of an
   atom written to a dump file may be slightly outside the simulation
   box.

.. warning::

   The output data does not contain any information
   about bonds stretching over periodic boundary conditions. Typically,
   your visz engine will thus not detect bonds stretching over periodic 
   boundary conditions.


----------


The VTK format uses a single snapshot of the system per file, thus
a wildcard "\*" must be included in the filename, as discussed below.
Otherwise the dump files will get overwritten with the new snapshot
each time.


----------


Dumps are performed on timesteps that are a multiple of N (including
timestep 0) and on the last timestep of a minimization if the
minimization converges.  Note that this means a dump will not be
performed on the initial timestep after the dump command is invoked,
if the current timestep is not a multiple of N.  This behavior can be
changed via the :doc:`dump\_modify first <dump_modify_vtk>` command, which
can also be useful if the dump command is invoked after a minimization
ended on an arbitrary timestep.  N can be changed between runs by
using the :doc:`dump\_modify every <dump_modify_vtk>` command.
The :doc:`dump\_modify every <dump_modify_vtk>` command
also allows a variable to be used to determine the sequence of
timesteps on which dump files are written.  In this mode a dump on the
first timestep of a run will also not be written unless the
:doc:`dump\_modify first <dump_modify_vtk>` command is used.

Dump filenames can contain two wildcard characters.  If a "\*"
character appears in the filename, then one file per snapshot is
written and the "\*" character is replaced with the timestep value.
For example, tmp.dump\*.vtk becomes tmp.dump0.vtk, tmp.dump10000.vtk,
tmp.dump20000.vtk, etc.  Note that the :doc:`dump\_modify pad <dump_modify_vtk>`
command can be used to insure all timestep numbers are the same length
(e.g. 00010), which can make it easier to read a series of dump files
in order with some post-processing tools.

To write the output file in parallel the filename must end with either ".pvtu"
or ".pvtp". If we assume that the filename is "data.pvtu" then one file with
exactly this name is written. This contains the link to the processor data which
is written in "data\_X.vtu" files, where X (= 0,...,P-1) is the ID of the
processor that wrote the file. Thus, in order to visualize all the data in
ParaView one would open the "data.pvtu" file. Writing output in a parallel
fashion can be particularly important on distributed HPC clusters and can
significantly reduce runtime.

.. note::

   The legacy ".vtk" file type does not support parallel writing.

By default, P = the number of processors meaning one file per
processor, but P can be set to a smaller value via the *nfile* or
*fileper* keywords of the :doc:`dump\_modify <dump_modify_vtk>` command.
These options can be the most efficient way of writing out dump files
when running on large numbers of processors.

Note that using the "\*" character and parallel file types together can produce a
large number of small dump files!

If *dump\_modify binary* is used, the dump file(s) is/are written in binary
format. A binary dump file will be about the same size as a text version, but
will typically write out much faster. Additionally, *dump\_modify compressor* can
be used to specify the writing of compressed binary data. Options for
compressors include:


.. parsed-literal::

   none: No compression
   zlib: Zlib compression
   lz4: Lz4 compression (VTK >= 8.0 required)

Restrictions
""""""""""""


The *local/gran/vtk* style does not support writing of gzipped dump files,
and does not support triclinic simulation boxes.

To be able to use *local/gran/vtk*\ , you have to link to the VTK library,
please adapt your Makefile accordingly. You must compile Packfall with the
-DLAMMPS\_VTK option - see the :ref:`Making Packfall <start_2>`
section of the documentation.

The *local/gran/vtk* dump style neither supports buffering nor custom format strings.

Related commands
""""""""""""""""

:doc:`dump <dump>`, :doc:`dump image <dump_image>`, :doc:`dump\_modify <dump_modify>`,
:doc:`undump <undump>`

Default
"""""""

By default, files are written in ASCII format. If the file extension is not
one of .vtk, .vtp or .vtu, the legacy VTK file format is used.


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
