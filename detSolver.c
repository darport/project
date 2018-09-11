#include "Game.h"
#include "MainAux.h"
#include "Solver.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>


int recval(int x, int y,int size, Game *game){
	int op;
		if(x == size && y==0){ /* if it is the last cell in the board */
			return 1;
		}
		if(game->board[x][y].value !=0){
			if(y< size -1){
				return recval(x,y+1,size,game);
			}
			return recval(x+1,0,size,game);
		}
		op = game->board[x][y].value;
		while(op<size){
			op++;
			if(isValid(game,x,y,op,0)){ /* checking of op is valid */
				game->board[x][y].value = op;
				if(y< size - 1){
					if (recval(x,y+1,size,game) == 1){
						return 1;
					}
					continue;
				}
				if (recval(x+1,0,size,game)==1){
					return 1;
				}
			} /* if op is not valid, moving to the next iteration */
		}
		game->board[x][y].value = 0; /* if there are no valid options for the cell */
		return 0;
}
int validateChange(Game *game){
	int flag, x=0,y=0;
	flag =  recval(x,y,game->size,game);
	return flag;
}


int recDet(int x, int y, int ** cpy,int size,Game *game){
	int op;

	if(x== size && y==0){ /* if it is the last cell in the board */
		return 1;
	}
	if(cpy[x][y] !=0){
		if(y< size -1){
			return recDet(x,y+1,cpy,size,game);
		}
		return recDet(x+1,0,cpy,size,game);
	}
	op = cpy[x][y];
	while(op<size){
		op++;
		if(isValid(game,x,y,op,0)){ /* checking of op is valid */
			cpy[x][y] = op;
			if(y< size - 1){
				if (recDet(x,y+1,cpy,size,game) == 1){
					return 1;
				}
				continue;
			}
			if (recDet(x+1,0,cpy,size,game)==1){
				return 1;
			}
		} /* if op is not valid, moving to the next iteration */
	}
	cpy[x][y] = 0; /* if there are no valid options for the cell */
	return 0;
}

int detBacktracking(Game *game){
	int x = 0, y = 0,flag;
	int ** cpy = copyBoard(game->board, game->size);
	flag =  recDet(x,y,cpy,game->size,game);
	freeBoardInt(cpy, game->size);
	return flag;
}


