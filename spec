Name: @PACKAGE@
Summary: BG Libraries Collection
Version: @VERSION@
Release: 1
License: GPL
Group: Development/Libraries
Source: http://untroubled.org/%{name}/%{name}-%{version}.tar.gz
BuildRoot: %{_tmppath}/%{name}-buildroot
BuildRequires: libtool
URL: http://untroubled.org/%{name}/
Packager: Bruce Guenter <bruce@untroubled.org>

%description
BG Libraries Collection

%package devel
Summary: Development includes and libraries for bglibs
Requires: bglibs = %{version}
Group: Development/Libraries

%description devel
The bglibs-devel package contains the header and object files necessary
for developing programs which use the bglibs libraries.

%prep
%setup
echo gcc "%{optflags}" -g >conf-cc
echo gcc -g -L. >conf-ld
echo %{_includedir} >conf-include
echo %{_libdir} >conf-lib
echo %{_bindir} >conf-bin
echo %{_mandir} >conf-man

%build
make

%install
rm -fr %{buildroot}
make install_prefix=%{buildroot} install

%clean
rm -rf %{buildroot}

%files
%defattr(-,root,root)
%doc ANNOUNCEMENT COPYING NEWS README
%{_libdir}/*.so.*
%{_bindir}/bg-installer

%files devel
%defattr(-,root,root)
%{_includedir}/bglibs
%{_libdir}/*.a
%{_libdir}/*.la
%{_libdir}/*.so
%{_libdir}/*/*.a
%{_bindir}/cli-generate
%{_bindir}/crc-gentab
%{_mandir}/*/*
