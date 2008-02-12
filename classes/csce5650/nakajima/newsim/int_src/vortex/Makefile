###############################################################################
# SPECtools95 
#   Version 0.2
#   This makefile should NOT be changed other than to customize for a 
#   particular benchmark.  Users may overide variables by placing them
#   in the Makefile.spec file.  Or by using the "runspec" program to 
#   build the Makefile.spec file for them.

###############################################################################
#	BENCHMARK	-- standard definitions for this benchmark

NUMBER    = 147
NAME      = vortex
EXEBASE   = vortex
OFILES    = bitvec bmt bmt0 bmt01 bmt10 bmtlib bmtobj core01 dba dbm0 dbm1 dbm2 domain draw07 draw7 drawlib drawobj emplib empobj env0 env01 env1 fm gdbm grp0 grp1 grp2 hm iam ifm im km list01 mem00 mem01 mem10 oa0 oa1 oadmp obj01 odbm ogrp om point primal pstub query rect rects sa shell sm testobj tm trans00 trans01 trans10 trans20 tree0 tree00 tree01 ut vchunk vdbm voa vom
LIBS      = -lm
LD        = $(CC) $(CFLAGS)


###############################################################################
#	DEFINES		-- standard definitions for SPEC
#                          these may be overriden in Makefile.spec
BENCHMARK = $(NUMBER).$(NAME)
EXT       = 
OBJS      = $(addsuffix $(OBJ),$(OFILES))
CC        = cc
CFLAGS    = $(EXTRA_CFLAGS) $(FDO_FLAGS) $(OPTIMIZE)
FC        = f77
FFLAGS    = $(EXTRA_FFLAGS) $(FDO_FLAGS) $(OPTIMIZE)
LDFLAGS   = $(EXTRA_LDFLAGS) $(FDO_LDFLAGS)
OPTIMIZE  = $(EXTRA_OPTIMIZE) -O
OBJOPT    = -c -o $@
OBJ       = .o
SPECSHELL = /bin/sh
ECHO      = echo
BINDIR    = ../exe$(TUNE)
EXENAME   = $(EXEBASE)$(EXT)
BINARY    = $(BINDIR)/$(EXENAME)

ifeq ($(FDO),pass1)
        FDO_FLAGS=$(PASS1) $(PASS1_CFLAGS)
	FDO_LDFLAGS=$(PASS1_LDFLAGS)
endif
ifeq ($(FDO),pass2)
        FDO_FLAGS=$(PASS2) $(PASS2_CFLAGS)
	FDO_LDFLAGS=$(PASS2_LDFLAGS)
endif


###############################################################################
#	VENDOR		-- where we include any vendor specific information
ifneq ($(NOINC),yes)
	include Makefile.spec
endif


###############################################################################
#	TARGETS		-- Rules to build benchmark executables
error: 
	@$(ECHO) "This makefile is only used for building binaries now"
	@$(ECHO) "to run the SPEC suite please use 'runspec'"
	@$(ECHO)

build: $(EXTRA_TARGETS) $(BINARY)

.SUFFIXES: $(OBJ)
.c$(OBJ):
	$(CC) $(OBJOPT) $(CFLAGS) $<

# template build target, should work for simple builds
$(BINARY): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(LIBS) $(EXTRA_LIBS)

rr: $(OBJS)
	cc -D__RISC_64__ -Wl,-r -non_shared -o vortex.rr $(OBJS) $(LIBS) $(EXTRA_LIBS)

# to clean things up
.PHONY: clean clobber
clean:
ifeq ($(OBJ),.c)
	rm -rf core *.err *.out
else
	rm -rf $(OBJS) core *.err *.out
endif

clobber: clean
	rm -rf $(BINARY)
