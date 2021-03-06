#include <stdlib.h>
#include <stdio.h>
#include "Game.h"
#include "LinkedList.h"
#include "ErrorHandler.h"

int isEmpty(Game *game){
	int i,j, len = game->size;
	for(i=0 ; i <len; i++){
		for(j = 0; j < len; j++){
			if(game->board[i][j].value != 0){
				return 0;
			}
		}
	}
	return 1;
}

void freeBoard(Cell **temp,int size){
	int i;
	for(i = 0; i < size; i++){
		free(temp[i]);
	}
	free(temp);
}

void initCell(Game *game, int i, int j){
	game->board[i][j].value = 0;
	game->board[i][j].fixed = 0;
	game->board[i][j].marked = 0;
	game->board[i][j].needChange = 0;
	game->board[i][j].valChange = 0;
	game->solved[i][j].value = 0;
	game->solved[i][j].fixed = 0;
	game->solved[i][j].marked = 0;
	game->solved[i][j].needChange = 0;
	game->solved[i][j].valChange = 0;
}

void initNode(Node *head){
    head->x = 0;
    head->y = 0;
    head->currZ = 0;
    head->prevZ = 0;
    head->next = NULL;
    head->prev = NULL;
}


void initializeOps(Game *game){
	game->ops->next = NULL;
	game->ops->prev = NULL;
	initNode(game->ops->head);
}

void cleanBoard(Game * game){
	int i, j;
	for(i = 0; i<game->size; i++){
		for(j =0; j< game->size; j++){
			game->board[i][j].fixed = 0;
			game->board[i][j].marked = 0;
			game->board[i][j].value = 0;
		}
	}
}

void freeGame(Game *game){
	if(game->board != NULL){
		freeBoard(game->board,game->size);
	}
	if(game->solved != NULL){
		freeBoard(game->solved,game->size);
	}

	if(game->ops != NULL){
        while(game->ops->prev != NULL){
        	game->ops = game->ops->prev;
        }
		freeList(game->ops);
		game->ops = NULL;
	}
	if(game != NULL){
	    free(game);
	}
}

int isErroneous(Game *game){
	int i,j;
	for(i=0; i<game->size; i++){
		for(j=0; j<game->size; j++){
			if(game->board[i][j].marked){
				return 1; /* there is an erroneous value in the board */
			}
		}
	}
	return 0;
}

int validR(Game *game, int row, int col, int option, int markErroneous){
	int j;
	for(j = 0; j<game->size; j++){
		if(game->board[row][j].value == option){
			if(j == col){
				continue;
			}
			/* if the option exists in the row, it is not valid */
			if(markErroneous && !game->board[row][j].fixed){
				game->board[row][j].marked = 1;
			}
			return 0;
		}
	}
	return 1;
}

int validC(Game *game, int row, int col, int option, int markErroneous) {
	int i;
	for(i = 0; i<game->size; i++){
		if(game->board[i][col].value == option){
			if(i == row){
				continue;
			}
			/* if the option exists in the column, it is not valid */
			if(markErroneous && !game->board[i][col].fixed){
				game->board[i][col].marked = 1;
			}
			return 0;
		}
	}
	return 1;
}

int validB(Game *game, int row, int col, int option, int markErroneous){
	int startRow = (row/game->rowsInBlock)*game->rowsInBlock;
	int startCol = (col/game->colsInBlock)*game->colsInBlock;
	int endRow = startRow + game->rowsInBlock;
	int endCol = startCol + game->colsInBlock;
	int i, j;
	for(i=startRow; i<endRow; i++){
		for(j=startCol; j<endCol;j++){
			if(game->board[i][j].value == option){
				if(i == row && j == col){
					continue;
				}
				/* if the option exists in the block, it is not valid */
				if(markErroneous && !game->board[i][j].fixed){
					game->board[i][j].marked = 1;
				}
				return 0;
			}
		}
	}
	return 1;
}
int isValid(Game *game, int row, int col, int option, int markErroneous){
	return (validR(game, row, col, option, markErroneous) && validC(game, row, col, option, markErroneous)
			&& validB (game, row, col, option, markErroneous));
}

void updateErroneous(Game *game){
	int i,j,value, size=game->size;
	for(i=0; i<size; i++){
		for(j=0; j<size; j++){
			value = game->board[i][j].value;
			if(value == 0 || isValid(game,i,j,value,0)){
				game->board[i][j].marked = 0;
			}
			else{
				game->board[i][j].marked = 1;
			}
		}
	}
}

int options(Game *game ,int i, int j){
	int op, count = 0, valToChange = 0;
	for(op = 1; op<=game->size; op++){
		if(isValid(game,i,j,op,0)){
			count++;
			if(count > 1){
				return 0;
			}
			valToChange = op;
		}
	}
    /* if count == 1, valToChange !=0 and if count == 0 valToChange = 0. if count > 1, 0 was returned inside the loop */
    return valToChange;
}

void markChanges(Game *game){
	int i,j, singleOp;
	for(i=0 ; i < game->size; i++){
		for(j=0; j < game->size; j++){
			if(game->board[i][j].value != 0){
				continue;
			}
			singleOp = options(game,i,j);
			if(singleOp > 0){
				game->board[i][j].needChange = 1;
				game->board[i][j].valChange = singleOp;
			}
			else{
				game->board[i][j].needChange = 0;
			}
		}
	}
}

int setChanges(Game *game){
	int i,j, flag = 0;
	Link *temp;
	Link *new = (Link*)malloc(sizeof(Link));
	if(new == NULL){
		return memoryError();
	}
	new->head = NULL;
	new->next = NULL;
	new->prev = game->ops;
	game->ops->next = new;
	game->ops = new;
	for(i=0 ; i < game->size; i++){
		for(j=0; j < game->size; j++){
			if(game->board[i][j].needChange){
				flag = 1;
				set(game,i,j,game->board[i][j].valChange,0,1);
				game->board[i][j].value = game->board[i][j].valChange;
				game->board[i][j].needChange = 0;
			}
		}
	}
	if(flag == 0){
		temp = game->ops;
		game->ops = game->ops->prev;
		game->ops->next = NULL;
		free(temp);
	}
	return 1;
}
