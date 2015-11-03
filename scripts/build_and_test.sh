#!/bin/bash
mkdir -p _build
(
    set -e
    cd _build
    cmake -DBUILD_MOOGLI=ON .. | tee __configure__.log
    make -j4  | tee __build__.log
    ctest --output-on-failure | tee __test__.log
)
