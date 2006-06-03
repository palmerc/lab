#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/param.h>

const size_t MAX_LINE = 256;

typedef struct {
    char *cmd;
    char *cmd_loc;
    char **argv;
    int *argv_count;
} command_t;

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
		perror(".ysh_profile");
		result = "ysh % ";
		*prompt = (char *)malloc(sizeof(char) * (strlen(result) + 1));
		strcpy(*prompt, result); 
		result = "/bin:/sbin:/usr/bin:/usr/sbin";
		*search_path = (char *)malloc(sizeof(char) * (strlen(result) + 1));
		strcpy(*search_path, result);
        return 1;
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

int main() {
    char *prompt;
    char *search_path;
    pid_t pid;
    pid_t pid_two;
	int fd[2];
    
    char line[MAX_LINE];

    size_t command_list_len;
    command_t *command_list;
    command_list = (command_t*)malloc(sizeof(command_t)*MAX_LINE);
    
    /* Ignore Ctrl-C */
    signal(SIGINT, SIG_IGN);
    profile_importer(&prompt, &search_path);
    
    printf("%s", prompt);
    
	/* Before calling fork we create the pipe */
    if (pipe(fd) < 0)
        perror("pipe error");
    
    int readflags = fcntl(fd[0], F_GETFD, 0);
    int writeflags = fcntl(fd[1], F_GETFD, 0);
       
    readflags |= FD_CLOEXEC;
    writeflags |= FD_CLOEXEC;
    fcntl(fd[0], F_SETFD, readflags);
    fcntl(fd[1], F_SETFD, writeflags);

    /* The main loop terminates on Ctrl-D */
	while(fgets(line, MAX_LINE, stdin) != NULL) {
        if (line[strlen(line) - 1] == '\n')
            line[strlen(line) - 1] = '\0';
        
        parse_cl(line, command_list, &command_list_len);
        
		int i = 0;
		
        pid = fork();
        
        switch(pid) {
            case -1:
                perror("fork() error");
                break;
            case 0:
                printf("process: %d - last\n", i);
                
                for (i = 0; i < command_list_len - 1; i++) {
                    pid_two = fork();
                    
                    switch(pid_two) {
                        case -1 :
                            perror("fork() error");
                            break;
                        case 0:
                            if ((command_list_len > 1) && (i == 0)) {
                                printf("process: 0 - first\n");
                                close(fd[0]); /* Close the read end */
                                if (fd[1] != STDOUT_FILENO) {
                                    if (dup2(fd[1], STDOUT_FILENO) != STDOUT_FILENO)
                                        perror("dup2 error to stdout");
                                    dup2(fd[1], STDOUT_FILENO);
                                    close(fd[1]);
                                }
                            } else if ((command_list_len > 1) && (i != command_list_len - 1)) {
                                printf("process: %d - middle\n", i);
                                if (fd[0] != STDIN_FILENO) {
                                    if (dup2(fd[0], STDIN_FILENO) != STDIN_FILENO)
                                        perror("dup2 error to stdin");
                                    close(fd[0]);
                                }
                                if (fd[1] != STDOUT_FILENO) {
                                    if (dup2(fd[1], STDOUT_FILENO) != STDOUT_FILENO)
                                        perror("dup2 error to stdout");
                                    dup2(fd[1], STDOUT_FILENO);
                                    close(fd[1]);
                                }
                            }
                            path_finder(search_path, command_list[i].argv[0]);
                            execv(command_list[i].argv[0], command_list[i].argv);
                            exit(0);
                            break;
                        default:
                            if (waitpid(pid_two, NULL, 0) < 0)
                                perror("waitpid error");

                    } /* switch two */
                } 
                close(fd[1]); /* Close the write end */
                if (fd[0] != STDIN_FILENO) {
                    if (dup2(fd[0], STDIN_FILENO) != STDIN_FILENO)
                        perror("dup2 error to stdin");
                    close(fd[0]);
                }
                path_finder(search_path, command_list[command_list_len - 1].argv[0]);
                execv(command_list[command_list_len - 1].argv[0], command_list[command_list_len - 1].argv);
                exit(0);
                break;
            default:
                if (waitpid(pid, NULL, 0) < 0)
                    perror("waitpid error");

                //path_finder(search_path, command_list[command_list_len - 1].argv[0]);
                //execv(command_list[command_list_len - 1].argv[0], command_list[command_list_len - 1].argv);
                //exit(0);
                break;
        }        
        printf("%s", prompt);
	}
	printf("\n");
    free(prompt);
    free(search_path);
	return 0;
}

