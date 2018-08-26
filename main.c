#include "Game.h"
#include "Parser.h"
#include <stdio.h>
#include <stdlib.h>


/*check commit and pus */
int main(){
	Game *game;
	int i; /*,j,size;*/
	game = (Game *)malloc(sizeof(game));
	game->markErrors = 1;
	/*
	 *  ****initialization moved to the open files methodes****
	 *
	game->rowsInBlock = 4;
	game->colsInBlock = 5;
	game->size = 20;
	size = game->size;
	game->board = (Cell **)malloc(game->size*sizeof(Cell*));
	game->solved = (Cell **)malloc(game->size*sizeof(Cell*));
	for(i=0; i<game->size; i++){
		game->board[i] = (Cell *)malloc(game->size*sizeof(Cell));
		game->solved[i] = (Cell *)malloc(game->size*sizeof(Cell));

	}
	for(i=0; i<size; i++){
		for(j=0; j<size; j++){
			game->board[i][j].value = 1;
			game->board[i][j].fixed = 1;
			game->board[i][j].marked = 0;
		}
	}
	*/
	while(getCommand(game) != 0){
		printBoard(game);
		printf("comand done \n");
		fflush(stdout);
	}
	/*printBoard(game);
	printf("the value has changed ?? %d", game->markErrors);*/
	if(game->board == NULL){
		return 1;
	}
	for(i = 0; i < game->size; i++){
		free(game->board[i]);
		free(game->solved[i]);
	}
	free(game->board);
	free(game->solved);
	return 1;
}

