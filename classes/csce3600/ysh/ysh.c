#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <wait.h>

const size_t MAX_LINE = 256;
char *prompt;
char *search_path;

/* function prototype */
pid_t waitpid(pid_t pid, int *status, int options);
void substr(char *string, int start, int stop);


void handle_signal(int sig)
{
	//printf("\nCaught signal %d\n%s", sig, prompt);
	//fflush(stdout);
}

int profile_importer(char *prompt, char *search_path) {
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
        if (file_line[strlen(file_line) - 1] == '\n')
            file_line[strlen(file_line) - 1] = '\0';
        
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
               
                prompt = (char *)malloc(sizeof(char) * (strlen(result) + 1));
                strcpy(prompt, result);
            }
            if (strcmp(result, "PATH") == 0) {
                result = strtok(NULL, delims);
                search_path = (char *)malloc(sizeof(char) * (strlen(result) + 1));
                strcpy(search_path, result);
            }
            result = strtok(NULL, delims);
        }
    }
    fclose(in);
    return 0;   
}

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

void parse_cl(char *line, char **env_argv, size_t *env_argv_len) {
    char *buf = malloc(sizeof(char) * (strlen(line) + 1));
    char *arg;

    strcpy(buf, line);
    
    int i = 0;
    int j = 0;
    int start = 0;
    while (buf[i] != '\0') {
        if (buf[i] == ' ') {
            substr(buf, start, i-1);
            if (strlen(buf) > 0) {
                arg = (char *)malloc(sizeof(char) * strlen(buf)+1);
                strcpy(arg, buf);
                env_argv[j] = arg;
                j++;
            }
            start = i+1;
        }
        strcpy(buf, line);
        i++;
    }
    substr(buf, start, strlen(line)-1);
    if (strlen(buf) > 0) {
        arg = (char *)malloc(sizeof(char) * strlen(buf)+1);
        strcpy(arg, buf);
        env_argv[j] = arg;
        j++;
    }
    env_argv[j] = NULL;
    *env_argv_len = j;
    free(buf);
}

void path_finder(char *search_path, char *prog_name) {
    struct stat buf;
    //char *ptr;
    
    printf("%s\n", search_path);
    stat(prog_name, &buf);
    if (S_ISREG(buf.st_mode))
        printf("%s\n", (char *)buf.st_mode);
        //return pathname;
}

int main()
{
    pid_t pid;
    int stat_val;
    
    char line[MAX_LINE];

    size_t env_argv_len;
    char **env_argv;
    env_argv = (char**) malloc(sizeof(char *) * MAX_LINE);
    
    profile_importer(prompt, search_path);
    
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
                path_finder(search_path, env_argv[0]);
                execv(env_argv[0], env_argv);
                int arg_index;
                for (arg_index=0; arg_index < env_argv_len; arg_index++)
                    free(env_argv[arg_index]);
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
    free(search_path);
	return 0;
}
