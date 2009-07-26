#!/usr/local/bin/perl -w

use strict;

my( %data );
my( %calc_sp );

&main;
exit 0;

# subroutine #######################################################
sub main{
    foreach my $data ( &search_result_data ){
	my( $val ) = &grep_clock_data( $data );

	if( $val ne "NULL" ){
	    $_ = $data;
	    if( /^(.+:.+):.+:.+$/ ){
		$data{ $1 } = $val;
	    }
	}
    }

    #foreach ( sort keys %data ){
	#print STDERR $_, $data{ $_ }, "\n";
    #}

    &calc_data;
}

sub calc_data{
    my( %sp_data, %mp_data );
    my( %perf_perf, %perf_re, %none_perf, %none_re );

    foreach ( sort keys %data ){
	if( /-mp_/ ){
	    $mp_data{ $_ } = $data{ $_ };
	}else{
	    $sp_data{ $_ } = $data{ $_ };
	}
    }

    foreach my $sim_model_bm ( sort keys %mp_data ){
	my( $sim, $model, $bm, $val, $sim_model );

	$_ = $sim_model_bm;
	if( /^(\w+)(-.+):(.+)$/ ){
	    ( $sim, $model, $bm ) = ( $1, $2, $3 );
	}

	$sim_model = "$sim$model";

	# mp vs reg_perf-sp_perf
	if( defined $sp_data{ "sp-reg_perf-sp_perfect:$bm" } ){
	    $val = $sp_data{ "sp-reg_perf-sp_perfect:$bm" } /
		$mp_data{ $sim_model_bm };
	    push( @{ $perf_perf{ $sim_model } }, "$bm:$val" );
	}

	# mp vs reg_perf-sp-re
	if( defined $sp_data{ "sp-reg_perf-sp_reorder:$bm" } ){
	    $val = $sp_data{ "sp-reg_perf-sp_reorder:$bm" } /
		$mp_data{ $sim_model_bm };
	    push( @{ $perf_re{ $sim_model } }, "$bm:$val" );
	}

	# mp vs reg_none-sp_perf
	if( defined $sp_data{ "sp-reg_none-sp_perfect:$bm" } ){
	    $val = $sp_data{ "sp-reg_none-sp_perfect:$bm" } /
		$mp_data{ $sim_model_bm };
	    push( @{ $none_perf{ $sim_model } }, "$bm:$val" );
	}

	# mp vs reg_none-sp-re
	if( defined $sp_data{ "sp-reg_none-sp_reorder:$bm" } ){
	    $val = $sp_data{ "sp-reg_none-sp_reorder:$bm" } /
		$mp_data{ $sim_model_bm };
	    push( @{ $none_re{ $sim_model } }, "$bm:$val" );
	}
    }

    # print
    foreach ( sort keys %perf_perf ){
	print "$_ vs sp-reg_perf-sp_perfect\n";
	foreach ( @{ $perf_perf{ $_ } } ){
	    if( /(.+):(.+)/ ){
		printf "$1:%f;\n", $2;
	    }
	}
    }

    # print
    foreach ( sort keys %perf_re ){
	print "$_ vs sp-reg_perf-sp_reorder\n";
	foreach ( @{ $perf_re{ $_ } } ){
	    if( /(.+):(.+)/ ){
		printf "$1:%f;\n", $2;
	    }
	}
    }

    # print
    foreach ( sort keys %none_perf ){
	print "$_ vs sp-reg_none-sp_perfect\n";
	foreach ( @{ $none_perf{ $_ } } ){
	    if( /(.+):(.+)/ ){
		printf "$1:%f;\n", $2;
	    }
	}
    }

    # print
    foreach ( sort keys %none_re ){
	print "$_ vs sp-reg_none-sp_reorder\n";
	foreach ( @{ $none_re{ $_ } } ){
	    if( /(.+):(.+)/ ){
		printf "$1:%f;\n", $2;
	    }
	}
    }
}

sub search_result_data{
    my( @grep, @data );

    if( ( defined $ENV{ 'REMOTEHOST' } && $ENV{ 'REMOTEHOST' } eq "calchome" )
	|| $ENV{ 'HOST' } eq "calchome" ){
	my( $proc ) = "$ENV{ 'HOME' }/proc";

	@grep = `grep -h 'result' $proc/*.result`;

	if( $? != 0 ){
	    die "grep $proc/*.result";
	}
    }

    foreach ( @grep ){
	### $RESULT_DIR/go/PD-mp_blind-reg_256-fork-send-sp_perfect
	if( m!^/export/(\w+)/.+/result/(\w+)/([-\w]+)$! ){
	    # model:bm:host:path
	    push( @data, "$3:$2:$1:$_" );
	}
    }

    return @data;
}

sub grep_clock_data{
    ( $_ ) = @_;

    if( /^.+:.+:(.+):(.+)$/ ){
	my( $host ) = $1;
	my( $fname ) = $2;

	$fname =~ s/export/work/;

	open( FILE, "$fname" ) || return "NULL";

	while( <FILE> ){
	    if( /^clock\s.+:\s(\d+)$/ ){
		return $1;
	    }
	}

	return "NULL";
    }else{
	die "grep_data";
    }
}
