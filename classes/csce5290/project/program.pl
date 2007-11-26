#!/usr/bin/perl

use strict;
use XML::Simple;
use Data::Dumper;
use HTML::Strip;
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

my $person = $webPagesXml->{'name'};
foreach my $documentId (keys %{$webPagesXml->{'WEPS:document'}}) {
	print "\n\nKey: $documentId\n";
	my $fileNameTmp = "/tmp/weps$documentId.tmp";
	open(TEMP, ">", "$fileNameTmp");
	print TEMP $webPagesXml->{'WEPS:document'}{$documentId}{'content'};
	close(TEMP);
	my $tidyHtml = `/usr/bin/tidy -q -i -c -b $fileNameTmp 2>/dev/null`;
	`rm $fileNameTmp`;
	my $hs = HTML::Strip->new();
	my $strippedHtml = $hs->parse($tidyHtml);
	$hs->eof;
	$strippedHtml =~ s/\s\s*/ /g;
	print $strippedHtml;
}

print Dumper($webDataXml);
