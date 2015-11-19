#!/bin/bash
echo "|| INFO : This script should be called from top subdirectory"
set -x
if which gbp; then
    echo "Packager is intalled"
else
(
    echo "Installing packager"
    cd /tmp
    git clone --depth 5 https://github.com/marquiz/git-buildpackage-rpm 
    cd git-buildpackage-rpm 
    sudo python setup.py install 
)
fi
gbp buildpackage-rpm  --git-ignore-new --git-ignore-branch \
    --git-spec-file=rpm/moose.spec
