#include <stdio.h>

int memoryError() {
    printf("Error: Memory allocation failed\n");
    return -1;
}

int closeFileError() {
    printf("Error: File cannot be closed\n");
    return -1;
}

int openFileError(int mode) {
    if(mode == 2) {
        printf("Error: File cannot be opened\n");
    }
    else{
        printf("Error: File doesn't exist or cannot be opened\n");
    }
    return 0;
}

int fileWriteError() {
    printf("Error: Cannot write to file\n");
    return -1;
}

int erroneous() {
    printf("Error: board contains erroneous values\n");
    return 0;
}
