%global branch 3.0.2
%global version 3.0.2
%define _unpackaged_files_terminate_build 0 
Name: moose-src
Group: Applications/Biology
Summary: Neuronal modeling software spanning molecules, electrophysiology and networks

Version: 3.0.2
Release: 1%{?dist}
Url: http://github.com/BhallaLab/moose
Source0: moose-%{version}.tar.gz

License: GPL-3.0

BuildRequires: gsl-devel
BuildRequires: gcc-c++
BuildRequires: cmake
BuildRequires: python-devel
BuildRequires: libbz2-devel
BuildRequires: numpy
BuildRequires: libxml2-devel

%description

MOOSE is the Multiscale Object-Oriented Simulation Environment.  It is designed
to simulate neural systems ranging from subcellular components and biochemical
reactions to complex models of single neurons, circuits, and large networks.
MOOSE can operate at many levels of detail, from stochastic chemical
computations, to multicompartment single-neuron models, to spiking neuron
network models.

MOOSE is a simulation environment, not just a numerical engine. It provides the
essentials by way of object-oriented representations of model concepts and fast
numerical solvers, but its scope is much broader.  It has a scripting interface
with Python, graphical displays with Matplotlib, PyQt, and OpenGL, and support
for many model formats.

%package -n moose
Summary: MOOSE simulator with python binding and GUI
Group: Application/Biology
Conflicts: moose-core moose-gui moose-python
%description -n moose
This is full package of MOOSE simulator. Its contains python bindings and GUI.

Requires: libbz2
Requires: libxml2
Requires: gsl
Requires: bzip2
Requires: python-matplotlib
Requires: numpy
Requires: moose-core
Requires: PyQt4
Requires: moose-python
Requires: python-networkx
Requires: python-suds


%package -n moose-core
Summary: MOOSE simulator library.
Group: Applications/Biology
%description -n moose-core
This package contains C++ core of MOOSE simulator. For general use you should
install moose-python.

Requires: libbz2
Requires: libxml2
Requires: gsl
Requires: bzip2

%package -n moose-python
Summary: Python interface of MOOSE
Group: Applications/Biology
Conflicts: moose
%description -n moose-python
This package contains python interface of MOOSE simulator.

Requires: python-matplotlib
Requires: numpy
Requires: moose-core

%package -n moose-gui
Summary: GUI frontend
Group: Applications/Biology
Conflicts: moose
%description -n moose-gui
GUI frontend of MOOSE simulator.

Requires: PyQt4
Requires: moose-python
Requires: python-networkx
Requires: python-suds

%prep
%setup -q -n moose-%{version}

%build
mkdir -p _build
cd _build
cmake -DWITH_DOC=OFF -DCMAKE_INSTALL_PREFIX=%{buildroot}/usr .. && make -j`nproc`

%install
cd _build && make install && cd ..
mkdir -p %{buildroot}/usr/lib/moose/gui
cp -r moose-gui/* %{buildroot}/usr/lib/moose/gui/
install package_data/moosegui %{buildroot}/usr/bin/
# Now install the python module.
cd moose-core/python && python setup.py install \
    --prefix=%{_prefix} --root=%{buildroot}
# Note: direct installation causes following
# http://lists.opensuse.org/opensuse-factory/2012-01/msg00235.html
find "%{buildroot}%{python_sitelib}/" -name "*.pyc" -exec %__rm {} \;
%__python -c 'import compileall; 
compileall.compile_dir("%{buildroot}/%{python_sitelib}/"
    , ddir="%{python_sitelib}/", force=1)'

%files -n moose-core
/usr/bin/moose.bin
/usr/bin/moose
/usr/lib/*.so

%files -n moose-python
%defattr(-,root,root)
%{python_sitelib}/*

%files -n moose-gui
%defattr(-,root,root)
%dir /usr/lib/moose
%dir /usr/lib/moose/gui
%dir /usr/lib/moose/moose-examples
/usr/lib/moose/gui
/usr/bin/moosegui
/usr/lib/moose/moose-examples

%files -n moose
%defattr(-,root,root)
/usr/bin/moose.bin
/usr/lib/libmoose.so
%defattr(-,root,root)
%{python_sitelib}/*
%dir /usr/lib/moose
%dir /usr/lib/moose/gui
%dir /usr/lib/moose/moose-examples
/usr/lib/moose/gui
/usr/bin/moosegui
/usr/lib/moose/moose-examples
