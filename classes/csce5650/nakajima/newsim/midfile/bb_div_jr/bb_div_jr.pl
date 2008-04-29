#!/usr/bin/perl -w

# analysis jump table and make bb_info

####################################################################
#
# sslittle-na-sstrix-objdump -Dw *.ss > objdumpを入力ファイルとする。
# objdumpから、jr命令の飛び先の集合である、jump tableを解析する。
# 求めたleaderと分岐先/ジャンプ先より、succeccor, predecessorの集合
# を基本ブロック毎に求め、基本ブロック情報bb_infoを作成する。
#
####################################################################

# main
my( $true, $false ) = ( 1, 0 );
# 引数の処理
my( $objdump, $bb_info, $jr_table ) = &init;
# jump table
my( %jump_table, $total_func );
# bb info
my( %target, @j_pc, @return_pc, @break_pc, @leader, %succ, %pred );

# jump tableの解析
&search_jump_table;
&print_jump_table;

# bb_infoの生成
&make_all_bb_info;
exit 0;
# main end

# subroutine #######################################################
#
# init: 引数の処理
# eof_text: text segmentの終了を検査
#
# search_jump_table: 各jump tableのbase addressと飛び先のpcを探索
# print_jump_table: 各jr $2命令のpcについて、対応するjump_tableを出力
#
# make_all_bb_info: 全関数のbb_infoを計算
# branch_jump_pc: 関数内の分岐命令、jump命令、関数呼び出しを検索
# make_bb_info: 関数内のleaderを計算し、successorとpredecessorを計算
# print_bb_info: 関数毎にbb_infoを出力
#
####################################################################

# 引数の処理
sub init{
    my( $objdump, $bb_info, $jr_table );

    if( @ARGV == 2 && $ARGV[0] eq "-dir" ){
	# filename
	$objdump = $ARGV[1] . "/objdump";
	$jr_table = $ARGV[1] . "/jr_table";
	$bb_info = $ARGV[1] . "/bb_info";

	open( INFO, ">$bb_info" ) || die " can't open $bb_info";

	return( $objdump, $bb_info, $jr_table )
    }else{
	# usage
	printf "Usage: $0 -dir [TARGET DIR]\n";
	printf "\ttarget file \"objdump\"\n";
	exit 1;
    }
}

# text segmentの終了を検査
sub eof_text{
    ( $_ ) = @_;
    if( /^Disassembly of section .rdata:$/ ){
	return $true;
    }else{
	return $false;
    }
}

# 各jump tableのbase addressと飛び先のpcを探索
sub search_jump_table{
    my( @table, @jr_pc );

    open( OBJ, "$objdump" ) || die "Can't open $objdump:";
    $total_func = 0;

    # ファイルチェックとヘッダをスキップ
    while( <OBJ> ){
	if( /^$/ ){
	    next;
	}

	if( /^Disassembly of section .text:$/ ){
	    if( <OBJ> =~ /^$/ ){
		last;
	    }else{
		die "File format error $objdump";
	    }
	}elsif( !/^.+\sfile format ss-coff-little$/ ){
	    die "File format error $objdump:";
	}
    }

    print STDERR "\rsearching jr \$2";

    # レジスタjump命令の検索
    while( <OBJ> ){
	if( /^00\w+\s\<.+$/ ){
	    # 関数の開始
	    $total_func ++;
	}elsif( /^\s+(\w+):.+\s\sjr\s\$2/ ){
	    # jr $2命令
	    push( @jr_pc, hex $1 );
	}elsif( &eof_text( $_ ) == $true ){
	    last;
	}
    }

    # 先頭pcからpopできるように順番を変更
    @jr_pc = reverse @jr_pc;

    my( $table_flag ) = ( $false );

    # jump tableの探索
    while( <OBJ> ){
	if( $table_flag == $false ){
	    # jump tableのheaderを探索
	    if( /^1.+\s\<\$L\d+\>:$/ ){
		$table_flag = $true;
	    }elsif( !@jr_pc ){
		last;
	    }
	}else{
	    # jump table内
	    if( /^1.+\s\s0x00(\w+):00(\w+)$/ ){
		# jump table内のdata
		my( $addr_1, $addr_2 ) = ( hex $1, hex $2 );

		push( @table, $addr_1 ) unless $addr_1 == 0;
		push( @table, $addr_2 ) unless $addr_2 == 0;
	    }elsif( /^1.+:\s([\da-f ]+)\s[\w\.]+\s.+$/ ){
		# jump table内のdata (op codeに変換されてしまっている場合)
		my( @two ) = split( " ", $1 );
		my( $addr_1 ) = ( hex $two[3].$two[2].$two[1].$two[0] );
		my( $addr_2 ) = ( hex $two[7].$two[6].$two[5].$two[4] );

		push( @table, $addr_1 ) unless $addr_1 == 0;
		push( @table, $addr_2 ) unless $addr_2 == 0;
	    }elsif( /^$/ ){
		# jump tableの終端
		# @tableのsort, unique
		my( %seen, $pc );

		foreach $pc ( sort @table ){
		    push( @{ $jump_table{ $jr_pc[-1] } }, $pc )
			unless $seen{ $pc } ++;
		}

		# 対応するjr命令のpcを削除
		pop( @jr_pc );
		# クリア
		undef @table;
		$table_flag = $false;
	    }elsif( /^\s.+$/ ){
		next;
	    }else{
		die "jump table error";
	    }
	}
    }
}

