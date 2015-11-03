#!/bin/bash
echo "Updating moose branches in subtree"
(
git subtree pull --prefix moose-core https://github.com/BhallaLab/moose-core master --squash
git subtree pull --prefix moose-gui https://github.com/BhallaLab/moose-gui master --squash
git subtree pull --prefix moose-examples https://github.com/BhallaLab/moose-examples master --squash
)
