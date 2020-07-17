#
# Makefile --Build rules for travesty.
#
PACKAGE = travesty
package-type = rpm deb
language = c nroff 

C_MAIN_SRC = travesty.c
C_SRC = travesty.c
MAN1_SRC = travesty.1

include makeshift.mk version.mk package.mk

$(C_MAIN): -lxtd

install: install-all
uninstall: uninstall-all
