#
# TRAVESTY.SPEC --RPM build configuration for travesty.
#
%define debug_package %{nil}
Summary: A semi-random text scrambler.
Name: %{name}
Version: %{version}
Release: %{release}
License: MIT, Copyright (c) 2015 Tim Rose
URL: git@github.com:tim-rose/travesty.git
Group: Miscellaneous/Utilities
Packager: Timothy Rose <tim.rose@acm.org>
AutoReqProv: no

%description
Travesty applies character sequence probabilities of its input
text to generate random text.
