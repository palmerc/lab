#!/usr/bin/perl

use strict;
use XML::Simple;
use Data::Dumper;

my ($trainData, $testData);
my $xml;
my ($trainFile, $testFile);
my $word;
my (%senseCount, %senseTotals);
my $baseCorrect = 0;

sub cleanString {
	my $string = shift;
	chomp $string;
	$string =~ s/[(),.?!"'-;]//g;
	$string =~ s/\s+/ /g;
	$string =~ s/^\s+//;
	$string =~ s/\s+$//;
	
	return $string;
}

sub addOneSmoothing {

}

if (scalar(@ARGV) < 2) {
	#print("Usage: $0 trainFile testFile\n");
	exit 1;
} else {
	$trainFile = $ARGV[0];
	$testFile = $ARGV[1];
}

$xml = new XML::Simple;
$trainData = $xml->XMLin($trainFile);
$testData = $xml->XMLin($testFile);

# Retrieve each instance and its sense.
foreach my $instance (keys %{$trainData->{'instance'}}) {
	#print $instance . "\n";
	my ($firstHalf, $secondHalf);
	my $answer = $trainData->{'instance'}{$instance}{'answer'}{'senseid'};
	my $sense = (split(/%/, $answer))[1];
	$word = $trainData->{'instance'}{$instance}{'context'}{'head'};
	$firstHalf = $trainData->{'instance'}{$instance}{'context'}{'content'}[0];
	$secondHalf = $trainData->{'instance'}{$instance}{'context'}{'content'}[1];

	$firstHalf = cleanString($firstHalf);
	$secondHalf = cleanString($secondHalf);

	#print ">$firstHalf<\n";

	my @firstArray = split(/\s/, $firstHalf);
	my @secondArray = split(/\s/, $secondHalf);

	# I will count the positions of the individual appearences of the words.
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
	$senseTotals{$sense}{'count'} += 4;

	# Count the occurence of the two words on either side of our word to disambiguate
	$senseCount{$sense}{$firstArray[-2]}[0] += 1;
	$senseCount{$sense}{$firstArray[-1]}[1] += 1;
	$senseCount{$sense}{$secondArray[0]}[2] += 1;
	$senseCount{$sense}{$secondArray[1]}[3] += 1;

}

foreach my $sense (keys %senseCount) {
	foreach my $feature (keys %{$senseCount{$sense}}) {
		for (my $i = 0; $i < 4; $i++) {
			#print "$sense, $feature, $i\n";
			if ($senseCount{$sense}{$feature}[$i] == 0) {
				next;
			}
			$senseCount{$sense}{$feature}[$i] = log ($senseCount{$sense}{$feature}[$i] / $senseTotals{$sense}{'count'}) / log (2) . "\n";
		}
	}
}

# Sum the total number of sense instances
my $senseSum = 0;
foreach my $sense (keys %senseTotals) {
	$senseSum += $senseTotals{$sense}{'count'};
}

# Perform the test
my $correctTotal = 0;
my $totalTotal = 0;
foreach my $instance (keys %{$testData->{'instance'}}) {
	my $winner = -1;
	my $winningSense = "";
	my ($firstHalf, $secondHalf);
	my $answer = $testData->{'instance'}{$instance}{'answer'}{'senseid'};
	my $correctSense = (split(/%/, $answer))[1];
	$word = $testData->{'instance'}{$instance}{'context'}{'head'};
	$firstHalf = $testData->{'instance'}{$instance}{'context'}{'content'}[0];
	$secondHalf = $testData->{'instance'}{$instance}{'context'}{'content'}[1];

	$firstHalf = cleanString($firstHalf);
	$secondHalf = cleanString($secondHalf);

	my @firstArray = split(/\s/, $firstHalf);
	my @secondArray = split(/\s/, $secondHalf);

	#print "\n$word $answer\n";
	foreach my $sense (keys %senseCount) {
		my $sumProb = 0;
		$sumProb = $senseCount{$sense}{$firstArray[-2]}[0];
		$sumProb += $senseCount{$sense}{$firstArray[-1]}[1];
		$sumProb += $senseCount{$sense}{$secondArray[0]}[2];
		$sumProb += $senseCount{$sense}{$secondArray[1]}[3];
		$sumProb += log($senseTotals{$sense}{'count'} / $senseSum)/log(2);
		$sumProb = abs($sumProb);
		if ($winner < $sumProb) {
			$winner = $sumProb;
			$winningSense = $sense;	
		}
	}
	if ($winningSense eq $correctSense) {
		$correctTotal += 1;
	}
	$totalTotal += 1;
}
print "$word accuracy " . $correctTotal / $totalTotal * 100 . "%\n";

exit;
# Dump the sense hash
print "$word\n";
foreach my $sense (keys %senseCount) {
	print "$sense:\n";
	foreach my $feature (keys %{$senseCount{$sense}}) {
		for (my $i = 0; $i < 4; $i++) {
			print "\t$feature [$i] $senseCount{$sense}{$feature}[$i]\n";
		}
	}
}

