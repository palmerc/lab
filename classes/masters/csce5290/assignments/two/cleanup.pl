#!/usr/bin/perl -Wall

use strict;

binmode(STDOUT, ":utf8");
	
	
foreach my $line (<STDIN>) {
	$line =~ s/\s+/ /g;
	$line =~ s/^\s+//;
	$line =~ s/\s+$//;
	$line =~ s/``/"/g;
	$line =~ s/''/"/g;
	$line =~ s/\s\/[(SYM)|\)]//g;
	$line =~ s/^\/[(SYM)|\)]//g;
	$line =~ s/\/\/NN//g;
	print $line;
}