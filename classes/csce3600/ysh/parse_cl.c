#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const size_t MAX_LINE = 256;

void substr(char *string, int start, int stop) {
    char *string_ptr = string;
    char *buf = malloc(sizeof(char) * (strlen(string) + 1));
    char *buf_temp = buf;
          
    string += start;
    while ((*string != '\0') && (string <= (string_ptr + stop)))
        *buf++ = *string++;

    *buf = '\0';
    bzero(string_ptr, strlen(buf_temp) + 1);
    strncpy(string_ptr, buf_temp, strlen(buf_temp));
       
    free(buf_temp);
}

void parse_cl(char *line, char **env_argv, size_t *env_argv_len) {
    char *buf = malloc(sizeof(char) * (strlen(line) + 1));
    
    char *arg;

    //bzero(buf, MAX_LINE+1);
    strncpy(buf, line, strlen(line));

    int i = 0;
    int j = 0;
    int start = 0;
    while (buf[i] != '\0') {
        if (buf[i] == ' ') {
            substr(buf, start, i-1);
            if (strlen(buf) > 0) {
                arg = (char *)malloc(sizeof(char) * strlen(buf)+1);
                strncpy(arg, buf, strlen(buf)+1);
                env_argv[j] = arg;
                j++;
            }
            start = i+1;
            strncpy(buf, line, strlen(line));
        }
        i++;
    }
    //printf("start %d\n", start);
    substr(buf, start, strlen(line)-1);
    //printf("buf %s\n", buf);
    if (strlen(buf) > 0) {
        arg = (char *)malloc(sizeof(char) * strlen(buf)+1);
        strncpy(arg, buf, strlen(buf));
        env_argv[j] = arg;
        j++;
    }
    //printf("variables %d\n", j);
    *env_argv_len = j;
    free(buf);
}

int main() {
    size_t env_argv_len;
    char **env_argv;
    env_argv = (char**) malloc(sizeof(char *) * MAX_LINE);
    
    char string3[] = {"ls -la cameron.c\n\0"};
    char string4[] = {"1 2 cameron is here today and would like this to break again\n"};
    //char string2[] = {"0 1 2 3 4 5 6 7 8 9\n"};
    char string1[] = {"/bin/gzcat\n"};
    //char string3[] = {"0 1 2 3 4 5 6 7 8 9 "};
    //char string4[] = {" 0 1 2 3 4 5 6 7 8 9"};
    char string2[] = {"/bin/zcat ysh.tar.gz"};
    char string5[] = {"I  double  space   triple single cameronpalmer"};
    
    int i;
    
    printf("String 1\n");
    parse_cl(string1, env_argv, &env_argv_len);
    for (i=0; i < env_argv_len; i++)
        printf("=>%s<=", env_argv[i]);
    printf("\n");
    
    printf("String 2\n");
    parse_cl(string2, env_argv, &env_argv_len);
    for (i=0; i < env_argv_len; i++)
        printf("=>%s<=", env_argv[i]);
    printf("\n");
    
    printf("String 3\n");
    parse_cl(string3, env_argv, &env_argv_len);
    for (i=0; i < env_argv_len; i++) 
        printf("=>%s<=", env_argv[i]);
    printf("\n");
    
    printf("String 4\n");
    parse_cl(string4, env_argv, &env_argv_len);
    for (i=0; i < env_argv_len; i++)
        printf("=>%s<=", env_argv[i]);
    printf("\n");
    
    printf("String 5\n");
    parse_cl(string5, env_argv, &env_argv_len);
    for (i=0; i < env_argv_len; i++)
        printf("=>%s<=", env_argv[i]);
    printf("\n");
        
    return 0;    
}
