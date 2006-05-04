#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

const size_t MAX_LINE = 256;
pid_t waitpid(pid_t pid, int *status, int options);
char c = '\0'; /* Null Terminator */
char *prompt;
char *search_path;

void handle_signal(int sig)
{
	//printf("\nCaught signal %d\n%s", sig, prompt);
	//fflush(stdout);
}

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

int profile_importer(char *prompt) {
    FILE *in;
    char file_line[MAX_LINE+1];
    char delims[] = "=";
    char *result = NULL;
    char single_quote[] = { "\'" };

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
            int start = 0;
            int stop = 0;
            if (strcmp(result, "PROMPT") == 0) {
                result = strtok(NULL, delims);
                
                start = strcspn(result, single_quote) + 1;
                stop = strlen(result);
                substr(result, start, stop);
               
                start = 0;
                stop = strcspn(result, single_quote) - 1;
                substr(result, start, stop);
               
                strncpy(prompt, result, strlen(result));
            }
            if (strcmp(result, "PATH") == 0) {
                result = strtok(NULL, delims);
                strncpy(search_path, result, strlen(result));
            }
            result = strtok(NULL, delims);
        }
    }
    fclose(in);
    return 0;   
}

void parse_cl(char *line, char **env_argv, size_t *env_argv_len) {
    char buf[MAX_LINE + 1];
    char *arg;

    strncpy(buf, line, strlen(line));

    int i = 0;
    int j = 0;
    int start = 0;
    while (buf[i] != '\0') {
        if (buf[i] == ' ') {
            substr(buf, start, i-1);
            if (strlen(buf) > 0) {
                arg = (char *)malloc(sizeof(char) * strlen(buf)+1);
                strncpy(arg, buf, strlen(buf));
                env_argv[j] = arg;
                j++;
            }
            start = i+1;
        }
        strncpy(buf, line, strlen(line));
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
}

int main()
{
    char line[MAX_LINE];
    //char *line;
    char *tmp_path;
    prompt = (char *)malloc(sizeof(char) * (MAX_LINE + 1));
    search_path = (char *)malloc(sizeof(char) * (MAX_LINE + 1));
    tmp_path = (char *)malloc(sizeof(char) * (MAX_LINE + 1));
    pid_t pid;
    int stat_val;
    char **env_argv;
    env_argv = (char**) malloc(sizeof(char *) * 100);
    size_t env_argv_len;
    
    signal(SIGINT, SIG_IGN);
	signal(SIGINT, handle_signal);
    
    profile_importer(prompt);
    char *env_envp[2];
    bzero(env_envp, sizeof(char) * 2);
    
    strcat(tmp_path, "PATH=");
    strcat(tmp_path, search_path);
    strcat(search_path, tmp_path);
    free(tmp_path);
    env_envp[0] = search_path;
    env_envp[1] = 0;
    printf("%s", prompt);
    
    /* The main loop terminates on Ctrl-D */
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
