#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string>

using namespace std;

void appendDotIfNeeded(string& content) {
    if (!content.empty() && content.back() != '.') {
        content += '.';
    }
}

int main() {
    // Define the file names
    string fileNames[] = {"file1.txt", "file2.txt", "file3.txt"};

    // Create pipes for communication with child processes
    int pipefd[3][2];
    for (int i = 0; i < 3; i++) {
        if (pipe(pipefd[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    // Fork three child processes to read file contents
    for (int i = 0; i < 3; i++) {
        pid_t child_pid = fork();
        if (child_pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (child_pid == 0) {
            // Child process
            close(pipefd[i][0]);  // Close read end of the pipe

            // Read file content
            ifstream file(fileNames[i]);
            if (file.is_open()) {
                string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
                appendDotIfNeeded(content);
                write(pipefd[i][1], content.c_str(), content.length());
            } else {
                cerr << "Child " << (i + 1) << " failed to read file: " << fileNames[i] << endl;
            }

            close(pipefd[i][1]);  // Close write end of the pipe
            exit(EXIT_SUCCESS);
        }
    }

    // Parent process
    wait(NULL);  // Wait for all child processes to complete

    // Concatenate the contents received from child processes
    string concatenatedContent = "";
    for (int i = 0; i < 3; i++) {
        close(pipefd[i][1]);  // Close write end of the pipe

        char buffer[4096];
        int bytesRead = read(pipefd[i][0], buffer, sizeof(buffer));

        if (bytesRead > 0) {
            concatenatedContent += string(buffer, bytesRead);
        }

        close(pipefd[i][0]);  // Close read end of the pipe
    }

    // Write the concatenated content to output_bash.txt
    ofstream output("output_bash.txt");
    if (output.is_open()) {
        output << concatenatedContent;
        output.close();
        cout << "Concatenated content has been written to output_bash.txt" << endl;
    } else {
        cerr << "Failed to open output_bash.txt for writing" << endl;
    }

    return 0;
}

