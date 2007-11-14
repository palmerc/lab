#!/usr/bin/perl

use strict;
use XML::Simple;
use Data::Dumper;

my $data;
my $xml;
my $inputFile;
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
foreach my $sense (keys %{$data->{'instance'}}) {
#	print "\n\n";
	my $answer = $data->{'instance'}{$sense}{'answer'}{'senseid'};
	my $sense = (split(/%/, $answer))[1];
	my $word = $data->{'instance'}{$sense}{'context'}{'head'};
	my $firstHalf = $data->{'instance'}{$sense}{'context'}{'content'}[0];
	my $secondHalf = $data->{'instance'}{$sense}{'context'}{'content'}[1];

	chomp($firstHalf);
	chomp($secondHalf);
	$firstHalf =~ s/[(),.?!"'-]//g;
	$secondHalf =~ s/[(),.?!"'-]//g;
	$firstHalf =~ s/\s+/ /g;
	$secondHalf =~ s/\s+/ /g;

	my @firstArray = split(/\s/, $firstHalf);
	my @secondArray = split(/\s/, $secondHalf);

#	print "$firstArray[-2] $firstArray[-1]\n";
#	print "$word\n";
#	print "$secondArray[-2] $secondArray[-1]\n";

	$senseCount{$word}{$sense} += 1;
	my $mostCommonSense = (sort({$senseCount{$word}{$b} <=> $senseCount{$word}{$a}} keys %{$senseCount{$word}}))[0];
	if ($sense eq $mostCommonSense) {
		print "$sense == $mostCommonSense\n";
		$baseCorrect += 1;
	}
	$totalCount += 1;
}

print "Total baseline accuracy: " . $baseCorrect / $totalCount * 100 . "%.\n";
print Dumper(%senseCount);
