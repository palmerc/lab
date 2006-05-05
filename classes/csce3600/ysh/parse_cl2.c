#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

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

    //strncpy(buf, line, strlen(line));
    strcpy(buf, line);
    
    int i = 0;
    int j = 0;
    int start = 0;
    while (buf[i] != '\0') {
        if (buf[i] == ' ') {
            substr(buf, start, i-1);
            if (strlen(buf) > 0) {
                arg = (char *)malloc(sizeof(char) * strlen(buf)+1);
                //strncpy(arg, buf, strlen(buf)+1);
                strcpy(arg, buf);
                env_argv[j] = arg;
                j++;
            }
            start = i+1;
        }
        //strncpy(buf, line, strlen(line));
        strcpy(buf, line);
        i++;
    }
    //printf("start %d\n", start);
    substr(buf, start, strlen(line)-1);
    //printf("buf %s\n", buf);
    if (strlen(buf) > 0) {
        arg = (char *)malloc(sizeof(char) * strlen(buf)+1);
        //strncpy(arg, buf, strlen(buf)+1);
        strcpy(arg, buf);
        env_argv[j] = arg;
        j++;
    }
    env_argv[j] = NULL;
    //printf("variables %d\n", j);
    *env_argv_len = j;
    free(buf);
}

int main() {
    char line[MAX_LINE];
    size_t env_argv_len;
    char **env_argv;
    env_argv = (char**) malloc(sizeof(char *) * MAX_LINE);
    pid_t pid;
    int stat_val;
    
    while(fgets(line, MAX_LINE, stdin) != NULL) {
        if (line[strlen(line) - 1] == '\n')
            line[strlen(line) - 1] = '\0';
    
        parse_cl(line, env_argv, &env_argv_len);
        
        pid = fork();
        switch(pid) {
            case -1:
                perror("fork() error");
                break;
            case 0:
                execv(env_argv[0], env_argv);
                printf("String processed\n");
                int i;
                for (i=0; i < env_argv_len; i++) {
                    printf("=>%s<=",env_argv[i]);
                    free(env_argv[i]);
                    printf("Freed %d\n", (size_t)env_argv[i]);
                }
                printf("\n");
                exit(0);
                break;
            default:
                if ((pid = waitpid(pid, &stat_val, 0)) < 0)
                    perror("waitpid error");
                break;
        }       
    }
        
    return 0;    
}
