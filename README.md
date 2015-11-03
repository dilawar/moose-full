[![Build Status](https://travis-ci.org/BhallaLab/moose-full.svg)](https://travis-ci.org/BhallaLab/moose-full)

This repository is used to build MOOSE packages.

In addition to MOOSE and MOOGLI source code, this repository also contains
source code of essential dependencies `libsbml` and `gsl`.

If you are looking for latest MOOSE source code, [go
here](https://github.com/BhallaLab/moose).


# Debian/Ubuntu

Launchpad is a great place to build debian packages. If you are Ubuntu user,
here is our [PPA](https://launchpad.net/~bhallalab/+archive/ubuntu/moose)

For detailed instruction on how to use ppa, [see this answer](http://askubuntu.com/questions/4983/what-are-ppas-and-how-do-i-use-them)

Or alternatively, just do the following in a terminal.


~~~
sudo -E add-apt-repository ppa:bhallalab/moose -y
sudo apt-get update
sudo apt-get install moose-gui moose-python 
~~~

If you also want `moogli` (under developement).

~~~~
sudo apt-get install moogli
~~~~


# RPM

RPM packages for various Linux distributions can be found at [Open Build Service](http://software.opensuse.org/download.html?project=home%3Amoose&package=moose). This page describes how to install packages 
on your distribution.


# Bugs etc

Please report them here or write to `dilawars@ncbs.res.in`.
