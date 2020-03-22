#
# Makefile --Build rules for travesty.
#
language = c nroff 

C_MAIN_SRC = travesty.c
C_SRC = travesty.c
MAN1_SRC = travesty.1

include devkit.mk

$(C_MAIN): -lxtd

install: install-all
uninstall: uninstall-all
