%global branch 3.0.2

%global version 3.0.2

%define _unpackaged_files_terminate_build 0 
Name: moose-src
Group: Applications/Biology
Summary: MOOSE is the Multiscale Object-Oriented Simulation Environment.


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

MOOSE is the Multiscale Object-Oriented Simulation Environment.

It is designed to simulate neural systems ranging from subcellular components
and biochemical reactions to complex models of single neurons, circuits, and
large networks. MOOSE can operate at many levels of detail, from stochastic
chemical computations, to multicompartment single-neuron models, to spiking
neuron network models.

MOOSE is a simulation environment, not just a numerical engine. It provides the
essentials by way of object-oriented representations of model concepts and fast
numerical solvers, but its scope is much broader.  It has a scripting interface
with Python, graphical displays with Matplotlib, PyQt, and OpenGL, and support
for many model formats.

MOOSE can read kinetic models in SBML and GENESIS kkit formats, from
BioModels.net and DOQCS. MOOSE also supports electrical models specified in
NeuroML and Genesis .p formats, and can load over 30,000 morphology files from
NeuroMorpho.org (.swc format). 


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

%prep
%setup -q -n moose-%{version}

%build
mkdir -p _build
cd _build
cmake -DWITH_DOC=OFF -DCMAKE_INSTALL_PREFIX=%{buildroot}/usr .. 
make -j`nproc`

%install
cd _build && make install && cd ..
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
