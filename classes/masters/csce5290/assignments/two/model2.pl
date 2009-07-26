#!/usr/bin/perl

use strict;

my @forward;
my @arrayToTag;
my @tagArray;
my @wordArray;
my @percentages;
my %tagBigramHash;
my %wordTagHash;
my %tagToArray;
my %prefixWordHash;
my %prefixTagHash;
my %tagListHash;
my $totalCorrectCount = 0;
my $totalWrongCount = 0;
my $totalWordCount = 0;

sub Forward {
	# Initialize arrays;
	@forward = ();
	
	my $firstWord = $wordArray[0];
	# What tags does the first word have?
	
	if ($firstWord =~ /^[0-9][0-9.]*$/) {
		$wordTagHash{"$firstWord"}{"CD"}{"count"} = 1;
		$wordTagHash{"$firstWord"}{"CD"}{"probability"} = log(1 / $prefixWordHash{"NN"}{'count'}) / log(2);
	}
	
	if (!exists $wordTagHash{"$firstWord"}) {
		$wordTagHash{"$firstWord"}{"NN"}{"count"} = 1;
		$wordTagHash{"$firstWord"}{"NN"}{"probability"} = log(1 / $prefixWordHash{"NN"}{'count'}) / log(2);
	}

	# Initialization
	foreach my $tag (keys %{$wordTagHash{"$firstWord"}} ) {
		if (!exists $tagBigramHash{". $tag"}) {
			$tagBigramHash{". $tag"}{'probability'} = log (1 / $prefixTagHash{"."}{'count'}) / log(2);
			$tagListHash{"$tag"} = 0;
			push @arrayToTag, $tag;
			my $i = scalar (@arrayToTag) - 1;
			$tagToArray{"$i"} = $i;
		}
		
		if (!exists $wordTagHash{"$firstWord"}{"$tag"}{'probability'}) {
			$wordTagHash{"$firstWord"}{"$tag"}{'probability'} = log(1 / ($prefixWordHash{"$tag"}{'count'} + scalar(keys(%wordTagHash)))) / log(2);
		}
		
		my $tagIndex = $tagToArray{"$tag"};
		
		$forward[$tagIndex][0] =
			$wordTagHash{"$firstWord"}{"$tag"}{'probability'} * $tagBigramHash{". $tag"}{'probability'};
				
		print "0 $firstWord $tag $forward[$tagIndex][0]\n";
	}
	
	# Recursion
	# Start on the second word to the end of the sentence
	for (my $w = 1; $w < scalar @wordArray; $w++) {
		my $word = $wordArray[$w];
		if ($word =~ /^[0-9][0-9.]*$/) {
			$wordTagHash{"$word"}{"CD"}{"count"} = 1;
			$wordTagHash{"$word"}{"CD"}{"probability"} = log(1 / ($prefixWordHash{"NN"}{'count'} + scalar(keys(%wordTagHash)))) / log(2);
		}
		if (!exists $wordTagHash{"$word"}) {
			$wordTagHash{"$word"}{"NN"}{"count"} = 1;
			$wordTagHash{"$word"}{"NN"}{"probability"} = log(1 / ($prefixWordHash{"NN"}{'count'}  + scalar(keys(%wordTagHash)))) / log(2);
		}
		# Lookup the tags for this word and iterate over them
		foreach my $tag (keys %{$wordTagHash{"$word"}} ) {
			my $tagIndex = $tagToArray{"$tag"};
			my $prevWord = $wordArray[$w - 1];
			
			my $sumner = 0;
			
			# SUM. Look at the previous word's tags
			foreach my $prevTag (keys %{$wordTagHash{"$prevWord"}} ) {
				my $j = $tagToArray{"$prevTag"};
				if (!exists $tagBigramHash{"$prevTag $tag"}) {
					$tagBigramHash{"$prevTag $tag"}{'probability'} = log (1 / $prefixTagHash{"$prevTag"}{'count'}) / log(2);
					$tagListHash{"$tag"} = 0;
					push @arrayToTag, $tag;
					my $i = scalar (@arrayToTag) - 1;
					$tagToArray{"$i"} = $i;
				}
				$sumner = $forward[$j][$w-1] * abs($tagBigramHash{"$prevTag $tag"}{'probability'});
			}
			
			$forward[$tagIndex][$w] = abs($wordTagHash{"$word"}{"$tag"}{'probability'}) * $sumner;
			print "$w $word $tag $forward[$tagIndex][$w]\n";
		}
	}
	
	for (my $w = 1; $w < scalar @wordArray; $w++) {
		my $word = $wordArray[$w];
		foreach my $tag (keys %{$wordTagHash{"$word"}} ) {
			my $tagIndex = $tagToArray{"$tag"};
			
			my $sumner = 0;
			foreach my $otherTags (keys %{$wordTagHash{"$word"}}) {
				my $j = $tagToArray{"$otherTags"};
				$sumner += $forward[$j][$w]; 
			}
			print "\n$word $tag == ". $forward[$tagIndex][$w] / $sumner . "\n";
		}
	}
	my @reversed = ();
	print "Hand     -> @tagArray\n";
	#print "Generated-> @reversed\n";
	
	my $sentenceCorrectCount = 0;
	my $sentenceWrongCount = 0;
	my $sentenceWordCount = 0;
	for (my $i = 0; $i < scalar @reversed; $i++) {
		if ($reversed[$i] eq $tagArray[$i]) {
			$totalCorrectCount += 1;
			$sentenceCorrectCount += 1;
		} else {
			$totalWrongCount += 1;
			$sentenceWrongCount += 1;
		}
		$totalWordCount += 1;
		$sentenceWordCount += 1;
	}
	if ($sentenceWordCount > 0) {
		#print "Sentence correct " . $sentenceCorrectCount/$sentenceWordCount*100 . "%.\n";
		push @percentages, $sentenceCorrectCount/$sentenceWordCount*100;
	}
}

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

