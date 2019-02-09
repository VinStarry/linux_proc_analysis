#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "window_proc.h"

int pid1, pid2, pid3;

void killForks(int sig_no);
void childrenKilled(int sig_no);

int main(int argc, char *argv[]) {

    signal(SIGINT, killForks);

    pid1 = fork();
    if (pid1 < 0) {
        /* failed to create the first child process */
#ifdef _DEBUG
        perror("Fail to create the first child process!\n");
#endif
    }
    else if (pid1 == 0) {
        /* this is child process 1 executing */
#ifdef _DEBUG
        printf("Success to create the first child process, %d\n", getpid());
#endif
        signal(SIGUSR1, childrenKilled);
        wind_proc(argc, argv, 0);
    }
    else {
        pid2 = fork();
        if (pid2 < 0) {
            /* failed to create the second child process */
#ifdef _DEBUG
            perror("Fail to create the second child process!\n");
#endif
        }
        else if (pid2 == 0) {
            /* this is child process 2 executing */
#ifdef _DEBUG
            printf("Success to create the second child process, %d\n", getpid());
#endif
            signal(SIGUSR1, childrenKilled);
            wind_proc(argc, argv, 1);
        }
        else {
            pid3 = fork();
            if (pid3 < 0) {
                /* failed to create the third child process */
#ifdef _DEBUG
                perror("Fail to create the third child process!\n");
#endif
            }
            else if (pid3 == 0) {
                /* this is child process 3 executing */
#ifdef _DEBUG
                printf("Success to create the third child process, %d\n", getpid());
#endif
                signal(SIGUSR1, childrenKilled);
                wind_proc(argc, argv, 2);
            }
        }
        // this is main process
        // wait two child process to end
        waitpid(pid1, NULL, 0);
        waitpid(pid2, NULL, 0);
        waitpid(pid3, NULL, 0);
#ifdef _DEBUG
        printf("Parent Process is killed!\n");
#endif
    }
    return 0;
}


/**
    Send SIGUSR1 signal to two processes
    @param sig_no: specify which process to kill
*/
void killForks(int sig_no) {
    switch (sig_no) {
        case SIGINT:
#ifdef _DEBUG
            printf("Receive SIGINT signal, kill forks\n");
#endif
            kill(pid1, SIGUSR1);
            kill(pid2, SIGUSR1);
            kill(pid3, SIGUSR1);
            break;

        default:
#ifdef _DEBUG
            printf("Something wrong, kill forks!\n");
#endif

            kill(pid1, SIGUSR1);
            kill(pid2, SIGUSR1);
            kill(pid3, SIGUSR1);
            break;
    }
}

/**
    kill two child processes, use sig_no to determine which process to kill
    @param sig_no: specify which process to kill
*/
void childrenKilled(int sig_no) {
    if (pid1 == 0) {
#ifdef _DEBUG
        printf("Child Process 1 is Killed by Parent!\n");
#endif
        _exit(0);
    }
    if (pid2 == 0) {
#ifdef _DEBUG
        printf("Child Process 2 is Killed by Parent!\n");
#endif
        _exit(0);
    }
    if (pid3 == 0) {
#ifdef _DEBUG
        printf("Child Process 3 is Killed by Parent!\n");
#endif
        _exit(0);
    }
}