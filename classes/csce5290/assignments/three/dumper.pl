#!/usr/bin/perl

use strict;
use XML::Simple;
use Data::Dumper;

my $data;
my $xml;
my $inputFile;

if (scalar(@ARGV) < 1) {
	print("Usage: $0 inputFile\n");
	exit 1;
} else {
	$inputFile = $ARGV[0];
}

$xml = new XML::Simple;
$data = $xml->XMLin($inputFile);

print Dumper($data);
