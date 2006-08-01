#!/usr/bin/perl -w
 
#
# use modules
#
use strict;
use CGI qw(:standard);
 
#
# Subroutine prototypes
#
sub process_arguments($);
sub process_dig_output($);
 
#
# Constants
#
sub DIGCMD() { return "/usr/local/bin/dig"; }
 
#
# Main() variables
#
my $DigArguments = "";
my $inputline;
my $queryvalues;
 
#
# Main()
#
if ($ENV{'REQUEST_METHOD'} eq "GET") {
   $main::inputline = $ENV{'QUERY_STRING'};
}    
if ($ENV{'REQUEST_METHOD'} eq "POST") {
   $main::inputline = <STDIN>;
}
$main::inputline = lc($main::inputline);
$main::queryvalues = {split(/&|=/,$main::inputline)};
 
print <<END_OF_HTML;
Content-type: text/html
 
<HTML>
   <HEAD>
      <TITLE>
         DIG - Domain Information Groper
      </TITLE>
   </HEAD>
   <BODY>
      <DIV align=center>
         <H1>
            DIG - Domain Information Groper
         </H1>
         <HR noshade width=50%>
      </DIV>
      <BR>
      <FORM action="?" method=post>
         <TABLE align=center border=0 cellpadding=0 cellspacing=2>
            <TR>
               <TD>
                  Name Server:
               </TD>
               <TD>
                  <INPUT type="text" name="host" size=36 value="$main::queryvalues->{'host'}">
               </TD>
            </TR>
            <TR>
               <TD>
                  Domain Name:
               </TD>
               <TD>
                  <INPUT type="text" name="domain" size=36 value="$main::queryvalues->{'domain'}">
               </TD>
            </TR>
            <TR>
               <TD>
                  Query Type:
               </TD>
               <TD>
                  <SELECT name="type">
                     <OPTION value="ANY"> Any (ANY)
                     <OPTION value="A"> Address (A)
                     <OPTION value="MX"> Mail Exchange (MX)
                     <OPTION value="PTR"> Pointer (PTR)
                     <OPTION value="NS"> Name Server (NS)
                     <OPTION value="SOA"> Start Of Authority (SOA)
                     <OPTION value="TXT"> Text (TXT)
                     <OPTION value="HINFO"> Host Info (HINFO)
                     <OPTION value="AXFR"> Zone Transfer (AXFR)
                  </SELECT>
               </TD>
            </TR>
            <TR>
               <TD colspan=2 align=center>
                  <INPUT type="SUBMIT" value="Perform query">
               </TD>
            </TR>
         </TABLE>
      </FORM>
      <HR noshade>
END_OF_HTML
 
 
$main::DigArguments = process_arguments($main::queryvalues); 
 
process_dig_output($main::DigArguments);
 
print <<END_OF_HTML;
<HR noshade>
<A HREF="/">Return to GFO web page</A><BR>
<TABLE align=left border=0 cellpadding=0 cellspacing=0>
<TR><TD>Version 1.0&nbsp;</TD><TD>&nbsp;-&nbsp;</TD><TD>Written by</TD><TD>&nbsp;<A href="mailto:user\@example.com">Cameron Palmer</A>&nbsp;</TD><TD>&nbsp;-&nbsp;</TD><TD>&nbsp;June 2002</TD></TR>
<TR><TD>Version 1.1</TD><TD>&nbsp;-&nbsp;</TD><TD>Modified by</TD><TD>&nbsp;<A href="mailto:user\@example.com">Robert Lanning</A>&nbsp;</TD><TD>&nbsp;-&nbsp;</TD><TD>&nbsp;June 2002</TD></TR>
</TABLE>
</BODY>
</HTML>
END_OF_HTML
 
#
# Subroutine definitions
#
sub process_arguments {
   my $queryvalues = shift(@_);
   my $host = $queryvalues->{'host'};
   my $domain = $queryvalues->{'domain'};
   my $type = $queryvalues->{'type'};
   my $arguments;
   if ($host =~ /^[A-Z0-9.-_]+$/i) {
      $host = "@" . $host;
   } else {
      $host = "";
   }
   if ($domain =~ /^[A-Z0-9.-]+$/i) {
      if ($domain =~ /^(\d{1,3})\.(\d{1,3})\.(\d{1,3})\.(\d{1,3})$/) {
         $domain = "$4.$3.$2.$1.in-addr.arpa";
      } else { 
         $domain =~ tr/A-Z/a-z/;
      } 
   } else {
      $domain = "";
   }
   if (!$type =~ /^(a|ptr|mx|ns|soa|hinfo|txt|any|axfr)$/) {
      $type = "a";
   }
   if (($domain =~ /in-addr\.arpa$/) && ($type eq "a")) {
      $type = "ptr";
   }
   $arguments = join(" ",$domain,$type,$host); 
   if ($domain eq "") {
      return "";
   } else {
      return $arguments; 
   }
}
 
sub process_dig_output {
   my $dig_arguments = shift(@_);
   print "<TABLE align=center border=1 cellpadding=1 cellspacing=0><TR><TD>\n";
   print "<pre>\n";
   open(DIG,DIGCMD . " $dig_arguments|");
   while(<DIG>) { 
      if (/^;/) {
         print; 
      } else {
         s/(\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}\.in-addr\.arpa\.)/<a HREF="?host=$main::queryvalues->{'host'}&type=PTR&domain=$1">$1<\/a>/;
         s/^([A-Za-z0-9-._]+\.)/<a HREF="?host=$main::queryvalues->{'host'}&type=A&domain=$1">$1<\/a>/;
         s/(\d{1,3})\.(\d{1,3})\.(\d{1,3})\.(\d{1,3})$/<a HREF="?host=$main::queryvalues->{'host'}&type=PTR&domain=$4.$3.$2.$1.in-addr.arpa">$1.$2.$3.$4<\/a>/;
         s/([A-Za-z0-9-._]+\.$)/<a HREF="?host=$main::queryvalues->{'host'}&type=A&domain=$1">$1<\/a>/;
         s/(IN\sSOA\s+)([A-Za-z0-9-._]+\.)\s+([A-Za-z0-9-_]+)\.([A-Za-z0-9-._]+\.)\s/$1<a HREF="?host=$main::queryvalues->{'host'}&type=A&domain=$2">$2<\/a> <a HREF="mailto:$3\@$4">$3.$4<\/a>/;
         print;
      }
   }
   close(DIG);
   print "</TD></TR></TABLE>\n";
}