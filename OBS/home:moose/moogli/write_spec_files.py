#!/usr/bin/env python
"""
write_spec_files.py: 

    This script generates spec file for various distributions.

    Last modified: Mon Aug 10 22:28:30 2015
"""
    
__author__           = "Dilawar Singh"
__copyright__        = "Copyright 2013, Dilawar Singh and NCBS Bangalore"
__credits__          = ["NCBS Bangalore"]
__license__          = "GNU GPL"
__version__          = "1.0.0"
__maintainer__       = "Dilawar Singh"
__email__            = "dilawars@ncbs.res.in"
__status__           = "Development"

import re
import os
import sys

moogliBuildReq = [
        "cmake"
        , "gcc-c++"
        , "PyQt4-devel"
        , "PyQt"
        , "qt-devel"
        , "sip-devel"
        ]

moogliReq = [ "OpenSceneGraph", "python-qt4" ]

repos_ = { "CentOS" : [6, 7]
        , "Fedora" : [20, 21, 22, 23 ]
        , "RHEL" : [ 7 ]
        , "Arch" : ["Core", "Extra"]
        , "openSUSE" : [ "12.3", "13.1", "13.2", "Tumbleweed", "Factory_ARM", 'Leap_42.1' ]
        , "CentOS"  : [ "6", "7" ]
        , "SLE"   : [ "11_SP3", "11_SP4", "12" ]
        }

_alternative = {
        'openSUSE' : { 
            'PyQt' : 'python-qt4'
            , 'qt-devel' : 'libqt4-devel' 
            , 'sip-devel' : 'python-sip'
            , 'qt4-devel' : 'libqt4'
            , 'PyQt4-devel' : 'python-qt4-devel'
            , 'PyQt4' : 'python-qt4'
            },
        'CentOS' : {
            'PyQt' : 'PyQt4'
            },
        }

def get_alternative_name(repoName, name):
    global _alternative
    rep = _alternative.get(repoName, None)
    if rep:
        alt = rep.get(name, None)
        if alt:
            return alt
        else:
            return name
    else:
        return name


class SpecFile():

    def __init__(self, repository, version):
        self.repository = repository
        self.version = version
        self.architecture = "i586"
        self.url = None
        self.specfileName = "moogli-{}_{}.spec".format(self.repository, version)
        self.templateText = None
        with open("moogli.spec.template", "r") as f:
            self.templateText = f.read()

    def writeSpecFile(self, **kwargs):
        print("++ Writing spec file for %s" % self.repository)

        # moogli - build
        moogliBuildReqText = "\n".join(
                [ "BuildRequires: %s" % get_alternative_name(self.repository, x) for x in moogliBuildReq]
                )
        self.templateText = self.templateText.replace("<<MoogliBuildRequires>>"
                , moogliBuildReqText
                )

        moogliReqText = "\n".join(
                [ "Requires: %s" % get_alternative_name(self.repository, x) for x in moogliReq ]
                )

        self.templateText = self.templateText.replace("<<moogliRequires>>"
                , moogliReqText
                )

        # Just get the moose-core and moose-python build requirements.
        print("Writing specfile: {}".format(self.specfileName))
        with open(self.specfileName, "w") as specFile:
            specFile.write(self.templateText)
        
def main():
    global repos_
    for r in repos_:
        repo, versions = r, repos_[r]
        for version in versions:
            sl = SpecFile(repo, version)
            sl.writeSpecFile()
    
if __name__ == '__main__':
    main()
