#!/usr/bin/perl -W

use strict;

use open ":utf8";
binmode(STDOUT, ":utf8");

for my $line (<STDIN>) {
	$line =~ s/& # x(\d+) ;//g;
	print $line;
}
