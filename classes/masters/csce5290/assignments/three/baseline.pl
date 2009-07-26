#!/usr/bin/perl

use strict;
use XML::Simple;

my $data;
my $xml;
my ($testFile, $trainFile);
my %senseCount;
my $baseCorrect = 0;
my $totalCount = 0;


if (scalar(@ARGV) < 2) {
	print("Usage: $0 trainFile testFile\n");
	exit 1;
} else {
	$trainFile = $ARGV[0];
	$testFile = $ARGV[1];
}
$xml = new XML::Simple;
my $trainData = $xml->XMLin($trainFile);
my $testData = $xml->XMLin($testFile);

# Train the baseline 
foreach my $instance (keys %{$trainData->{'instance'}}) {
	my $answer = $trainData->{'instance'}{$instance}{'answer'}{'senseid'};
	my ($word, $correctSense) = split(/%/, $answer);

	$senseCount{$word}{$correctSense} += 1;
}

# Perform the Test
foreach my $instance (keys %{$testData->{'instance'}}) {
	my $answer = $testData->{'instance'}{$instance}{'answer'}{'senseid'};
	my ($word, $correctSense) = split(/%/, $answer);
	
	# Find the most common sense of this word.
	my $mostCommonSense = (sort({$senseCount{$word}{$b} <=> $senseCount{$word}{$a}} keys %{$senseCount{$word}}))[0];

	if ($correctSense eq $mostCommonSense) {
		$baseCorrect += 1;
	}
	$totalCount += 1;
}

print "Baseline: " . $baseCorrect / $totalCount * 100 . "%.\n";
