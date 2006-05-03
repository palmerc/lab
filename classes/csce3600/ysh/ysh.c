#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

const MAX_LINE = 256;
char c = '\0'; /* Null Terminator */
char *prompt;
char *search_path;

void handle_signal(int sig)
{
	printf("\nCaught signal %d\n%s", sig, prompt);
	fflush(stdout);
}

void substr(char *string, int start, int stop) {
    char *temp = malloc(sizeof(char) * (strlen(string) + 1));
          
    printf("substr start=>%s<=\n", string);
    
    strncpy(temp, "\0", 1);
    if (strlen(string) < stop) {
        stop = strlen(string);
    }
    
    int i = 0; /* The position of the start of the slice */
    int j = 0; /* The position in the new string */
    for (i = start; i <= stop; i++) {
        temp[j] = string[i];
        j++;
    }
    strncpy(string, "\0", strlen(string) + 1);
    strncpy(string, temp, (stop - start));
    
    printf("substr end=>%s<=\n", string);
    
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

void parse_cl(char *line, char *env_argv) {
    char *copy = line;
    char temp[MAX_LINE + 1];
    int start = 0;
    int i = 0;
    int j = 0;

    strncpy(temp, line, strlen(copy));
    //printf("parse_cl start=>%s\n", copy);
    while (copy[i] != '\0') {
        //printf("parse_cl loop =>%d\n", i);
        if (copy[i] == ' ') {
            //printf("parse_cl space =>%d\n", j);
            substr(temp, start, i);
            //strncat(temp, '\0', 1);
            //strncpy(&env_argv[j], temp, strlen(temp));
            
            //printf("parse_cl argv =>%s<=\n", &env_argv[j]);
            start = i + 1;
            j++;
        }
        bzero(temp, strlen(copy));
        strncpy(temp, copy, strlen(copy));
        i++;
    }
    //printf("parse_cl final\n", j);
    substr((char *)temp, start, i);
    //strncat(temp, '\0', 1);
    strncpy(&env_argv[j], temp, strlen(temp));
    for (i=0; i < sizeof(&env_argv); i++)
        printf("parse_cl argv[%d] =>%s<=\n", i, &env_argv[i]);
    //printf("parse_cl end=>%s\n", temp);
    //strncpy(&env_argv[j++], NULL, 1);   
}

int main()
{
    char line[MAX_LINE];
    //char *line;
    char *tmp_path;
    prompt = (char *)malloc(sizeof(char) * (MAX_LINE + 1));
    search_path = (char *)malloc(sizeof(char) * (MAX_LINE + 1));
    tmp_path = (char *)malloc(sizeof(char) * (MAX_LINE + 1));
	signal(SIGINT, SIG_IGN);
	signal(SIGINT, handle_signal);
    //line = (char *)malloc(sizeof(char) * (MAX_LINE + 1));    
    pid_t pid, child_pid;
    int stat_val;
    //char env_argv[MAX_LINE];
    char env_argv[100];
    //env_argv = (char *)malloc(sizeof(char) * (MAX_LINE + 1));
    
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
	while(fgets(line, MAX_LINE, stdin) != NULL) {
        char env_argv[MAX_LINE];
        if (line[strlen(line) - 1] == '\n')
            line[strlen(line) - 1] = '\0';
        
        pid = fork();
        switch(pid) {
            case -1:
                perror("fork() error");
                break;
            case 0:
                bzero(env_argv, 100);
                parse_cl(line, env_argv);
                int i = 0;
                //for (i=0; i < sizeof(env_argv); i++)
                //    printf("parse_cl argv[%d] =>%s<=\n", i, env_argv[i]);
                execlp(env_argv, env_argv, NULL);
                //printf("Exec Failure\n");
                exit(0);
                break;
            default:
                if ((pid = waitpid(pid, &stat_val, 0)) < 0)
                    perror("waitpid error");
                printf("%s", prompt);
                break;
        }       
	}
	printf("\n");
    free(prompt);
	return 0;
}
