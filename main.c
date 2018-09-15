#include "Game.h"
#include "Parser.h"
#include <stdio.h>
#include <stdlib.h>
#include "Game.h"
#include "Parser.h"
#include "ErrorHandler.h"
#include "MainAux.h"
#include <time.h>

int main(){
	Game *game;
	srand((unsigned int)time(NULL));
	game = (Game*)malloc(sizeof(Game));
	if(game == NULL){
		return memoryError();
	}
	game->board = NULL;
	game->solved = NULL;
	game->ops = (Link*)malloc(sizeof(Link));
	if(game->ops == NULL){
		return memoryError();
	}
	game->ops->head = (Node*)malloc(sizeof(Node));
	if(game->ops->head == NULL){
		return memoryError();
	}
	initializeOps(game);
	game->markErrors = 1;

	printf("Sudoku\n------\n");
	printf("Enter your command:\n");
	while(getCommand(game) != -1){
		printf("Enter your command:\n");
	}

	freeGame(game);
	return 1;
}

