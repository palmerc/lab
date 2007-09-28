#!/usr/bin/perl -W

use strict;

use open ":utf8";
binmode(STDOUT, ":utf8");

our %bigramHash;
sub Sorter {
	$bigramHash{$a}{'count'} <=> $bigramHash{$b}{'count'};
}

if ($#ARGV < 0) {
	print "Usage $0: file\n";
	exit 1;
}

open(FH, $ARGV[0]);

my @document;
for my $line (<FH>) {
	chomp $line;
	$line = lc $line;
	@document = (@document, $line);
}

close(FH);

my $bigramCount = 0;
# Load up the letter pairs into a hash with their individual counts.
foreach my $line (@document) {
#	print $line . "\n";
	my $previous = "\x{2318}";
	my @chars = split("", $line);
	foreach my $char (@chars) {
		my $key = $previous . $char;
		my $reversekey = $char . $previous;
		# Add one to the bigram count
		if (exists $bigramHash{"$key"}) {
			$bigramHash{"$key"}{'count'} =
				$bigramHash{"$key"}{'count'} + 1;
			#print $key . " " . $bigramHash{"$key"}{'count'} . "\n";
		} else {
			$bigramHash{"$key"}{'count'} = 1;
			$bigramCount += 1;
			#print $key . " " . $bigramHash{"$key"}{'count'} . "\n";
			if ($previous ne "\x{2318}" && !exists $bigramHash{"$reversekey"}) {
				$bigramHash{"$reversekey"}{'count'} = 0;
				$bigramCount += 1;
				#print "reverse " . $reversekey . "\n";
			}
		}
		$previous = $char;
	}
}

# Count the number of bigram occurences < 10 for smoothing
my $current = 0;
my $previous = 0;
my $counter = 0;
my %bigramClass;
foreach my $key (sort Sorter (keys(%bigramHash))) {
	$current = $bigramHash{"$key"}{'count'};
	if ($previous <= 10) {
		if ($current == $previous) { $counter += 1; }
		else { $bigramClass{"$previous"} = $counter }
	} else {
		last;
	}
	$previous = $current;
}

# Generate the bigram prefix counts
my %prefixLetterHash;
foreach my $key (keys(%bigramHash)) {
	my $prefixLetter = (split("", $key))[0];
	$prefixLetterHash{"$prefixLetter"}{'count'} +=
		$bigramHash{"$key"}{'count'};
}

foreach my $key (keys(%bigramHash)) {
	my $class = $bigramHash{"$key"}{'count'};
	if ($class < 10) {
		#print "Hello K=$key C=$class BC=" . $bigramClass{"$class"} . " TBC=$bigramCount\n";
		my $nextClass = $class + 1;
		$bigramHash{"$key"}{'probability'} = log((($nextClass)*($bigramClass{"$nextClass"} / $bigramClass{"$class"})) / $bigramCount) / log(2);
		#print $bigramHash{"$key"}{'probability'} . "\n";
	}
}

# Print out the model
foreach my $key (keys(%bigramHash)) {
	my $prefixLetter = (split("", $key))[0];
	if ($bigramHash{"$key"}{'count'} >= 10) {
		$bigramHash{"$key"}{'probability'} =
		# Get log base two of the probability
			log($bigramHash{"$key"}{'count'} / $prefixLetterHash{"$prefixLetter"}{'count'}) / log(2);
	}
}

# Dump the hash
foreach my $key (sort keys(%bigramHash)) {
	print $key . " => " . $bigramHash{"$key"}{'probability'} . "\n";
}
