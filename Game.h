#include "LinkedList.h"

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
	Cell **board ;
	Cell **solved;
	Node *ops;
	int rowsInBlock;
	int colsInBlock;
	int size;
} Game;



int solve(Game *game, char *fileName);
int edit(Game *game, char *fileName);
int markErrors(Game *game, int mark);
/*int printBoard(Game *game);*/
int set(Game *game, int x, int y, int z);
int validate(Game *game);
int generate(Game *game, int x, int y);
int undo(Game *game);
int redo(Game *game);
int save(Game *game, char *fileName);
int hint(Game *game, int x, int y);
int numSolutions(Game *game);
int autofill(Game *game);
int reset(Game *game);
int exitGame(Game *game);
#endif /* GAME_H_ */
