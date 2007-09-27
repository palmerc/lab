#!/usr/bin/perl -W

use strict;

use open ":utf8";
binmode(STDOUT, ":utf8");
open FRENCH, "<French.word.model";
open ITALIAN, "<Italian.word.model";
open ENGLISH, "<English.word.model";
open TEST, "<LangId.test.utf8";

print "Read in the models\n";
# Read in language models
my %frenchModel;
foreach my $line (<FRENCH>) {
	$line =~ m/([^\s]+) ([^\s]+) => -?(\d+)/;
	my $key = $1 . " " . $2;
	my $value = $3;
	$frenchModel{"$key"} = $value;
}
my %italianModel;
foreach my $line (<ITALIAN>) {
	$line =~ m/([^\s]+) ([^\s]+) => -?(\d+)/;
	my $key = $1 . " " . $2;
	my $value = $3;
	$italianModel{"$key"} = $value;
}
my %englishModel;
foreach my $line (<ENGLISH>) {
	$line =~ m/([^\s]+) ([^\s]+) => -?(\d+)/;
	my $key = $1 . " " . $2;
	my $value = $3;
	$englishModel{"$key"} = $value;
}

print "Generate results\n"; # Generate results
my $counter = 1;
my $language;
foreach my $line (<TEST>) {
	chomp $line;
	$line = lc $line;
	my $frenchProb = 0;
	my $italianProb = 0;
	my $englishProb = 0;
	my $previous = "\x{2318}";
	my @words = split(" ", $line);
	my $winner = 0;
	my @winnerArray = ();
	foreach my $word (@words) {
                my $key = $previous . " " . $word;
		if (exists $frenchModel{"$key"}) {
			$frenchProb += $frenchModel{"$key"};
		}
		if (exists $italianModel{"$key"}) {
			$italianProb += $italianModel{"$key"};
		}
		if (exists $englishModel{"$key"}) {
			$englishProb += $englishModel{"$key"};
		}
		$previous = $word;
	}
	if ($frenchProb != 0) {
		push(@winnerArray, $frenchProb);
	}
	if ($italianProb != 0) {
		push(@winnerArray, $italianProb);
	}
	if ($englishProb != 0) {
		push(@winnerArray, $englishProb);	
	}
	if (@winnerArray > 0) {
		$winner = (sort {$b <=> $a} @winnerArray)[0];
		#print "$winner\n";
	}
	if ($winner == $frenchProb) {
		$language = "French"; 
	} elsif ($winner == $italianProb) {
		$language = "Italian";
	} elsif ($winner == $englishProb) {
		$language = "English";
	} else {
		$language = "Unidentified";
	}
	#print "$line F=$frenchProb I=$italianProb E=$englishProb\n";
	print $counter . " " . $language . "\n";
	$counter++;
}
