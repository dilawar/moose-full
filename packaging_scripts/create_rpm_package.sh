#!/bin/bash
if which gbp; then
    echo "Packager is intalled"
else
(
    echo "Installing packager"
    cd /tmp
    git clone --depth 1 https://github.com/marquiz/git-buildpackage-rpm 
    cd git-buildpackage-rpm 
    sudo python setup.py install 
)
