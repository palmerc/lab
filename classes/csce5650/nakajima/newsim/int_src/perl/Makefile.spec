# The following two entries are REQUIRED
TUNE=/base
EXT=.base

# These are vendor supplied overrides for 134.perl base base
CC               = cc
# EXTRA_CFLAGS     = -DI_TIME  -p
EXTRA_CFLAGS     = -DI_TIME
EXTRA_LDFLAGS    = -non_shared -om
OBJ              = .c
OBJOPT           = 
#OPTIMIZE         = -migrate -std1 -O5 -ifo -p
OPTIMIZE         = -migrate -std1 -O5 -ifo
VENDOR           = digital
action           = validate
config           = default.cfg
endian           = 305419896
ext              = base
mach             = default
max_active_compares = 10
notes01          = Baseline Optimizations: -O5 -ifo -non_shared -om
notes02          = Portibility Flags: 124.m88ksim: -DLEHOST  134.perl: -DI_TIME
notes03          = 147.vortex: -D__RISC_64__
notes04          = 
notes05          = Compiler invokation: cc -migrate -std1 (DEC C with -std1 for strict ANSI)
notes06          = 
notes07          = 
notes08          = 
notes09          = 
notes10          = 
output_format    = raw
report_depth     = 
run              = all
shell            = /bin/sh
size             = ref
tune             = base
vendor           = anon

# These are evil nasty includes to build vendor targets
