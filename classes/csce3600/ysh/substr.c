#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/* This function wants to provide string slices */
/* Coded by Cameron L Palmer, student */
/* Based in part on a conversation with Dr. Phil Sweany, UNT */
void substr(char *string, int start, int stop) {
    char *string_ptr = string;
    char *buf = malloc(sizeof(char) * (strlen(string) + 1));
    char *buf_temp = buf;
          
    //bzero(buf, strlen(string) + 1);
    string += start;
    while ((*string != '\0') && (string <= (string_ptr + stop)))
        *buf++ = *string++;

    *buf = '\0';
    bzero(string_ptr, strlen(buf_temp) + 1);
    strncpy(string_ptr, buf_temp, strlen(buf_temp));
       
    free(buf_temp);
}

int main() {
    /* Yes these are pointers */
    char string1[] = {"0123456789"};
    char string2[] = {"0123456789"};
    char string3[] = {"0123456789"};
    
    substr(string1, 0, 5);
    printf("String 1 =>%s<=\n", string1);
    
    substr(string2, 5, 8);
    printf("String 2 =>%s<=\n", string2);
    
    substr(string3, 2, 7);
    printf("String 3 =>%s<=\n", string3);
    
    return 0;
}
