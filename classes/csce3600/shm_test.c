#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>

int main() 
{
    int shmid;
    pid_t pid;
    
    struct this_message
    {
        char message[256];
    } *message_ptr;
    
    memset(message_ptr->message, '\0', sizeof(message_ptr->message));
    
    shmid=shmget((key_t)1234, sizeof(message_ptr), 0666 | IPC_CREAT);
    message_ptr=shmat(shmid, 0, 0);
    
    pid = fork();
    
    switch (pid)
    {
    case -1:
        perror("Error forking");
        exit(EXIT_FAILURE);
        break;
    case 0:
        /* child */
        (void) shmat(shmid, 0, 0);
        strncpy(message_ptr->message, "cameron", 256);
        break;
    default:
        /* parent */
        wait();
        printf("The parent says %s\n", message_ptr->message);
    }
    exit(EXIT_SUCCESS);
}