# 各jr $2命令のpcについて、対応するjump_tableを出力
sub print_jump_table{
    my( $pc, $target_pc );

    open( JR_TABLE, ">$jr_table" ) || die " can't open $jr_table";

    foreach $pc ( sort keys %jump_table ){
	printf JR_TABLE "%x:", $pc;
	foreach $target_pc ( @{ $jump_table{ $pc } } ){
	    printf JR_TABLE "0x%x,", $target_pc;
	}
	printf JR_TABLE "\n";
    }

    close( JR_TABLE );
    print STDERR "\rcreate jump table\n";
}

# 全関数のbb_infoを計算
sub make_all_bb_info{
    open( OBJ, "$objdump" ) || die "Can't open $objdump:";

    # ファイルチェックとヘッダをスキップ
    while( <OBJ> ){
	if( /^$/ ){
	    next;
	}

	if( /^Disassembly of section .text:$/ ){
	    if( <OBJ> =~ /^$/ ){
		last;
	    }else{
		die "File format error $objdump";
	    }
	}elsif( !/^.+\sfile format ss-coff-little$/ ){
	    die "File format error $objdump:";
	}
    }

    my( $func, $func_start_pc, $func_end_pc, $fname ) = 0;

    # 関数毎にbb_infoを作成
    while( <OBJ> ){
	if( /^\s+.+:.+$/ ){
	    # nop命令以外の命令
	    # 関数内の分岐命令、jump命令、関数呼び出しを検索
	    ( $func_end_pc ) = &branch_jump_pc( $_, $fname, $func_end_pc );
	}elsif( /^00(\w+)\s\<(\w+)\>:$/ ){
	    # 関数の開始
	    ( $func_start_pc, $fname ) = ( hex $1, $2 );
	}elsif( /^$/ ){
	    # 関数毎にbb_infoを出力
	    &make_bb_info( $func_start_pc, $func_end_pc );
	    &print_bb_info( $func, $fname );
	    $func ++;
	    &clear;
	}elsif( &eof_text( $_ ) == $true ){
	    # 関数毎にbb_infoを出力
	    &make_bb_info( $func_start_pc, $func_end_pc );
	    &print_bb_info( $func, $fname );
	    $func ++;
	    &clear;
	    print STDERR "\n!!! create all bb_info $func!!!\n";
	    return;
	}elsif( /^1.+$/ ){
	    die "section .rdata";
	}
    }
}

# 次の関数のための処理
sub clear{
    undef %target;
    undef @j_pc;
    undef @return_pc;
    undef @break_pc;
    undef @leader;
    undef %succ;
    undef %pred;
}

