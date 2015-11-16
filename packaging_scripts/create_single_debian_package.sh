#!/bin/bash
PKG_NAME=moose-3.0.2
(
    mkdir -p ../_build/$PKG_NAME
    cp -r DEBIAN ../_build/$PKG_NAME/
    cd .. && cd _build
    cmake -DCMAKE_INSTALL_PREFIX=`pwd`/$PKG_NAME ..
    make -j4
    make install
    dpkg-deb -b $PKG_NAME
)
