#!/bin/bash
echo "Adding master branch in subtree"
(
    git subtree add  --prefix moose-core https://github.com/BhallaLab/moose-core master --squash
    git subtree add  --prefix moose-gui https://github.com/BhallaLab/moose-gui master --squash
    git subtree add  --prefix moose-examples https://github.com/BhallaLab/moose-examples master --squash
)
