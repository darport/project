#include "Game.h"

#ifndef MAINAUX_H_
#define MAINAUX_H_

void boardCopy(Game *game);
void freeBoard(Cell **temp,int size);
void initializeOps(Game *game);
void cleanBoard(Game * game);
void freeGame(Game *game);
int memoryError();
int fileError();
int erroneous();
int isErroneous(Game *game);
int validR(Game *game, int row,int col, int option, int markErroneous);
int validC(Game *game, int row, int col, int option, int markErroneous);
int validB(Game *game, int row, int col, int option, int markErroneous);
int isValid(Game *game, int row, int col, int option, int markErroneous);
void updateErroneous(Game *game);
int options(Game *game ,int i, int j);
void markChanges(Game *game);
void setChanges(Game *game);


#endif /* MAINAUX_H_ */
