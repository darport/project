#include "LinkedList.h"
#include <stdio.h>
#ifndef GAME_H_
#define GAME_H_

/* CELL: a structure for the cell in the game boards. properties:
 * value: the value of the cell
 * fixed: 1 if the cell is fixed and 0 otherwise
 * marked: 1 if the cell contains erroneous values and 0 otherwise
 * needChange: 1 if the cell has a single legal option for its value, used in autofill
 * valChange:  the single value for the cell, if the cel.needChange == 1 */
typedef struct cell_t{
	int value;
	int fixed;
	int marked;
	int needChange;
	int valChange;
} Cell;

/* GAME: a structure for the game. properties:
 * mode: the game mode - 0 fot init mode, 1 for solve mode and 2 for edit mode
 * markErrors: 1 if mark errors is on and 0 otherwise
 * board: a matrix of cells that represents the game board. this board is printed to the user during the game
 * solved: a matrix of cells that represents the solved board
 * ops: the undo-redo list - a list of lists
 * rowsInBlock: number of rows in each block
 * colsInBlock: number of columns in each block
 * size: size of block, i.e rowsInBlock * colsInBlock */
typedef struct game_t{
	int mode;
	int markErrors;
	Cell **board;
	Cell **solved;
	Link *ops;
	int rowsInBlock;
	int colsInBlock;
	int size;
} Game;

int resetOps(Game *game);
int isFull(Game *game);
void setData(Game *game, char *data, int i, int j);
int openFile(Game *game, char *fileName, int mode);
int openEmpty(Game *game);
int solve(Game *game, char *fileName);


int edit(Game *game, char *fileName);

/* the functions is called when the user enters the "mark_errors x" command and updates the game's markErrors property */
int markErrors(Game *game, int mark);

/* the function gets separator length and prints a separator. used in print board function below */
void printSep(int sepLen);

/* the function is called when the user enters the "print_board" command and prints the game board */
int printBoard(Game *game);

/* the function is called when the user enters the "set x y z" command.
 * the function checks if the cell x y is fixed and if so, prints a message and ignores the command.
 * in addition, the function updates it in the user board and prints it.
 * in solve mode, if the board is full after the command, the function prints messages to the user */
int set(Game *game, int x, int y, int z,int show, int type);

/* the function is called when the user enters the "validate" command.
 * the function checks if the user board is solvable using ILP. returns 1 if the board is solvable and 0 otherwise */
int validate(Game *game);

int existValue(Game *game, int row, int col);
int generateHelp(Game *game,int x);
int generate(Game *game, int x, int y);
int undoHelp(Game *game,int reset);
int undo(Game *game, int show,int reset);
int redoHelp(Game *game, int dontShow);
int redo(Game *game);
int saveToFile(Game *game,char *fileName);
int save(Game *game, char *fileName);
int hint(Game *game, int x, int y);
int numSolutions(Game *game);
int autofill(Game *game);
int reset(Game *game);

/* the function is called when the user enters the "exit" command.
 * the function prints exit message and returns -1, i.e exits the game */
int exitGame();

#endif /* GAME_H_ */
