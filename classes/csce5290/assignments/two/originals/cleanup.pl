#!/usr/bin/perl -W

use strict;
use open ":utf8";

open(FH, "$ARGV[0]") or die("Unable to open input file."); 
my @document = ();
foreach my $line (<FH>) {
	chomp $line;
	$line =~ s/& # x([0-9A-Fa-f]+) ;/chr(hex("0x$1"))/ge;
	$line =~ s/-//g;
	$line =~ s/(\w)(\p{IsPunct})/$1 $2/g;
	$line =~ s/(\p{IsPunct})(\w)/$1 $2/g;
	$line =~ s/\s\s*/ /g;
	$line =~ s/^\s//g;
	$line =~ s/\s$//g;
	$line .= "\n";
	push @document, $line;
}
close(FH);

open(FH, ">$ARGV[0]") or die("Unable to open input file.");
foreach my $line (@document) {
	print FH $line;
}
close(FH);
