#!/usr/bin/perl -w

#
# make midfile script
#  Time-stamp: <04/01/14 19:41:54 nakajima>
#

use POSIX qw(strftime);
use strict;

my( $log_file ) = "$ENV{ 'BIN_DIR' }/err_make_mid";

&main;
exit 0;

# subroutine #######################################################

sub main{
    # create errlog
    &print_log;

    # make clean; make
    &make_mid_bin;
    # objdump, bb, du, icd, loop
    &create_data;
    # brn_pred, val_pred
    &print_sys("$ENV{ 'SCRIPT_BIN' } -presim");

    # remove log
    &print_sys("rm $log_file");
}

sub make_mid_bin{
    my( @prog_dir ) = qw(ana_mem_dep du_chain icd_info loop_del_pc posdom);

    foreach ( @prog_dir ){
	&print_sys("cd $ENV{ 'MID_DIR' }/$_ && make c && make");
    }

    &print_sys("cd $ENV{ 'SIM_DIR' }/SS/ && make clean && make");
}

sub create_data{
    my( $ss_objdump ) = ("sslittle-na-sstrix-objdump");
    my( @benchmarks ) = qw(compress95 gcc go ijpeg li m88ksim perl vortex);
    #my( @benchmarks ) = qw(applu apsi fpppp hydro2d mgrid
	#		   su2cor swim tomcatv turb3d wave5);
    my( @programs ) = qw(bb_div_jr/bb_div_jr.pl
			 obj2asm/obj2asm.pl
			 posdom/make_posdom
			 du_chain/ana_dataflow
			 loop_del_pc/ana_loop_pc
			 icd_info/ana_indirect_cd );

    foreach ( @benchmarks ){
	my( $data_dir ) = ("$ENV{ 'DATA_DIR' }/$_");
	my( $bin );

	if( $_ eq "gcc" ){
	    $bin = "$ENV{ 'BENCH_BIN_DIR' }/cc1.ss";
	}else{
	    $bin = "$ENV{ 'BENCH_BIN_DIR' }/$_.ss";
	}

	&print_sys("$ss_objdump -Dw $bin > $data_dir/objdump");

	foreach ( @programs ){
	    &print_sys("$ENV{ 'MID_DIR' }/$_ -dir $data_dir");
	}

	print_log_add("end $_");
    }
}

sub print_sys{
    my( $execute ) = @_;

    &print_log_add("$execute");
    system("$execute");

    if( $? != 0 ){
	die;
    }
}

sub print_log{
    open( LOG, ">$log_file" ) || die " can't open $log_file";

    print LOG "#make midfile script";
    print LOG strftime("%D %H:%M:", localtime);
    print LOG "\n";

    close( LOG );
}

sub print_log_add{
    my( $message ) = @_;

    open( LOG, ">>$log_file" ) || die " can't open $log_file";

    print LOG "$message\n";

    close( LOG );
}
