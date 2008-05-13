#!/usr/bin/perl -w

# The other part of my little hack to list all the tests the simulator would
# run given the original run file by nakajima.
# - Cameron Palmer, May 2008

#
# run_newsim.pl
#  Time-stamp: <04/01/04 19:55:46 nakajima>
#

use POSIX qw(strftime);
use strict;

# bool
my( $true, $false ) = ( 0, 1 );

# env
my( $host ) = qq($ENV{ 'HOST' });
my( $home ) = qq($ENV{ 'HOME' });
my( $sim_dir ) = qq($ENV{ 'SIM_DIR' });
my( $mid_dir ) = qq($ENV{ 'MID_DIR' });
my( $result_dir ) = qq($ENV{ 'RESULT_DIR' });
my( $data_dir ) = qq($ENV{ 'DATA_DIR' });
my( $log_dir) = qq($ENV{ 'LOG_DIR' });
my( $input_dir ) = qq($ENV{ 'INPUT_DIR' });
my( $bench_bin_dir ) = qq($ENV{ 'BENCH_BIN_DIR' });
my( $trace_bin ) = qq($ENV{ 'TRACE_BIN' });
my( $newsim ) = qq($ENV{ 'NEWSIM_BIN' });
my( $presim ) = qq($mid_dir/presim/presim);

my( $argment, $fname, %loop );

&main;
exit 0;

# subroutine #######################################################

# main
sub main{
    if (@ARGV == 0) {
	&read_run;
        &loop_simulation;
    }
}

sub read_run{
    open( RUN, "$home/run" ) || die " can't open $home/run";

    while( <RUN> ){
	if( /^HOST:$host$/ ){
	    last;
	}elsif( /EOF/ ){
	    print "$host not found\n";
	    exit 1;
        }
    }

    while( <RUN> ){
	chomp;

	if( /^FILE:(.+)$/ ){
	    $fname = $1;
	}elsif( /^ARG:(.*)$/ ){
	    $argment = $1;
	}elsif( /^(\w+):\[(.+)\]$/ ){
	    $_ = $2;
	    push( @{ $loop{$1} }, split);
	}elsif( /^$/ ){
	    last;
	}elsif( /^HOST/ ){
	    die "read_run $_ $!";
	}else{
	    die "read_run $_ $!";
	}
    }

    if( !defined $fname ){
	die "undefined FILE $!";
    }elsif( !defined $argment ){
	die "undefined ARG $!";
    }elsif( !defined $loop{'SIM'} ){
	die "undefined SIM $!";
    }elsif( !defined $loop{'BM'} ){
	die "undefined BM $!";
    }elsif( !defined $loop{'LP'} ){
	die "undefined LP $!";
    }

    close(RUN);
}

# loop simulation
sub loop_simulation {
    # log filename
    foreach my $sim ( @{ $loop{'SIM'} } ){
	foreach my $lp ( @{ $loop{'LP'} } ){
	    foreach my $bm ( @{ $loop{'BM'} } ){
		# simulation result file
		my( $out ) = "$result_dir/$bm/$sim$fname$lp";
                print "$out\n";
            }
	}
    }
}

