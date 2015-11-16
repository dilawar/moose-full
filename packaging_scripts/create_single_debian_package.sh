#!/bin/bash
PKG_NAME=moose-3.0.2
PKG_DIR=`pwd`/$PKG_NAME
#rm -rf $PKG_DIR/*
mkdir -p $PKG_DIR
cp -r DEBIAN $PKG_DIR/
cmake -DCMAKE_INSTALL_PREFIX=$PKG_DIR/usr ..
make -j4
make install
dpkg-deb -b $PKG_DIR
ls -lh *.deb
