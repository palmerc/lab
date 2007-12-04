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


my $person = $webPagesXml->{'name'};
foreach my $documentId (keys %{$webPagesXml->{'WEPS:document'}}) {
	print "\n\nKey: $documentId\n";
	my $fileNameTmp = "/tmp/weps$documentId.tmp";
	open(TEMP, ">", "$fileNameTmp");
	print TEMP $webPagesXml->{'WEPS:document'}{$documentId}{'content'};
	close(TEMP);
	# Call tidy to cleanup the html, indent it, clean it, make it bare
	my $tidyHtml = `/usr/bin/tidy -q -i -c -b $fileNameTmp 2>/dev/null`;
	`rm $fileNameTmp`;
	# This will strip the HTML out of the document
	my $hs = HTML::Strip->new();
	my $strippedHtml = $hs->parse($tidyHtml);
	$hs->eof;
	# Clean out the stopwords from each text
	my @words = grep { !$stopwords->{$_} } split ' ', $strippedHtml;

	# Stem the remaining words
	my $stemmed_words_anon_array = $stemmer->stem(@words);
	$strippedHtml = join ' ', @$stemmed_words_anon_array;
	# Turn multiple blank characters into a single space.
	$strippedHtml =~ s/\s\s*/ /g;
	print $strippedHtml;
}

print Dumper($webDataXml);
