#!/usr/bin/perl -w

use strict;

srand();
our @route;
our $home_mult = 10;

sub loadMap {
    my $fh = shift;
    my $map_array = shift;
    my $first_line = <$fh>;
    foreach my $line (<$fh>) {
        chomp $line;
        my ($row, $col, $difficulty) = split(/,/, $line);
        $map_array->[$row][$col] = $difficulty;
    }
}

sub printPath {
    my $map_array = shift;
    # Behold the ridiculous reference syntax in perl
    my $num_rows = $#{$map_array};
    my $num_cols = $#{$map_array->[0]};
    
    for (my $move = 0; $move < scalar(@route); $move++) {
        my ($cur_row, $cur_col) = @{$route[$move]};
        #print "$cur_row, $cur_col\n";
        $map_array->[$cur_row][$cur_col] = '...';
    }
    
    for (my $row = 0; $row <= $num_rows; $row++) {
        for (my $col = 0; $col <= $num_cols; $col++) {
            print $map_array->[$row][$col] . " ";
        }
        print "\n";
    }
}

sub printMap {
    my $map_array = shift;
    # Behold the ridiculous reference syntax in perl
    my $num_rows = $#{$map_array};
    my $num_cols = $#{$map_array->[0]};
    for (my $row = 0; $row <= $num_rows; $row++) {
        print "Row $row: ";
        for (my $col = 0; $col <= $num_cols; $col++) {
            print $map_array->[$row][$col] . " ";
        }
        print "\n";
    }
}

sub getSurroundingPositions {
    my ($cur_row, $cur_col, $map_array) = @_;
    my $num_rows = $#{$map_array};
    my $num_cols = $#{$map_array->[0]};
    
    my ($north, $east, $south, $west);
    #print "getSurroundingPositions: $cur_row, $cur_col, $map_array\n";
    # Check that you can move north
    if ($cur_row > 0) {
        $north = $map_array->[$cur_row - 1][$cur_col];
    } else {
        $north = 0.0;
    }
    # Check that you can move east
    if ($cur_col < $num_cols) {
        $east = $map_array->[$cur_row][$cur_col + 1];
    } else {
        $east = 0.0;
    }
    # Check that you can move south
    if ($cur_row < $num_rows) {
        $south = $map_array->[$cur_row + 1][$cur_col];
    } else {
        $south = 0.0;
    }
    # Check that you can move west
    if ($cur_col > 0) {
        $west = $map_array->[$cur_row][$cur_col - 1];
    } else {
        $west = 0.0;
    }
    
    return ($north, $east, $south, $west);
}

sub moveIt {
    my ($cur_row, $cur_col, $cur_dif, $last_move, $end_row, $end_col, $map_array, @surroundings) = @_;
    my ($north, $east, $south, $west) = @surroundings;
    my $num_rows = $#{$map_array};
    my $num_cols = $#{$map_array->[0]};
    
    if ($last_move eq 'N') { $north = 0; }
    if ($last_move eq 'E') { $east = 0; }
    if ($last_move eq 'S') { $south = 0; }
    if ($last_move eq 'W') { $west = 0; }
    
     
    if ($north != 0) {
        $north = (1/$north) * 100;
        if (abs($end_row - ($cur_row - 1)) < abs($end_row - $cur_row)) { $north *= $home_mult; }
    }
    if ($east != 0) {
        $east = (1/$east) * 100;
        
        if (abs($end_col - ($cur_col + 1)) < abs($end_col - $cur_col)) { $east *= $home_mult; }
    }
    if ($south != 0) {
        $south = (1/$south) * 100;
        
        if (abs($end_row - ($cur_row + 1)) < abs($end_row - $cur_row)) { $south *= $home_mult; }
    }
    if ($west != 0) {
        $west = (1/$west) * 100;
        
        if (abs($end_col - ($cur_col - 1)) < abs($end_col - $cur_col)) { $west *= $home_mult; }
    }
    
 #   print "probabilities: $north, $east, $south, $west\n";
    my $total_dif = $north + $east + $south + $west;
    #print "total: $total_dif\n";
    my $direction = int(rand($total_dif));
    #print "direction: $direction\n";
    my $north_sec = $north;
    my $east_sec = $north_sec + $east;
    my $south_sec = $east_sec + $south;
    my $west_sec = $south_sec + $west;
    #print "sections: $north_sec $east_sec $south_sec $west_sec\n";
    if ($direction < $north_sec) {
        $last_move = 'N';
        $cur_row--;
    } elsif ($direction < $east_sec) {
        $last_move = 'E';
        $cur_col++;
    } elsif ($direction < $south_sec) {
        $last_move = 'S';
        $cur_row++;
    } elsif ($direction < $west_sec) {
        $last_move = 'W';
        $cur_col--;
    } else {
        print "Oops\n";
    }
    
    return ($cur_row, $cur_col, $last_move);
}

my $num_rows = 0;
my $num_cols = 0;

my @map_array;
my @surroundings;
my $counter = 0;
my $move_sum = 0;
my $low;
my $highest = 0;

for (my $i=0; $i < 1000; $i++) {
   `sleep 1`;
   system "clear";
   my $start_row = 5;
   my $start_col = 1;
   my $end_row = 9;
   my $end_col = 21;
   my $cur_row = $start_row;
   my $cur_col = $start_col;
   my $difficulty = 0.0;
   my $move_count = 0;
   my $last_move = 'B';
   
   
   
   open(IN, "<terrain.csv");
   loadMap(\*IN, \@map_array);
   #printMap(\@map_array);

   while (($cur_row != $end_row) || ($cur_col != $end_col)) {
   #    print "Step $move_count: $cur_row, $cur_col\n";
   #    print "Stage 1: $cur_row, $cur_col, @map_array\n";
       $difficulty = $map_array[$cur_row][$cur_col];
       @surroundings = getSurroundingPositions($cur_row, $cur_col, \@map_array);
   #    print "Stage 2: $cur_row, $cur_col, @surroundings\n";
       ($cur_row, $cur_col, $last_move) = moveIt($cur_row, $cur_col, $difficulty, $last_move, $end_row, $end_col, \@map_array, @surroundings);
       $route[$move_count] = [ ($cur_row, $cur_col) ];
   #    print "Stage 3: $cur_row, $cur_col\n";
       $move_count++;
   }

   if (!$low) {
      $low = 1000;
   }
   if ($low > $move_count) {
      $low = $move_count;
   }
   if ($highest < $move_count) {
      $highest = $move_count;
   }

   print "Step $move_count: $cur_row, $cur_col\n\n";
   #print "@route";
   printPath(\@map_array);
   $counter++;
   $move_sum += $move_count;
   
print "Iteration: $counter\n";
print "Average Moves: " . int($move_sum/$counter) . "\n";
   close(IN);
}

#print "Move Count: $move_count\n";
print "$low $highest\n";
