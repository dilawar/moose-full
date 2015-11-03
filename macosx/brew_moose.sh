#!/bin/bash
set -x

rm -f /Library/Caches/Homebrew/moose*.tar.gz
rm -f /Library/Caches/Homebrew/moose*.zip
cp ./moose.rb /usr/local/Library/Formula/moose.rb

brew uninstall moose
brew -v install moose --debug | tee _build.log 

echo "Checking brew script for submission"
brew audit --strict moose
brew -v test moose
