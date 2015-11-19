# -*- coding: utf-8 -*-
"""setup.py: 

This script is used with cmake to install moogli.

This must be called as 

    python cmake_modules/setup.py install

NOTE: This repository is clone of `https://github.com/aviralg/moogli`.

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
long_description = open(os.path.join('.', "README.rst")).read()

# check if _moogli.so is available or not.
soFile = os.path.join('.', 'moogli', 'core', '_moogli.so')
if not os.path.exists(soFile):
    print("[ERROR] %s does not exits. Please build it using cmake" % soFile)
    print("We are in %s" % os.getcwd())
    quit()

setup(name='moogli',
        author='Aviral Goel',
        author_email='aviralg@ncbs.res.in',
        maintainer='Dilawar Singh',
        maintainer_email='dilawars@ncbs.res.in',
        version="0.5.0",
        url='https://github.com/BhallaLab/moogli',
        download_url='https://github.com/aviralg/moogli',
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
        package_dir = { 'moogli.core' : 'moogli/core' },
        package_data = { 'moogli.core' : [ '_moogli.so' ] },
        )
