#!/bin/bash
set +e
set -x
export PYTHONPATH=""
ARCH=amd64
DIST=wheezy
git buildpackage --git-ignore-new --git-no-sign-tags --git-dist=$DIST \
    --git-arch=$ARCH
echo "installing"
sudo dpkg -i ../*.deb
python -c 'import moose'
python -c 'import moogli'
echo "now uninstalling"
sudo apt-get remove moose-core moose-doc moose-gui moose-moogli moose-moogli
echo "Checking if moose-python is still there"
python -c 'import moose' || echo "Successfully uninstalled"
python -c 'import moogli' || echo "Successfully uninstalled"
