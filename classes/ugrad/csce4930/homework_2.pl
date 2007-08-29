#!/usr/bin/perl -w

use strict;

my ($die, $coin, $accum, $move, $runs);
$accum = 0;
$move = 0;
$runs = 0;

srand();

while (1) {
   
for (my $i=0; $i < 100000; $i++) {
   
   $coin = int(rand(2));
   $die = int(rand(6));
   $die += 1;
   if ($coin == 0) {
      $move = -1 * $die;
#      print "MOVE: $move\n";
   } else {
      $move = 1 * $die;
#      print "MOVE: $move\n";
   }
   $accum += $move;
}
$runs++;
print "Average " . int($accum/$runs) . "\n";
}

print "The sum of the moves is $accum\n";
