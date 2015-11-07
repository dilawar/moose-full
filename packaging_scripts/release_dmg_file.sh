#!/bin/bash
set -x

LABEL=Moose_3.0.2

# Not all build dependencies are required at runtime. Lets just disable them.
DMGFILE="$1"
if [ ! $DMGFILE ]; then
    echo "USAGE: $0 dmg_file"
    exit
fi

TEMPDMG="$DMGFILE"_temp.dmg
rm -f $TEMPDMG
cp $DMGFILE $TEMPDMG

echo "|| Detaching dmg file"
hdiutil detach /Volumes/$LABEL

echo "|| Mouting DMG file"
hdiutil attach $TEMPDMG

(
    echo "Removing the temp files"
    cd /Volumes/$LABEL
    du -sh /Volumes/$LABEL
    ./bin/brew uninstall cmake gcc
    du -sh /Volumes/$LABEL
)

# Create another image from this folder.
OUTFILE="$DMGFILE"_RW.dmg
rm -f $OUTFILE
hdiutil create -volname $LABEL -srcfolder /Volumes/$LABEL \
    -ov -format UDRW "$OUTFILE"

ls -lh *.dmg

echo "|| Detaching .."
hdiutil detach /Volumes/$LABEL

echo "|| Compressing "
hdiutil convert "$OUTFILE" -format UDBZ -o "$LABEL"_OSX.dmg

