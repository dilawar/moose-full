#!/bin/bash
set -e
set -x
brew install moose
brew install moogli
sudo hdiutil create ~/Desktop/moose-3.0.2.dmg -srcdevice /usr/local -volname MOOSE
