#!/usr/bin/perl -W

use strict;

use open ":utf8";
binmode(STDOUT, ":utf8");
open FRENCH, "<French.model";
open ITALIAN, "<Italian.model";
open ENGLISH, "<English.model";
open TEST, "<../originals/LangId.test.utf8";

# Read in language models
my %frenchModel;
foreach my $line (<FRENCH>) {
	$line =~ m/(.{2}) => -?(\d+)/;
	my $key = $1;
	my $value = $2;
	$frenchModel{"$key"} = $value;
}
my %italianModel;
foreach my $line (<ITALIAN>) {
	$line =~ m/(.{2}) => -?(\d+)/;
	my $key = $1;
	my $value = $2;
	$italianModel{"$key"} = $value;
}
my %englishModel;
foreach my $line (<ENGLISH>) {
	$line =~ m/(.{2}) => -?(\d+)/;
	my $key = $1;
	my $value = $2;
	$englishModel{"$key"} = $value;
}

# Generate results
my $counter = 1;
my $language;
foreach my $line (<TEST>) {
	chomp $line;
	my $frenchProb;
	my $italianProb;
	my $englishProb;
	my $previous = "\x{2318}";
	my @chars = split("", $line);
	foreach my $char (@chars) {
                my $key = $previous . $char;
		if (exists $frenchModel{"$key"}) {
			$frenchProb += $frenchModel{"$key"};
		}
		if (exists $italianModel{"$key"}) {
			$italianProb += $italianModel{"$key"};
		}
		if (exists $englishModel{"$key"}) {
			$englishProb += $englishModel{"$key"};
		}
		$previous = $char;
	}
	my @stupidArray = ($frenchProb, $italianProb, $englishProb);
	#print "\n$line F=$frenchProb I=$italianProb E=$englishProb\n";
	my $winner = (sort {$a <=> $b} @stupidArray)[0];
	if ($winner == $frenchProb) {$language = "French"};
	if ($winner == $italianProb) {$language = "Italian"};
	if ($winner == $englishProb) {$language = "English"};
	print $counter . " " . $language . "\n";
	$counter++;
}
