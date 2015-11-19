# -*- coding: utf-8 -*-

"""
The following resources have been really helpful in making this setup script -

https://github.com/numpy/numpy/blob/master/setup.py
https://packaging.python.org/en/latest/index.html
https://github.com/pypa/sampleproject/blob/master/setup.py
https://docs.python.org/2/distutils/apiref.html
http://stackoverflow.com/questions/3207219/how-to-list-all-files-of-a-directory-in-python
"""

from PyQt4 import pyqtconfig
import PyQt4
import sipdistutils
import sys
import os
import platform
osname_ = platform.system()

from distutils.core import setup, Extension

qcfg = pyqtconfig.Configuration()
qcfg.pyqt_sip_flags
remaining = sys.argv[1:]

print("[INFO] Sip flags: %s" % qcfg.pyqt_sip_flags)
print("[INFO] On system: %s" % osname_)

sys.argv = [sys.argv[0],
            'build_ext', 
            '--sip-opts=-I{} -e -g {}'.format(qcfg.pyqt_sip_dir,
                                              qcfg.pyqt_sip_flags)]
sys.argv.extend(remaining)

here = os.path.dirname(__file__)

# print(sys.argv)
# RESOURCES -
# https://docs.python.org/2.7/distutils/apiref.html?highlight=setup#distutils.core.setup
# http://pyqt.sourceforge.net/Docs/sip4/distutils.html#ref-distutils
# https://wiki.python.org/moin/EmbedingPyQtTutorial


# list of object files to be passed to the linker.
# These files must not have extensions, as the default extension
# for the compiler is used.
#os.environ["CC"]="g++"
extra_objects = []

# list of libraries to link against
libraries = ["QtCore",
             "QtGui",
             "QtOpenGL",
             "osg",
             "osgFX",
             "osgUtil",
             "osgFX",
             "osgGA",
             "osgQt",
             "osgAnimation",
             "osgViewer",
             "osgQt",
             "osgManipulator",
             "osgSim",
             "osgText"]


# list of directories to search for libraries at link-time
library_dirs = []

# list of directories to search for shared (dynamically loaded) libraries at run-time
runtime_library_dirs = PyQt4.__path__

# additional command line options for the compiler command line
extra_compile_args = ["-O2",
                      "-std=c++0x",
                      "-Wno-reorder",
                      "-Wno-overloaded-virtual"]

# additional command line options for the linker command line
if osname_ == 'Linux':
    extra_link_args = ["-fPIC", "-shared"]
elif osname_ == 'Windows':
    # Who cares
    pass
else:
    # On MacOSX, leave them empty.
    extra_link_args = []

# specify include directories to search
include_dirs = [ "./moogli/bin/include/" ]

includes = [ 
    "include/qt4/",
    "include/qt4/QtCore/",
    "include/qt4/QtGui/",
    "include/qt4/QtOpenGL/",
    "include/QtCore/",
    "include/QtGui/",
    "include/QtOpenGL/",
    ]

prefixes = [ "/usr", "/usr/local", "/opt" ]
for p in prefixes:
    include_dirs += [ os.path.join(p, x) for x in includes ]

# define pre-processor macros
define_macros = []

# undefine pre-processor macros
undef_macros = []

moogli = Extension(name="moogli.core._moogli",
                   sources=[
                       # "moogli/bin/src/core/CoordinateSystem.cpp",
                       #      "moogli/bin/src/core/Network.cpp",
                       #      "moogli/bin/src/core/Visualizable.cpp",
                       #      "moogli/bin/src/core/Neuron.cpp",
                       #      "moogli/bin/src/core/ElectricalCompartment.cpp",
                       #      "moogli/bin/src/core/ChemicalCompartment.cpp",
                       #      "moogli/bin/src/core/Visualizables.cpp",
                       #      "moogli/bin/src/view/NetworkViewer.cpp",
                       #      "moogli/bin/src/handlers/GeometrySelector.cpp",
                       #      "moogli/bin/src/utility/record.cpp",
                       #      "moogli/bin/src/utility/globals.cpp",
                       #      "moogli/bin/src/utility/conversions.cpp",
                       #      "moogli/bin/src/utility/numerics.cpp",
                       #      "moogli/bin/src/callbacks/CaptureView.cpp",
                       #      "moogli/bin/moc/NetworkViewer.moc.cpp",
                            "moogli/bin/src/shapes/Shape.cpp",
                            "moogli/bin/src/shapes/Frustum.cpp",
                            "moogli/bin/src/shapes/Sphere.cpp",
                            "moogli/bin/src/widgets/ColorBar.cpp",
                            "moogli/bin/src/view/View.cpp",
                            "moogli/bin/src/view/Viewer.cpp",
                            "moogli/bin/moc/Viewer.moc.cpp",
                            "moogli/bin/sip/moogli.sip"

                   ],
                   include_dirs=include_dirs,
                   extra_compile_args=extra_compile_args,
                   extra_link_args=extra_link_args,
                   library_dirs=library_dirs,
                   libraries=libraries,
                   extra_objects=extra_objects,
                   runtime_library_dirs=runtime_library_dirs,
                   define_macros=define_macros,
                   undef_macros=undef_macros)

with open(os.path.join(here, 'requirements.txt')) as f:
    requires = f.read().splitlines()

long_description = open(os.path.join(here, "README.rst")).read()

# scripts_dir = os.path.join(here, "scripts")
# scripts = [os.path.join(scripts_dir, fn) for fn in next(os.walk(scripts_dir))[2]]

setup(name='moogli',
      author='Aviral Goel',
      author_email='aviralg@ncbs.res.in',
      maintainer='Aviral Goel',
      maintainer_email='aviralg@ncbs.res.in',
      version="0.5.0",
      url='',
      download_url='',
      description="A 3D visualizer for neuronal networks",
      long_description=long_description,
      classifiers=['Development Status :: 3 - Alpha',
                   'Environment :: X11 Applications :: Qt',
                   'Intended Audience :: Science/Research',
                   'License :: OSI Approved :: GNU General Public License v2 (GPLv2)',
                   'Programming Language :: Python :: 2.6',
                   'Programming Language :: Python :: 2.7',
                   'Programming Language :: C++',
                   'Natural Language :: English',
                   'Operating System :: OS Independent',
                   'Topic :: Scientific/Engineering'],
      license='GPLv2',
      requires=requires,
      include_package_data = True,
      packages=[ 
                "moogli",
                "moogli.core",
                "moogli.widgets",
                "moogli.extensions",
                "moogli.extensions.moose",
                "moogli.visualization",
                "moogli.visualization.pipeline"
                ],
      package_data = { 'moogli.core' : [ 'moogli/core/_moogli.so' ]},
      ext_modules=[moogli],
      cmdclass={'build_ext': sipdistutils.build_ext},
      )
