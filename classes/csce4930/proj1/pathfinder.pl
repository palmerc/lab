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
        print "$cur_row, $cur_col\n";
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
    my ($cur_row, $cur_col, $cur_dif, $end_row, $end_col, $map_array, @surroundings) = @_;
    my ($north, $east, $south, $west) = @surroundings;
    my $num_rows = $#{$map_array};
    my $num_cols = $#{$map_array->[0]};
    
    $cur_dif = (6 - $cur_dif) * 10;
    if ($north != 0) {
        $north = (6 - $north) * 10;
        if (abs($end_row - ($cur_row - 1)) < abs($end_row - $cur_row)) { print "north\n"; $north *= $home_mult; }
    }
    if ($east != 0) {
        $east = (6 - $east) * 10; 
        if (abs($end_col - ($cur_col + 1)) < abs($end_col - $cur_col)) { print "east\n"; $east *= $home_mult; }
    }
    if ($south != 0) {
        $south = (6 - $south) * 10;
        if (abs($end_row - ($cur_row + 1)) < abs($end_row - $cur_row)) { print "south\n"; $south *= $home_mult; }
    }
    if ($west != 0) {
        $west = (6 - $west) * 10;
        if (abs($end_col - ($cur_col - 1)) < abs($end_col - $cur_col)) { print "west\n"; $west *= $home_mult; }
    }
    print "Probabilities: $north, $east, $south, $west\n";
    my $total_dif = $north + $east + $south + $west;
    print "total: $total_dif\n";
    my $direction = int(rand($total_dif));
    print "direction: $direction\n";
    my $north_sec = $north;
    my $east_sec = $north_sec + $east;
    my $south_sec = $east_sec + $south;
    my $west_sec = $south_sec + $west;
    print "$north_sec $east_sec $south_sec $west_sec\n";
    if ($direction < $north_sec) {
        $cur_row--;
    } elsif ($direction < $east_sec) {
        $cur_col++;
    } elsif ($direction < $south_sec) {
        $cur_row++;
    } elsif ($direction < $west_sec) {
        $cur_col--;
    } else {
        print "Oops\n";
    }
    
    return ($cur_row, $cur_col);
}

my $num_rows = 0;
my $num_cols = 0;
my @map_array;
my @surroundings;

my $start_row = 5;
my $start_col = 1;
my $end_row = 9;
my $end_col = 21;
my $cur_row = $start_row;
my $cur_col = $start_col;
my $difficulty = 0.0;
my $move_count = 0;


open(IN, "<terrain.csv");

loadMap(\*IN, \@map_array);
printMap(\@map_array);

while (($cur_row != $end_row) || ($cur_col != $end_col)) {
    print "Step $move_count: $cur_row, $cur_col\n";
#    print "Stage 1: $cur_row, $cur_col, @map_array\n";
    $difficulty = $map_array[$cur_row][$cur_col];
    @surroundings = getSurroundingPositions($cur_row, $cur_col, \@map_array);
#    print "Stage 2: $cur_row, $cur_col, @surroundings\n";
    ($cur_row, $cur_col) = moveIt($cur_row, $cur_col, $difficulty, $end_row, $end_col, \@map_array, @surroundings);
    $route[$move_count] = [ ($cur_row, $cur_col) ];
#    print "Stage 3: $cur_row, $cur_col\n";
    $move_count++;
}
print "Step $move_count: $cur_row, $cur_col\n\n";
#print "@route";
printPath(\@map_array);
print "\nMove Count: $move_count\n";

close(IN);