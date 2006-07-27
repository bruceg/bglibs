%define basedir /usr/local/bglibs
%define _libdir %{basedir}/lib
%define _includedir %{basedir}/include

Name: @PACKAGE@
Summary: BG Libraries Collection
Version: @VERSION@
Release: 1
License: GPL
Group: Development/Libraries
Source: http://untroubled.org/@PACKAGE@/@PACKAGE@-@VERSION@.tar.gz
BuildRoot: %{_tmppath}/@PACKAGE@-buildroot
BuildRequires: libtool
URL: http://untroubled.org/@PACKAGE@/
Packager: Bruce Guenter <bruce@untroubled.org>
Provides: bglibs-devel = %{version}

%description
BG Libraries Collection

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

cp -a doc %{buildroot}%{basedir}

%clean
rm -rf %{buildroot}

%post
fgrep -x /usr/local/bglibs/lib /etc/ld.so.conf >/dev/null \
|| echo /usr/local/bglibs/lib >>/etc/ld.so.conf
/sbin/ldconfig

%files
%defattr(-,root,root)
%doc ANNOUNCEMENT COPYING NEWS README
%{basedir}
%{_bindir}/*
%{_mandir}/*/*
