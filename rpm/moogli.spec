%global branch 0.5.0
%global version 0.5.0
%define _unpackaged_files_terminate_build 0 
Name: moogli-src
Group: Applications/Biology
Summary: 3D neuronal network activity visualizer

Version: 0.5.0
Release: 1%{?dist}
Url: http://github.com/aviralg/moogli
Source0: moogli-%{version}.tar.gz

License: GPL-3.0

BuildRequires: gcc-c++
BuildRequires: cmake
BuildRequires: python-devel
BuildRequires: python-setuptools
BuildRequires: qt-devel
BuildRequires: PyQt4-devel

%description

Moogli is a 3D neuronal network activity visualizer. It can show neuronal network morphologies and visualize electrical and chemical activity from compartmental modeling simulations.

%package -n moogli
Summary: 3D neuronal network activity visualizer.
Group: Application/Biology
%description -n moogli

Moogli is a 3D neuronal network activity visualizer. It can show neuronal network morphologies and visualize electrical and chemical activity from compartmental modeling simulations.

Requires: moose-gui
Requires: PyQt4

%prep
%setup -q -n moogli-%{version}

%build
cmake -DCMAKE_INSTALL_PREFIX=%{buildroot}/usr . 
make -j`nproc`

%install
python moogli/cmake_modules/setup.py install --prefix=%{_prefix} \
    --root=%{buildroot}

%files -n moogli
%defattr(-,root,root)
%{python2_sitelib}/*

