#!/bin/bash
set -e
set -x
sudo brew install moose
sudo brew install moogli
sudo hdiutil create ~/Desktop/moose-3.0.2.dmg -srcdevice /usr/local -volname MOOSE
