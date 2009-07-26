/* @(#) File name: opn_output.c   Ver: 3.1   Cntl date: 1/20/89 14:17:28 */
static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";


#include "functions.h"
#include <string.h>

FILE *fpin, *fplog;

void open_output (char *filename)
{
    int     i;
    char    inname[100], outname[100];
    char   *inext, *logext;

    inext = ".simcpuin";
    logext = ".simcpulog";

    i = 0;
    while ((filename[i] != '.') && filename[i])
    {
	inname[i] = filename[i];
	i++;
    }
    inname[i] = '\0';

    strcpy(outname, inname);
    strcat(inname, inext);
    strcat(outname, logext);

    if ((fpin = fopen (inname, "w")) == NULL)
    {
	printf ("\n Cannot open output file : %s \n", inname);
	exit (0);
    }

    if ((fplog = fopen (outname, "w")) == NULL)
    {
	printf ("\n Cannot open output file : %s \n", outname);
	exit (0);
    }

    printf (" outputin filename = %s \n", inname);
    printf (" outputlog filename = %s \n", outname);

}
