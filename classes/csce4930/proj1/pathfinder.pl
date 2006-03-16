#!/usr/bin/perl -w

use strict;

srand();

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
    for (my $row = 0; $row <= $num_rows; $row++) {
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
        $north = 0;
    }
    # Check that you can move east
    if ($cur_col <= $num_cols) {
        $east = $map_array->[$cur_row][$cur_col + 1];
    } else {
        $east = 0;
    }
    # Check that you can move south
    if ($cur_row <= $num_rows) {
        $south = $map_array->[$cur_row + 1][$cur_col];
    } else {
        $south = 0;
    }
    # Check that you can move west
    if ($cur_col > 0) {
        $west = $map_array->[$cur_row][$cur_col - 1];
    } else {
        $west = 0;
    }
    
    return ($north, $east, $south, $west);
}

sub moveIt {
    my ($cur_row, $cur_col, @surroundings) = @_;
    my ($north, $east, $south, $west) = @surroundings;
    
    my $direction = int(rand(4));
    
    if (($direction == 0) && ($north != 0)) {
        $cur_row--;
    } elsif (($direction == 1) && ($east != 0)) {
        $cur_col++;
    } elsif (($direction == 2) && ($south != 0)) {
        $cur_row++;
    } elsif (($direction == 3) && ($west != 0)) {
        $cur_col--;
    } else {
        print "Oops\n";
    }
    
    return ($cur_row, $cur_col);
}

my $num_rows = 0;
my $num_cols = 0;
my @map_array;

my $start_row = 5;
my $start_col = 1;
my $end_row = 9;
my $end_col = 21;
my $cur_row = $start_row;
my $cur_col = $start_col;
my $move_count = 0;

open(IN, "<terrain.csv");

loadMap(\*IN, \@map_array);


while (($cur_row != $end_row) && ($cur_col != $end_col)) {
    print "Step $move_count: $cur_row, $cur_col\n";
#    print "Stage 1: $cur_row, $cur_col, @map_array\n";
    my @surroundings = getSurroundingPositions($cur_row, $cur_col, \@map_array);
#    print "Stage 2: $cur_row, $cur_col, @surroundings\n";
    ($cur_row, $cur_col) = moveIt($cur_row, $cur_col, @surroundings);
#    print "Stage 3: $cur_row, $cur_col\n";
    $move_count++;
}

printPath(\@map_array);
print "Move Count: $move_count\n";

close(IN);