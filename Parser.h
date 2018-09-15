#include "Game.h"

#ifndef PARSER_H_
#define PARSER_H_

/* the function prints the message for invalid command and returns 0 */
int printInvalid();

/* the function gets a number and returns 1 if its in the right range for the set/hint commands and 0 otherwise */
int inRange(Game *game, int num);

/* the function gets a number and returns 1 if its in the right range fot the generate command and 0 otherwise */
int inRangeGen(Game *game, int num);

/* the function gets a string and returns 1 if the string represent a number and 0 otherwise */
int isInt(char *num);

/* the function gets an array of params and its length and returns 1 if the array is full with not NULL values and 0 otherwise.
 * this function is used to check that each command gets the amount of params its need to get */
int checkParams(char **params, int len);

/* the function gets from the user the parameters for the solve command.
 * if the parameters are valid , i.e at least 1 filename - the function executes the solve command.
 * if the parameters are not valid, the function prints a message and gets back to the main for the next command (returns 0) */
int callSolve(Game *game);

/* the function gets from the user the parameters for the edit command.
 * if there is a filename parameter, the function executes the edit command with the filename.
 * otherwise, the function executes the edit command without the filename. */
int callEdit(Game *game);

/* the function gets from the user the parameters for the mark_errors command.
 * if the parameters are valid, i.e at least 1 parameter which is 0 or 1 - the function executes the mark_errors command.
 * if the parameters are not valid, the function prints a message and gets back to the main for the next command (returns 0) */
int callMarkErrors(Game *game);

/* the function gets from the user the parameters for the set command.
 * if the parameters are valid, i.e at least 3 parameters in the right range - the function executes the set command.
 * if the parameters are not valid, the function prints a message and gets back to the main for the next command (returns 0) */
int callSet(Game *game);

/* the function gets from the user the parameters for the generate command.
 * if the parameters are valid, i.e at least 2 parameters in the right range - the function executes the generate command.
 * if the parameters are not valid, the function prints a message and gets back to the main for the next command (returns 0) */
int callGenerate(Game *game);

/* the function gets from the user the parameters for the save command.
 * if the parameters are valid, i.e least 1 filename - the function executes the save command.
 * if the parameters are not valid, the function prints a message and gets back to the main for the next command (returns 0) */
int callSave(Game *game);

/* the function gets from the user the parameters for the hint command.
 * if the parameters are valid, i.e at least 2 parameters in the right range - the function executes the hint command.
 * if the parameters are not valid, the function prints a message and gets back to the main for the next command (returns 0) */
int callHint(Game *game);

/* the function gets a single command from the user via the console and checks if the command is valid.
 * if the command is valid, the function calls other functions that executes the command.
 * if the command is not valid, the function prints a message and gets back to the main for the next command (returns 0) .*/
int getCommand(Game *game);

#endif /* PARSER_H_ */
