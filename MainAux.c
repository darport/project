#include <stdio.h>
#include "Game.h"

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
	int i;
	for(i = 0; i < game->size; i++){
		free(game->board[i]);
		free(game->solved[i]);
	}
	free(game->board);
	free(game->solved);
	freeList(game->ops);
	/*free(game); for unknown reason this line terminates the program */
}

int memoryError(){
	printf("memory allocation error\n");
	return 0;
}

int fileError(){
	printf("closing file error\n");
	return 0;
}

int erroneous(){
	printf("Error: board contains erroneous values\n");
	return 0;
}

int isErroneous(Game *game){
	int i,j;
	for(i=0; i<game->size; i++){
		for(int j=0; j<game->size; j++){
			if(game->board[i][j].marked){
				return 1; /* there is an erroneous value in the board */
			}
		}
	}
	return 0;
}

int validR(int option, int row,int col, Game *game, int markErroneus){
	int j;
	for(j = 0; j<game->size; j++){
		if(game->board[row][j].value == option){
			if(j == col){
				continue;
			}
			/* if the option exists in the row, it is not valid */
			if(markErroneus && !game->board[row][j].fixed){
				game->board[row][j].marked = 1;
			}
			return 0;
		}
	}
	return 1;
}

int validC(int option,int row, int col, Game *game, int markErroneous){
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

int validB(int option, int row, int col, Game *game, int markErroneous){
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
int isValid(Game *game, int i, int j, int op, int markErroneous){
	return (validR(op,i,j,game,markErroneous) && validC(op,i,j,game,markErroneous)
			&& validB(op,i,j,game,markErroneous));
}

int options(Game *game ,int i, int j){
	int op, count = 0, valToChange;
	for(op = 1; op<=game->size; op++){
		if(isValid(game,i,j,op)){
			count++;
			if(count > 1){
				return 0;
			}
			valToChange = op;
		}
	}
	if(count == 1){
		return valToChange;
	}
	return 0;
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

void setChanges(Game *game){
	int i,j, singleOp;
	for(i=0 ; i < game->size; i++){
		for(j=0; j < game->size; j++){
			if(game->board[i][j].needChange){
				game->board[i][j].value = game->board[i][j].valChange;
				game->board[i][j].needChange = 0;
			}
		}
	}
}
