#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char *cmd;
    char *cmd_loc;
    char **argv;
    int *argv_count;
} command_t;

void substr(char *string, int start, int stop) {
    char *string_ptr = string;
    char *buf = malloc(sizeof(char) * (strlen(string) + 1));
    char *buf_temp = buf;
          
    string += start;
    while ((*string != '\0') && (string <= (string_ptr + stop)))
        *buf++ = *string++;

    *buf = '\0';
    strcpy(string_ptr, buf_temp);
       
    free(buf_temp);
}

void parse_cl(char *line, command_t *command_list, size_t *command_list_len) {
    char *buf;
    buf = (char *)malloc(sizeof(char) * (strlen(line)+1));
    char *arg;
    
    strncpy(buf, line, strlen(line)+1);
    
    int bg_process = 0;
    int pipe_count = 0; /* number of pipes */
    int arg_count = 0; /* number of arguments */
    int i = 0; /* position in the string */
    int start = 0;
    //int end = 0;
    command_list[pipe_count].argv = (char **)malloc(sizeof(char *)*256);
    while (buf[i] != '\0') {
        if (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n') {
            /* This means this is a seperate thing */
            /* Turn the argument into a little string and add it to the array */
            substr(buf, start, i-1);
            if (strlen(buf) > 0) {
                arg = (char *)malloc(sizeof(char) * (strlen(buf)+1));
                strncpy(arg, buf, strlen(buf)+1);
                //printf("space: %s\n", arg);
                command_list[pipe_count].argv[arg_count] = arg;
                arg_count++;
            }
            start = i+1;
            //printf("%s\n", buf);
            strncpy(buf, line, strlen(buf)+1);
        } else if (buf[i] == '.') {
            //printf("Dot\n");
            /* If I find a dot, should mean the current directory */
            
        } else if (buf[i] == '-') {
            //printf("Minus\n");
            /* This means an option to a command */
        } else if (buf[i] == '*') {
            //printf("Asterisk\n");
            /* This is a wildcard and should be expanded */
        } else if (buf[i] == '?') {
            //printf("Question\n");
            /* This is a single character wildcard and should be expanded */
        } else if (buf[i] == '~') {
            //printf("Tilde\n");
            /* This means the users home directory */
        } else if (buf[i] == '&') {
            //printf("Ampersand\n");
            /* This indicates a background process
               or if following a > (eg. 2>&1 tie stdout to stderr) */
            bg_process = 1;
        } else if (buf[i] == '|') {
            //printf("Pipe\n");
            /* Connect stdout of the previous app to the stdin of the next */
            /* Close out the previous string before moving to the new one */
            substr(buf, start, i-1);
            if (strlen(buf) > 0) {
                arg = (char *)malloc(sizeof(char *) * (strlen(buf)+1));
                strncpy(arg, buf, strlen(buf)+1);
                printf("pipe: %s\n", arg);
                command_list[pipe_count].argv[arg_count] = arg;
                arg_count++;
            }
            start = i+1;
            //printf("%s\n", buf);
            strncpy(buf, line, strlen(buf)+1);
            /* Close out the previous one */
            //printf("Pipecount %d Argcount %d\n", pipe_count, arg_count);
            //command_list[pipe_count].argv[arg_count] = NULL;
            
            arg_count = 0;
            command_list[++pipe_count].argv = (char **)malloc(sizeof(char *)*256);
        } else if (buf[i] == '>') {
            //printf("Stdout\n");
            /* Connect stdout to whatever follows */
        } else if (buf[i] == '<') {
            //printf("Stdin\n");
            /* Connect stdin to whatever follows */
        } else if (buf[i] == '/' 
                || isdigit(buf[i]) 
                || isalpha(buf[i])) {
            //printf("Character: %c\n", buf[i]);
            /* A regular character */
        } else {
            printf("error parsing");
        }
        i++;
    }
    /* Null was encountered so grab the last thing on the line */
    substr(buf, start, i-1);
    if (strlen(buf) > 0) {
        arg = (char *)malloc(sizeof(char) * (strlen(buf)+1));
        strncpy(arg, buf, strlen(buf)+1);
        command_list[pipe_count].argv[arg_count] = arg;
        arg_count++;
    }
    /* Close the last item */
    //printf("Lastcount %d Argcount %d\n", pipe_count, arg_count);
    //command_list[pipe_count].argv[arg_count] = NULL;
    *command_list_len = pipe_count + 1;
}


int main() {
    size_t command_list_len;
    command_t *command_list;
    command_list = (command_t*)malloc(sizeof(command_t)*256);
    
    /* Yes these are pointers */
    char string1[] = {"ls -la | grep -r malloc | wc -l"};
    char string2[] = {"ls -la | grep -r malloc | wc -l > file"};
    char string3[] = {"ls -la | wc -l > file"};
    char string4[] = {"find . -name * > file &"};
    
    printf("String1\n");
    parse_cl(string1, command_list, &command_list_len);
    int i, j;
    //printf("%s %s %s\n", command_list[1].argv[0], command_list[1].argv[1], command_list[1].argv[2]);
    for (i = 0; i < command_list_len; i++) {
        j = 0;
        while (command_list[i].argv[j] != NULL) {
            printf("%d %d {%s}\n", i, j, command_list[i].argv[j]);
            j++;
        }
    }
    printf("\n");
    
    printf("String2\n");
    parse_cl(string2, command_list, &command_list_len);
    for (i = 0; i < command_list_len; i++) {
        j = 0;
        while (command_list[i].argv[j] != NULL) {
            printf("%d %d {%s}\n", i, j, command_list[i].argv[j]);
            j++;
        }
    }
    printf("\n");
    
    printf("String3\n");
    parse_cl(string3, command_list, &command_list_len);
    for (i = 0; i < command_list_len; i++) {
        j = 0;
        while (command_list[i].argv[j] != NULL) {
            printf("%d %d {%s}\n", i, j, command_list[i].argv[j]);
            j++;
        }
    }
    printf("\n");
    
    printf("String4\n");
    parse_cl(string4, command_list, &command_list_len);
        for (i = 0; i < command_list_len; i++) {
        j = 0;
        while (command_list[i].argv[j] != NULL) {
            printf("%d %d {%s}\n", i, j, command_list[i].argv[j]);
            j++;
        }
    }
    printf("\n");
        
    return 0;
}
