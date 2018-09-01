#include <stdio.h>
#include <stdlib.h>
#include "Game.h"
#include "MainAux.h"

int printBoard(Game *game){
    int sepLen = 4*game->size + game->rowsInBlock + 1, i, j, val;
    for(i = 0; i<game->size; i++){
        if(i%(game->rowsInBlock) == 0){
            printSep(sepLen);
            printf("\n");
        }
        for(j = 0; j < game->size; j++){
            if(j%(game->colsInBlock) == 0){
                printf("|");
            }
            printf(" ");
            val = game->board[i][j].value;
            if(val != 0){
                printf("%2d", game->board[i][j].value);
            }
            else{
                printf("  ");
            }
            if(game->board[i][j].fixed == 1){
                printf(".");
            }
            else{
                if(game->markErrors ||game->mode == 2)
                {
                    if(game->board[i][j].marked == 1){
                        printf("*");
                    }
                    else{
                        if( j!= game->size - 1){
                            printf(" ");
                        }
                    }
                }
                else{
                    if( j!= game->size - 1){
                        printf(" ");
                    }
                }

            }
            if(j == game->size - 1){
                printf("|");
                printf("\n");
            }
        }
    }
    printSep(sepLen);
    return 1;
}

int main(){
    Game *game;
    Cell **board;
    int i,j,size;

    game = (Game *)malloc(sizeof(game));
    game->rowsInBlock = 4;
    game->colsInBlock = 5;
    game->size = 20;
    size = game->size;
    board = (Cell **)malloc(game->size*sizeof(Cell*));
    if(board == NULL){
        return memoryError(); /* check if we need to close the file */
    }
    for(i=0; i<game->size; i++){
        board[i] = (Cell *)malloc(game->size*sizeof(Cell));
        if(board[i] == NULL){
            return memoryError();
        }
    }

    for(i=0; i<size; i++){
        for(j=0; j<size; j++){
            game->board[i][j].value = 1;
        }
    }

    game->board = board;
    printBoard(game);
    freeGame(game);
    return 1;
}
