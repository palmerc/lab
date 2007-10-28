#!/usr/bin/perl -W

use strict;

use open ":utf8";
binmode(STDOUT, ":utf8");

if ($#ARGV < 0) {
	print "Usage $0: file\n";
	exit 1;
}

open(FH, $ARGV[0]);

my @document;
foreach my $line (<FH>) {
	chomp $line;
	$line = lc $line;
	@document = (@document, $line);
}

close(FH);

# Load up the letter pairs into a hash with their individual counts.
my %bigramHash;
foreach my $line (@document) {
	my $previous = "\x{2318}";
	my @words = split(" ", $line);
	foreach my $word (@words) {
		my $key = $previous . " " . $word;
		my $reversekey = $word . " " . $previous;
		# Add one to the bigram count
		#print $key . "\n";
		if (exists $bigramHash{"$key"}) {
			$bigramHash{"$key"}{'count'} =
				$bigramHash{"$key"}{'count'} + 1;
		} else {
			$bigramHash{"$key"}{'count'} = 1;
			if ($previous ne "\x{2318}" && !exists $bigramHash{"$reversekey"}) {
				$bigramHash{"$reversekey"}{'count'} = 0;
			}
		}
		$previous = $word;
	}
}

# Add one to all the counts in the matrix
foreach my $key (keys(%bigramHash)) {
	$bigramHash{"$key"}{'count'} += 1;
}

# Generate the bigram prefix counts
my %prefixWordHash;
foreach my $key (keys(%bigramHash)) {
	my $prefixWord = (split(" ", $key))[0];
	$prefixWordHash{"$prefixWord"}{'count'} +=
		$bigramHash{"$key"}{'count'};
}

# Print out the model
foreach my $key (keys(%bigramHash)) {
	my $prefixWord = (split(" ", $key))[0];
	$bigramHash{"$key"}{'probability'} =
		# Get log base two of the probability
		log($bigramHash{"$key"}{'count'} / $prefixWordHash{"$prefixWord"}{'count'}) / log(2);
}

# Dump the hash
foreach my $key (sort keys(%bigramHash)) {
	print $key . " => " . $bigramHash{"$key"}{'probability'} . "\n";
}
