#!/bin/bash

echo "||NOTICE
If you are using this script on MacOSX 10.11.2, be careful about the following
error:
    illegal instruction: 4
I could get everything working fine on MacOSX- 10.8 
"

set -x
set -e

CURRDIR=`pwd`

# Unset any enviroment PYTHONPATH. They can confuse us.
unset PYTHONPATH
PATH=/usr/bin:/bin:/usr/sbin:/sbin:/usr/X11/bin

APPNAME="Moose"
VERSION="3.0.2"
MAC_NAME=`sw_vers -productVersion`
PKGNAME="${APPNAME}_${VERSION}"

VOLNAME="${PKGNAME}"

### SAFETY
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
    hdiutil create -srcfolder "${STAGING_DIR}" -volname "${PKGNAME}" \
        -format UDRW -size 900M "${DMG_TMP}"
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
PORT_PREFIX="/Volumes/${VOLNAME}"
export PATH=${PORT_PREFIX}/bin:$PATH
(
    cd $PORT_PREFIX
    if [ ! -f $PORT_PREFIX/bin/port ]; then
        curl -O https://distfiles.macports.org/MacPorts-2.3.3.tar.bz2
        tar xf MacPorts-2.3.3.tar.bz2
        cd MacPorts-2.3.3
        ./configure --prefix=$PORT_PREFIX \
            --with-applications-dir=$PORT_PREFIX/Applications
        make 
        make install
    else
        echo "[I] Port exists. Not installing"
    fi
    #echo "Copying moose.rb and moogli.rb"
    #cp $CURRDIR/../macosx/*.rb $PORT_PREFIX/Library/Formula/

    # This even works without python.
    $PORT_PREFIX/bin/port -v install python 
    $PORT_PREFIX/bin/port -v install matplotlib
    $PORT_PREFIX/bin/port -v install networkx
    #$PORT_PREFIX/bin/port -v install moose --with-gui | tee "$CURRDIR/__port_moose_log__"

    # Delete unneccessay files.
    echo "|| Deleting port files"
    find ${PORT_PREFIX} -type f -maxdepth 1 -print0 | xargs -I file rm -f file

    echo "|| TODO: Delete more here if not needed. Such as build tools etc.."
    rm -rf $APPNAME.app

    # Also write apple script
    MOOSEPATH=${PORT_PREFIX}/lib/python2.7/site-packages
    cat > $PORT_PREFIX/moosegui <<EOF
#!/bin/bash
touch \$HOME/.bash_profile
source \$HOME/.bash_profile
if [[ "\${PYTHONPATH}" == *"${MOOSEPATH}"* ]]; then
    echo "[INFO] PYTHONPATH aleady contains ${MOOSEPATH}"
else
    # Also write to .bash_profile, so that we can use it.
    echo "[INFO] Adding ${MOOSEPATH} to PYTHONPATH"
    echo "export PYTHONPATH=${MOOSEPATH}:\$PYTHONPATH" >> \$HOME/.bash_profile
    source \$HOME/.bash_profile
fi
# make sure that for current runtime, we have correct path.
export PYTHONPATH=${MOOSEPATH}:\$PYTHONPATH
exec ${PORT_PREFIX}/bin/moosegui
EOF
    chmod a+x $PORT_PREFIX/moosegui
)

################ COPY THE .app ##########################################
## DO NOT USE APP BUNDLE.
## Use simple shell script to launch moose.
###echo "|| Copying the APP to directory"
####cp -rpf "${APPNAME}.app" "$PORT_PREFIX"
###APPEXE="${APPNAME}.app/Contents/MacOS/${APPNAME}"
###mkdir -p `dirname $APPEXE`
#### create the executable.
###cat > ${APPEXE} <<-EOF
####!/bin/bash
###${PORT_PREFIX}/bin/moosegui
###EOF
###chmod +x ${APPEXE}

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

echo "TODO. Now resize and compress using hdiutil"
echo "|| use: hdiutil convert a.dmg -format UDBZ -o b.dmg"

#### TODO: Resize the harddisk and compress it for distribution if tests are OK.
##set +e
##DISKSIZE=`du -sh /Volumes/"${VOLNAME}"`
##echo "Overall disk size is $DISKSIZE"
##echo "|| Blowing up port cache"
##rm -rf `${PORT_PREFIX}/bin/port --cache`
##
##DISKSIZE=`du -sh /Volumes/"${VOLNAME}"`

## Finally detach the device
detach_device
