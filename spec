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
echo /usr/local/bglibs/include >conf-include
echo /usr/local/bglibs/lib >conf-lib

%build
make

%install
rm -fr %{buildroot}
mkdir -p %{buildroot}/usr/local/bglibs/{include,lib}
make install_prefix=%{buildroot} install

cp -a doc %{buildroot}/usr/local/bglibs

%clean
rm -rf %{buildroot}

%files
%defattr(-,root,root)
%doc ANNOUNCEMENT COPYING NEWS README
/usr/local/bglibs
