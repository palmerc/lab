#!/usr/bin/perl

### Subroutines prototyped
sub tystream_processor();

$read_chunk_size = 262144;
### Process the command line arguments 
### Basically grab the filename to read in and process 
while ($arg = shift(@ARGV)) {
    push (@cloptions,$arg);
}

$filename = $cloptions[0];
sysopen (TYFILE, $filename, O_RDONLY); # || die "Unable to open file: $!\n";  
tystream_processor();

close(TYFILE);


### SUBROUTINES SECTION
sub tystream_file_open($) {
   my @path = @_ ;
   return 0; 
} 

sub tystream_processor() {

   ### Defined MPEG start codes and header types 
   $MPEG_START = "000001";
   $M2_VIDSEQ = "B3";
   $M2_SEQEXT = "B5";
   $M2_GRPHDR = "B8";
   $M2_PICHDR = "00";
   $M2_PICEXT = "B5";
   $PES_VID = "E0";
   $PES_AUD = "C0";
   $PES_END = "FFFF";
   $check_magic = 1;
   $check_TOC = 1;
   $first_pass = 1;
   $lineposition = 0; 
   $actualread0 = 1;
   ### First one is always free :)
   ### Two buffers are used so we don't miss anything in the last two bytes 
#   $actualread0 = sysread(TYFILE, $read_data0, $read_chunk_size); 
#   @array_of_data0 = split(//, $read_data0);
#   map {$_ = join("",unpack("H*", $_)); } @array_of_data0;

   while ($actualread0 = sysread(TYFILE, $read_data0, $read_chunk_size)) {
   #while ($actualread0 > 0) {
   #  $arrayph = 0;
   #   $actualread0 = sysread(TYFILE, $read_data0, $read_chunk_size);
      @array_of_data0 = split(//, $read_data0);
      map {$_ = join("",unpack("H*", $_)); } @array_of_data0;

      ### Deal with first four bytes. If consistent data can be found in fourth
      ### byte (0x80) as a signature of tystreams we could add logic to check
      ### for this.

      if ($check_magic == 1) {
         $BYTE_0 = $array_of_data0[0]; ### Number of records in TOC
         $BYTE_1 = $array_of_data0[1]; ### Should be 0x00
         $BYTE_2 = $array_of_data0[2]; ### Should be 0x00
         $BYTE_3 = $array_of_data0[3]; ### Expect 0x80 Signature?

         print "\n\n****** TYSTREAM Report File ******\n\n";
         print $filename . "\n";
         print "Number of TOC records 0x" . $BYTE_0 . "\n";
         print (join(" ","First Four -",$BYTE_0,$BYTE_1,$BYTE_2,$BYTE_3) . "\n"); 
         $check_magic = 0;
      } #ENDIF

      if ($check_TOC == 1) {
         print "*** TYSTREAM Table Of Contents ***\n";
         $TOCount = hex($BYTE_0 .= "0");
         $TOCount += 4; ### Skip the preamble as you process the TOC
         for ($loopcount = 4; $loopcount < $TOCount; $loopcount++) {
            if ($lineposition == 15) {
               print $array_of_data0[$loopcount] . "\n";
               $lineposition = 0; 
            } else {
               print $array_of_data0[$loopcount] . " "; 
               $lineposition++;
            } #END 
         } #ENDFOR 
         $arrayph = $TOCount;
         $bytecount = $TOCount;
         $check_TOC = 0;
      } #ENDIF 
      
      if ($first_pass == 1) {
         print "\n\n****** TYSTREAM Body of Data ******\n\n";
         $first_pass = 0;
      }
     #while ($arrayph <= $actualread0) {
         $BYTE_0 = $array_of_data0[$arrayph];
     #    if ($actualread0 - $arrayph < 4) {
     #       $actualread1 = sysread(TYFILE, $read_data1, $read_chunk_size);
     #       @array_of_data1 = split(//, $read_data1);
     #       map {$_ = join("",unpack("H*", $_)); } @array_of_data1;
         if ($BYTE_0 eq "00") {
            if ($array_of_data0[$arrayph + 1] eq "00") {
               if ($array_of_data0[$arrayph + 2] eq "01") {
                  print "\nBytes in last section " . $bytecount . "\n";
                  $bytecount = 0; 
                  print "\n\n"; 
                  if ($array_of_data0[$arrayph + 3] eq "e0") {
                     print "TiVo MPEG-2 PES Video Header\n"; 
                     $video_segment = 1;
                  } 
                  elsif ($array_of_data0[$arrayph + 3] eq "c0") { 
                     print "TiVo MPEG-1 PES Audio Header\n";
                     $audio_segment = 1;
                  }
                  elsif ($array_of_data0[$arrayph + 3] eq "b3") {
                     print "MPEG-2 Video Sequence\n";
                     $video_seq = 1;
                  } 
                  elsif ($array_of_data0[$arrayph + 3] eq "b8") {
                     print "MPEG-2 Group Header\n";
                  } 
                  elsif ($array_of_data0[$arrayph + 3] eq "00") {
                     print "MPEG-2 Picture Header\n";
                     $picture_header = 1;
                  } 
                  elsif ($array_of_data0[$arrayph + 3] eq "b5") {
                     if ($picture_header == 1) {
                        print "Picture Coding Extension\n";
                        $picture_header = 0;
                     } 
                     elsif ($video_seq == 1) {
                        print "Sequence Header Extension\n";
                        $video_seq = 0;
                     }
                  } else {
                     $BYTE_3 = $array_of_data0[$arrayph + 3];
                     print "Segment number " . $BYTE_3 . "\n"; 
                  }
                  $lineposition = 0;
                  $startcode_present = 1;
               } else {
                  $startcode_present = 0; 
               } 
            } else {
               $startcode_present = 0;
            }
         } else {
            $startcode_present = 0;
         }
         if (startcode_present == 1) {
            print "\n" . $BYTE_0;
            $startcode_present = 0;
            $lineposition++;
         } else {
            if ($lineposition == 15) {
               print $BYTE_0 . "\n";
               $lineposition = 0;
            } else {
               print $BYTE_0 . " ";
               $lineposition++;
            } 
         }
         if ($arrayph == $actualread0) {
            $arrayph = 0;
         } 
         $arrayph++;
         $bytecount++;
      #} #ENDWHILE
   } #ENDWHILE 
   return 0;
}
