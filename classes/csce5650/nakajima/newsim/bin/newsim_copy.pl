#!/usr/local/bin/perl -w

#
# newsim_copy.pl
#  Time-stamp: <04/01/15 12:45:27 nakajima>
#

use strict;

my( $current_host ) = $ENV{ 'HOST' };
my( $user ) = $ENV{ 'USER' };
my( $sdn ) = $ENV{ 'SIM_DIR_NAME' }; # sim dir name: /work/$host/$user/$sdn

&main;
exit 0;

# subroutine #######################################################

sub main{
    if( @ARGV == 1 ){
	$_ = $ARGV[0];

	if( /^-sim$/ ){
	    &sim_copy_to_hosts;
	}elsif( /^-all$/ ){
	    &backup_newsim;
	}elsif( /^-make_sim$/ ){
	    &make_simulator;
	}elsif( /^-rm_result$/ ){
	    die;
	    &rm_result;
	}elsif( /^-mv_result$/ ){
	    &mv_result;
	}elsif( /^-killall_trace$/ ){
	    &killall_trace;
	}elsif( /^-killall_sim$/ ){
	    &killall_sim;
	}else{
	    &usage;
	}
    }else{
	&usage;
    }
} 

sub usage{
    print "usage: $0 [option]\n";
    print "\tsimulator copy option [ -sim | -all | -make_sim ]\n";
    print "\tother option [ -mv_result | -killall_trace | -killall_sim ]\n";
    exit 1;
}

sub sim_copy_to_hosts{
    my( $dest ) = "sim_src.tar";

    if( $current_host eq "calchome" ){
	print "sim_copy all machine\n";

	foreach ( &arp_hosts ){
	    my( $work_dir ) = "/export/$_/$user";

	    &print_sys("rcp $ENV{ 'HOME' }/$dest $_:$work_dir/");
	    #&print_sys("rsh $_ rm -fR $work_dir/$sdn/bin");
	    #&print_sys("rsh $_ rm -fR $work_dir/$sdn/midfile");
	    #&print_sys("rsh $_ rm -fR $work_dir/$sdn/src");
	    &print_sys("rsh $_ tar xf $work_dir/$dest -C $work_dir/");
	}
    }elsif( $current_host eq "capc5" ){
	# make c: mkdfile/* SS/ src/
	my( @prog_dir ) = qw(ana_mem_dep du_chain icd_info
			     loop_del_pc posdom presim);

	foreach ( @prog_dir ){
	    &print_sys("cd $ENV{ 'MID_DIR' }/$_ && make c");
	}
	&print_sys("cd $ENV{ 'SIM_DIR' }/SS/ && make clean");
	&print_sys("cd $ENV{ 'SIM_DIR' }/src && make c");

	# tar, rcp
	my( $work_dir ) = "$ENV{ 'WORK_DIR' }";
	my( $target ) = "$sdn/src $sdn/midfile $sdn/bin";

	&print_sys("cd $work_dir && tar cf $work_dir/$dest $target");
	&print_sys("rcp $work_dir/$dest calchome:");
	&print_sys("rcp $ENV{ 'BIN_DIR' }/newsim_copy.pl calchome:bin/");
	&print_sys("rcp $ENV{ 'HOME' }/run calchome:");

	foreach( qw(cados40 cados41) ){ 
	    &print_sys("rcp $work_dir/$dest $_:/export/$_/$user/");
	}
    }else{
	die "$current_host $!";
    }
}

sub backup_newsim{
    my( $dest ) = "sim.tar";

    if( $current_host eq "capc5" ){
	my( $work_dir ) = "$ENV{ 'WORK_DIR' }";

	#&print_sys("cd $work_dir/$sdn && rm -f log/*/* result/*/*");
	&print_sys("cd $work_dir && tar cf $work_dir/$dest $sdn");
	&print_sys("rcp $work_dir/$dest calchome:");

	foreach( qw(cados40 cados41) ){ 
	    &print_sys("rcp $work_dir/$dest $_:/export/$_/$user/");
	}
    }else{
	die "$current_host $!";
    }
}

sub rm_result{
    die "rm_result";

    if( $current_host eq "calchome" ){
	print "rm result data all\n";

	foreach ( &arp_hosts ){
	    my( $result_dir ) = "/export/$_/$user/$sdn/result";

	    die;
	    #&print_sys("rsh $_ rm -f $result_dir/*/*");
	}
    }else{
	die "$current_host $!";
    }
}

sub mv_result{
    my @bm = qw(compress95 gcc go ijpeg li m88ksim perl vortex);
    my $date = `date +%m%d`;

    if( $current_host eq "calchome" ){
	print "move result data all\n";

	foreach ( &arp_hosts ){
	    my( $sim_dir ) = "/export/$_/$user/$sdn";

	    &print_sys("rsh $_ 'cd $sim_dir && mv result Result_$date'");

	    foreach my $bm ( @bm ){
		&print_sys("mkdir -p $sim_dir/result/$bm");
	    }
	}
    }elsif( $current_host eq "capc5" ){
	my( $sim_dir ) = "/export/capc5/$user/$sdn";

	&print_sys("cd $sim_dir && mv result Result_$date");

	foreach my $bm ( @bm ){
	    &print_sys("mkdir -p $sim_dir/result/$bm");
	}
    }else{
	die "$current_host $!";
    }
}

sub killall_trace{
    if( $current_host eq "calchome" ){
	print "killall sim-bpred\n";

	foreach ( &arp_hosts ){
	    &print_sys("rsh $_ killall sim-bpred");
	}
    }elsif( $current_host eq "capc5" ){
	&print_sys("rsh $_ killall sim-bpred");
    }else{
	die "$current_host $!";
    }
}

sub killall_sim{
    if( $current_host eq "calchome" ){
	print "killall sim-bpred\n";

	foreach ( &arp_hosts ){
	    &print_sys("rsh $_ killall newsim");
	}
    }elsif( $current_host eq "capc5" ){
	&print_sys("rsh $_ killall newsim");
    }else{
	die "$current_host $!";
    }
}

sub make_simulator{
    if( $current_host eq "calchome" ){
	print "make simulator\n";

	foreach ( &arp_hosts ){
	    my( $source_dir ) = "/export/$_/$user/$sdn/src";

	    #&print_sys("rsh $_ 'cd $source_dir && make'");
	    &print_sys("rsh $_ 'cd $source_dir && make c && make' &");
	}
    }elsif( $current_host eq "capc5" ){
	my( $source_dir ) = "/export/capc5/$user/$sdn/src";

	&print_sys("cd $source_dir && make");
    }else{
	die "$current_host $!";
    }
}

##############################

sub arp_hosts{
    my( @arp ) = `arp -a`;
    my( @hosts );

    foreach ( @arp ){
        if( /^(\w+)\s\(/ ){
	    if( /incomplete/ ){
		next;
            }elsif( $1 =~ /calchome|calcserv/ ){
                # ignore host: server
                next;
            }else{
                push( @hosts, $1 );
            }
        }elsif( /^\?/ ){
            next;
        }else{
            die "$_ $!";
        }
    }

    return @hosts;
}

sub print_sys{
    my( $execute ) = @_;

    print "$execute\n";
    system("$execute");

    if( $? != 0 ){
	die;
    }
}
