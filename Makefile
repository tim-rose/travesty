#
# Makefile --Build rules for travesty.
#
BUILD_PATH = ../apex/libapex
PACKAGE = travesty
#package-type = rpm deb
package-type = deb
language = c nroff 

C_MAIN_SRC = travesty.c
C_SRC = travesty.c
MAN1_SRC = travesty.1

include makeshift.mk package.mk

$(C_MAIN): -lapex

install: install-all
uninstall: uninstall-all
