#!/usr/bin/perl

use strict;
use XML::Simple;
use Data::Dumper;

my $data;
my $xml;
my $inputFile;
my $word;
my %senseCount;
my $baseCorrect = 0;
my $totalCount = 0;


if (scalar(@ARGV) < 1) {
	print("Usage: $0 inputFile\n");
	exit 1;
} else {
	$inputFile = $ARGV[0];
}
print "inputFile: $inputFile\n";

$xml = new XML::Simple;
$data = $xml->XMLin($inputFile);

# Retrieve each instance and its sense.
foreach my $instance (keys %{$data->{'instance'}}) {
	print $instance . "\n";
	my ($firstHalf, $secondHalf);
	my $answer = $data->{'instance'}{$instance}{'answer'}{'senseid'};
	my $sense = (split(/%/, $answer))[1];
	$word = $data->{'instance'}{$instance}{'context'}{'head'};
	if (defined ($data->{'instance'}{$instance}{'context'}{'content'}[0])) {
		$firstHalf = $data->{'instance'}{$instance}{'context'}{'content'}[0];
	} else {
		$firstHalf = ". ";
	}
	$secondHalf = $data->{'instance'}{$instance}{'context'}{'content'}[1];

	chomp($firstHalf);
	chomp($secondHalf);
	$firstHalf =~ s/[(),.?!"'-;]//g;
	$secondHalf =~ s/[(),.?!"'-;]//g;
	$firstHalf =~ s/\s+/ /g;
	$secondHalf =~ s/\s+/ /g;
	$firstHalf =~ s/^\s+//;
	$firstHalf =~ s/\s+$//;
	$secondHalf =~ s/^\s+//;
	$secondHalf =~ s/\s+$//;

	print ">$firstHalf<\n";

	my @firstArray = split(/\s/, $firstHalf);
	my @secondArray = split(/\s/, $secondHalf);

	for (my $j = -1; $j > -3; $j--) {
		if (!exists $senseCount{$sense}{$firstArray[$j]}) { 
			for (my $i = 0; $i < 4; $i++) {
				$senseCount{$sense}{$firstArray[$j]}[$i] = 0; 
			}
		} 
	}
	for (my $j = 0; $j < 2; $j++) {
		if (!exists $senseCount{$sense}{$secondArray[$j]}) { 
			for (my $i = 0; $i < 4; $i++) {
				$senseCount{$sense}{$secondArray[$j]}[$i] = 0; 
			}
		} 
	}

	$senseCount{$sense}{$firstArray[-2]}[0] += 1;
	$senseCount{$sense}{$firstArray[-1]}[1] += 1;
	$senseCount{$sense}{$secondArray[0]}[2] += 1;
	$senseCount{$sense}{$secondArray[1]}[3] += 1;

	$totalCount += 1;
}

print "$word\n";
foreach my $sense (keys %senseCount) {
	print "$sense:\n";
	foreach my $word (keys %{$senseCount{$sense}}) {
		for (my $i = 0; $i < 4; $i++) {
			print "\t$word [$i] $senseCount{$sense}{$word}[$i]\n";
		}
	}
}

