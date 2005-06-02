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
make

%install
rm -fr %{buildroot}
mkdir -p %{buildroot}/usr/local/bglibs
make installer instcheck
./installer %{buildroot}
./instcheck %{buildroot}

cp -a doc %{buildroot}/usr/local/bglibs

%clean
rm -rf %{buildroot}

%files
%defattr(-,root,root)
%doc ANNOUNCEMENT COPYING NEWS README
/usr/local/bglibs
