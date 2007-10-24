#!/usr/bin/perl -Wall

use strict;

use open ":utf8";
binmode(STDOUT, ":utf8");

if ($#ARGV < 0) {
	print "Usage $0: file\n";
	exit 1;
}

open(FH, $ARGV[0]);
my @document = <FH>;
close(FH);

my %tagHash;
my %wordtagHash;
my $linecount = 1;
foreach my $line (@document) {
	my $pretag = ".";
	foreach my $wordtag (split(" ", $line)) {
		if ($wordtag =~ /([^\/]+)\/([^\/]+)/) {
			my $word = lc $1;
			my $curtag = $2;
			
			# Bigram probability of curtag|pretag
			my $keytag = $pretag . " " . $curtag;
			# reverse tag is for the add-one smoothing.
			my $reversekeytag = $curtag . " " . $pretag;
			
			# Add one to the bigram count
			if (exists $tagHash{"$keytag"}) {
				# if the key has been seen before add one to our count
				$tagHash{"$keytag"}{'count'} += 1;
			} else {
				# otherwise this is a new key. initialize to one and generate a
				# reversekey initialized to zero for later add one smoothing.
				$tagHash{"$keytag"}{'count'} = 1;
				if ($pretag ne "." && !exists $tagHash{"$reversekeytag"}) {
					$tagHash{"$reversekeytag"}{'count'} = 0;
				}
			}
			$pretag = $curtag;
			
			# Bigram probability of word|curtag
			my $keywtag = $curtag . " " . $word;

			# Add one to the bigram count
			if (exists $wordtagHash{"$keywtag"}) {
				# if the key has been seen before add one to our count
				$wordtagHash{"$keywtag"}{'count'} += 1;
			} else {
				# otherwise this is a new key. initialize to one and generate a
				# reversekey initialized to zero for later add one smoothing.
				$wordtagHash{"$keywtag"}{'count'} = 1;
			}
		} else {
			print STDERR "Token error on line $linecount, $wordtag";
		}
	}
	$linecount++;
}


# Add one to all the counts in the matrix
foreach my $key (keys(%tagHash)) {
	$tagHash{"$key"}{'count'} += 1;
}

# Generate the Tag|Tag bigram prefix counts
my %prefixTagHash;
foreach my $key (keys(%tagHash)) {
	my $prefixTag = (split(" ", $key))[0];
	$prefixTagHash{"$prefixTag"}{'count'} += $tagHash{"$key"}{'count'};
}

# Generate the Word|Tag bigram prefix counts
my %prefixWordTagHash;
foreach my $key (keys(%wordtagHash)) {
	my $prefixTag = (split(" ", $key))[0];
	$prefixWordTagHash{"$prefixTag"}{'count'} += $wordtagHash{"$key"}{'count'};
}

# Do the math
foreach my $key (keys(%tagHash)) {
	my $prefixTag = (split(" ", $key))[0];
	$tagHash{"$key"}{'probability'} =
		# Get log base two of the probability
		log($tagHash{"$key"}{'count'} / $prefixTagHash{"$prefixTag"}{'count'}) / log(2);
}

foreach my $key (keys(%wordtagHash)) {
	my $prefixTag = (split(" ", $key))[0];
	$wordtagHash{"$key"}{'probability'} =
		# Get log base two of the probability
		log($wordtagHash{"$key"}{'count'} / $prefixWordTagHash{"$prefixTag"}{'count'}) / log(2);
}

# Dump the hash
foreach my $key (sort keys(%tagHash)) {
	print $key . " => " . $tagHash{"$key"}{'probability'} . "\n";
}

foreach my $key (sort keys(%wordtagHash)) {
	print $key . " => " . $wordtagHash{"$key"}{'probability'} . "\n";
}
