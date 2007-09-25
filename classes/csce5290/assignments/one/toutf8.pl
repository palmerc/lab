#!/usr/bin/perl

binmode(STDOUT, ":utf8");

foreach $line (<STDIN>) {
	chomp $line;
	$line =~ s/\s\s*/ /g;
	$line =~ s/^\s//g;
	$line =~ s/\s$//g;
	print "$line\n";
}
