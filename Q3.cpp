#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

int main() {
    pid_t pid1, pid2;

    // Fork the first child process
    pid1 = fork();

    if (pid1 == -1) {
        cerr << "Fork failed" << endl;
        exit(1);
    }

    if (pid1 == 0) {
        // This is the first child process
        // Fork a second child process

        pid2 = fork();

        if (pid2 == -1) {
            cerr << "Fork failed" << endl;
            exit(1);
        }

        if (pid2 == 0) {
            // This is the second child process
            // Execute the 'ls' command with 'ls -l' arguments
            char *const args[] = {"ls", "-l", NULL};
            execvp("ls", args);

            // If execvp() fails, print an error message
            cerr << "execvp failed" << endl;
            exit(1);
        } else {
            // First child process waiting for the second child to finish
            int status;
            wait(&status);

            if (WIFEXITED(status)) {
                // Check the return status of the second child
                if (WEXITSTATUS(status) == 0) {
                    cout << "Child successful" << endl;
                } else {
                    cerr << "Child failed" << endl;
                }
            } else {
                cerr << "Child process did not exit normally" << endl;
            }
        }
    } else {
        // Parent process waiting for the first child to finish
        wait(NULL);
    }

    return 0;
}

