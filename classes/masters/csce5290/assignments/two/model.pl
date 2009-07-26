#!/usr/bin/perl

use strict;

my %treeBankTags = (
	"\$" => "dollar",
	"``" => "opening quotation mark",
	"''" => "closing quotation mark",
	"(" => "opening parenthesis",
	")" => "closing parenthesis",
	"," => "comma",
	"--" => "dash",
	"." => "sentence terminator",
	":" => "colon or ellipsis",
	"CC" => "conjunction, coordinating",
	"CD" => "numeral, cardinal",
	"DT" => "determiner",
	"EX" => "existential there",
	"FW" => "foreign word",
	"IN" => "preposition or conjunction, subordinating",
	"JJ" => "adjective or numeral, ordinal",
	"JJR" => "adjective, comparative",
	"JJS" => "adjective, superlative",
	"LS" => "list item marker",
	"MD" => "modal auxiliary",
	"NN" => "noun, common, singular or mass",
	"NP" => "noun, proper, singular (Added by Cameron)",
	"NNP" => "noun, proper, singular",
	"NNPS" => "noun, proper, plural",
	"NNS" => "noun, common, plural",
	"NPS" => "noun, proper, plural (Added by Cameron)",
	"PDT" => "pre-determiner",
	"POS" => "genitive marker",
	"PP" => "pronoun, personal (Added by Cameron)",
	"PP\$" => "pronoun, possessive (Added by Cameron)",
	"PRP" => "pronoun, personal",
	"PRP\$" => "pronoun, possessive",
	"RB" => "adverb",
	"RBR" => "adverb, comparative",
	"RBS" => "adverb, superlative",
	"RP" => "particle",
	"SYM" => "symbol",
	"TO" => "to as preposition or infinitive marker",
	"UH" => "interjection",
	"VB" => "verb, base form",
	"VBD" => "verb, past tense",
	"VBG" => "verb, present participle or gerund",
	"VBN" => "verb, past participle",
	"VBP" => "verb, present tense, not 3rd person singular",
	"VBZ" => "verb, present tense, 3rd person singular",
	"WDT" => "WH-determiner",
	"WP" => "WH-pronoun",
	"WP\$" => "WH-pronoun, possessive",
	"WRB" => "Wh-adverb"
);

my @viterbi;
my @backpointer;
my @arrayToTag;
my @tagArray;
my @wordArray;
my %tagBigramHash;
my %wordTagHash;
my %tagToArray;
my $totalCorrectCount = 0;
my $totalWrongCount = 0;
my $totalWordCount = 0;

sub Viterbi {
	print "\n\nViterbi\n";
	
	# Initialize arrays;
	@viterbi = ();
	@backpointer = ();
	
	# Initialization
	my $firstWord = $wordArray[0];
	
	# What tags does the first word have?
	foreach my $tag (keys %{$wordTagHash{"$firstWord"}} ) {
		my $tagIndex = $tagToArray{"$tag"};
		$viterbi[$tagIndex][0] = 
			$wordTagHash{"$firstWord"}{"$tag"}{'probability'} * $tagBigramHash{". $tag"}{'probability'};
		
		# backpointer will store the vertex we came from
		$backpointer[$tagIndex][0] = 0;
		print "0 $firstWord $tag $viterbi[$tagIndex][0] $arrayToTag[0]\n";
	}
	
	# Recursion
	# Start on the second word to the end of the sentence
	for (my $w = 1; $w < scalar @wordArray; $w++) {
		my $word = $wordArray[$w];
		
		# Lookup the tags for this word and iterate over them
		foreach my $tag (keys %{$wordTagHash{"$word"}} ) {
			my $tagIndex = $tagToArray{"$tag"};
			my $prevWord = $wordArray[$w - 1];
			
			my $argmax = 0;
			my %candidates = ();
			
			# MAX discovery. Look at the previous word's tags
			foreach my $prevTag (keys %{$wordTagHash{"$prevWord"}} ) {
				my $j = $tagToArray{"$prevTag"};
				$candidates{$j} = $viterbi[$j][$w-1] * abs($tagBigramHash{"$prevTag $tag"}{'probability'});
			}
			$argmax = (sort {$candidates{$a} <=> $candidates{$b}} keys %candidates)[0];
			my $max = $candidates{"$argmax"};

			$viterbi[$tagIndex][$w] = abs($wordTagHash{"$word"}{"$tag"}{'probability'}) * $max;
			$backpointer[$tagIndex][$w] = $argmax;
			print "$w $word $tag $viterbi[$tagIndex][$w] $arrayToTag[$argmax]\n";
		}
	}
	# Termination and path-readout
	my $W = scalar (@wordArray) - 1;
	my $argmax = 0;

	my @result;
	$argmax = $tagToArray{"."};
	push @result, ".";
	
	my $w;
	for ($w = $W - 1; $w >= 0; $w--) {
		$argmax = $backpointer[$argmax][$w + 1];
		push @result, $arrayToTag[$argmax];
	}

	my @reversed = reverse @result;
	print "Hand     -> @tagArray\n";
	print "Generated-> @reversed\n";
	
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
		print "Sentence correct " . $sentenceCorrectCount/$sentenceWordCount*100 . "%.\n";
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
my %prefixTagHash;
foreach my $key (keys(%tagBigramHash)) {
	my $prefixTag = (split(" ", $key))[0];
	$prefixTagHash{"$prefixTag"}{'count'} += $tagBigramHash{"$key"}{'count'};
}

my %tagListHash;
foreach my $tag (keys(%tagBigramHash)) {
	my $prefixTag = (split(" ", $tag))[0];
	$tagListHash{"$prefixTag"} = 0;
}

@arrayToTag = sort keys(%tagListHash);
for (my $i = 0; $i < scalar @arrayToTag; $i++) {
	$tagToArray{"$arrayToTag[$i]"} = $i;
}	

# Generate the Word|Tag bigram prefix counts
my %prefixWordHash;
foreach my $word (sort keys(%wordTagHash)) {
	foreach my $tag (keys(%{$wordTagHash{"$word"}})) {
		$prefixWordHash{"$tag"} += $wordTagHash{"$word"}{"$tag"}{'count'};
	}
}

# Do the math
foreach my $key (keys(%tagBigramHash)) {
	my $prefixTag = (split(" ", $key))[0];
	$tagBigramHash{"$key"}{'probability'} =
		# Get log base two of the probability
		log($tagBigramHash{"$key"}{'count'} / $prefixTagHash{"$prefixTag"}{'count'}) / log(2);
}

foreach my $word (keys(%wordTagHash)) {
	foreach my $tag (keys(%{$wordTagHash{"$word"}})) {
		$wordTagHash{"$word"}{"$tag"}{'probability'} =
		# Get log base two of the probability
		log($wordTagHash{"$word"}{"$tag"}{'count'} / $prefixWordHash{"$tag"}) / log(2);
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
		$wordtag =~ /([^\/]+)\/([^\/]+)/;
		my $word = lc $1;
		my $tag = $2;
		push @tagArray, $tag;
		push @wordArray, $word;
	}
	Viterbi();
}
print "\nOverall correct " . $totalCorrectCount/$totalWordCount*100 . "%.\n";
