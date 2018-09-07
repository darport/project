#include "Stack.h"

#ifndef SOLVER_H_
#define SOLVER_H_

void updateIndexForward(int *row, int *col, Game*game);
void updateIndexBack(int *row, int *col, Game*game);
void updateValue(Game *game, int *row, int *col, Stack *stk);
int ** copyBoard(Cell ** board, int size);
void freeBoardInt(int **board, int size);
void copyBack(Game *game, int ** copy, int size);
int eBacktracking(Game *game);

#endif /* SOLVER_H_ */
