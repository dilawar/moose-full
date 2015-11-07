#!/bin/bash
# NOTICE: DO not build on El Capitan. 10.8 is known to work everywhere.
set -x
set -e

CURRDIR=`pwd`
unset PYTHONPATH
PATH=/usr/bin:/bin:/usr/sbin:/sbin:/usr/X11/bin
export HOMEBREW_BUILD_FROM_SOURCE=YES
CFLAGS+=-march=native

APPNAME="Moose"
VERSION="3.0.2"
MAC_NAME=`sw_vers -productVersion`
PKGNAME="${APPNAME}_${VERSION}"

VOLNAME="${PKGNAME}"

#### SAFETY
set +e
echo "|| Detaching possibly attached disk"
hdiutil detach /Volumes/${PKGNAME}
set -e

DMGFILELABEL="${PKGNAME}"
THISDIR=`pwd`

# create the temp DMG file
STAGING_DIR=_Install
DMG_TMP="${PKGNAME}-${MAC_NAME}.dmg"
mkdir -p ${STAGING_DIR}

if [ ! -f "${DMG_TMP}" ]; then
    hdiutil create -srcfolder "${STAGING_DIR}" -volname "${PKGNAME}" -fs HFS+ \
          -fsargs "-c c=64,a=16,e=16" -format UDRW -size 600M "${DMG_TMP}"
else
    echo "DMG file $DMG_TMP exists. Mounting ..."
fi

# TODO
# mount it and save the device
DEVICE=$(hdiutil attach -readwrite -noverify "${DMG_TMP}" | \
         egrep '^/dev/' | sed 1q | awk '{print $1}')

############################# EXIT gacefully ################################ 
# Traps etc
# ALWAYS DETACH THE DEVICE BEFORE EXITING...
function detach_device 
{
    hdiutil detach "${DEVICE}"
    exit
}
trap detach_device SIGINT SIGTERM SIGKILL

sleep 1

echo "Install whatever you want now"
BREW_PREFIX="/Volumes/${VOLNAME}"
export PATH=${BREW_PREFIX}/bin:$PATH
(
    cd $BREW_PREFIX
    if [ ! -f $BREW_PREFIX/bin/brew ]; then
        curl -L https://github.com/Homebrew/homebrew/tarball/master | \
            tar xz --strip 1 -C $BREW_PREFIX
    else
        echo "[I] Brew exists. Not installing"
    fi
    echo "Copying moose.rb and moogli.rb"

    cp $CURRDIR/../macosx/*.rb $BREW_PREFIX/Library/Formula/

    # This even works without python.
    $BREW_PREFIX/bin/brew -v install python 
    $BREW_PREFIX/bin/brew -v install homebrew/python/matplotlib
    $BREW_PREFIX/bin/brew -v install moose | tee __brew_moose_log__
 
)

################ COPY THE .app ##########################################
cp -rpf "${APPNAME}.app" "$BREW_PREFIX"
APPEXE="${APPNAME}.app/Contents/MacOS/${APPNAME}"
mkdir -p `dirname $APPEXE`
# create the executable.
cat > ${APPEXE} <<-EOF
#!/bin/bash
${BREW_PREFIX}/bin/moosegui
EOF
chmod +x ${APPEXE}

################ INSTALL THE ICON ########################################
DMG_BACKGROUND_IMG="${CURRDIR}/moose_icon_large.png"
# Check the background image DPI and convert it if it isn't 72x72
_BACKGROUND_IMAGE_DPI_H=`sips -g dpiHeight ${DMG_BACKGROUND_IMG} | grep -Eo '[0-9]+\.[0-9]+'`
_BACKGROUND_IMAGE_DPI_W=`sips -g dpiWidth ${DMG_BACKGROUND_IMG} | grep -Eo '[0-9]+\.[0-9]+'`

if [ $(echo " $_BACKGROUND_IMAGE_DPI_H != 72.0 " | bc) -eq 1 -o $(echo " $_BACKGROUND_IMAGE_DPI_W != 72.0 " | bc) -eq 1 ]; then
    echo "WARNING: The background image's DPI is not 72."
    echo "This will result in distorted backgrounds on Mac OS X 10.7+."
    echo "I will convert it to 72 DPI for you."
    _DMG_BACKGROUND_TMP="${DMG_BACKGROUND_IMG%.*}"_dpifix."${DMG_BACKGROUND_IMG##*.}"
    sips -s dpiWidth 72 -s dpiHeight 72 ${DMG_BACKGROUND_IMG} --out ${_DMG_BACKGROUND_TMP}
    DMG_BACKGROUND_IMG="${_DMG_BACKGROUND_TMP}"
fi

############################################################################
# add a background image
mkdir -p /Volumes/"${VOLNAME}"/.background
cp "${DMG_BACKGROUND_IMG}" /Volumes/"${VOLNAME}"/.background/


### NOTICE: Following block does not work with X-window support.
#### tell the Finder to resize the window, set the background,
####  change the icon size, place the icons in the right position, etc.
###echo '
###   tell application "Finder"
###     tell disk "'${VOLNAME}'"
###           open
###           set current view of container window to icon view
###           set toolbar visible of container window to false
###           set statusbar visible of container window to false
###           set the bounds of container window to {400, 100, 920, 440}
###           set viewOptions to the icon view options of container window
###           set arrangement of viewOptions to not arranged
###           set icon size of viewOptions to 72
###           set background picture of viewOptions to file ".background:'${DMG_BACKGROUND_IMG}'"
###           set position of item "'${APP_NAME}'.app" of container window to {160, 205}
###           set position of item "Applications" of container window to {360, 205}
###           close
###           open
###           update without registering applications
###           delay 2
###     end tell
###   end tell
###' | osascript
###
###sync

echo "TODO. Now resize and compress using hdiutil"
echo "|| use: hdiutil convert a.dmg -format UDBZ -o b.dmg"

#### TODO: Resize the harddisk and compress it for distribution if tests are OK.
##set +e
##DISKSIZE=`du -sh /Volumes/"${VOLNAME}"`
##echo "Overall disk size is $DISKSIZE"
##echo "|| Blowing up brew cache"
##rm -rf `${BREW_PREFIX}/bin/brew --cache`
##
##DISKSIZE=`du -sh /Volumes/"${VOLNAME}"`

## Finally detach the device
detach_device
