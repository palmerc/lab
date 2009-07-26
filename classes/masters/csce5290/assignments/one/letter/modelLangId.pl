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
for my $line (<FH>) {
	chomp $line;
	$line = lc $line;
	@document = (@document, $line);
}

close(FH);

# Load up the letter pairs into a hash with their individual counts.
my %bigramHash;
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
			#print $key . " " . $bigramHash{"$key"}{'count'} . "\n";
			if ($previous ne "\x{2318}" && !exists $bigramHash{"$reversekey"}) {
				$bigramHash{"$reversekey"}{'count'} = 0;
				#print "reverse " . $reversekey . "\n";
			}
		}
		$previous = $char;
	}
}

# Add one to all the counts in the matrix
foreach my $key (keys(%bigramHash)) {
	$bigramHash{"$key"}{'count'} += 1;
}

# Generate the bigram prefix counts
my %prefixLetterHash;
foreach my $key (keys(%bigramHash)) {
	my $prefixLetter = (split("", $key))[0];
	$prefixLetterHash{"$prefixLetter"}{'count'} +=
		$bigramHash{"$key"}{'count'};
}

# Print out the model
foreach my $key (keys(%bigramHash)) {
	my $prefixLetter = (split("", $key))[0];
	$bigramHash{"$key"}{'probability'} =
		# Get log base two of the probability
		log($bigramHash{"$key"}{'count'} / $prefixLetterHash{"$prefixLetter"}{'count'}) / log(2);
}

# Dump the hash
foreach my $key (sort keys(%bigramHash)) {
	print $key . " => " . $bigramHash{"$key"}{'probability'} . "\n";
}