# sub set sim argment
sub sim_arg{
    my( $bm, $sim, $argment ) = @_;

    # simulation argment, shell argment(updrive), redirect filename
    my( $sim_arg, $sh_arg, $redir);

    if( $sim ne "" ){
	$sim_arg = "-dir $data_dir/$bm -sim_type $sim";
    }else{
	$sim_arg = "-dir $data_dir/$bm";
    }

    # sim-bpred log file
    $redir = "-redir:prog $log_dir/$bm/prog -redir:sim $log_dir/$bm/sim";

    $_ = $bm;
    if( /^compress95$/ ){
	# シミュレーション時間短縮
	#$sim_arg = "$sim_arg -spool 5000";
	$sh_arg = "echo 30000 e 2231 | $trace_bin $redir"
	    . " $bench_bin_dir/compress95.ss";
    }elsif( /^gcc$/ ){
	$sh_arg = "$trace_bin $redir $bench_bin_dir/cc1.ss"
	    . " -quiet -funroll-loops -fforce-mem -fcse-follow-jumps"
		. " -fcse-skip-blocks -fexpensive-optimizations"
		    . " -fstrength-reduce -fpeephole -fschedule-insns"
			. " -finline-functions -fschedule-insns2 -O"
			    . " genoutput.i -o genoutput.s";
    }elsif( /^go$/ ){
	$sh_arg = "$trace_bin $redir $bench_bin_dir/go.ss 6 9 2stone9.in";
    }elsif( /^ijpeg$/ ){
	$sim_arg = "$sim_arg -fastfwd 50000000";
	$sh_arg = "$trace_bin $redir $bench_bin_dir/ijpeg.ss"
	    . " -image_file specmun.ppm -compression.quality 50"
		. " -compression.optimize_coding 0 "
		    . " -compression.smoothing_factor 90 -difference.image 1"
			. " -difference.x_stride 10 -difference.y_stride 10"
			    . " -verbose 1 -GO.findoptcomp";
    }elsif( /^li$/ ){
	$sh_arg = "$trace_bin $redir $bench_bin_dir/li.ss train.lsp";
    }elsif( /^m88ksim$/ ){
	# シミュレーション時間短縮
	#$sim_arg = "$sim_arg -spool 5000";
	$sh_arg = "cat ctl.in | $trace_bin $redir $bench_bin_dir/m88ksim.ss";
    }elsif( /^perl$/ ){
	$sh_arg = "$trace_bin $redir $bench_bin_dir/perl.ss"
	    . " scrabbl.pl scrabbl.in";
    }elsif( /^vortex$/ ){
	$sim_arg = "$sim_arg -fastfwd 100000000";
	$sh_arg = "$trace_bin $redir $bench_bin_dir/vortex.ss vortex.in";
    }elsif( /^applu$/ ){
	$sh_arg = "$trace_bin $redir $bench_bin_dir/applu.ss < applu.in";
    }elsif( /^apsi$/ ){
	$sh_arg = "$trace_bin $redir $bench_bin_dir/apsi.ss";
    }elsif( /^fpppp$/ ){
	$sh_arg = "$trace_bin $redir $bench_bin_dir/fpppp.ss < natoms.in";
    }elsif( /^hydro2d$/ ){
	$sh_arg = "$trace_bin $redir $bench_bin_dir/hydro2d.ss HYDRO2D.MODEL"
	    ." < hydro2d.in";
    }elsif( /^mgrid$/ ){
	$sh_arg = "$trace_bin $redir $bench_bin_dir/mgrid.ss < mgrid.in";
    }elsif( /^su2cor$/ ){
	$sh_arg = "$trace_bin $redir $bench_bin_dir/su2cor.ss SU2COR.MODEL"
	    . " < su2cor.in";
    }elsif( /^swim$/ ){
	$sh_arg = "$trace_bin $redir $bench_bin_dir/swim.ss < swim.in";
    }elsif( /^tomcatv$/ ){
	$sh_arg = "$trace_bin $redir $bench_bin_dir/tomcatv.ss TOMCATV.MODEL"
	    . " < tomcatv.in";
    }elsif( /^turb3d$/ ){
	$sh_arg = "$trace_bin $redir -max\:inst 1400000000"
	    . " $bench_bin_dir/turb3d.ss < turb3d.in";
    }elsif( /^wave5$/ ){
	$sh_arg = "$trace_bin $redir $bench_bin_dir/wave5.ss < wave5.in";
    }else{
	# compress95 gcc go ijpeg li m88ksim perl vortex
	# applu apsi fpppp hydro2d mgrid su2cor swim tomcatv turb3d wave5
	die;
    }

    return("$sim_arg $argment -updrive /bin/sh -c '$sh_arg'");
}

# sub exec simulation
sub simulation{
    my( $bm, $out, $sim_arg ) = @_;

    # exec simulation
    print_log( $out, "$newsim $sim_arg" );
    system("cd $input_dir/$bm && $newsim $sim_arg 1>>$out 2>&1");

    if( $? == 0 ){
	&print_sys("cp $out $home/result/$bm/");

	return $true;
    }else{
	return $false;
    }
}

sub presim{
    # make simulator
    &print_sys("cd $mid_dir/presim && make clean && make");

    # presim data
    foreach my $bm ( qw(compress95 gcc go ijpeg li m88ksim perl vortex) ){
	# simulation
	my( $sim_arg ) = &sim_arg($bm, "", "");
	# simulation result file
	my( $out ) = "$result_dir/$bm/MID";

	# exec simulation
	print_log( $out, "$presim $sim_arg");
	system("cd $input_dir/$bm && $presim $sim_arg 1>>$out 2>&1");

	if( $? != 0 ){
	    die;
	}
    }
}

# sub print log
sub print_log_add{
    my( $log_file, $message ) = @_;

    # homeが死んだ場合
    open( LOG, ">>$log_file" ) || open( LOG, ">>$result_dir/errlog" )
	|| die " can't open $result_dir/errlog";

    if( $message eq "time" ){
	print LOG strftime("%D %H:%M:", localtime);
    }else{
	print LOG "$message\n";
    }

    close( LOG );
}

sub print_log{
    my( $log_file, $message ) = @_;

    open( LOG, ">$log_file" ) || die " can't open $log_file";

    if( $message eq "time" ){
	print LOG strftime("%D %H:%M:", localtime);
    }else{
	print LOG "$message\n";
    }

    close( LOG );
}

sub print_sys{
    my( $execute ) = @_;

    print "$execute\n";
    system("$execute");

    if( $? != 0 ){
	die "$execute";
    }
}
