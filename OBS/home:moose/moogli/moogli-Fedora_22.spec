%global branch 0.5.0
%global version 0.5.0
%define _unpackaged_files_terminate_build 0 
Name: moogli-src
Group: Applications/Biology
Summary: A visualization tool for neural simulations and morphology
Version: 0.5.0
Release: 1%{?dist}
Url: https://moose.ncbs.res.in/moolgi
Source0: moogli-%{version}.tar.gz

License: GPL-3.0

BuildRequires: cmake
BuildRequires: gcc-c++
BuildRequires: PyQt4-devel
BuildRequires: PyQt
BuildRequires: qt-devel
BuildRequires: sip-devel

%description
Moogli s a simulator independent openGL based visualization tool for neural
simulations.

Moogli (a sister project of MOOSE) is a simulator independent openGL based
visualization tool for neural simulations. Moogli can visualize morphology of
single/multiple neurons or network of neurons, and can also visualize activity
in these cells.

Moogli is like VLC player for neural simulations.

Requires: moose-gui

%package -n moogli
Summary: Visualizer for neural simulators
%description -n moogli
Moogli (a sister project of MOOSE) is a simulator independent openGL based
visualization tool for neural simulations. Moogli can visualize morphology of
single/multiple neurons or network of neurons, and can also visualize activity
in these cells.

Requires: OpenSceneGraph
Requires: python-qt4


%prep
%setup -q -n moogli-%{version}

%build
mkdir -p _build
cd _build && cmake .. && make -j`nproc`

%install
python moogli/cmake_modules/setup.py install --prefix=%{_prefix} --root=%{buildroot}

%files -n moogli
%defattr(-,root,root)
%{python_sitelib}/*

%changelog
