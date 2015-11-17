#!/bin/bash
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
rpmlint moose.spec
gbp buildpackage-rpm  --git-ignore-new --git-ignore-branch
