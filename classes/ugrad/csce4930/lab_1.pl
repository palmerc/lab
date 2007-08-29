#!/usr/bin/perl -w

use strict;

my ($i, $randomwins, $randomlosses, $alwayswins, $alwayslosses, $neverwins, $neverlosses);

srand();

for (my $i = 0; $i < 100000; $i++) {
   # Set the doors to zero
   my @doors = (0, 0, 0);
   my ($eliminated, $selectone, $prize, $final);
   my @remaining = (0, 0);

   # Choose and assign the prize door
   $prize = int(rand(3));
   $doors[$prize] = 1;
   #printf("Shhh - the prize is behind door %d\n", $prize + 1);

   # Choose a door
   $selectone = int(rand(3));
   #print("The contestant first chooses $selectone\n");

   # Eliminate a second door
   for (my $j = 0; $j < 3; $j++) {
      if ($j != $prize && $j != $selectone) {
         $eliminated = $j;
      }
   }
   #print("Monty eliminated $eliminated\n");

   # Randomly pick one of the last two doors

   $final = int(rand(2));
   if ($final == 0) {
   #   print "You win!\n";
      $randomwins++;
   } else {
   #   print "You lose!\n";
      $randomlosses++;
   }

   # Never switch doors
   if ($prize == $selectone) {
      $neverwins++;
   } else {
      $neverlosses++;
   }

   # Always switch doors
   if ($prize != $selectone) {
      $alwayswins++;
   } else {
      $alwayslosses++;
   }
   
}

print("Random: Wins -> $randomwins | Losses -> $randomlosses\n"); 
print("Never switch: Wins -> $neverwins | Losses -> $neverlosses\n"); 
print("Always switch: Wins -> $alwayswins | Losses -> $alwayslosses\n"); 

#Choose a door

#Host opens empty door
#Choose to stay or switch

#What is the outcome