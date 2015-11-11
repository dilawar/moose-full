#!/bin/bash
set -x
URL=http://download.opensuse.org/repositories/home:/moose/
echo "Trying $URL"
wget -r -np --spider -Arpm,deb $URL
