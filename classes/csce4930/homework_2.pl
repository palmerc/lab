#!/usr/bin/perl -w

use strict;

my ($die, $coin, $accum, $move);
$accum = 0;
$move = 0;

srand();

for (my $i=0; $i < 100000; $i++) {
   
   $coin = int(rand(2));
   $die = int(rand(6));
   $die += 1;
   if ($coin == 0) {
      $move = -1 * $die;
      print "MOVE: $move\n";
   } else {
      $move = 1 * $die;
      print "MOVE: $move\n";
   }
   $accum += $move;
}

print "The sum of the moves is $accum\n";
