#!/usr/bin/perl

$read_chunk_size = 262144;
$display_width = 16;
### Process the command line arguments 
### Basically grab the filename to read in and process 
while ($arg = shift(@ARGV)) {
    push (@cloptions,$arg);
}

$filename = $cloptions[0];
print $filename . "\n";

if (-r $filename) { ### Check to make sure file exists and is readable
    open(TYFILE_READ,$filename);
}
 else {
    die "Unable to open file $filename\n";
}

### Loop until EOF is read
while (sysread(TYFILE_READ, $read_data, $read_chunk_size)) { 
   @array_of_data = split(//, $read_data);
   map {$_ = join("",unpack("H*", $_)); } @array_of_data;

   for ($index = 0; $index <= $#array_of_data; $index++) {
      print $array_of_data[$index] . " ";
   }
   print "\n";
} 
### End of main while loop upon reaching EOF 

close(TYFILE_READ);
