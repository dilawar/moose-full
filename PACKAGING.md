# Packaging

MOOSE packages has the following directory structure w.r.t to prefix which is
`/usr` by default.

    bin/moose.bin
    bin/moosegui
    share/moose/moose-$(VERSION).tar.gz
    share/doc/moose 

The postinst scripts uses `python-pip` to install python bindings from
moose-$(VERSION).tar.gz file. This file is create by `python setup.py sdist`
command. It contains `_moose.so` along with python files required for python
interface to work.

`prerm` scripts must remove all installed files.
