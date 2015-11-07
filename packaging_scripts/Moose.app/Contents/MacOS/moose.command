#!/bin/bash

APPPATH="/Volumes/Moose_3.0.2"
MOOSEPATH="$APPPATH/lib/python2.7/site-packages/moose"

if grep -q "$MOOSEPATH" "$HOME/.bash_profile" ; then
    continue
else
    display notification  "Adding ${MOOSEPATH} to your PYTHONPATH. This happens only once." with title \
        "Setting up PYTHONPATH"
    echo "export PYTHONPATH=${MOOSEPATH}:$PYTHONPATH" >> $HOME/.bash_profile
    source $HOME/.bash_profile
fi

( cd ${APPPATH}/lib/moose/moose-gui && python mgui.py )
