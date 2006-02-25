#=======================================================================
# Makefile for files in /usr/local/src/prescript/prescript-2.2/
#
# [02-Aug-1999]
#=======================================================================
# Program definitions

PROGRAM		= prescript
PROGLIB		= $(PWD)
VERSION		= 2.2

#=======================================================================
# Installation locations

prefix		= /usr/local
bindir		= $(prefix)/bin
catdir		= $(prefix)/man/cat$(manext)
libdir		= $(libtop)/$(PROGRAM)-$(VERSION)
libtop		= $(prefix)/$(PROGLIB)/
mandir		= $(prefix)/man/man$(manext)
manext		= 1
shrlibdir	= $(libtop)/$(PROGRAM)-$(VERSION)
shrlibtop	= $(prefix)/share/$(PROGLIB)/

#=======================================================================
# Macro definitions

#PWD             =`pwd`

CAT		= cat

CHMOD		= chmod

CP		= /bin/cp -p
CP		= rcp -p

DETEX		= detex

DETEXFLAGS	= -m -n -s

DW		= dw

ISPELL		= ispell

LIBFILES	= /dev/null

LN		= ln

LS		= ls

MKDIR		= mkdir -p

MV		= /bin/mv

RM		= /bin/rm -f

SED		= sed

SHELL		= /bin/sh

SHRLIBFILES	= /dev/null

SORT		= sort

SPELL		= spell

SPELLFILTER	= $(CAT)
SPELLFILTER	= $(SED) -e 's/[0-9,.:;?&]/ /g'

#=======================================================================
# Compiler flags and source files

#=======================================================================
# Suffixes and extra rule(s)

#=======================================================================
# Targets:

all:
	@echo There is nothing to build in this directory; try \"make -n install\"

check:
	@echo 'There is no validation suite for this program (yet)'

clean:
	-$(RM) *.dw
	-$(RM) *.i
	-$(RM) *.o
	-$(RM) *.ser
	-$(RM) *~
	-$(RM) \#*
	-$(RM) a.out
	-$(RM) core

clobber:	distclean

distclean:	mostlyclean
	-$(RM) $(PROGRAM)

install:	install-exe install-lib install-shrlib install-man install-show

install-exe:	uninstall-exe
	$(SED) -e "s@/usr/local/src/prescript/prescript-2.2@$(PWD)@" <$(PROGRAM).sh >$(bindir)/$(PROGRAM)
	$(LN) $(bindir)/$(PROGRAM) $(bindir)/$(PROGRAM)-$(VERSION)
	$(CHMOD) 775 $(bindir)/$(PROGRAM) $(bindir)/$(PROGRAM)-$(VERSION)

install-lib:	uninstall-lib
##	$(MKDIR) $(libtop)
##	@if test -d $(libtop) ; then true ; else echo Cannot create $(libtop) ; exit 1 ; fi
##	$(MKDIR) $(libdir)
##	@if test -d $(libdir) ; then true ; else echo Cannot create $(libdir) ; exit 1 ; fi
##	$(CP) $(LIBFILES) $(libdir)/
##	$(CHMOD) 664 $(libdir)/*

install-man:	uninstall-man
	$(CP) $(PROGRAM).man $(mandir)/$(PROGRAM).$(manext)
	$(CHMOD) 664 $(mandir)/$(PROGRAM).$(manext)

install-show:
	@echo ''
	@echo Installed files...
	@$(LS) -l $(bindir)/$(PROGRAM) $(bindir)/$(PROGRAM)-$(VERSION) \
		$(mandir)/$(PROGRAM).$(manext)
	@if test -d $(libdir) ; then $(LS) -lR $(libdir) ; fi
	@echo ''

install-shrlib:	uninstall-shrlib
##	$(MKDIR) $(shrlibtop)
##	@if test -d $(shrlibtop) ; then true ; else echo Cannot create $(shrlibtop) ; exit 1 ; fi
##	$(MKDIR) $(shrlibdir)
##	@if test -d $(shrlibdir) ; then true ; else echo Cannot create $(shrlibdir) ; exit 1 ; fi
##	$(CP) $(SHRLIBFILES) $(shrlibdir)/
##	$(CHMOD) 664 $(shrlibdir)/*

maintainer-clean:	distclean
	@echo "This command is intended for maintainers to use;"
	@echo "it deletes files that may require special tools to rebuild."

mostlyclean:	clean

uninstall:	uninstall-exe uninstall-lib uninstall-shrlib uninstall-man

uninstall-exe:
	-$(RM) $(bindir)/$(PROGRAM)
	-$(RM) $(bindir)/$(PROGRAM)-$(VERSION)

uninstall-lib:
##	-$(RM) -r $(libdir)

uninstall-man:
	-$(RM) $(mandir)/$(PROGRAM).$(manext)
	-$(RM) $(catdir)/$(PROGRAM).$(manext)

uninstall-shrlib:
##	-$(RM) -r $(shrlibdir)

#=======================================================================
# File dependencies

#=======================================================================
