%global branch 3.0.2
%global version 3.0.2
%define _unpackaged_files_terminate_build 0 
Name: moose-src
Group: Applications/Biology
Summary: Neuronal modeling software spanning molecules, electrophysiology and networks

Version: 3.0.2
Release: 1%{?dist}
Url: http://sourceforge.net/projects/moose
Source0: moose-%{version}.tar.gz

License: GPL-3.0

BuildRequires: gsl-devel
BuildRequires: gcc-c++
BuildRequires: cmake
BuildRequires: python-devel
BuildRequires: python-setuptools
BuildRequires: libbz2-devel
BuildRequires: numpy
BuildRequires: libxml2-devel
%if 0%{?openscenegraph_dist}

%endif

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

MOOSE can read kinetic models in SBML and GENESIS kkit formats, from
BioModels.net and DOQCS. MOOSE also supports electrical models specified in
NeuroML and Genesis .p formats, and can load over 30,000 morphology files from
NeuroMorpho.org (.swc format).

%package -n moose
Summary: MOOSE simulator.
Group: Application/Biology
%description -n moose
This is meta package of MOOSE simulator. Its contains python bindings and GUI.

Requires: libbz2
Requires: libxml2
Requires: bzip2
Requires: python-matplotlib
Requires: numpy
Requires: PyQt4
Requires: python-networkx
Requires: python-suds

%prep
%setup -q -n moose-%{version}

%build
%if 0%{?openscenegraph_dist} 
cmake -DWITH_DOC=OFF -DBUILD_MOOGLI=TRUE  -DCMAKE_INSTALL_PREFIX=%{buildroot}/usr ..  && make -j`nproc`
%else
cmake -DWITH_DOC=OFF -DCMAKE_INSTALL_PREFIX=%{buildroot}/usr .. && make -j`nproc`
%endif

%install
make install
mkdir -p packaging_scripts/usr/lib/moose/gui
cp -r ../moose-gui/* packaging_scripts/usr/lib/moose/gui/
install ../package_data/moosegui %{buildroot}/usr/bin/
# Now install the python module.
cd ../moose-core/python/ && python setup.py install --prefix=%{_prefix} --root=%{buildroot}

%files -n moose
%defattr(-,root,root)
%dir usr/share/moose
%{python_sitearch}/*
%defattr(-,root,root)
%dir usr/lib/moose
%dir usr/lib/moose/gui
%dir usr/lib/moose/moose-examples
%dir usr/share/icons/moose
usr/lib/moose/gui
usr/bin/moosegui
usr/lib/moose/moose-examples
usr/share/applications/moose.desktop
usr/share/icons/moose/moose.png
