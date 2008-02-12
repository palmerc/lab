#!/usr/local/bin/perl -w

# objdumpから asm_du, asm_loop に変換するスクリプト

#
# debug用のprint多数あり。実行時、> /dev/nullをつけること。
#

####################################################################
#
# sslittle-na-sstrix-objdump -Dw *.ss > objdumpを入力ファイルとする。
#
# objdumpから、各命令のレジスタの定義と使用を関数毎に書き出す (asm_du)
# objdumpから、和差積商命令を関数毎に書き出す (asm_loop)
#
####################################################################

# main
my( $true, $false ) = ( 1, 0 );
# 引数の処理
my( $objdump, $asm_du, $asm_loop ) = &init;
# 関数内の命令
my( %inst );

&read_objdump;
exit 0;
# main end

# subroutine #######################################################
#
# init: 引数の処理
# eof_text: text segmentの終了を検査
#
# clear: 次の関数のための処理
# read_objdemp: 関数内の全命令をobjdumpから読み出す
# analysis_inst: 各命令のdest, sourceレジスタ番号を解析
# op_check: 命令の種類が和差積商であるかチェック
# print_asm: 関数毎に、pcとdest/src reg番号と、和差積商の命令を表示
#
####################################################################

# 引数の処理
sub init{
    my( $objdump, $asm_du );

    if( @ARGV == 2 && $ARGV[0] eq "-dir" ){
	# filename
	$objdump = $ARGV[1] . "/objdump";
	$asm_du = $ARGV[1] . "/asm_du";
	$asm_loop = $ARGV[1] . "/asm_loop";

	# header
	open( DU, ">$asm_du" ) || die " can't open $asm_du";
	open( LOOP, ">$asm_loop" ) || die " can't open $asm_loop";

	return( $objdump, $asm_du, $asm_loop );
    }else{
	# usage
	printf STDERR "Usage: $0 -dir [TARGET DIR]\n";
	printf STDERR "\ttarget file \"objdump\"\n";
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

# 関数内の全命令をobjdumpから読み出す
sub read_objdump{
    open( OBJ, $objdump ) || die "Can't open $objdump:";

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

    my( $func, $fname ) = ( 0 );

    while( <OBJ> ){
	if( /^\s+(\w+):\s.+\s\s(.+)$/ ){
	    # nopを除いた通常の命令
	    my( $pc, $inst ) = ( $1, $2 );
	    # 各命令のdest, sourceレジスタ番号を解析し、出力
	    #print "$pc:";
	    my( @regs ) = &analysis_inst( $inst );

	    push( @{ $inst{ $pc } }, @regs );
	}elsif( /^00[0-9a-f]+\s\<(\w+)\>:$/ ){
	    # 関数の開始
	    $fname = $1;
	}elsif( /^$/ ){
	    # 関数の終了
	    &print_asm( $func, $fname, $op );
	    &clear;
	    $func ++;
	}elsif( &eof_text( $_ ) == $true ){
	    # text segmentの終了
	    &print_asm( $func, $fname, $op );
	    $func ++;
	    print STDERR "\rcreate asm_du, asm_loop (total:$func)\n";
	    last;
	}
    }
}

# 次の関数のための処理
sub clear{
    undef %inst;
}

# 各命令のdest, sourceレジスタ番号を解析し、出力
sub analysis_inst{
    my( $dest, $src1, $src2, $op ) = ( 0, 0, 0 );
    ( $_  ) = @_;

    if( /^.+\>$/ ){
	# ... <...>
	if( /^(b\w+)\s\$(\d+),\$(\d+),\w+\s\<[\w\+]+\>$/ ){
	    # op $d,$d,pc <fname+offset>
	    #print "$_\tB $1 $2 $3\n";
	    ( $op, $src1, $src2 ) = ( $1, $2, $3 );
	}elsif( /^(b\w+)\s\$(\d+),\w+\s\<[\w\+]+\>$/ ){
	    # op $d,pc <fname+offset>
	    #print "$_\tB $1 $2\n";
	    ( $op, $src1 ) = ( $1, $2 );
	}elsif( /^(b\w+)\s\w+\s\<[\w\+]+\>$/ ){
	    # op pc <fname+offset>
	    #print "$_\tB $1\n";
	    ( $op, $src1 ) = ( $1, 66 );
	}elsif( /^(j\w*)\s\w+\s\<[\w\+]+\>$/ ){
	    # op pc <fname+offset>
	    if( $1 =~ /^(jal)/ ){
		#print "$_\tJAL $1 31\n";
		( $op, $dest ) = ( $1, 31 );
	    }else{
		#print "$_\tJ $1\n";
		$op = $1;
	    }
	}else{
	    die "op ... <...> : $_";
	}
    }elsif( /^([\w\.]+)\s\$([f\d]+),[-\d]+\(\$(\d+)\)$/ ){
	my( $reg1, $reg2 ) = ( $2, $3 );
	# op $d,0($d)
	if( $1 =~ /^(d*l[\w\.]+)$/ ){
	    #print "$_\tLD $1 $reg1 $reg2\n";
	    ( $op, $dest, $src1 ) = ( $1, $reg1, $reg2 );
	}elsif( $1 =~ /^(d*s[\w\.]+)$/ ){
	    #print "$_\tSW $1 $reg1 $reg2\n";
	    ( $op, $src1, $src2 ) = ( $1, $reg1, $reg2 );
	}else{
	    die "op \$d0(\$d) : $_";
	}
    }elsif( /^([\w\.]+)\s[-\d]+\(\$(\d+)\)$/ ){
	my( $reg1 ) = ( $2 );
	# op 0($d)
	if( $1 =~ /^(dsz)$/ ){
	    #print "$_\tDSZ $1 $reg1\n";
	    ( $op, $src1 ) = ( $1, $reg1 );
	}else{
	    die "op 0(\$d) : $_";
	}
    }elsif( /^([\w\.]+)\s\$([f\d]+),\$([f\d]+),\$([f\d]+)$/ ){
	my( $reg1, $reg2, $reg3 ) = ( $2, $3, $4 );
	# op $d,$d,$d
	if( $1 =~ /^(div[\w\.]*)$/ ){
	    #print "$_\tDIV $1 $reg1, $reg2, $reg3\n";
	    if( $reg1 eq "0" ){
		( $op, $dest, $src1, $src2 ) = ( $1, 64, $reg2, $reg3 );
	    }else{
		( $op, $dest, $src1, $src2 ) = ( $1, $reg1, $reg2, $reg3 );
	    }
	}else{
	    #print "$_\tR $1 $2 $3 $4\n";
	    ( $op, $dest, $src1, $src2 ) = ( $1, $reg1, $reg2, $reg3 );
	}
    }elsif( /^(\w+)\s\$(\d+),\$(\d+),[\w\-]+$/ ){
	# op $d,$d,imm
	#print "$_\tRI $1 $2 $3 imm\n";
	( $op, $dest, $src1, $src2 ) = ( $1, $2, $3, "C" );
    }elsif( /^([\w\.]+)\s\$([f\d]+),\$([f\d]+)$/ ){
	my( $reg1, $reg2 ) = ( $2, $3 );
	# op $d,$d
	if( $1 =~ /^(jalr)$/ ){
	    #print "$_\t JALR $1 $reg1 $reg2\n";
	    ( $op, $dest, $src1 ) = ( $1, $reg1, $reg2 );
	}elsif( $1=~ /^(mul[\w\.]*)$/ ){
	    #print "$_\t MULT $1 $reg1 $reg2\n";
	    ( $op, $dest, $src1, $src2 ) = ( $1, 64, $reg1, $reg2 );
	}elsif( $1 =~ /^(c\.[\w\.]*)/ ){
	    # c.lt $fcc $d $d
	    #print "$_\t  $1 $reg1 $reg2\n";
	    ( $op, $dest, $src1, $src2 ) = ( $1, 66, $reg1, $reg2 );
	}elsif( $1 =~ /^(d*mtc[\w\.]*)/ ){
	    # mtc $d $d
	    # mtc src dest
	    #print "$_\t  $1 $reg1 $reg2\n";
	    ( $op, $src1, $dest ) = ( $1, $reg1, $reg2 );
	}else{
	    #print "$_\t  $1 $reg1 $reg2\n";
	    ( $op, $dest, $src1 ) = ( $1, $reg1, $reg2 );
	}
    }elsif( /^(l\w+i)\s\$(\d+),\d+$/ ){
	# op $d,imm
	#print "$_\t $1 $2\n";
	( $op, $dest, $src1 ) = ( $1, $2, "C" );
    }elsif( /^(\w+)\s\$(\d+)$/ ){
	my( $reg1 ) = ( $2 );
	# op $d
	if( $1 =~ /^(jr)$/ ){ 
	    #print "$_\tJR $1 $reg1\n";
	    ( $op, $src1 ) = ( $1, $reg1 );
	}elsif( $1 =~/^(mflo)$/ ){
	    #print "$_\tMFLH $1 $reg1\n";
	    ( $op, $dest, $src1 ) = ( $1, $reg1, 65 );
	}elsif( $1 =~/^(mfhi)$/ ){
	    #print "$_\tMFLH $1 $reg1\n";
	    ( $op, $dest, $src1 ) = ( $1, $reg1, 64 );
	}else{
	    die "op \$d : $_";
	}
    }elsif( /^(break)\s$/ || /^(syscall)\s$/){
	# op
	#print "B/S $1 2 7 4 5 6\n";
	( $op, $dest, $src1, $src2 ) = ( $1, 2, 4, 5 );
    }else{
	die "ERROR : $_";
    }

    # floating point registerの場合
    if( $dest =~ /^f(\d+)$/ ){
	$dest = $1 + 32;
    }
    if( $src1 =~ /^f(\d+)$/ ){
	$src1 = $1 + 32;
    }
    if( $src2 =~ /^f(\d+)$/ ){
	$src2 = $1 + 32;
    }

    $op = &op_check( $op );

    return( $dest, $src1, $src2, $op );
}

# 和差積商のチェック
sub op_check{
    my( $op ) = @_;

    $_ = $op;

    if( /^add[\w\.]*$/ ){
	# add, addu, addiu, add.d
	$op = "ADD";
    }elsif( /^sub[\w\.]+$/ ){
	# subu, sub.d
	$op = "SUB";
    }elsif( /^mul[\w\.]*$/ ){
	# mult, multu, mul.s, mul.d
	$op = "MUL";
    }elsif( /^div[\w\.]*$/ ){
	# div, divu, div.s, div.d
	$op = "DIV";
    }elsif( /^(jal|jalr)$/ ){
	# jal, jalr
	$op = "JAL";
    }elsif( /^jr$/ ){
	# jr
	$op = "JR";
    }elsif( /^b.+$/ ){
	# b*
	$op = "B";
    }elsif( /^j$/ ){
	# j
	$op = "J";
    }else{
	undef $op;
    }

    return( $op );
}

# 関数毎に、pcとdest/src reg番号と、和差積商の命令を表示
sub print_asm{
    my( $func, $fname ) = @_;

    open( DU, ">>$asm_du" ) || die " can't open $asm_du";
    open( LOOP, ">>$asm_loop" ) || die " can't open $asm_loop";

    print DU "{$func:$fname\n";
    print LOOP "{$func:$fname\n";

    foreach my $pc ( sort keys %inst ){
	my( $dest, $src1, $src2, $op ) = ( @{ $inst{ $pc } } );

	if( defined $op ){
	    if( $op eq "JAL" ){
		# jal, jalr
		print DU "$pc:$dest,$src1 $src2:JAL;\n";
		print LOOP "$op:$pc:;\n";
	    }elsif( $op eq "JR" ){
		if( $src1 == 31 || $src2 == 31 ){
		    #jr ra
		    print DU "$pc:$dest,$src1 $src2:JR-RA;\n";
		}else{
		    #jr jump table
		    print LOOP "$op:$pc:;\n";
		}
	    }elsif( $op eq "B" || $op eq "J" ){
		print DU "$pc:$dest,$src1 $src2;\n";
		print LOOP "$op:$pc:;\n";
	    }else{
		print DU "$pc:$dest,$src1 $src2;\n";
		print LOOP "$op:$pc:$dest,$src1 $src2;\n";

		if( $dest ==64 ){
		    print DU "$pc:65,$src1 $src2;\n";
		    print LOOP "$op:$pc:65,$src1 $src2;\n";
		}
	    }
	}else{
	    print DU "$pc:$dest,$src1 $src2;\n";
	}
    }

    print DU "}\n";
    print LOOP "}\n";

    print STDERR "\rcreating asm_du, asm_loop ($func)";
}
