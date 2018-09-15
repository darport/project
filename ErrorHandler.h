#ifndef ERRORHANDLER_H_
#define ERRORHANDLER_H_

/* the function prints the message for memory allocation error and returns -1, i.e ends the game */
int memoryError();

/* the function prints the message for close file error and returns -1, i.e ends the game */
int closeFileError();

/* the function prints the message for file write error and returns -1, i.e ends the game */
int fileWriteError();

/* the function prints the message for open file error and returns 0 */
int openFileError(int mode);

/* the function prints the message for erroneous values in the game board and returns 0 */
int erroneous();

#endif /*ERRORHANDLER_H_*/
