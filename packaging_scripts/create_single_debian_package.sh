#!/bin/bash
set -x
set -e
PKG_NAME=moose-3.0.2
##PKG_DIR=`pwd`/$PKG_NAME
##sudo rm -rf $PKG_DIR
##mkdir -p $PKG_DIR
##cp -r DEBIAN $PKG_DIR/
### Skip the RPATH.
##cmake -DCMAKE_INSTALL_PREFIX=$PKG_DIR/usr -DCMAKE_SKIP_INSTALL_RPATH:BOOL=ON ..
##make -j`nproc`
##make install
##(
##    # Fix the _moose.so file.
##    ( find $PKG_DIR -name "*.so" -print0 | xargs -0 -I file strip file )
##    ( find $PKG_DIR -name "*.so" -print0 | xargs -0 -I file chrpath -d file )
##)
##sudo chown -c -R root:root $PKG_DIR
##dpkg-deb -b $PKG_DIR
##ls -lh *.deb
##lintian $PKG_NAME.deb
( 
    cd ..
    git buildpackage  --git-ignore-new
)
