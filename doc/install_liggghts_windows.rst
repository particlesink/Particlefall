Compile LIGGGHTS(R)-PUBLIC for Windows
======================================


----------


Description:
------------

This routine describes how to setup your system in order to compile LIGGGHTS(R)-PUBLIC for Windows.

Prerequisites:
--------------

All mentioned programs are available for free.

MPI:
^^^^

LIGGGHTS(R)-PUBLIC is a highly parallelized simulation engine. If you want to run simulations in parallel you have to install MPI (Message Passing Interface standard). We suggest to use the implementation of Microsoft, which is available `here <msmpi_>`_. You need the binaries and the header files (included in the SDK). Please follow the instructions of the installer.

.. _msmpi: http://msdn.microsoft.com/en-us/library/bb524831%28v=vs.85%29.aspx



Git:
^^^^

GIT is an open-source version control system. We provide LIGGGHTS(R)-PUBLIC via `github <gitHub_>`_. It is to your advantage to use any git program to keep your LIGGGHTS(R)-PUBLIC version up-to-date. We recommend to use `git-scm <gitbash_>`_ as it provides a minimum bash environment too.

.. _gitHub: http://github.com



.. _gitHelp: http://help.github.com/linux-set-up-git



.. _gitbash: http://git-scm.com



You can set up your GIT environment to use the SSH-Keys thus you don't have to enter your user and password multiple times. Check the GIT GUI documentation for more details.

**Important: Git-scm will ask during installation how it should handle newlines; please use "Checkout as-is, commit Unix-style line endings".**

Python or Cygwin:
^^^^^^^^^^^^^^^^^

A python script updates the VS-project, thus you have to install either `python <python_>`_ or `cygwin <cygwin_>`_. In case of cygwin you can use another GIT program, because cygwin provides also a bash environment. The description of the procedure assumes that you have installed cygwin.

.. _python: https://www.python.org/downloads/windows/



.. _cygwin: https://www.cygwin.com/



Visual studio:
^^^^^^^^^^^^^^

To compile LIGGGHTS(R)-PUBLIC you need a development environment. You can download Microsoft Visual Studio Express (current version: Express 2013 with Update 4 for Windows Desktop) for free from `www.visualstudio.com <vs_>`_. Therefore, you have to create an Microsoft account.

.. _vs: http://www.visualstudio.com



VTK support:
^^^^^^^^^^^^

A detailed description how to compile LIGGGHTS(R)-PUBLIC with VTK support will follow here.

Procedure:
----------

Basically the following steps have to be performed:

* download/clone your repository from `github <gitHub_>`_
* update auto-generated header-files
* update your VS-project
* compile your LIGGGHTS(R)-PUBLIC version


Download/clone your LIGGGHTS(R)-PUBLIC version:
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

In this tutorial we use the suggested `git-scm <gitbash_>`_. You can use either the *Git GUI* or the *Git Bash*\ . In case of a bash you can follow the instructions in the documentation `githubAccess\_non-public <gitMan_>`_. Otherwise copy the link to your repository, for instance *https://github.com/CFDEMproject/LIGGGHTS-COMPANY.git*\ , into the *Git GUI* and save the repository at your computer, e.g. *C:/repositories/LIGGGHTS-COMPANY*\ .

.. _gitMan: githubAccess\_non-public.html



In the directory *LIGGGHTS-COMPANY/src/WINDOWS* you find an README, which describes the following steps in detail.

Update auto-generated header-files:
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Start *cygwin* and change your current directory to the src-directory inside of your repository, for instance


.. parsed-literal::

   cd /cygdrive/c/repositories/LIGGGHTS-COMPANY/src

Update the header files by


.. parsed-literal::

   sh Make.sh styles
   sh Make.sh models

You can check the files by


.. parsed-literal::

   ls style_\*

which should output a list of style file headers.

Update your VS-project:
^^^^^^^^^^^^^^^^^^^^^^^

In order to update the Visual Studio project in die WINDOWS directory run following commands (still in cygwin):


.. parsed-literal::

   cd WINDOWS
   python update_project.py LIGGGHTS.vcxproj

Compile your LIGGGHTS(R)-PUBLIC version:
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Finally to compile your LIGGGHTS(R)-PUBLIC version open the file *LIGGGHTS\_VS2013.sln* with Visual Studio.

To compile with MPI:

#. Switch to the correct setting for your system. *Debug* or *Release* and *32bit* or *64bit*\ .
#. Check if the MPI include and lib directories are correctly set in the project properties of LIGGGHTS(R)-PUBLIC - Open the *LIGGGHTS/Properties* and go to *Configuration Properties/VC++ Directories*\ .
For instance, a common include path is *C:\Program Files x86\Microsoft SDKs\MPI\Include*.
#. Build the LIGGGHTS(R)-PUBLIC project.


To compile without MPI:

#. Build the STUBS project.
#. Build LIGGGHTS(R)-PUBLIC using Debug\_STUBS or Release\_STUBS configurations from the provided project. (use x64 for 64bit binary)


You have now generated the executable **LIGGGHTS.exe** in the sub-directory *LIGGGHTS-COMPANY\src\WINDOWS\Release*. You can add this directory to your PATH environment variable to shorten the call command.

Run an example
^^^^^^^^^^^^^^

You can start a LIGGGHTS(R)-PUBLIC simulation inside of *cygwin* or a *command prompt* (search for cmd.exe).
Assuming you use your cygwin, change to one example in your repository.


.. parsed-literal::

   cd /cygdrive/c/repositories/LIGGGHTS-COMPANY/examples/LIGGGHTS/Tutorials_public/chute_wear

and start the simulation by


.. parsed-literal::

   /cygdrive/c/repositories/LIGGGHTS-COMPANY/src/WINDOWS/Release/LIGGGHTS.exe -in in.chute_wear

If you added the Release-folder to your PATH variable the command shortens to


.. parsed-literal::

   LIGGGHTS.exe -in in.chute_wear


----------


Questions?
----------

If any questions remain, contact us.


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
