#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <inttypes.h>

#define BUFFER_SIZE 262144


int main(int argc, char *argv[]) 
{
  unsigned char buf[BUFFER_SIZE];
  
  long int num_read;
  int i,j, cols; 
  
  cols = 15;

  if (argc >1) {
    if (argv[1][0]=='-')
      if (argv[1][1]=='n') {
	sscanf(argv[2], "%i", &cols);
	cols--;
      }
  }
  

  j = 0;
  printf("Stream contents:\n");
  
  do {
    num_read = fread(buf, 1, BUFFER_SIZE, stdin);
    for(i=0; i<num_read; i++) {
      if ((buf[i] == 0x00) && (buf[i+1] == 0x00) && (buf[i+2] == 0x01)) { // video and PES start codes
	printf("\n\n %02x %02x %02x", buf[i], buf[i+1], buf[i+2]);
	//	fwrite(&buf[i],1,3,stderr);
	i += 3; j = 3; 
      }
      if ((buf[i] == 0xff) && (buf[i+1] == 0xfd)) { // audio start code
	printf("\n\n %02x %02x", buf[i], buf[i+1]);
	//	fwrite(&buf[i],1,2,stderr);
	i +=1; j = 1;
      }
      else {
	printf(" %02x", buf[i]);
	//	fwrite(&buf[i],1,1,stderr);
      }
      if (j == cols) { 
	printf("\n"); 
	j = 0; }
      else j++;
    } 
    //    fwrite(buf,1,num_read,stderr);
  } while (num_read == BUFFER_SIZE);
}
	
  




