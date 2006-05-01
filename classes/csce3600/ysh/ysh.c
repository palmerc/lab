#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

const MAX_LINE = 256;
char c = '\0'; /* Null Terminator */
char *line;
char *prompt;
char *search_path;

void handle_signal(int sig)
{
	printf("\nCaught signal %d\n%s", sig, prompt);
	fflush(stdout);
}

void substr(char *string, int start, int stop) {
    char *temp = malloc(sizeof(char) * (strlen(string) + 1));
    int i = 0;
    
    strncpy(temp, "\0", 1);
    //printf("I was given ->%s\n", string);
    
    if (strlen(string) < stop) {
        stop = strlen(string);
    }
    
    //printf("Chars before stop %d\n", stop);
    int j = 0;
    for (i = start; i <= stop; i++) {
        //printf("%c", string[i]);
        temp[j] = string[i];
        j++;
    }
    bzero(string, strlen(string)+1);
    strncpy(string, temp, stop);
    //printf("Length of string %d\n", strlen(string));
    free(temp);
}

int profile_importer(char *prompt) {
    FILE *in;
    char file_line[MAX_LINE+1];
    char delims[] = "=";
    char *result = NULL;
    char single_quote[] = "\'";

    /* fopen returns NULL if unable to open */
    in = fopen(".ysh_profile", "r");
    if (in == NULL) {
        return 1;
    } else {
        perror(".ysh_profile");
    }
    
    /* fgets returns a null pointer when it encounters EOF */
    while ((fgets(file_line, MAX_LINE, in)) != NULL) {
        result = strtok(file_line, delims);
        while (result != NULL) {
            //printf("I was given ->%s\n", result);
            int start = 0;
            int stop = 0;
            if (strcmp(result, "PROMPT") == 0) {
                result = strtok(NULL, delims);
                
                start = strcspn(result, single_quote) + 1;
                //stop = 7;
                stop = strlen(result);
                substr(result, start, stop);
                
                start = 0;
                stop = strcspn(result, single_quote);
                //stop = 7;
                //printf("STOP %d\n", stop);
                substr(result, start, stop);
                
                strncpy(prompt, result, MAX_LINE);
                //printf("%s", prompt);
            }
            if (strcmp(result, "PATH") == 0) {
                result = strtok(NULL, delims);
                strncpy(search_path, result, MAX_LINE);
                //printf("Search PATH=%s\n", search_path);
            }
            result = strtok(NULL, delims);
        }
    }
    fclose(in);
    return 0;   
}

int main()
{
    char *tmp_path;
    prompt = (char *)malloc(sizeof(char) * (MAX_LINE + 1));
    search_path = (char *)malloc(sizeof(char) * (MAX_LINE + 1));
    tmp_path = (char *)malloc(sizeof(char) * (MAX_LINE + 1));
	signal(SIGINT, SIG_IGN);
	signal(SIGINT, handle_signal);
    line = (char *)malloc(sizeof(char) * (MAX_LINE + 1));    
    pid_t pid, child_pid;
    int stat_val;
    char *env_argv[]={ "ls", "-l", "-a", 0 };
    
    profile_importer(prompt);
    
    char *env_envp[2];
    bzero(env_envp, sizeof(char) * 2);
    
    strcat(tmp_path, "PATH=");
    strcat(tmp_path, search_path);
    strcat(search_path, tmp_path);
    free(tmp_path);
    env_envp[0] = search_path;
    env_envp[1] = 0;
    //printf("ENV %s", env_envp[0]);    
    printf("%s", prompt);
    
    /* The main loop terminates on Ctrl-D */
	while(c != EOF) {
        c = getchar();
        /* We need to store the characters until we receive a return */
        switch (c) {
            case '\n':
                pid = fork();
                switch(pid) {
                    case -1:
                        perror("fork() error");
                        break;
                    case 0:
                        execvp(env_argv[0], env_argv);
                        //printf("Exec Failure\n");
                        exit(0);
                        break;
                    default:
                        child_pid = wait(&stat_val);
                        printf("%s", prompt);
                        bzero(line, strlen(line));
                        break;
                }
            default:
                strncat(line, &c, 1);
                break;
        }
        
	}
	printf("\n");
    free(prompt);
	return 0;
}
