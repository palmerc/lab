#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <wait.h>
#include <sys/param.h>

typedef char* str_t;
const size_t MAX_LINE = 256;

/* function prototype */
pid_t waitpid(pid_t pid, int *status, int options);
void substr(char *string, int start, int stop);

int profile_importer(char **prompt, char **search_path) {
    FILE *in;
    char file_line[MAX_LINE+1];
    char *result = NULL;
    char delims[] = { "=" };
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
               
                *prompt = (char *)malloc(sizeof(char) * (strlen(result) + 1));
                strcpy(*prompt, result);
            }
            if (strcmp(result, "PATH") == 0) {
                result = strtok(NULL, delims);
                *search_path = (char *)malloc(sizeof(char) * (strlen(result) + 1));
                strcpy(*search_path, result);
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

void parse_cl(char *line, str_t **env_argv, size_t *env_argv_len) {
    char *buf = malloc(sizeof(char) * (strlen(line) + 1));
    char *arg;
    *env_argv_len = 0;
    
    size_t pipe_argv_len;
    char **pipe_argv = (char **)malloc(sizeof(char) * MAX_LINE);
    
    strcpy(buf, line);
    int i = 0;
    int j = 0;
    int start = 0;
    while (buf[i] != '\0') {
        if (buf[i] == '|') {
            substr(buf, start, i-1);
            if (strlen(buf) > 0) {
                arg = (char *)malloc(sizeof(char) * strlen(buf)+1);
                strcpy(arg, buf);
                pipe_argv[j] = arg;
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
        pipe_argv[j] = arg;
        j++;
    }
    pipe_argv[j] = NULL;
    pipe_argv_len = j;
    
    int k;
    for (k = 0; k < pipe_argv_len; k++)
        printf("%d: %s\n", k, pipe_argv[k]);

    
    int pipe_count = 0;
    while (pipe_count < pipe_argv_len) {
        i = 0;
        j = 0;
        start = 0;
        strcpy(buf, pipe_argv[pipe_count]);
        while (buf[i] != '\0') {
            if (buf[i] == ' ') {
                substr(buf, start, i-1);
                if (strlen(buf) > 0) {
                    arg = (char *)malloc(sizeof(char) * strlen(buf)+1);
                    strcpy(arg, buf);
                    env_argv[pipe_count] = arg;
                    j++;
                }
                start = i+1;
            }
            strcpy(buf, pipe_argv[pipe_count]);
            i++;
        }
        substr(buf, start, strlen(line)-1);
        if (strlen(buf) > 0) {
            arg = (char *)malloc(sizeof(char) * strlen(buf)+1);
            strcpy(arg, buf);
            env_argv[pipe_count] = arg;
            j++;
        }
        env_argv[pipe_count] = NULL;
        *env_argv_len = j;
    }
    //for (k = 0; k < *env_argv_len; k++)
    //    printf("%d: %s\n", k, env_argv[k]);
    free(buf);
}

void path_finder(char *search_path, char *prog_name) {
    struct stat buf;
    char delims[] = { ":" };
    char *prefix = NULL;
    char temp[MAX_LINE];
     
    if ((prog_name[0] != '/') && (prog_name[0] != '.')) {
        prefix = strtok(search_path, delims);
        while (prefix != NULL) {
            strcpy(temp, prefix);
            strcat(temp, "/");
            strcat(temp, prog_name);
            stat(temp, &buf);    
            if (S_ISREG(buf.st_mode)) {
                realloc(prog_name, strlen(temp)+1);
                strcpy(prog_name, temp);
                break;
            }
            prefix = strtok(NULL, delims);     
        }
    }
}

void exec_process(char **env_argv, size_t env_argv_len) {
    int fd[2];
    //int stat_val;
    pid_t pid;
    //int i;
        
    /* Before calling fork we create the pipe */
    if (pipe(fd) < 0)
        perror("pipe error");
    
    int readflags = fcntl(fd[0], F_GETFD, 0);
    int writeflags = fcntl(fd[1], F_GETFD, 0);
       
    readflags |= FD_CLOEXEC;
    writeflags |= FD_CLOEXEC;
    fcntl(fd[0], F_SETFD, readflags);
    fcntl(fd[1], F_SETFD, writeflags);
 
    /* We fork the sender child */
    if ((pid=fork()) < 0)
        perror("fork error");

    if (pid == 0) {
        close(fd[0]); /* Close the read end */
        if (fd[1] != STDOUT_FILENO) {
            if (dup2(fd[1], STDOUT_FILENO) != STDOUT_FILENO)
                perror("dup2 error to stdout");
            dup2(fd[1], STDOUT_FILENO);
            close(fd[1]);
        }
        execl("/bin/ps","ps","auxw",(char *)0);
        exit(0);
    }
    
    
    
    /* We fork the receiver child */
    if ((pid=fork()) < 0)
        perror("fork error");
        
    if (pid == 0) {
        close(fd[1]); /* Close the write end */
        if (fd[0] != STDIN_FILENO) {
            if (dup2(fd[0], STDIN_FILENO) != STDIN_FILENO)
                perror("dup2 error to stdin");
            close(fd[0]);
        }
        execl("/usr/bin/wc","wc","-l",(char *)0);
        
        exit(0);
    }
}

int main()
{
    char *prompt;
    char *search_path;
    pid_t pid;
    int stat_val;
    
    char line[MAX_LINE];

    size_t env_argv_len;
    str_t **env_argv;
    env_argv = (str_t *)malloc(sizeof(str_t *) * 256);
                
    //char in[MAX_LINE];
    //char out[MAX_LINE];
    //int bg = 0;
    
    /* Ignore Ctrl-C */
    signal(SIGINT, SIG_IGN);
    profile_importer(&prompt, &search_path);
    
    printf("%s", prompt);
    
    /* The main loop terminates on Ctrl-D */
	while(fgets(line, MAX_LINE, stdin) != NULL) {
        if (line[strlen(line) - 1] == '\n')
            line[strlen(line) - 1] = '\0';
        
        //linizer(line, pipe_argv, &pipe_argv_len, stdin, stdout, bg);
        parse_cl(line, &env_argv, &env_argv_len);
        //int i;
        //for (i = 0; i < env_argv_len; i++)
        //    printf("%d: %s\n", i, *env_argv[i]);
        //tokenizer(line);
        
        pid = fork();
        switch(pid) {
            case -1:
                perror("fork() error");
                break;
            case 0:
                /* path_finder(search_path, env_argv[0]);
                execv(env_argv[0], env_argv);
                exec_process(env_argv, env_argv_len); */
                //int arg_index;
                //for (arg_index=0; arg_index < env_argv_len; arg_index++)
                //    free(env_argv[arg_index]);
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
