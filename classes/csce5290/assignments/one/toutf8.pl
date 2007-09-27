#!/usr/bin/perl

binmode(STDOUT, ":utf8");

foreach $line (<STDIN>) {
	chomp $line;
	$line =~ s/& # x\d+ ;//g;
	$line =~ s/-//g;
	$line =~ s/(\w)([,'"?.!:])/$1 $2/g;
	$line =~ s/([,'"?.!:])(\w)/$1 $2/g;
	$line =~ s/\s\s*/ /g;
	$line =~ s/^\s//g;
	$line =~ s/\s$//g;
	print "$line\n";
}
