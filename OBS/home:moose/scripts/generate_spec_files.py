#!/usr/bin/env python
import os
import sys

packages = [ 'moose'
        , 'moogli'
        , 'moose-python'
        , 'moose-gui'
        , 'libmoose'
        ]

def generate_script(package):
    print("Info: Generating for %s" % package)
    pkgDir = os.path.join('..', package)
    if not os.path.isdir(pkgDir):
        raise UserWarning("%s does not exists" % pkgDir)


def main():
    for p in packages:
        generate_script(p)

if __name__ == '__main__':
    main()
