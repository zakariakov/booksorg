%global owner zakariakov
%global commit #Write Commit Here

Name: booksorg
Version: 0.3.1
Release: 1%{?dist}
License: GPLv3
Summary: Books Organizer
URL: https://github.com/%{owner}/%{name}
Source0: https://github.com/%{owner}/%{name}/archive/%{commit}/%{name}-%{commit}.tar.gz
BuildRequires: gcc-c++
BuildRequires: qt5-devel
BuildRequires: dejavu-serif-fonts
BuildRequires: poppler-qt5-devel
BuildRequires: qt5-qtsvg-devel
Requires: qt5-qtbase

%description
 Books Organizer an organizer for pdf files based on sqlite and with a built-in reader. 
 Bring your favorite PDF pages all in one! Make your own extract pages from existing ones.

%prep
%setup -q -n %{name}-%{commit}

%build
%qmake_qt5
%make_build

%install
%make_install INSTALL_ROOT=%{buildroot}

# Register as an application to be visible in the software center
#
# NOTE: It would be *awesome* if this file was maintained by the upstream
# project, translated and installed into the right place during `make install`.
#
# See http://www.freedesktop.org/software/appstream/docs/ for more details.
#
mkdir -p $RPM_BUILD_ROOT%{_datadir}/appdata
cat > $RPM_BUILD_ROOT%{_datadir}/appdata/%{name}.appdata.xml <<EOF
<?xml version="1.0" encoding="UTF-8"?>
<!-- Copyright 2017 Mosaab Alzoubi <moceap@hotmail.com> -->
<!--
EmailAddress: moceap@hotmail.com
SentUpstream: 2017-2-18
-->
<application>
  <id type="desktop">%{name}.desktop</id>
  <metadata_license>CC0-1.0</metadata_license>
  <summary>Books Organizer</summary>
  <description>
    <p>
        Books Organizer an organizer for pdf files based on sqlite and with a built-in reader. 
        Bring your favorite PDF pages all in one! Make your own extract pages from existing ones.
    </p>
  </description>
  <url type="homepage">https://github.com/%{owner}/%{name}</url>
  <screenshots>
    <screenshot type="default">https://raw.githubusercontent.com/zakariakov/screenshots/master/booksorg.png</screenshot>
  </screenshots>
  <updatecontact>moceap@hotmail.com</updatecontact>
</application>
EOF


%files
%{_bindir}/%{name}
%{_datadir}/applications/%{name}.desktop
%{_datadir}/icons/hicolor/scalable/apps/%{name}.svg
%{_datadir}/appdata/%{name}.appdata.xml

%changelog
* Thu Oct 1 2020 Mosaab Alzoubi <moceap@hotmail.com> - 0.3.1-1
- Updated to 0.3.1
- Built for F33
- Use Qt5

* Mon Mar 27 2017 Mosaab Alzoubi <moceap@hotmail.com> - 0.3-2
- Imported from http://download.opensuse.org/repositories/home:/kovax/CentOS_CentOS-6/src
- Clean the spec
