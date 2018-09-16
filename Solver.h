#include "Stack.h"

#ifndef SOLVER_H_
#define SOLVER_H_
/*the function updates the indexes of row and col when moving forword with same solution*/
void updateIndexForward(int *row, int *col, Game*game);
/*the function updates the indexes of row and col when moving back to find a different solution*/
void updateIndexBack(int *row, int *col, Game*game);
/*the function updates the value of the board[row][col]*/
void updateValue(Game *game, int *row, int *col, Stack *stk);
/*the function copying the values of the board to a two dimensional board of ints and returns it*/
int ** copyBoard(Cell ** board, int size);
/*the function releases the sources of the temporary int board we use*/
void freeBoardInt(int **board, int size);
/*the function restore the game board original values using the copied board of ints*/
void copyBack(Game *game, int ** copy, int size);
/*the function uses the backtracking aalgorithm in order to find the amount of solutions possible for the current game board
 * return the amount of solutions found for the current board*/
int eBacktracking(Game *game);

#endif /* SOLVER_H_ */
