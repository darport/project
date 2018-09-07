#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "Game.h"

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

int printInvalid(){
	printf("ERROR: invalid command\n");
	return 0;
}

int isInt(char *num){
	int j = 0, numLen;
	numLen = (int)strlen(num) - 1;
	while(j<numLen ){
		if(!isdigit(num[j]))
			return -1;
		j++;
	}
	return atoi(num);
}

int callSolve(Game *game){
	char *fileName;
	fileName = strtok(NULL," \t\r\n");
	if(fileName == NULL){
		return printInvalid();
	}
	return solve(game, fileName);
}

int callEdit(Game *game){
	char *fileName;
	fileName = strtok(NULL," \t\r\n");
	return edit(game, fileName);
}

int callMarkErrors(Game *game){
	char *mark;
	int digit;
	mark = strtok(NULL, " \t\r\n");
	if(mark == NULL){ /* no number was entered after "mark_errors" */
		return printInvalid(); /* check the error */
	}
	digit = isInt(mark); /* returns -1 if digit is not an integer */
	if(digit == 0 || digit == 1){
		return markErrors(game, digit);
	}
	printf("Error: the value should be 0 or 1\n");
	return 0;
}
int inRange(Game *game, int num){
	if(num < 1 || num >(game->rowsInBlock * game->colsInBlock)){
		return 0;
	}
	return 1;
}

int checkParams(char **params, int len){
	int i;
	for(i = 0; i < len; i++){
		if(params[i] == NULL){
			return 0;
		}
	}
	return 1;
}

int callSet (Game *game){
	int i = 0, check, x, y, z;
	char *params[3];
	while (i<3){ /* getting the parameters for set command */
	   params[i] = strtok(NULL, " \t\r\n");
	   i++;
	 }
	check = checkParams(params, 3);
	if(check == 1){
		x = isInt(params[0]);
		y = isInt(params[1]);
		z = isInt(params[2]);
		if(inRange(game, x) && inRange(game, y) && (inRange(game, z) || z == 0)){
			return set(game, x-1, y-1, z, 1, 0); /* i added the '-1' so it will suit our presentation of the board*/
		}
		printf("Error: value not in range 0-%d\n", game->colsInBlock*game->rowsInBlock);
		return 0;
	}
	return printInvalid();
}
int callGenerate (Game *game){
	int i = 0, x, y, check;
	char *params[2];
	while (i<2){ /* getting the parameters for set command */
	   params[i] = strtok(NULL, " \t\r\n");
	   i++;
	}
	check = checkParams(params,2);
	if(check == 1){
		x = isInt(params[0]);
		y = isInt(params[1]);
		if(inRange(game, x) && inRange(game, y)){
			return generate(game, x, y);
		}
		printf("Error: value not in range 0-%d\n", game->colsInBlock*game->rowsInBlock);
		return 0;
	}
	return printInvalid();
}

int callSave(Game *game){
	char *fileName;
	fileName = strtok(NULL, " \t\r\n");
	if(fileName == NULL){
		return printInvalid();
	}
	return save(game, fileName);
}

int callHint(Game *game){
	int i = 0, check, x, y;
	char *params[2];
	while (i<2){ /* getting the parameters for set command */
	   params[i] = strtok(NULL, " \t\r\n");
	   i++;
	 }
	check = checkParams(params, 2);
	if(check == 1){
		x = isInt(params[0]);
		y = isInt(params[1]);
		if(inRange(game, x) && inRange(game, y)){
			return hint(game, x, y);
		}
		printf("Error: value not in range 1-%d\n", game->colsInBlock*game->rowsInBlock);
		return 0;
	}
	return printInvalid();
}

int getCommand(Game *game){
	int charRead;
	static char cmd[257];
	char *commandType;
	charRead = strlen(fgets(cmd,258,stdin)) -1 ;/*check the numbes*/
	if( charRead > 256){
		return printInvalid();
	}
	commandType = strtok(cmd," \t\r\n");
	if(commandType == 0){ /* if no command was entered, returning to the main */
		return 0;
	}
	/* available in all modes */
	if(strcmp(commandType,"solve") == 0){
		return callSolve(game);
	}
	if(strcmp(commandType,"edit") == 0){
		return callEdit(game);
	}
	if(strcmp(commandType,"exit") == 0){
		return exitGame(game);
	}

	/* available in solve and edit mode */
	if(game->mode == 1 || game->mode == 2){
		if(strcmp(commandType,"validate") ==0){
			return validate(game);
		}
		if(strcmp(commandType,"set") == 0){
			return callSet(game);
		}
		if(strcmp(commandType,"print_board") == 0){
			return printBoard(game);
		}
		if(strcmp(commandType,"undo") == 0){
			return undo(game,1);
		}
		if(strcmp(commandType,"redo") == 0){
			return redo(game);
		}
		if(strcmp(commandType,"save") == 0){
			return callSave(game);
		}
		if(strcmp(commandType,"num_solutions") == 0){
			return numSolutions(game);
		}
		if(strcmp(commandType,"reset") == 0){
			return reset(game);
		}
	}

	/* available in solve mode */
	if(game->mode == 1){
		if(strcmp(commandType,"mark_errors") ==0){
			return callMarkErrors(game);
		}
		if(strcmp(commandType,"hint") == 0){
			return callHint(game);
		}
		if(strcmp(commandType,"autofill") == 0){
			return autofill(game);
		}
	}

	/* available in edit mode */
	if(game->mode == 2){
		if(strcmp(commandType,"generate") ==0){
			if(isEmpty(game)){
				return callGenerate(game);
			}
			printf("Error: board is not empty\n");
			return 0;
		}
	}
	return printInvalid();
}
