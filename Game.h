#include "LinkedList.h"
#include <stdio.h>
#ifndef GAME_H_
#define GAME_H_

typedef struct cell_t{
	int value;
	int fixed;
	int marked;
	int needChange;
	int valChange;
} Cell;

typedef struct game_t{
	int mode; /* 0-init 1-solve 2-edit*/
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
int markErrors(Game *game, int mark);
void printSep(int sepLen);
int printBoard(Game *game);
int set(Game *game, int x, int y, int z,int show, int type);
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
int exitGame();
#endif /* GAME_H_ */
