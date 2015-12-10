#!/bin/bash
# Build packaing using osc.
set -x
set -e
echo "Building for repo $1"
if [ ! -f ./_service:recompress:tar_scm:moose-3.0.2.tar.gz ]; then
    osc service run
fi
if [[ "$1" == *"Debian"* ]]; then
    osc build --noservice $1 --no-verify  | tee __build_$1.log
elif [[ "$1" == *"Ubuntu"* ]]; then
    osc build --noservice $1 --no-verify  | tee __build_$1.log
else
    osc build --noservice $1 moose-$1.spec --no-verify | tee __build_$1.log
fi
