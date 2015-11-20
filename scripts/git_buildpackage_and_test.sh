#!/bin/bash
set +e
set -x
function build_deb
{
    echo "Building for $2 with arch $1"
    git buildpackage --git-ignore-new --git-no-sign-tags --git-dist=$1 --git-arch=$2
    #git buildpackage --git-ignore-new --git-no-sign-tags --git-pbuilder --git-dist=$1 --git-arch=$2
}

export PYTHONPATH=""
build_deb wheezy amd64
build_deb wheezy i386

echo "installing"
sudo dpkg -i ../*amd64*.deb
python -c 'import moose'
python -c 'import moogli'
echo "now uninstalling"
sudo apt-get remove moose 
echo "Checking if moose-python is still there"
python -c 'import moose' || echo "Successfully uninstalled"
python -c 'import moogli' || echo "Successfully uninstalled"
