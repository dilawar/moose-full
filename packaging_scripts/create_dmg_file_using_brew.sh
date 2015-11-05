#!/bin/bash
PKGNAME="MooseGhevar"
VOLNAME="${PKGNAME}"
VERSION="3.0.2"
DMGFILELABEL="${PKGNAME}"
THISDIR=`pwd`

# create the temp DMG file
STAGING_DIR=_Install
DMG_TMP="__${PKGNAME}__.dmg"
mkdir -p ${STAGING_DIR}

if [ ! -f "${DMG_TMP}" ]; then
    hdiutil create -srcfolder "${STAGING_DIR}" -volname "${PKGNAME}" -fs HFS+ \
          -fsargs "-c c=64,a=16,e=16" -format UDRW -size 1G "${DMG_TMP}"
else
    echo "DMG file $DMG_TMP exists. Mounting ..."
fi

# TODO
# mount it and save the device
#DEVICE=$(hdiutil attach -readwrite -noverify "${DMG_TMP}" | \
#         egrep '^/dev/' | sed 1q | awk '{print $1}')

sleep 2
pushd /Volumes/"${VOLNAME}"
popd 

echo "Do the thingy now" 
BREW_PREFIX="/Volumes/${VOLNAME}"
(
    cd $BREW_PREFIX
    if [ ! -f $BREW_PREFIX/bin/brew ]; then
        curl -L https://github.com/Homebrew/homebrew/tarball/master | \
            tar xz --strip 1 -C $BREW_PREFIX
    else
        echo "[I] Brew exists. Not installing"
    fi
    echo "Copying moose.rb and moogli.rb"
    cp $THISDIR/../macosx/*.rb $BREW_PREFIX/Library/Formula/
    $BREW_PREFIX/bin/brew -v install moose
)


#echo "Detaching ${DEVICE}"
#hdiutil detach "${DEVICE}"