# 関数内の分岐命令、jump命令、関数呼び出しを検索
sub branch_jump_pc{
    my( $fname, $pc, $op, $target_pc, $target_fname, $reg, $end_pc );
    ( $_, $fname, $end_pc ) = @_;

    if( /^\s+(\w+):\s.+\s\s(.+)$/ ){
	( $pc, $_ ) = ( hex $1, $2 );
    }

    if( /^.+\>$/ ){
	# ...<...>
	if( /^(b\w+)\s.+,(\w+)\s\<(\w+)\+\w+\>$/ ){
	    # op $d,$d,pc <fname+offset>
	    # 分岐命令
	    ( $op, $target_pc, $target_fname ) = ( $1, hex $2, $3 );

	    if( $fname eq $target_fname ){
		if( $fname eq "__brk" && $op eq "bne" ){
		    # syscall命令の次のbne命令 (<syscall_error>関数に飛ぶ)
		    # ！！！分岐先が1つの分岐命令とした！！！
		    push( @{ $target{ $pc } }, $pc + 8 );
		}else{
		    # 通常の分岐命令
		    push( @{ $target{ $pc } }, $pc + 8 );
		    push( @{ $target{ $pc } }, $target_pc );
		}
	    }else{
		# syscall命令の次のbne命令
		if( $fname eq "_exit" ){
		    # _exit関数の終端bne命令 (関数内にjr $ra命令がない)
		    # ！！！関数の終端として扱う！！！
		    push( @return_pc, $pc );
		}else{
		    # syscall命令の次のbne命令 (<syscall_error>関数に飛ぶ)
		    # ！！！分岐先が1つの分岐命令とした！！！
		    push( @{ $target{ $pc } }, $pc + 8 );
		}
	    }
	}elsif( /^j\s(\w+)\s\<\w+\+\w+\>$/ ){
	    # op pc <fname+offset>
	    # j命令 (飛び先が関数内)
	    $target_pc= hex $1;
	    push( @{ $target{ $pc } }, $target_pc );
	    push( @j_pc, $pc );
	}elsif( /^j\s(\w+)\s\<syscall_error\>$/ ){
	    # j命令 (飛び先が関数外)
	    if( $fname eq "__handler" ){
		# ！！！関数の終端として扱う！！！
		push( @return_pc, $pc );
	    }else{
		# ！！！nop命令として扱う！！！
		return( $end_pc );
	    }
	}elsif( /^jal\s\w+\s\<(\w+)\>$/ ){
	    # op pc <fname>
	    # jal命令
	    $target_fname = $1;

	    if( $fname eq "__start" && $target_fname eq "exit" ){
		# __start関数の終端jalr命令 (関数内にjr $ra命令がない)
		# ！！！関数の終端として扱う！！！
		push( @return_pc, $pc );
	    }else{
		push( @{ $target{ $pc } }, $pc + 8 );
	    }
	}elsif( /^b\w+\s(\w+)\s\<\w+\+\w+\>$/ ){
	    # op pc <fname+offset>
	    # bc1f分岐命令
	    $target_pc = hex $1;

	    push( @{ $target{ $pc } }, $pc + 8 );
	    push( @{ $target{ $pc } }, $target_pc );
	}elsif( /^b\w+\s.+,(\w+)\s\<(\w+)\>$/ ){
	    # op pc <fname>
	    ( $target_pc, $target_fname ) = ( hex $1, $2 );

	    # 通常の分岐命令 (関数の先頭に分岐)
	    push( @{ $target{ $pc } }, $pc + 8 );
	    push( @{ $target{ $pc } }, $target_pc );
	}elsif( /^j\s(\w+)\s\<(\w+)\>$/ && $fname eq "__setjmp" ){
	    # __setjmp関数の終端がj命令
	    # ！！！関数の終端として扱う！！！
	    push( @return_pc, $pc );
	}else{
	    print;
	    die;
	}
    }elsif( /^jalr\s.+$/ ){
	# jalr命令 (function call)
	push( @{ $target{ $pc } }, $pc + 8 );
    }elsif( /^jr\s\$(\d+)$/ ){
	# jr命令
	$reg = $1;

	if( $reg == 31 ){
	    # jr $ra命令 (関数の終了)
	    push( @return_pc, $pc );
	}elsif( $reg == 2 ){
	    # jr $2命令 (対応するjump tableがtarget)
	    push( @{ $target{ $pc } }, @{ $jump_table{ $pc } } );
	}else{
	    die "$_\n jr $reg";
	}
    }elsif( $fname eq "__sigreturn" && /^syscall\s$/ ){
	# __sigreturn関数のsyscall命令 (関数内にjr $ra命令がない)
	# ！！！関数の終端として扱う！！！
	push( @return_pc, $pc );
    }elsif( /^break\s$/ ){
	# break命令
	# 直前に分岐命令がある
	# ！！！関数の終了！！！
	push( @break_pc, $pc );

	# abort関数のbreak命令は、直前の分岐命令がない
	# ！！！汚いコードだけど、leaderを先に求めておく！！！
	if( $fname eq "abort" ){
	    push( @leader, $pc + 8 );
	}
    }

    # 関数の終了pc (leaderを求めるため、8足す)
    return( $pc + 8 );
}

