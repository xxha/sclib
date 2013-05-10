Name:           harctl
Version:        1.0
Release:        2
Summary:        Harman Boot API Control Program

License:        Copyright Wind River

Source:		%{name}-%{version}-%{release}.tar.bz2

Group:          Applications/System

%description
This is a program that implements the Harman Boot API.
It allows for control of the boot process within the
Harman x-loader.

%prep
%setup -q -c %{name}


%build
make %{?_smp_mflags}


%install
rm -rf $RPM_BUILD_ROOT
mkdir -p %{buildroot}%{_bindir}
install -m 755 harctl %{buildroot}%{_bindir}/harctl


%files
%{_bindir}/harctl


%doc


%changelog
