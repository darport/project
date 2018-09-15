#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "Game.h"
#include "MainAux.h"

int printInvalid(){
	printf("ERROR: invalid command\n");
	return 0;
}

int isInt(char *num){
	int j = 0, numLen;
	numLen = (int)strlen(num);
	while(j < numLen ){
		if(!isdigit(num[j]))
			return -1;
		j++;
	}

	return atoi(num);
}

int callSolve(Game *game){
	char *fileName;
	fileName = strtok(NULL," \t\r\n"); /* getting the filename for the solve command */
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
		return printInvalid();
	}
	/* checking that the parameters are integers in the right range */
	digit = isInt(mark);
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
	check = checkParams(params, 3); /* checking that there are at least 3 parameters */
	if(check == 1){
		/* checking that the parameters are integers in the right range */
		x = isInt(params[0]);
		y = isInt(params[1]);
		z = isInt(params[2]);

		if(inRange(game, x) && inRange(game, y) && (inRange(game, z) || z == 0)){
			return set(game, y-1, x-1, z, 1, 0); /* i added the '-1' so it will suit our presentation of the board*/
		}
		printf("Error: value not in range 0-%d\n", game->colsInBlock*game->rowsInBlock);
		return 0;
	}
	return printInvalid();
}

int inRangeGen(Game *game, int num){
	if(num < 0 || num >(game->size*game->size)){
		return 0;
	}
	return 1;
}

int callGenerate (Game *game){
	int i = 0, x, y, check;
	char *params[2];
	while (i<2){ /* getting the parameters for generate command */
	   params[i] = strtok(NULL, " \t\r\n");
	   i++;
	}
	check = checkParams(params,2); /* checking that there are at least 2 parameters */
	if(check == 1){
		/* checking that the parameters are integers in the right range */
		x = isInt(params[0]);
		y = isInt(params[1]);
		if(inRangeGen(game,x)  && inRangeGen(game, y)) {
			if(isEmpty(game)) {
				return generate(game,x,y);
			}
			printf("Error: board is not empty\n");
			return 0;
		}
		printf("Error: value not in range 0-%d\n", game->size*game->size);
		return 0;
	}
	return printInvalid();
}

int callSave(Game *game){
	char *fileName;
	fileName = strtok(NULL, " \t\r\n"); /* getting the filename for the save command */
	if(fileName == NULL) {
		return printInvalid();
	}
	return save(game, fileName);
}

int callHint(Game *game){
	int i = 0, check, x, y;
	char *params[2];
	while (i<2){ /* getting the parameters for hint command */
	   params[i] = strtok(NULL, " \t\r\n");
	   i++;
	 }
	check = checkParams(params, 2); /* checking thar there are at least 2 parameters */
	if(check == 1) {
		/* checking that the parameters are integers in the right range */
		x = isInt(params[0]);
		y = isInt(params[1]);
		if(inRange(game, x) && inRange(game, y)){
			return hint(game, y - 1, x - 1);
		}
		printf("Error: value not in range 1-%d\n", game->colsInBlock*game->rowsInBlock);
		return 0;
	}
	return printInvalid();
}

int getCommand(Game *game){
	int i,charRead;
	static char cmd[257];
	char *commandType;
	char x;
	for(i = 0; i<257; i++){
		cmd[i] = '0';
	}
	/* checking EOF */
	if(fgets(cmd,258,stdin) == 0){
		return exitGame();
	}
	charRead = (int)strlen(cmd) ;
	/*checking the command length is not longer than 256 characters */
	if( charRead > 256){
		x = (char)getchar();
		while(x != '\n' && x != EOF){
			x = (char)getchar();
		}
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
		return exitGame();
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
			return undo(game,1,0);
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
			return callGenerate(game);
		}
	}
	return printInvalid();
}
