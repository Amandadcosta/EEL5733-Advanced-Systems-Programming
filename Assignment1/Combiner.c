#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


int main(int argc, char *argv[])
{
    int pipe_fd[2];                                     

    if (pipe(pipe_fd) == -1)                            
        perror("failed");

    switch (fork()) {   //returns pid
    case -1:
        printf("fork error");

    case 0: // entered child1             
        if (close(pipe_fd[0]) == -1)  //close reading end                  
            printf("close pipe1");


        if (pipe_fd[1] != STDOUT_FILENO) {              
            if (dup2(pipe_fd[1], STDOUT_FILENO) == -1) //redirect the write end to stdout or  fd[1]
                printf("dup2\n");
            if (close(pipe_fd[1]) == -1)
                printf("close pipe1");
        }

        //execl("mapper", "mapper",NULL);
        
        execlp("Mapper","Mapper", (char *) NULL);    
        printf("executing Mapper");

    default:    //parent executes this        
        break;
    }
    //parent creates child2 that is reducer
    switch (fork()) {
    case -1:
        perror("fork error");

    case 0:             
        if (close(pipe_fd[1]) == -1)      //close the write end               
            printf("close pipe2");


        if (pipe_fd[0] != STDIN_FILENO) {               
            if (dup2(pipe_fd[0], STDIN_FILENO) == -1)//redirect read end to stdin or fd[0]
                printf("dup2");
            if (close(pipe_fd[0]) == -1)    // close read end
                printf("close pipe2");
        }

        //execl("reducer", "reducer",NULL);
        execlp("Reducer","Reducer", (char *) NULL); 
        printf("executing reducer\n");

    default:
        break;
    }


    if (close(pipe_fd[0]) == -1)
        printf("close pipe\n"); //close write end of pipe for parent
    if (close(pipe_fd[1]) == -1)
        printf("close pipe\n"); //close read end of pipe for parent
    //wait for both children to terminate
    if (wait(NULL) == -1)
        perror("wait");
    if (wait(NULL) == -1)
        perror("wait");

    exit(EXIT_SUCCESS);
}
