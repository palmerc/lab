#!/usr/bin/perl -w

use strict;

my $out;

open(OUTPUT, ">./file.in");

for (my $i = 0; $i < 128000; $i++) {
   print OUTPUT "********";
}
