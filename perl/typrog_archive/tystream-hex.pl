#!/usr/bin/perl

$read_chunk_size = 4096;
### Process the command line arguments 
### Basically grab the filename to read in and process 

$filename = $ARGV[0];
print $filename . "\n";

if (-r $filename) { ### Check to make sure file exists and is readable
    open(TYFILE_READ,"<$filename");
} else {
    die "Unable to open file $filename\n";
}

### Loop until EOF is read
while (! eof(TYFILE_READ)) { 
   sysread(TYFILE_READ, $read_data, $read_chunk_size);
   @array_of_data = split(//, $read_data);
   map {$_ = join("",unpack("H*", $_)); } @array_of_data;

   print join(" ",@array_of_data);
} 
### End of main while loop upon reaching EOF 

close(TYFILE_READ);
