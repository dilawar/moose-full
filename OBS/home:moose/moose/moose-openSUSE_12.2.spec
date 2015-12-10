%global branch 3.0.2
%define _unpackaged_files_terminate_build 0 
Name: moose
Group: Applications/Biology
Summary: MOOSE is the Multiscale Object-Oriented Simulation Environment
Version: 3.0.2
Release: 1%{?dist}
Url: http://sourceforge.net/projects/moose
Source0: moose-%{branch}.tar.gz

License: GPL-3.0

BuildRequires: gcc-c++
BuildRequires: cmake
BuildRequires: python-devel
BuildRequires: python-setuptools
BuildRequires: libxml2-devel
BuildRequires: libbz2-devel
BuildRequires: doxygen
BuildRequires: openmpi-devel
BuildRequires: numpy
BuildRequires: python-qt4
BuildRequires: python-qt4-devel
BuildRequires: python-sip-devel
BuildRequires: libOpenSceneGraph1

%description
MOOSE is the base and numerical core for large, detailed simulations
including Computational Neuroscience and Systems Biology. MOOSE spans
the range from single molecules to subcellular networks, from single
cells to neuronal networks, and to still larger systems. It is
backwards-compatible with GENESIS, and forward compatible with Python
and XML-based model definition standards like SBML and NeuroML.

MOOSE uses Python as its primary scripting language. For backward
compatibility we have a GENESIS scripting module, but this is
deprecated. MOOSE uses Qt/OpenGL for its graphical interface. The
entire GUI is written in Python, and the MOOSE numerical code is
written in C++.

Requires: moose-gui
Requires: moose-python
Requires: moose-doc
%if 0%{?fedora} 
Requires: moose-moogli
%endif

%package -n moose-python
Summary: Python-2 interface for %{name}
%description -n moose-python
This package contains python interface of MOOSE simulator.

Requires: python-matplotlib-qt4
Requires: libxml2
Requires: bzip2
Requires: python-networkx

%package -n moose-gui
Summary: GUI frontend
%description -n moose-gui
GUI frontend. It uses openscenegraph to visualize neural networks.

Requires: PyQt4
Requires: PyOpenGL
Requires: moose-python

%package -n moose-doc
Summary: MOOSE documentation
%description -n moose-doc
This package contains user and developer documentation.

%if 0%{?fedora} 

%package -n moose-moogli
Summary: Visualizer for neural simulators
%description -n moose-moogli
Moogli (a sister project of MOOSE) is a simulator independent openGL based
visualization tool for neural simulations. Moogli can visualize morphology of
single/multiple neurons or network of neurons, and can also visualize activity
in these cells.

Requires: OpenSceneGraph

%endif

%prep
%setup -q -n moose-%{branch}

%build
mkdir -p _build
%if 0%{?fedora} 
cd _build && cmake -DBUILD_MOOGLI=TRUE .. && make 
%else
cd _build && cmake -DBUILD_MOOGLI=FALSE .. && make
%endif

%install
cd _build && make install DESTDIR=$RPM_BUILD_ROOT

%files -n moose

%files -n moose-python
%defattr(-,root,root)
%dir %{_prefix}/lib/moose
%{_prefix}/lib/moose/moose
%{_prefix}/lib/moose/libmumbl
%{_prefix}/lib/moose/setup.py

%post -n moose-python
mkdir -p /etc/moose
cd %{_prefix}/lib/moose && python setup.py install --record /etc/moose/installed_files.txt

%preun -n moose-python
cd /etc/moose && cat installed_files.txt | xargs rm -rf
if [ -d /etc/moose ]; then
    rm -rf /etc/moose
fi
if [ -d %{_prefix}/lib/moose ]; then
    rm -rf ${_prefix}/lib/moose 
fi

%files -n moose-gui
%defattr(-,root,root)
%{_bindir}/moosegui
%dir %{_prefix}/lib/moose
%dir %{_prefix}/lib/moose/gui
%{_prefix}/lib/moose/gui

%files -n moose-doc
%{_prefix}/share/doc/moose

%if 0%{?fedora}
%files -n moose-moogli
%dir %{_prefix}/lib/moogli
%dir %{_prefix}/lib/moogli/moogli
%{_prefix}/lib/moogli/moogli/moogli.so
%{_prefix}/lib/moogli/moogli/__init__.py
%{_prefix}/lib/moogli/setup-moogli.py 

%post -n moose-moogli
mkdir -p /etc/moogli
cd ${_prefix}/lib/moogli && python setup-moogli.py install --record /ect/moogli/installed_files.txt

%preun -n moose-moogli
cd /etc/moogli && cat installed_files.txt | xargs rm -rf
if [ -d /etc/moogli ]; then
    rm -rf /etc/moogli
fi
if [ -d %{_prefix}/lib/moogli ]; then
    rm -rf %{_prefix}/lib/moogli 
fi
%endif
