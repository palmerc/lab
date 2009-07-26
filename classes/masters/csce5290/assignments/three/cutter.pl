#!/usr/bin/perl

use strict;
use XML::Simple;

my $data;
my $xml;
my $inputFile;
my $testFile;
my $trainFile;


if (scalar(@ARGV) < 3) {
	print("Usage: $0 inputFile testFile trainFile\n");
	exit 1;
} else {
	$inputFile = $ARGV[0];
	$testFile = $ARGV[1];
	$trainFile = $ARGV[2];
}

open(FH, "<", $inputFile);
open(TEST, ">", $testFile);
open(TRAIN, ">", $trainFile);
print TEST "<?xml version='1.0'?>\n<document>\n";
print TRAIN "<?xml version='1.0'?>\n<document>\n";

my $xml = new XML::Simple;
my $data = $xml->XMLin($inputFile);

my $fiveFifths = scalar keys %{$data->{'instance'}};

my $count = 0;
foreach my $line (<FH>) {
	if ($count < $fiveFifths / 5 * 7) {
		print TEST $line;
	} else {
		print TRAIN $line;
	}
	$count += 1;
}

print TEST "</document>\n";
print TRAIN "</document>\n";

close(FH);
close(TEST);
close(TRAIN);
