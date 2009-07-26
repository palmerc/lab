#!/bin/bash

#
# : simulation env
#

#
# work
#

# hostname
HOST=calc10; export HOST
TERM=linux; export TERM
PATH=$PATH:$HOME/Development/bin; export PATH
# work dir
WORK_DIR=$HOME/Development/nakajima; export WORK_DIR

#
# simulator
#

# simulator dir name
SIM_DIR_NAME=newsim; export SIM_DIR_NAME
# simulator dir
SIM_DIR=$WORK_DIR/$SIM_DIR_NAME; export SIM_DIR
# simulation midfile data dir
DATA_DIR=$SIM_DIR/data; export DATA_DIR
# sim-bpred log dir
LOG_DIR=$SIM_DIR/log; export LOG_DIR
# simulation result dir
RESULT_DIR=$SIM_DIR/result; export RESULT_DIR
# benchmark sim-input dir
INPUT_DIR=$SIM_DIR/input; export INPUT_DIR
# simulator bin
NEWSIM_BIN=$SIM_DIR/src/newsim; export NEWSIM_BIN
# simulator source dir
SOURCE_DIR=$SIM_DIR/src; export SOURCE_DIR

# midfile dir
MID_DIR=$SIM_DIR/midfile; export MID_DIR

#
# work's bin
#

# work's bin dir
BIN_DIR=$SIM_DIR/bin; export BIN_DIR
# sim-bpred
TRACE_BIN=$SIM_DIR/SS/sim-bpred; export TRACE_BIN
# simulation script bin
SCRIPT_BIN=$BIN_DIR/run_newsim.pl; export SCRIPT_BIN

#
# SPEC
#

# benchmark binary file
BENCH_BIN_DIR=$SIM_DIR/ss_precompiled_spec; export BENCH_BIN_DIR
