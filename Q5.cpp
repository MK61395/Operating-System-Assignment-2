#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdlib>
#include <dirent.h>

using namespace std;

const int MAX_FILES = 100; // Maximum number of files in the directory

/// Function to perform file encryption using a Caesar cipher
void encryptFile(const string& filePath) {
    ifstream inputFile(filePath);
    if (!inputFile) {
        cerr << "Error opening file for encryption: " << filePath << endl;
        return;
    }

    // Create an output file with ".enc" extension
    string outputFilePath = filePath + ".enc";
    ofstream outputFile(outputFilePath);
    if (!outputFile) {
        cerr << "Error creating encrypted file: " << outputFilePath << endl;
        return;
    }

    int shift = 3; // Caesar cipher shift value (for demonstration purposes)

    char ch;
    while (inputFile.get(ch)) {
        if (isalpha(ch)) {
            char base = isupper(ch) ? 'A' : 'a';
            ch = static_cast<char>((ch - base + shift) % 26 + base);
        }
        outputFile.put(ch);
    }

    cout << "Encryption complete. Encrypted file saved as: " << outputFilePath << endl;
}

// Function to perform file decryption using a Caesar cipher
void decryptFile(const string& filePath) {
    ifstream inputFile(filePath);
    if (!inputFile) {
        cerr << "Error opening file for decryption: " << filePath << endl;
        return;
    }

    // Determine the output file name by removing ".enc" extension
    string outputFilePath = filePath.substr(0, filePath.length() - 4);
    ofstream outputFile(outputFilePath);
    if (!outputFile) {
        cerr << "Error creating decrypted file: " << outputFilePath << endl;
        return;
    }

    int shift = 3; // Caesar cipher shift value (for demonstration purposes)

    char ch;
    while (inputFile.get(ch)) {
        if (isalpha(ch)) {
            char base = isupper(ch) ? 'A' : 'a';
            ch = static_cast<char>((ch - base - shift + 26) % 26 + base);
        }
        outputFile.put(ch);
    }

    cout << "Decryption complete. Decrypted file saved as: " << outputFilePath << endl;
}


int main() {
     string directoryPath;
    cout << "Enter the directory path: ";
    cin >> directoryPath;

    string fileNames[MAX_FILES]; // Array to store file names
    int fileCount = 0;

    // Read and display the list of files in the directory
    cout << "Files in the directory:" << endl;
    DIR* dirp = opendir(directoryPath.c_str());
    if (dirp == nullptr) {
        cerr << "Error opening directory." << endl;
        return 1;
    }

    struct dirent * dp;
    while ((dp = readdir(dirp)) != nullptr && fileCount < MAX_FILES) {
        fileNames[fileCount] = dp->d_name;
        fileCount++;
        cout << fileCount << ". " << fileNames[fileCount - 1] << endl;
    }
    closedir(dirp);

    cout << "Enter the indexes of files to process (e.g., 1 2 4): ";
    string selectedIndexes;
    cin.ignore();
    getline(cin, selectedIndexes);

    istringstream indexStream(selectedIndexes);
    int index;
    int selectedFiles[MAX_FILES]; // Array to store selected file indexes
    int selectedFileCount = 0;

    // Parse user-selected file indexes
    while (indexStream >> index) {
        if (index >= 1 && index <= fileCount) {
            selectedFiles[selectedFileCount] = index - 1; // Adjust for 0-based index
            selectedFileCount++;
        } else {
            cerr << "Invalid file index: " << index << endl;
        }
    }

    // Create processes for concurrent encryption/decryption of selected files
    for (int i = 0; i < selectedFileCount; i++) {
        pid_t pid = fork();

        if (pid == -1) {
            cerr << "Fork failed." << endl;
            return 1;
        } else if (pid == 0) {
            // Child process
            string selectedFile = directoryPath + "/" + fileNames[selectedFiles[i]];

            // Determine if encryption or decryption is requested based on file extension
            size_t extensionPos = selectedFile.find_last_of('.');
            if (extensionPos != string::npos && selectedFile.substr(extensionPos) == ".enc") {
                decryptFile(selectedFile);
                cout << "Decrypted: " << selectedFile << endl;
            } else {
                encryptFile(selectedFile);
                cout << "Encrypted: " << selectedFile << endl;
            }

            exit(0);
        }
    }

    // Wait for all child processes to complete
    int status;
    while (wait(&status) > 0) {
        // Handle completed child processes
    }

    return 0;
}

