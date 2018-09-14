#include "Game.h"
#include "Parser.h"
#include <stdio.h>
#include <stdlib.h>
#include "MainAux.h"


/*check commit and pus */
int main(){


	Game *game;
	game = (Game*)malloc(sizeof(Game));
	game->board = NULL;
	game->solved = NULL;
	game->ops = (Link*)malloc(sizeof(Link));
	game->ops->head = (Node*)malloc(sizeof(Node));
	initializeOps(game);
	game->markErrors = 1;


	printf("Sudoku\n------\n");
	fflush(stdout);
	printf("Enter your command:\n");
	while(getCommand(game) != -1){
		printf("Enter your command:\n");
		fflush(stdout);
	}

	return 1;
}

