#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
using namespace std;

void merge(int arr[], int left[], int leftSize, int right[], int rightSize) {
    int i = 0, j = 0, k = 0;

    while (i < leftSize && j < rightSize) {
        if (left[i] <= right[j]) {
            arr[k++] = left[i++];
        } else {
            arr[k++] = right[j++];
        }
    }

    while (i < leftSize) {
        arr[k++] = left[i++];
    }

    while (j < rightSize) {
        arr[k++] = right[j++];
    }
}

void merge_sort(int arr[], int size) {
    if (size <= 1) {
        return;
    }

    int mid = size / 2;
    int left[mid];
    int right[size - mid];

    for (int i = 0; i < mid; i++) {
        left[i] = arr[i];
    }

    for (int i = mid; i < size; i++) {
        right[i - mid] = arr[i];
    }

    pid_t pid1, pid2;
    int pipefd[2];

    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid1 = fork();

    if (pid1 == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid1 == 0) {
        // Child process 1
        close(pipefd[0]); // Close the read end of the pipe

        merge_sort(left, mid);

        write(pipefd[1], left, sizeof(left));
        close(pipefd[1]); // Close the write end of the pipe

        exit(EXIT_SUCCESS);
    } else {
        pid2 = fork();

        if (pid2 == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid2 == 0) {
            // Child process 2
            close(pipefd[0]); // Close the read end of the pipe

            merge_sort(right, size - mid);

            write(pipefd[1], right, sizeof(right));
            close(pipefd[1]); // Close the write end of the pipe

            exit(EXIT_SUCCESS);
        } else {
            // Parent process
            int status;
            waitpid(pid1, &status, 0);
            waitpid(pid2, &status, 0);

            close(pipefd[1]); // Close the write end of the pipe

            int leftSorted[mid];
            int rightSorted[size - mid];

            read(pipefd[0], leftSorted, sizeof(leftSorted));
            read(pipefd[0], rightSorted, sizeof(rightSorted));

            close(pipefd[0]); // Close the read end of the pipe

            merge(arr, leftSorted, mid, rightSorted, size - mid);
        }
    }
}

int main() {
    int size;
    cout << "Enter the size of the array: ";
    cin >> size;

    int arr[size];
    cout << "Enter the elements of the array: ";
    for (int i = 0; i < size; i++) {
        cin >> arr[i];
    }

    merge_sort(arr, size);

    cout << "Sorted array: ";
    for (int i = 0; i < size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    return 0;
}

