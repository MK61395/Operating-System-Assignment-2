#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
using namespace std;

void createChildProcess(int childNumber) {
    pid_t pid = fork(); // Create a new process

    if (pid == -1) {
        // Error occurred while forking
        perror("fork");
        exit(1);
    }

    if (pid == 0) {
        // This code is executed by the child process
        cout << "P" << childNumber << ": ID = " << getpid() << ", Parent ID = " << getppid() << endl;
        exit(0);
    }
}

int main() {
    cout << "Parent process (P0): ID = " << getpid() << endl;

    // Create three child processes
    for (int i = 1; i <= 3; i++) {
        createChildProcess(i);
    }

    // Wait for all child processes to finish
    for (int i = 1; i <= 3; i++) {
        wait(NULL);
    }

    cout << "Parent process (P0) is exiting." << endl;
    return 0;
}

