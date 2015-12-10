#!/bin/bash
# Build packaing using osc.
set -x
specs=`find . -maxdepth 1 -type f -name "*.spec"`
## Download source code if not exists.
if [ ! -f ./_service:recompress:tar_scm:moose-3.0.2.tar.gz ]; then
    echo "Downloading source code"
    osc service run
else
    echo "Using old source code"
fi

for spec in $specs; do 
    echo "Building for $spec"
    repo=${spec/.\/moose-/}
    repo=${repo/.spec/}
    osc build --noservice --ccache $repo $spec  | tee __build__.log
    if [ $? -eq 0 ]; then
        echo "$spec" >> __success_spec__
    else
        echo "$spec" >> __failed_spec__
    fi
done
