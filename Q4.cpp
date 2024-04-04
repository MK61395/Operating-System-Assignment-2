#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

int main() {
    // Print current environment variables
    cout << "Current environment variables:" << endl;
    cout << "USER: " << getenv("USER") << endl;
    cout << "TERM: " << getenv("TERM") << endl;
    cout << "PATH: " << getenv("PATH") << endl;

    // Store environment variable values
    const char* arg1 = getenv("USER");
    const char* arg2 = getenv("TERM");
    const char* arg3 = getenv("PATH");

    // Prepare arguments for execve()
    const char* scriptPath = "script.sh";
    char* vectorArg[] = {const_cast<char*>(scriptPath), const_cast<char*>(arg1), const_cast<char*>(arg2), const_cast<char*>(arg3), NULL};

    // Prepare environment for execve()
    char* vectorEnv[] = {"OS2022=5ma32zw", NULL};

    // Execute the shell script using execve()
    if (execve(scriptPath, vectorArg, vectorEnv) == -1) {
        perror("execve");
        exit(EXIT_FAILURE);
    }

    // This code will not be reached if execve() is successful
    cout << "This line should not be reached." << endl;

    return 0;
}
