#!/usr/bin/perl -w

use strict;

sub printPath {
    my $map_array = shift;
    # Behold the ridiculous reference syntax in perl
    my $num_rows = $#{$map_array};
    my $num_cols = $#{$map_array->[0]};
    for (my $row = 0; $row <= $num_rows; $row++) {
        for (my $col = 0; $col <= $num_cols; $col++) {
            print $map_array->[$row][$col];
        }
        print "\n";
    }
}

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

my $num_rows = 0;
my $num_cols = 0;
my @map_array;

my $start_row = 5;
my $start_col = 1;
my $end_row = 9;
my $end_col = 21;

open(IN, "<terrain.csv");

loadMap(\*IN, \@map_array);
printPath(\@map_array);

close(IN);