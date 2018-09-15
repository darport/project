#ifndef MAINAUX_H_
#define MAINAUX_H_

void freeBoard(Cell **temp,int size);
void initNode(Node *head);
void initializeOps(Game *game);
void cleanBoard(Game * game);
void freeGame(Game *game);
int isErroneous(Game *game);
int validR(Game *game, int row,int col, int option, int markErroneous);
int validC(Game *game, int row, int col, int option, int markErroneous);
int validB(Game *game, int row, int col, int option, int markErroneous);
int isValid(Game *game, int row, int col, int option, int markErroneous);
void updateErroneous(Game *game);
int options(Game *game ,int i, int j);
void markChanges(Game *game);
int setChanges(Game *game);
void initCell(Game *game, int i, int j);
#endif /* MAINAUX_H_ */
