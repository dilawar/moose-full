# -*- coding: utf-8 -*-
"""setup_cmake.py: 

This script is used with cmake to install moogli.

"""
    
__author__           = "Dilawar Singh"
__copyright__        = "Copyright 2015, Dilawar Singh and NCBS Bangalore"
__credits__          = ["NCBS Bangalore"]
__license__          = "GNU GPL"
__version__          = "1.0.0"
__maintainer__       = "Dilawar Singh"
__email__            = "dilawars@ncbs.res.in"
__status__           = "Development"


import sys
import os
from distutils.core import setup, Extension

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
