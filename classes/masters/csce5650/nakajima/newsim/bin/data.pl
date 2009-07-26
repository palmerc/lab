#!/usr/local/bin/perl -w

#
# data.pl
#  Time-stamp: <04/01/15 12:43:37 nakajima>
#

use strict;

# bool
my( $true, $false ) = ( 0, 1 );

# env
my( $home ) = qq($ENV{ 'HOME' });
my( $user ) = qq($ENV{ 'USER' });
my( $host ) = qq($ENV{ 'HOST' });
my( $sim_dir_name ) = qq($ENV{ 'SIM_DIR_NAME' });

# return val
my( $rv ) = $true;

# global
my( $argment, $fname, %loop );

# main routine
&main;
exit 0;

# subroutine #######################################################
sub main{
    if( $host eq "capc5" ){
	if( &read_run( $host ) == $true ){
	    &loop_sim_result( $host );
	}
	undef %loop;
    }else{
	foreach my $host ( &arp_hosts ){
	    if( &read_run( $host ) == $true ){
		&loop_sim_result( $host );
	    }
	    undef %loop;
	}
    }

    if( $rv == $false ){
	print "ERROR\n";
	exit 1;
    }
}

sub read_run{
    my( $host ) = @_;

    if( @ARGV == 0 ){
	open( RUN, "$home/run" ) || die " can't open $home/run";
    }elsif( @ARGV == 1 ){ 
	open( RUN, "$ARGV[0]" ) || die " can't open $ARGV[0]";
    }else{
	# usage
	die "usage: $0 [run script]";
    }

    while( <RUN> ){
	if( /^HOST:$host$/ ){
	    last;
	}elsif( /EOF/ ){
	    print STDERR "$host not found\n";
	    return $false;
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

    close(RUN);

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

    return $true;
}

# loop
sub loop_sim_result{
    my( $host ) = @_;
    my( $result_dir ) = "/work/$host/$user/$sim_dir_name/result";

    print STDERR "$host\n";

    # exec
    foreach my $sim ( @{ $loop{'SIM'} } ){
	foreach my $lp ( @{ $loop{'LP'} } ){
	    my( $filename ) = "$sim$fname$lp";

	    print $filename, "\n";

	    foreach my $bm ( @{ $loop{'BM'} } ){
		my( $dir_filename ) = "$result_dir/$bm/$filename";

		#print $out, " ";
		my( $ipc ) = &grep_ipc($dir_filename);

		# ここでhashにいれようかなぁ
		if( $ipc eq "NULL" || $ipc eq "NoFile" ){
		    print $ipc, " ";
		    $rv = $false;
		}else{
		    printf("%3.1f ", $ipc);
		}
	    }
	    print "\n";
	}
    }
}

##############################

sub grep_ipc{
    my( $file ) = @_;

    open( FILE, "$file" ) || return "NoFile";

    while( <FILE> ){
	if( /^ipc\s.+:\s([\d\.]+)$/ ){
	    return $1;
	}
    }

    return "NULL";
}

##############################

sub arp_hosts{
    my( @arp ) = `arp -a`;
    my( @hosts );

    foreach ( @arp ){
        if( /^(\w+)\s\(192\.168\.0\.\d+/ ){
            if( $1 =~ /calchome|calcserv/ ){
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
