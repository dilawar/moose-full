#!/bin/bash
# TODO: No simple way to install python files without including the source
# files. Not worth it.
set -e
PKG_PATH=`pwd`/moose-3.0.2-Linux-amd64.deb

if [ ! -f $PKG_PATH ]; then
    echo "Building package"
    (
        cd ..
        mkdir -p _build
        cd _build
        cmake -DCMAKE_INSTALL_PREFIX=_install ..
        make -j`nproc` | tee $0.log
        cpack -GDEB -DCPACK_SET_DESTDIR=ON 
    )
    cp ../_build/*.deb .
else
    echo "$PKG_PATH exists"
fi

echo " Extracting "

TEMPDIR=/tmp/moose_debian
if [ ! -d "$TEMPDIR/usr" ]; then
    dpkg-deb -x $PKG_PATH $TEMPDIR
fi

(
    cd $TEMPDIR
    find . -type f
    find . -type f | grep "_build\/_install"
)
#rm -rf $TEMPDIR
