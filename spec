Name: @PACKAGE@
Summary: BG Libraries Collection
Version: @VERSION@
Release: 1
Copyright: GPL
Group: Development/Libraries
Source: http://untroubled.org/@PACKAGE@/@PACKAGE@-@VERSION@.tar.gz
BuildRoot: %{_tmppath}/@PACKAGE@-buildroot
#BuildArch: noarch
URL: http://untroubled.org/@PACKAGE@/
Packager: Bruce Guenter <bruceg@em.ca>

%description
BG Libraries Collection

%prep
%setup
echo gcc "%{optflags}" -g >conf-cc
echo gcc -g -L. >conf-ld

%build
make libraries

%install
rm -fr %{buildroot}
#make install_prefix=%{buildroot} bindir=%{_bindir} mandir=%{_mandir} install
rm -f conf_bin.c insthier.o installer instcheck
echo %{buildroot}/usr/local/include/bglibs >conf-inc
echo %{buildroot}/usr/local/lib/bglibs >conf-lib
make installer instcheck

mkdir -p %{buildroot}/usr/local/{include,lib}/bglibs
./installer
./instcheck

%clean
rm -rf %{buildroot}

%files
%defattr(-,root,root)
%doc ANNOUNCEMENT COPYING NEWS README
/usr/local/include/bglibs
/usr/local/lib/bglibs
