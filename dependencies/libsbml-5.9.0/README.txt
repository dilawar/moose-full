
                            l i b S B M L

	    Sarah Keating, Akiya Jouraku, Frank Bergmann,
		   Ben Bornstein and Michael Hucka
	   with contributions from (in alphabetical order):
     Gordon Ball, Bill Denney, Christoph Flamm, Akira Funahashi,
     Ralph Gauges, Martin Ginkel, Alex Gutteridge, Stefan Hoops,
    Totte Karlsson, Moriyoshi Koizumi, Ben Kovitz, Rainer Machne,
          Nicolas Rodriguez, Lucian Smith, and many others.

  Please see the file NEWS.txt for a log of recent changes in libSBML

        More information about libSBML is available online at
                   http://sbml.org/Software/libSBML

       Please report problems with libSBML using the tracker at
            http://sbml.org/Software/libSBML/issue-tracker

  Mailing lists and online web forums for discussing libSBML are at
                        http://sbml.org/Forums

    To contact the core libSBML developers directly, send email to
		       libsbml-team@caltech.edu

   ,---------------------------------------------------------------.
  | Table of contents                                               |
  | 0. Foreword                                                     |
  | 1. What's new in this release?                                  |
  | 2. Quick start                                                  |
  | 3. Introduction                                                 |
  | 4. Detailed instructions for configuring and installing LibSBML |
  | 5. Reporting bugs and other problems                            |
  | 6. Mailing lists                                                |
  | 7. Licensing and distribution                                   |
  | 8. Acknowledgments                                              |
   `---------------------------------------------------------------'
                  Date of last update to this file:
        $Date: 2013-02-06 14:17:19 +0100 (Wed, 06 Feb 2013) $


0. FOREWORD
======================================================================

Article citations are crucial to our academic careers.  If you use
libSBML and you publish papers about your software, we ask that you
please cite the libSBML paper:

  Bornstein, B. J., Keating, S. M., Jouraku, A., and Hucka M. (2008)
  LibSBML: An API Library for SBML. Bioinformatics, 24(6):880-881.


1. WHAT'S NEW IN THIS RELEASE?
======================================================================

Please see the file NEWS.txt for more information about recent changes
in libSBML.  For a complete list of changes, please see the revision 
logs in the source code repository at 

  http://sbml.svn.sourceforge.net/viewvc/sbml/


2. QUICK START
======================================================================

Detailed instructions for building, installing and using libSBML
are available as part of the documentation; which is available
both online and as a separate download.

http://sbml.org/Software/libSBML/docs/cpp-api/libsbml-installation.html

Here we highlight the basic information.

2.1 Precompiled binaries
-------------------------

Check the download site for libSBML on SourceForge.net for possible 
installation packages for your operating system. We supply 
ready-to-install precompiled binaries for many systems.

  http://sourceforge.net/projects/sbml/files/libsbml/


2.2 CMake
---------

CMake (www.cmake.org) is the new build system for libsbml and can be
used on all platforms.  Using CMake is the only way to compile
libsbml with packages from source (libsbml without packages can
still be built from source with the old GNU makefiles system, see below).

To build using CMake, the first step is to create a new build
directory.  Typically, this is created as a new subdirectory of
the libsbml source tree, as libsbml/build/

The simplest step from there is to run the CMake GUI, 'cmake-gui'.
If run from the command line, the easiest method is to cd to the new
'build' directory, then run 'cmake-gui' with the root directory of
the libsbml source as an argument, or '..':

cd libsbml/
mkdir build
cd build/
cmake-gui ..

This will let it automatically find the source code and know where
to put the binaries.  If you run cmake-gui in some other way, you'll
need to tell it the location of the source code (libsbml/) and where
to build the binaries (libsbml/build/).

From there, push the 'Configure' button.  This will take you to a
new window where you are asked what to use to build libsbml.  On
Unix, selecting 'Unix Makefiles' is probably a good option; on Windows,
Visual Studios or Cygwin/MSys will work; just be sure to use the same
system as the software for which you plan to use libsbml.

Selecting 'Finish' will take you back to the original window, and
configuration will start.  It is likely that one or more systems will
not be found, or will need to be reset.  In particular, you may need
to give CMake the location of various dependency libraries.  If they 
are all installed in the same place, you need only
fill in the 'LIBSBML_DEPENDENCY_DIR' option; if they are installed in
different places, you will need to fill out the various *_INCLUDE_DIR
and *_LIBRARY options by hand (CMake will search in obvious places first
to try to find them itself).

Each time you give CMake more information, click the 'Configure'
button, and it will use that information to continue configuring
your build.  Each time it adds a new option it had not showed you before,
it will highlight that option in red.  If it lacks the information it
needs to complete configuration, it will give you the 'Error in
configuration process, project files may be invalid' error.

The options you are most likely to need to change are shown by default,
and include which xml parsing library to use (one of WITH_LIBXML,
WITH_EXPAT, or WITH_XERCES must be checked), what language bindings to
create (such as java (WITH_JAVA) or matlab (WITH_MATLAB)), and what
packages to include (such as the hierarchical model composition package
(ENABLE_COMP) or layout (ENABLE_LAYOUT)).  Other options can be found
by selecting the 'Advanced' checkbox at the top of the window.

Once 'Configure' makes it through with no errors, you can then click on
the 'Generate' button, which will create the necessary files for whatever
system you selected.  A Makefile will be generated for unix makefiles; a
'libsbml.sln' file for Visual Studio.  From there, use those files as they
were intended to be used, and compile libsbml!

It is not necessary to use the cmake gui to create your build system.
Once you know what options you want to set, it is possible to set them
all with command line options to 'cmake'.  In all cases, the format is

cmake -DOPTION=setting

in other words, '-D', the option you want to set, an equal sign, and the
value to which you want to set it.  For example, if the various dependency
libraries could be found automatically, and you wished to build libsbml
with java bindings and the layout package, you would change to your
newly-created 'build' directory, and type:

cmake -DENABLE_LAYOUT=ON -DWITH_JAVA=ON


2.3 GNU makefiles (Linux, MacOS X, FreeBSD, and Solaris)
-------------------------------------------------------

For builds of libsbml that do not include any packages (with the exception
of layout), the old GNU make system is available.  At the Unix command
prompt, untar the distribution, cd into it (e.g.,libsbml-5.8.0/), and
first type:

  ./configure

LibSBML will try to find and use libxml2 as its XML parser library.
If you do not have libxml2 version 2.6.16 or later on your system, the
configure step will fail.  In that case, you can try using the Expat
or Xerces libraries instead.  For Expat, use

  ./configure --with-expat

and for Xerces, use

  ./configure --with-xerces

By default, libSBML only builds the C and C++ API library.  If you
want to configure libSBML to build the Java, C#, Python, Perl, MATLAB, 
Ruby and/or Octave API libraries as well, add the flags --with-java,
 --with-csharp, --with-python, --with-perl, --with-matlab, --with-ruby, 
and/or --with-octave to the configure command.  You can combine
options as you need.  (To see what other options are available, run
the configure command with --help option.)

Depending on your system, you may need to tell the configure program
where to find some of these extra components by adding a directory
path after the option.  For example, if you wish to use a copy of Java
whose components are in /usr/local (with files in /usr/local/bin and
/usr/local/lib), use

  ./configure --with-java=/usr/local

Once you've successfully configured libSBML, run the following two
commands to build and install it:

  make                   # Note: use 'gmake' on FreeBSD
  make install           # Note: use 'gmake install' on FreeBSD

To compile C or C++ programs that use libSBML with GCC, use a command
such as the following, where -lsbml tells the compiler to link with
the installed libSBML library:

  gcc -o myapp.c myapp.c -lsbml

If the compiler cannot find the library, you probably need to adjust
the settings of your environment variable LD_LIBRARY_PATH (called
DYLD_LIBRARY_PATH on MacOS) or use ldconfig to adjust the library
search paths on your computer.  Please refer to the full libSBML
documentation for more information on this and related matters.

Documentation for libSBML is available as a separate download from the
same locations as the libSBML distribution (namely, the SBML project
SourceForge and the http://sbml.org/Software/libSBML web page).  You
may also regenerate the documentation from the source code distribution 
if you have Doxygen version 1.5.8 or later installed and have
configured libSBML with the --with-doxygen flag.  Then you can execute
the following to generate and install the libSBML documentation files:

  make install-docs       # Note: use 'gmake install-docs' on FreeBSD

To uninstall the libSBML installed by the above 'make install' command,
cd into the top directory of the libSBML source tree and run the following
command:

  make uninstall


3. INTRODUCTION
======================================================================

This README file describes libSBML, a library for reading, writing and
manipulating files and data streams containing the Systems Biology
Markup Language (SBML).  The library supports all levels and versions
of SBML, up to Level 3 Version 1 Core.

The library is written in ISO standard C and C++ and currently
provides an API for the languages C, C++, C#, Java, MATLAB, Octave,
Perl, Python, R and Ruby.  LibSBML is known to run on Linux, Windows,
MacOS X, FreeBSD and Solaris, but is portable and support for other
platforms should be straightforward to implement.

LibSBML is entirely open-source and all specifications and source code
are freely and publicly available.  For more information about SBML,
please visit the website http://sbml.org/.

Feature Highlights:
-------------------

* Full SBML Support.  All constructs in SBML Levels 1, 2 and 3 are
  supported.  For compatibility with some technically incorrect but
  popular Level 1 applications and models, the parser recognizes and
  stores notes and annotations defined for the top-level <sbml>
  element (logging a warning).

* Unified SBML Level 1, Level 2, and Level 3 object models.  All
  objects have getSBMLDocument(), getModel(), getLevel(), and
  getVersion(), methods among other things.

* Full XML and SBML Validation.  All XML and Schema warning, error and
  fatal error messages are logged with line and column number
  information and may be retrieved and manipulated programmatically.

* Dimensional analysis and unit checking.  LibSBML implements a
  thorough system for dimensional analysis and checking units of
  quantities in a model.  The validation rules for units that are
  specified in SBML Level 2 and Level 3 are fully implemented,
  including checking units in mathematical formulas.

* Access to SBML annotations and notes as XML objects.  Annotations
  and notes in libSBML 4.x are read and manipulated as XML structures;
  a text-string interface is available for backward compatibility with
  the libSBML 2.x series.  Further, in order to facilitate the support
  of MIRIAM compatible annotations, there are new object classes
  ModelHistory and CVTerm.  These classes facilitate the creation and
  addition of RDF annotations inside <annotation> elements by
  providing parsing and manipulation functions that treat the
  annotations in terms of XMLNode objects implemented by the new XML
  layer.  Both ModelHistory and CVTerm follow the general libSBML
  format of providing getters and setters for each variable stored
  within the class.

* Support for compressed SBML files.  If an SBML file name ends in
  .gz, .zip or .bz2, libSBML will automatically uncompress the file
  upon reading it.  Similarly, if the file to be written has one of
  those extensions, libSBML will write it out in compressed form.
  (The compression library incorporated by libSBML is MiniZip 1.01e,
  written by Gilles Vollant and made freely available for all uses
  including commercial applications.)

* Parser abstraction layer.  LibSBML relies on third-party XML parser
  libraries, but thanks to its implementation of a custom abstraction
  layer (named LIBLAX), libSBML can use any of the three most popular
  XML parser libraries: Expat, Apache Xerces-C++, and Libxml2.
  LibSBML provides identical functionality and checking of XML syntax
  is now available no matter which one is used.  SBML Documents are
  parsed and manipulated in the Unicode codepage for efficiency;
  however, strings are transcoded to the local code page for SBML
  structures.

* Small memory footprint and fast runtime.  The parser is event-based
  (SAX2) and loads SBML data into C++ structures that mirror the SBML
  specification.
      
* Interfaces for C, C++, C#, Java, MATLAB, Octave, Python, Perl, R and
  Ruby.  The C and C++ interfaces are implemented natively; the C#,
  Java, Perl, Python, R and Ruby interfaces are implemented using SWIG,
  the Simplified Wrapper Interface Generator; and the rest are
  implemented using custom hand-written interface code.

* Well tested: libSBML has over 4300 unit tests and over 11,000
  individual assertions.  The entire library was written using the
  test-first approach popularized by Kent Beck and eXtreme
  Programming, where it's one of the 12 principles.
    
* Written in portable, pure ISO C and C++.  The build system uses GNU
  tools (Autoconf, GNU Make) to build shared and static libraries.

* Complete user manual.  The manual is generated from the source code
  and closely reflects the actual API.


4. DETAILED INSTRUCTIONS FOR CONFIGURING AND INSTALLING LIBSBML
======================================================================

Detailed instructions for building and configuring libSBML are now
included as part of the full documentation available for libSBML.
You may find the documentation online at 

  http://sbml.org/Software/libSBML

You may also download a compressed archive of the formatted
documentation from the same place you found this libSBML source
distribution (e.g., from http://sourceforge.net/projects/sbml/).

Lastly you can format the documentation from the sources, which are
provided as part of this libSBML source distribution in the "docs"
subdirectory.  (To do that, however, you will need certain additional
software tools such as Doxygen and a full latex distribution.)

Please see NEWS.txt and the following bundled README files for details 
of the current libSBML-5:

  (1) docs/00README-HowToUsePackageExtension.txt

 (For package developers)

  (2) docs/00README-ExtensionSupportClasses.txt
  (3) docs/00README-HowToImplementPackageExtension.txt
  (4) docs/00README-ChangesInSBase.txt


5. REPORTING BUGS AND OTHER PROBLEMS
======================================================================

We invite you to report bugs and other problems using the issue
tracker for libSBML on SourceForge.  The following URL will take you
there directly:

  http://sbml.org/Software/libSBML/issue-tracker

You can also ask questions on the 'sbml-interoperability' mailing
list, either over email or using the web forum interface (see below).
This may even have advantages, such as that other people may also have
experienced the issue and offer a workaround more quickly than the
libSBML developers can respond.


6. MAILING LISTS
======================================================================

There are two kinds of mailing lists available: normal discussion
lists for humans, and a SVN change notification list.

Discussion lists
----------------

All discussion lists, their web interfaces and their RSS feeds are at

                       http://sbml.org/Forums/

If you use SBML, we urge you to sign up for sbml-announce, the SBML
announcements mailing list.  It is a low-volume, broadcast-only list.

If you use libSBML, we also encourage you to subscribe to or monitor
via RSS the 'sbml-interoperability' list, where people discuss the
development, use, and interoperability of software that supports SBML,
including libSBML.

If you are interested in helping to modify libSBML, or just want to
know about deeper issues and technical topics, you are welcome to
subscribe to the 'libsbml-development' mailing list.  Being a member
of libsbml-development will enable you to keep in touch with the
latest developments in libSBML as well as to ask questions and share
your experiences with fellow developers and users of libSBML.

SVN notification
----------------

If you are obtaining your libSBML files from SVN, you may wish to
subscribe to the mailing list sbml-svn, to be apprised of changes to
the SVN repository as soon as they are committed.  You can join the
list by visiting the following URL:

  https://lists.sourceforge.net/lists/listinfo/sbml-svn


7. LICENSING AND DISTRIBUTION
======================================================================

LibSBML incorporates a third-party software library, MiniZip
1.01e, copyright (C) 1998-2005 Gilles Vollant, released under
terms compatible with the LGPL.  Please see the file
src/compress/00README.txt for more information about MiniZip
1.01e and its license terms.

Licensing and Distribution Terms for libSBML:

Copyright (C) 2009-2013 jointly by the following organizations: 
    1. California Institute of Technology, Pasadena, CA, USA
    2. EMBL European Bioinformatics Institute (EBML-EBI), Hinxton, UK
 
Copyright (C) 2006-2008 by the California Institute of Technology,
    Pasadena, CA, USA 
 
Copyright (C) 2002-2005 jointly by the following organizations: 
    1. California Institute of Technology, Pasadena, CA, USA
    2. Japan Science and Technology Agency, Japan

LibSBML is free software; you can redistribute it and/or modify 
it under the terms of the GNU Lesser General Public License as
published by the Free Software Foundation; either version 2.1 of
the License, or any later version.

This software is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY, WITHOUT EVEN THE IMPLIED WARRANTY OF
MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  The software
and documentation provided hereunder is on an "as is" basis, and
the copyright holders have no obligations to provide maintenance,
support, updates, enhancements or modifications.  In no event
shall the copyright holders be liable to any party for direct,
indirect, special, incidental or consequential damages, including
lost profits, arising out of the use of this software and its
documentation, even if the copyright holders have been advised of
the possibility of such damage.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library in the file named "COPYING.txt"
included with the software distribution.  A copy is also
available online at the Internet address
http://sbml.org/software/libsbml/COPYING.html for your
convenience.  You may also write to obtain a copy from the Free
Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.


8. ACKNOWLEDGMENTS
======================================================================

This and other projects of the SBML Team have been supported by the
following organizations: the National Institutes of Health (USA) under
grants R01 GM070923 and R01 GM077671; the International Joint Research
Program of NEDO (Japan); the JST ERATO-SORST Program (Japan); the
Japanese Ministry of Agriculture; the Japanese Ministry of Education,
Culture, Sports, Science and Technology; the BBSRC e-Science
Initiative (UK); the DARPA IPTO Bio-Computation Program (USA); the
Army Research Office's Institute for Collaborative Biotechnologies
(USA); the Air Force Office of Scientific Research (USA); the
California Institute of Technology (USA); the University of
Hertfordshire (UK); the Molecular Sciences Institute (USA); the
Systems Biology Institute (Japan); and Keio University (Japan).

The libSBML authors are also grateful to Gilles Vollant for writing
MiniZip 1.01e and making it freely available.  LibSBML incorporates
MiniZip to support reading and writing compressed SBML files.


-----------------------------------------------
File author: M. Hucka, B. Bornstein, S. Keating
Last Modified: $Date: 2013-02-06 14:17:19 +0100 (Wed, 06 Feb 2013) $
Last Modified By: $Author: sarahkeating $
$HeadURL: https://svn.code.sf.net/p/sbml/code/trunk/libsbml/README.txt $
-----------------------------------------------

# The following is for [X]Emacs users.  Please leave in place.
# Local Variables:
# fill-column: 70
# End:
