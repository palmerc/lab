#!/usr/bin/perl -w

use strict;

open(OUT, ">terrain.csv");

print OUT "row,column,difficulty\n";

my $i = 0;
while ($i < 18) {
    my $j = 0;
    while ($j < 24) {
        print OUT "$i,$j,1\n";
        $j++;
    }
    $i++;
}

close(OUT);