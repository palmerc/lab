#!/usr/bin/perl -Wall

use strict;

use open ":utf8";
binmode(STDOUT, ":utf8");

my @viterbi;
my @backpointer;

if ($#ARGV < 1) {
	print "Usage $0: model test\n";
	exit 1;
}

open(FH, $ARGV[0]);
my @model_document = <FH>;
close(FH);

open(FH, $ARGV[1]);
my @test_document = <FH>;
close(FH);

my %tagHash;
my %wordTagHash;
my $linecount = 1;
foreach my $line (@model_document) {
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
			# Add one to the bigram count
			if (exists $wordTagHash{"$word"}{"$curtag"}) {
				# if the key has been seen before add one to our count
				$wordTagHash{"$word"}{"$curtag"}{'count'} += 1;
			} else {
				# otherwise this is a new key. initialize to one and generate a
				# reversekey initialized to zero for later add one smoothing.
				$wordTagHash{"$word"}{"$curtag"}{'count'} = 1;
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
my %prefixWordHash;
foreach my $word (sort keys(%wordTagHash)) {
	foreach my $tag (keys(%{$wordTagHash{"$word"}})) {
		$prefixWordHash{"$word"} += $wordTagHash{"$word"}{"$tag"}{'count'};
	}
	print "PREFIX $word, " . $prefixWordHash{"$word"};
}

# Do the math
foreach my $key (keys(%tagHash)) {
	my $prefixTag = (split(" ", $key))[0];
	$tagHash{"$key"}{'probability'} =
		# Get log base two of the probability
		log($tagHash{"$key"}{'count'} / $prefixTagHash{"$prefixTag"}{'count'}) / log(2);
}

foreach my $word (keys(%wordTagHash)) {
	foreach my $tag (keys(%{$wordTagHash{"$word"}})) {
		$wordTagHash{"$word"}{"$tag"}{'probability'} =
		# Get log base two of the probability
		log($wordTagHash{"$word"}{"$tag"}{'count'}) / $prefixWordHash{"$word"}) / log(2);
		print "$word, $tag, " . $wordTagHash{"$word"}{"$tag"}{'probability'} . "\n";
	}
}

my @tagArray;
my @wordArray;
foreach my $line (@test_document) {
	@tagArray = ();
	@wordArray = ();
	foreach my $wordtag (split(" ", $line)) {
		$wordtag =~ /([^\/]+)\/([^\/]+)/;
		my $word = lc $1;
		my $tag = $2;
		push @tagArray, $tag;
		push @wordArray, $word;
	}
}