my $linecount = 1;
foreach my $line (@model_document) {
	$line =~ s/\s+/ /g;
	$line =~ s/^\s+//;
	$line =~ s/\s+$//;
	chomp $line;
	
	my $pretag = ".";
	foreach my $wordtag (split(/\s/, $line)) {
		if ($wordtag =~ m#^(.+)/([^/]+)$#) {
			my $word = lc $1;
			my $curtag = $2;

			# Bigram probability of curtag|pretag
			my $keytag = $pretag . " " . $curtag;
			# reverse tag is for the add-one smoothing.
			my $reversekeytag = $curtag . " " . $pretag;
			
			# Add one to the bigram count
			if (exists $tagBigramHash{"$keytag"}) {
				# if the key has been seen before add one to our count
				$tagBigramHash{"$keytag"}{'count'} += 1;
			} else {
				# otherwise this is a new key. initialize to one and generate a
				# reversekey initialized to zero for later add one smoothing.
				$tagBigramHash{"$keytag"}{'count'} = 1;
				if ($pretag ne "." && !exists $tagBigramHash{"$reversekeytag"}) {
					$tagBigramHash{"$reversekeytag"}{'count'} = 0;
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
			#print STDERR "Token error on line $linecount, $wordtag";
		}
	}
	$linecount++;
}

# Add one to all the counts in the matrix
foreach my $key (keys(%tagBigramHash)) {
	$tagBigramHash{"$key"}{'count'} += 1;
}

foreach my $word (keys(%wordTagHash)) {
	foreach my $tag (keys(%{$wordTagHash{"$word"}})) {
		$wordTagHash{"$word"}{"$tag"}{'count'} += 1;
	}
}

# Generate the Tag|Tag bigram prefix counts
foreach my $key (keys(%tagBigramHash)) {
	my $prefixTag = (split(" ", $key))[0];
	$prefixTagHash{"$prefixTag"}{'count'} += $tagBigramHash{"$key"}{'count'};
}

foreach my $tag (keys(%tagBigramHash)) {
	my $prefixTag = (split(" ", $tag))[0];
	$tagListHash{"$prefixTag"} = 0;
}

@arrayToTag = sort keys(%tagListHash);
for (my $i = 0; $i < scalar @arrayToTag; $i++) {
	$tagToArray{"$arrayToTag[$i]"} = $i;
}

# Generate the Word|Tag bigram prefix counts
foreach my $word (sort keys(%wordTagHash)) {
	foreach my $tag (keys(%{$wordTagHash{"$word"}})) {
		$prefixWordHash{"$tag"}{'count'} += $wordTagHash{"$word"}{"$tag"}{'count'};
	}
}

# Do the math
foreach my $key (keys(%tagBigramHash)) {
	my $prefixTag = (split(" ", $key))[0];
	$tagBigramHash{"$key"}{'probability'} =
		# Get log base two of the probability
		log($tagBigramHash{"$key"}{'count'} / ($prefixTagHash{"$prefixTag"}{'count'} + scalar(keys(%tagListHash)))) / log(2);
}

foreach my $word (keys(%wordTagHash)) {
	foreach my $tag (keys(%{$wordTagHash{"$word"}})) {
		$wordTagHash{"$word"}{"$tag"}{'probability'} =
			# Get log base two of the probability
			log($wordTagHash{"$word"}{"$tag"}{'count'} / ($prefixWordHash{"$tag"}{'count'} + scalar(keys(%wordTagHash))))/ log(2);
		#print "$word, $tag, " . $wordTagHash{"$word"}{"$tag"}{'probability'} . "\n";
	}
}

foreach my $line (@test_document) {
	$line =~ s/\s+/ /g;
	$line =~ s/^\s+//;
	$line =~ s/\s+$//;
	chomp $line;
	
	@tagArray = ();
	@wordArray = ();
	foreach my $wordtag (split(" ", $line)) {
		$wordtag =~ m#^(.+)/([^/]+)$#;
		my $word = lc $1;
		my $tag = $2;
		push @tagArray, $tag;
		push @wordArray, $word;
	}
	Forward();
}
#print "\nOverall correct " . $totalCorrectCount/$totalWordCount*100 . "%.\n";
#my $sum = 0;
#foreach my $value (@percentages) {
#	$sum += $value;
#}
#print "Averaged averages " . $sum / scalar(@percentages) . "% \n\n";
