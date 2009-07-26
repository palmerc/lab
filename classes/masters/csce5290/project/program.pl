#!/usr/bin/perl

use strict;
use XML::Simple;
use Data::Dumper;
use HTML::Strip;
use Lingua::StopWords qw( getStopWords );
use Lingua::Stem;
binmode(STDOUT, ":utf8");

my $data;
my ($xml, $webDataXml, $webPagesXml);
my ($webDataFile, $webPagesFile);

if (scalar(@ARGV) < 2) {
	print("Usage: $0 webDataFile webPagesFile\n");
	exit 1;
} else {
	$webDataFile = $ARGV[0];
	$webPagesFile = $ARGV[1];
}

$xml = new XML::Simple;
$webDataXml = $xml->XMLin($webDataFile);
$webPagesXml = $xml->XMLin($webPagesFile);
my $stopwords = getStopWords('en', 'UTF-8');
my $stemmer = Lingua::Stem->new(-locale => 'EN');
$stemmer->stem_caching({ -level => 2 });

my $documentCount = 0;
my (%termHash, %weightHash);
my $person = $webPagesXml->{'name'};
foreach my $documentId (keys %{$webPagesXml->{'WEPS:document'}}) {
	#print "\n\nKey: $documentId\n";
	$documentCount += 1;
	my $fileNameTmp = "/tmp/weps$documentId.tmp";
	open(TEMP, ">", "$fileNameTmp");
	print TEMP $webPagesXml->{'WEPS:document'}{$documentId}{'content'};
	close(TEMP);
	# Call tidy to cleanup the html, indent it, clean it, make it bare
	my $tidyHtml = `/usr/bin/tidy -q -i $fileNameTmp 2>/dev/null`;
	`rm $fileNameTmp`;
	# This will strip the HTML out of the document
	my $hs = HTML::Strip->new();
	my $strippedHtml = $hs->parse($tidyHtml);
	$hs->eof;
	$strippedHtml =~ s/\s\s*/ /g;
	# Clean out the stopwords from each text
	my @words = grep { !$stopwords->{$_} } split ' ', $strippedHtml;

	# Stem the remaining words
	my $stemmedWordsAnonArray = $stemmer->stem(@words);
	# This gives you how many times a term appears in a document
	foreach my $term (@$stemmedWordsAnonArray) {
		$termHash{'byDocument'}{$documentId}{$term} += 1;
	}

	#$strippedHtml = join ' ', @$stemmedWordsAnonArray;
	# Turn multiple blank characters into a single space.
	#print $strippedHtml;
}

# This will give you the number of documents a particular term appears in
foreach my $documentId (keys %{$termHash{'byDocument'}}) {
	foreach my $term (keys %{$termHash{'byDocument'}{$documentId}}) {
		$termHash{'byTerm'}{$term} += 1;
	}
}

foreach my $documentId (keys %{$termHash{'byDocument'}}) {
	foreach my $term (keys %{$termHash{'byDocument'}{$documentId}}) {
		# weight equals term frequency in a document times the log of number of 
		# documents divided by how many documents the term appeared in
		$weightHash{'byDocument'}{$documentId}{'term'}{$term} = $termHash{'byDocument'}{$documentId}{$term} * (log($documentCount/$termHash{'byTerm'}) / log(2));
	}
}

# Calculate the cosine normalization factor
foreach my $documentId (keys %{$weightHash{'byDocument'}}) {
	my $documentSum = 0; 
	foreach my $term (keys %{$weightHash{'byDocument'}{$documentId}{'term'}}) {
		$documentSum += $weightHash{'byDocument'}{$documentId}{'term'}{$term}**2;
	}
	$weightHash{'byDocument'}{$documentId}{'cnf'} = sqrt($documentSum);
}

my @orderedDocs = sort keys %{$termHash{'byDocument'}};
foreach my $documentId (@orderedDocs) {
	print "Document: $documentId\n";
	foreach my $innerDocumentId (@orderedDocs) {
		my $similaritySum = 0;
		foreach my $term (keys %{$termHash{'byDocument'}{$documentId}}) {
			foreach my $innerTerm (keys %{$termHash{'byDocument'}{$innerDocumentId}}) {
				if ($documentId == $innerDocumentId) {
					next;
				} else {
					if ($term == $innerTerm) {
						$similaritySum = ($weightHash{'byDocument'}{$documentId}{'term'}{$term}/$weightHash{'byDocument'}{$documentId}{'cnf'}) * ($weightHash{'byDocument'}{$innerDocumentId}{'term'}{$innerTerm}/$weightHash{'byDocument'}{$innerDocumentId}{'cnf'});
					}
				}
			}
		}
		print "    Similarity sum: $innerDocumentId -- $similaritySum\n";
		
	}
}

print Dumper(%weightHash);
