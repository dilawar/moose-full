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

echo "|WARN| Unsetting PATH"
unset PATH
export PATH=/usr/bin:/bin:/usr/sbin:/sbin:/usr/X11/bin

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
DMG_TMP="${PKGNAME}-${MAC_NAME}_LOCAL.dmg"
mkdir -p ${STAGING_DIR}

SIZE=1G
echo "|| Creating dmg file of $SIZE size"
if [ ! -f "${DMG_TMP}" ]; then
    hdiutil create -srcfolder "${STAGING_DIR}" -volname "${PKGNAME}" \
        -format UDRW -size $SIZE "${DMG_TMP}"
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
    WORKDIR=/tmp/_work
    mkdir -p $WORKDIR
    cd $PORT_PREFIX
    ## NOTICE: Try to build using Xcode.

    # 1. Get cmake and install it to cmake/bin 
    CMAKE_BIN=$PORT_PREFIX/cmake/bin/cmake
    if [ -f $CMAKE_BIN ]; then
        echo "|| cmake is available at $CMAKE_BIN "
    else
        echo "No $CMAKE_BIN found. Building one"
        cd $WORKDIR
        curl -O https://cmake.org/files/v3.3/cmake-3.3.2.tar.gz
        tar xvf cmake-3.3.2.tar.gz
        cd cmake-3.3.2 || ls -l
        ./bootstrap
        ./configure --prefix=$PORT_PREFIX/cmake
        make -j3
        make install
        cd $PORT_PREFIX
    fi

    # 2. Install moose-full using cmake.
    MOOSE_PREFIX=$PORT_PREFIX/moose
    export PYTHONPATH=$MOOSE_PREFIX/lib/python2.7/site-packages
    if python -c 'import moose'; then
        echo "|STATUS| MOOSE is installed and could be imported"
    else
        cd $WORKDIR
        if [ ! -d moose-full ]; then
            echo "Cloning moose-full"
            git clone --depth 1 https://github.com/BhallaLab/moose-full
        else
            cd moose-full && git pull && cd ..
        fi
        set -e
        cd moose-full
        mkdir -p _build && cd _build
        $CMAKE_BIN -DCMAKE_INSTALL_PREFIX=$MOOSE_PREFIX .. 
        make -j3
        # Override the install step.
        cd ../moose-core/python && python setup.py install --prefix=$MOOSE_PREFIX
        python -c 'import moose'
        set +e
        cd $WORKDIR
        cd ..
    fi

    # 3. Let's get PyQt4 and openscenegraph.
    QTPREFIX=$PORT_PREFIX/qt4.8
    mkdir -p $QTPREFIX
    (
        cd $WORKDIR
        QTDIR="qt-everywhere-opensource-src-4.8.7"
        if [ ! -f "${QTDIR}.tar.gz" ]; then
            echo "|| Downloading qt"
            curl -O http://master.qt.io/official_releases/qt/4.8/4.8.7/$QTDIR.tar.gz
        else
            echo "|| Qt is already downloaded"
        fi
        if [ ! -d "$QTDIR" ]; then
            tar xvf $QTDIR.tar.gz
        else
            echo "||| Qt is already extracted to $QTDIR"
        fi
        cd $QTDIR
        # Delete all license files so qt it does not prompt us to accept
        # license.
        if [ ! -d $QTPREFIX/lib ]; then
            ./configure --prefix=$QTPREFIX -silent -opensource -stl -no-qt3support \
                -confirm-license -nomake examples -nomake demos
            make -j3
            make install
        else
            echo "||| Qt seems to be installed. Here are the list of libs"
            ls -lh "$QTPREFIX/lib"
        fi
    )

    # 4. Install SIP 
    SIPPREFIX=$PORT_PREFIX/sip
    mkdir -p $SIPPREFIX
    (
        cd $WORKDIR
        SIPHEAD=0cbb680b4f69
        if [ ! -f $SIPHEAD.tar.gz ]; then
            echo "|| Downloading SIP"
            curl -O "https://www.riverbankcomputing.com/hg/sip/archive/$SIPHEAD.tar.gz"
        else
            echo "|| SIP is already downloaded"
        fi
        if [ ! -d sip-$SIPHEAD ]; then
            echo "||| Extracting SIP"
            tar xvf $SIPHEAD.tar.gz 
        else
            echo "||| Already extracted"
        fi

        cd sip-$SIPHEAD
        if [ -f $SIPPREFIX/bin/sip ]; then
            echo "|| sip is already installed"
        else
            echo "|| installing sip"
            python build.py prepare
            python configure.py -b $SIPPREFIX/bin -d $SIPPREFIX/lib/python2.7 \
                -e $SIPPREFIX/include -v $SIPPREFIX/include/sip
            make -j3
            make install
        fi

    )
        


    ##||| Install startup scripts.
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

## Finally detach the device
detach_device
