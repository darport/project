#include <stdio.h>
#include <stdlib.h>
#include "Game.h"
#include "Stack.h"
#include "mainAux.h"

void updateIndexForward(int *row, int *col, Game *game){
    if(*col < game->size-1){
        *col = *col + 1;
    }
    else{
        *row = *row + 1;
        *col=0;
    }
}

void updateIndexBack(int *row, int *col, Game *game){
    if(*col > 0){
        *col = *col - 1;
    }
    else{
        *row = *row - 1;
        *col=game->size -1;
    }
}

void updateValue(Game *game, int *row, int *col, Stack *stk){
    game->board[*row][*col].value++;
    while (!isValid(game,*row,*col,game->board[*row][*col].value,0) &&
           game->board[*row][*col].value<=game->size){
        game->board[*row][*col].value++;
    }
    if(game->board[*row][*col].value <= game->size){ /*checking why the upper while loop ended : valid or no valid value*/
        updateIndexForward(row,col,game);
    }
    else{ /* no valid values left */
        pop(stk);
        game->board[*row][*col].value = 0;
        updateIndexBack(row,col,game);
    }
}

int ** copyBoard(Cell ** board, int size){
    int i, j;
    int ** copy = (int **)malloc(size*sizeof(int*));
    for(i = 0; i<size; i++){
        copy[i] = (int *)malloc(size*sizeof(int));
    }
    for(i = 0; i<size; i++){
        for(j = 0; j<size; j++){
            copy[i][j] = board[i][j].value;
        }
    }
    return copy;
}

void freeBoardInt(int **board, int size){
    int i;
    for(i = 0; i<size; i++){
        free(board[i]);
    }
    free(board);
}

void copyBack(Game *game, int ** copy, int size){
    int i,j;
    for(i = 0; i<size; i++){
        for(j = 0; j<size; j++){
            game->board[i][j].value = copy[i][j];
        }
    }
}

int eBacktracking(Game *game){
    int counter = 0, row = 0, col = 0, flag; /*flag = 1 means that value was 0 before*/
    Stack *stk;
    int **copy;
    copy = copyBoard(game->board, game->size);
    initialize(stk);
    while(!(row == -1 && col == game->size -1)){
        if(row == game->size){
            counter++;
            row--; /* back to the right bottom */
            col = game->size-1;
            continue;
        }
        if(stk->size > row*game->size + col){ /*this condition says the were going bacwards*/
            flag = top(stk);
            if(flag){
                updateValue(game,&row, &col, stk);
            }
            else { /* if flag == 0, i.e this cell was filled before */
                pop(stk);
                updateIndexBack(&row,&col,game);
            }
        }
        else{ /*else going forward with the matrix*/
            if(game->board[row][col].value != 0){
                push(stk,0);
                updateIndexForward(&row,&col,game);
            }
            else{ /* the cell is 0 */
                push(stk,1);
                updateValue(game, &row, &col,stk);
            }
        }
    }
    copyBack(game, copy, game->size);
    freeBoardInt(copy, game->size);
    freeStk(stk);
    return counter;
}
