#!/bin/bash
set -x
export PYTHONPATH=../moose-core/python:../moogli
python -c 'import moose'
python -c 'import moogli'