# 関数内のleaderを計算し、successorとpredecessorを計算
sub make_bb_info{
    my( $start_pc, $end_pc ) = @_;
    my( %start, %end, $pc, $target_pc, $bb, $to );

    # %target, @return_pc, $start_pc, $end_pcより、leaderを求める
    push( @leader, $start_pc );
    push( @leader, $end_pc );
    foreach $pc ( keys %target ){
	push( @leader, @{ $target{ $pc } } );
    }
    foreach $pc ( @return_pc ){
	push( @leader, $pc + 8 );
    }

    # 無条件j命令の次の基本ブロックに制御が移行しない、
    # 特殊な関数に備える。それ以外の関数においては、冗長
    foreach $pc ( @j_pc ){
	push( @leader, $pc + 8 );
    }

    # @leaderのsort, unique
    my( %seen, @temp );

    @temp = sort @leader;
    undef @leader;
    foreach $pc ( @temp ){
	push( @leader, $pc ) unless $seen{ $pc } ++;
    }

    # start pc/end pcの計算
    foreach( $bb = 0; $bb < $#leader; $bb ++ ){
	$start{ $leader[$bb] } = $bb;
	$end{ $leader[$bb + 1] - 8 } = $bb;
    }

    # 関数の入口
    #push( @{ $pred{ 0 } }, "in" );

    # targetより、successorとpredecessorを求める
    foreach $pc ( keys %target ){
	my( $bb ) = $end{ $pc };

	foreach $target_pc ( @{ $target{ $pc } } ){
	    # 分岐, j, jal, jalr, jr $2命令のpcから、target pcへの流れ
	    my( $to ) = $start{ $target_pc };

	    if( !defined $bb || !defined $to ){
		die;
	    }

	    push( @{ $succ{ $bb } }, $to );
	    push( @{ $pred{ $to } }, $bb );
	}
    }

    # return_pcより、successorにexitを挿入
    foreach $pc ( @return_pc ){
	my( $bb, $to ) = ( $end{ $pc }, $start{ $pc + 8 } );

	push( @{ $succ{ $bb } }, "exit" );

	# jr ra命令が関数内に複数存在する場合
	if( defined $to ){
	    push( @{ $pred{ $to } }, "IN" );
	}
    }

    # break命令
    #！！！とりあえず関数の終了！！！
    foreach $pc ( @break_pc ){
	my( $bb ) = ( $end{ $pc } );

	if( defined $bb ){
	    push( @{ $succ{ $bb } }, "break" );
	}else{
	    die;
	}
    }

    # leaderの手前の命令が、通常の命令の場合
    foreach( $bb = 0; $bb < $#leader; $bb ++ ){
	if( !defined $succ{ $bb } && ( $bb == 0 || defined $pred{ $bb } ) ){
	    $to = $bb + 1;
	    push( @{ $succ{ $bb } }, $to );
	    push( @{ $pred{ $to } }, $bb );
	}
    }
}

# 関数毎にbb_infoを出力
sub print_bb_info{
    my( $func, $fname, $bb );
    ( $func, $fname ) = @_;

    open( INFO, ">>$bb_info" ) || die " can't open $bb_info";

    # start function
    print INFO "{$func:$fname\n";

    foreach( $bb = 0; $bb < $#leader; $bb ++ ){
	# bb number, start pc, end pc
	printf INFO "%d:%x:%x:", $bb, $leader[$bb], $leader[$bb+1] - 8;

	# successor
	foreach $bb ( sort { $a <=> $b } @{ $succ{ $bb } } ){
	    print INFO "$bb ";
	}
	print INFO ":";

	# predecessor
	if( defined $pred{ $bb } ){
	    foreach $bb ( sort { $a <=> $b } @{ $pred{ $bb } } ){
		print INFO "$bb ";
	    }
	}
    	print INFO ";\n";
    }

    # end function
    print INFO "}\n";
    print STDERR "\rcreating bb_info ($func/$total_func)";
}
