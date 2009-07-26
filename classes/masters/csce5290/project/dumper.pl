#!/usr/bin/perl

use strict;
use XML::Simple;
use Data::Dumper;
use HTML::Strip;
binmode(STDOUT, ":utf8");

my $data;
my $xml;
my $inputFile;

if (scalar(@ARGV) < 1) {
	print("Usage: $0 inputFile\n");
	exit 1;
} else {
	$inputFile = $ARGV[0];
}

$xml = new XML::Simple;
$data = $xml->XMLin($inputFile);

my $person = $data->{'name'};
foreach my $documentId (keys %{$data->{'WEPS:document'}}) {
	print "\n\nKey: $documentId\n";
	my $fileNameTmp = "/tmp/weps$documentId.tmp";
	open(TEMP, ">", "$fileNameTmp");
	print TEMP $data->{'WEPS:document'}{$documentId}{'content'};
	close(TEMP);
	my $tidyHtml = `/usr/bin/tidy -q -i -c -b $fileNameTmp 2>/dev/null`;
	`rm $fileNameTmp`;
	my $hs = HTML::Strip->new();
	my $strippedHtml = $hs->parse($tidyHtml);
	$hs->eof;
	$strippedHtml =~ s/\s\s*/ /g;
	print $strippedHtml;
}
