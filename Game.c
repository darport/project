#include <stdio.h>
#include <stdlib.h>
#include "Game.h"
#include "Solver.h"
#include "mainAux.h"
#include "ILPSolver.h"

int isFull(Game *game){
    int i,j;

    for(i = 0; i<game->size; i++){
        for(j = 0; j< game->size; j++){
            if(game->board[i][j].value == 0){
                return 0;
            }
        }
    }
    return 1;
}

void setData(Game *game, char *data, int i, int j){
    int value = 0;
    game->board[i][j].fixed = 0;
    game->board[i][j].marked = 0;
    game->board[i][j].needChange = 0;
    game->board[i][j].valChange = 0;
    while(*data != '\0'){
        if(*data == '.'){ /* checking only . and not * because no erroneous values are allowed here */
            game->board[i][j].fixed = 1;
            break;
        }
        value *= 10;
        value += (*data - '0'); /*we did it incorrectly before, forgot to decrease '0'*/
        data++;
    }
    game->board[i][j].value = value;
}

int openFile(Game *game, char *fileName){ /*instead of initializing cell** and then assigning, i immediately assigned to game->board and solved,
 	 	 	 	 	 	 	 	 	 	 	 data * changed to data[2]*/
    int i,j,m,n,size;
    char data[3];
    FILE *fp = fopen(fileName, "r");
    if(fp == NULL){
        printf("Error: File doesn't exist or cannot be opened\n");
        return 0;
    }
    fscanf(fp,"%d", &m);
    fscanf(fp,"%d", &n);
    game->colsInBlock = n;
    game->rowsInBlock = m;
    game->size = m*n;
    size = game->size;
    game->board = (Cell **)malloc(size*sizeof(Cell*));
    game->solved = (Cell **)malloc(size*sizeof(Cell*));

    if(game->board == NULL){
        return memoryError(); /* check if we need to close the file */
    }
    if(game->solved == NULL){
    	return memoryError(); /* check if we need to close the file */
    }
    for(i=0; i<size; i++){
        game->board[i] = (Cell *)malloc(size*sizeof(Cell));
        if(game->board[i] == NULL){
            return memoryError();
        }
    }
    for(i=0; i<size; i++){
        game->solved[i] = (Cell *)malloc(game->size*sizeof(Cell));
        if(game->solved[i] == NULL){
            return memoryError();
        }
    }

    for(i = 0; i<size; i++){
        for(j = 0; j<size; j++){
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
            fscanf(fp,"%s",data);
            setData(game,data,i,j);
        }
    }

    fclose(fp);
    return 1;
}

int openEmpty(Game *game){    /*instead of initializing cell** and then assigning, i immediately assigned to game->board and solved*/
    int i,j;
    game->board = (Cell **)malloc(game->size*sizeof(Cell*));
    if(game->board == NULL){
        return memoryError();
    }
    for(i=0; i<game->size; i++){
        game->board[i] = (Cell *)malloc(game->size*sizeof(Cell));
        if(game->board == NULL){
            return memoryError();
        }
    }

    game->solved = (Cell **)malloc(game->size*sizeof(Cell*));
    if(game->solved == NULL){
        return memoryError();
    }
    for(i=0; i<game->size; i++){
        game->solved[i] = (Cell *)malloc(game->size*sizeof(Cell));
        if(game->solved[i] == NULL){
            return memoryError();
        }
    }
    /* check if we need to initialize the fields to 0 */
    for(i = 0; i < game->size; i++){
        for(j = 0; j< game->size; j++){
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
    }
    return 1;
}

int solve(Game *game, char *fileName){
	int success;
	success = openFile(game,fileName);
	if(success == 1){
		game->mode = 1;
	}
    return success;
}

int edit(Game *game, char *x){
	int success;
	if(x == NULL){
		game->mode = 2;
		game->size = 9;
		game->colsInBlock = 3;
		game->rowsInBlock = 3;
		return openEmpty(game);
	}
	success = openFile(game,x);
	if(success == 1){
		game->mode = 2;
	}
    return success;
}

int markErrors(Game *game, int mark){
    game->markErrors = mark;
    return 1;
}

void printSep(int sepLen){
    int i;
    for(i = 0; i<sepLen; i++){
        printf("-");
    }
}

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
                if(game->markErrors || game->mode == 2)
                {
                    if(game->board[i][j].marked == 1){
                        printf("*");
                    }
                    else{
                        /*if( j!= game->size - 1){ why did we put this if statment*/
                            printf(" ");
                        /*}*/
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
    printf("\n");
    return 1;
}

int validate(Game *game){
    int valid;
    if(isErroneous(game)){
        printf("Error: board contains erroneous values\n");
        return 0;
    }
    valid = 0;
   /* valid = ILP(game);*/
    if(valid){
        printf("Validation passed: board is solvable\n");
    }
    else{
        printf("Validation failed: board is unsolvable");
        return 0;
    }
    return 1;
}




int set(Game *game, int x, int y, int z,int show,int type){
    /*Node *temp;*/
	Link *temp;
    int mark;
    temp = (Link *)malloc(sizeof(Link));
    if(temp == NULL){
    	return memoryError();
    }
    /* checking if the cell is fixed */
    if(game->board[x][y].fixed){
        printf("Error: cell is fixed\n");
        return 0;
    }
    if(game->board[x][y].value == z){
    	return 0;
    }
    /* clearing any move beyond the current move from the undo-redo list */

   	if(game->ops->next != NULL){
   		temp = game->ops->next;
   	}
   	else{
   		temp = NULL;
  	}

   	if(temp != NULL){
   		freeList(temp);
   	}
   	/*adding a new link for set operation or a new node for autofill operation*/
   	if(!type){
   		addLink(&game->ops, x, y, z, game->board[x][y].value,type);
   	}
   	else{
   		addNode(&game->ops->head, x, y, z, game->board[x][y].value,type);
   	}


    /* if the value z is erroneous, mark will be 1 and all the non fixed cells that b
     * become erroneous because of z will be marked by isValid */
    mark = !isValid(game,x,y,z,1);
    game->board[x][y].value = z;
    game->board[x][y].marked = mark;
    /* updating erroneous cells and printing the board */
    updateErroneous(game);
    if(show){
    	printBoard(game);
    }
    if(game->mode == 1){
        /* if the board is full, validating the board */
        if(isFull(game)){
        	if(!show){
        		printBoard(game);
        	}
            if(validate(game)){
                printf("Puzzle solved successfully\n");
                game->mode = 0;
            }
            else{
                printf("Puzzle solution erroneous\n");
            }
        }
    }
    return 1;
}


int existValue(Game *game, int row, int col){
    int i;
    for(i =0; i<game->size; i++){
        if(isValid(game,row,col,i,0)){
            return 1;
        }
    }
    return 0;
}

int generateHelp(Game *game, int x){
    int i,row,col, z;
    /* generating puzzle with randomly filling x cells with legal values */
    for(i = 0; i<x; i++){
        row = rand() % game->size;
        col = rand() % game->size;
        while(game->board[row][col].value == 0){
            row = rand() % game->size;
            col = rand() % game->size;
        }
        z = rand() % game->size + 1;
        if(!existValue(game,row,col)){
            return 0;
        }
        while(!isValid(game,row,col,z,0)){
            z = rand() % game->size + 1;
        }
        game->board[row][col].value = z;
    }
    /*
    if(ILP(game) != 1){
        return 0;
    }*/
    return 1;
}

int generate(Game *game, int x, int y){
    int i, row, col;
    cleanBoard(game); /* check */
    for(i = 0; i<1000; i++){
        if(generateHelp(game,x)){
            break;
        }
        cleanBoard(game);
    }
    if(i == 1000){
        printf("Error: puzzle generator failed\n");
        return 0;
    }
    for(i = 0; i<y; i++){
        row = rand() % game->size;
        col = rand() % game->size;
        while(game->board[row][col].fixed){
            row = rand() % game->size;
            col = rand() % game->size;
        }
        game->board[row][col].marked = 1; /* we use the marked field out of context,
		just to mark cells that have been chosen to be filled */
    }
    for(row = 0; row<game->size; row++){
        for(col =0; col< game->size; col++){
            if(!game->board[row][col].marked){
                game->board[row][col].value = 0;
            }
            else{ /* the cell is marked, we need to set it back to 0 */
                game->board[row][col].marked = 0;
            }
        }
    }
    return 1;
}

int undoHelp(Game *game){
	int x, y, z,curr,flag = 1;
	x = game->ops->head->x;
   	y = game->ops->head->y;
   	z = game->ops->head->prevZ;
   	game->board[x][y].value = z;
   	curr = game->ops->head->currZ;
   	if(game->ops->head->prev != NULL){
   		game->ops->head = game->ops->head->prev;
   	}
   	else{
   		flag = 0;
   	}
   	updateErroneous(game);
   	/*curr = game->ops->currZ; moved up 8 lines*/
   	if(curr != 0 && z != 0){
   		printf("Undo %d,%d: from %d to %d\n", x + 1, y + 1,curr, z);
	}
   	else if(curr == 0 && z!= 0){
   		printf("Undo %d,%d: from _to %d\n", x + 1, y + 1,z);
   	}
   	else if(curr!=0){ /* in this case z == 0 because the first condition was curr != 0 && z != 0 */
   		printf("Undo %d,%d: from %d to _\n", x + 1, y + 1,curr);
   	}
   	return flag;
}


int undo(Game *game, int show){
    /*int x, y, z,curr,type,typePrev = 0;*/
    /*
    if(game->ops == NULL){
    	printf("Error: no moves to undo\n");
    	return 0;
    }
    */
    if(game->ops->prev == NULL){
    	printf("Error: no moves to undo\n");
    	return 0;
    }

    while(undoHelp(game)){}
    game->ops = game->ops->prev;

    if(show){printBoard(game);}
    return 1;
   /*
    do{
    	type = game->ops->type;
    	if(type == 1){
    		typePrev = game->ops->prev->type;
    	}
    	x = game->ops->x;
    	y = game->ops->y;
    	z = game->ops->prevZ;
    	game->board[x][y].value = z;
    	curr = game->ops->currZ;
    	game->ops = game->ops->prev;
    	updateErroneous(game);
    	/*curr = game->ops->currZ; moved up 8 lines*/
    	/*if(curr != 0 && z != 0){
    		printf("Undo %d,%d: from %d to %d\n", x + 1, y + 1,curr, z);
    	}
    	else if(curr == 0 && z!= 0){
    		printf("Undo %d,%d: from _to %d\n", x + 1, y + 1,z);
       		}
        	else if(curr!=0){ /* in this case z == 0 because the first condition was curr != 0 && z != 0
        		printf("Undo %d,%d: from %d to _\n", x + 1, y + 1,curr);
        		}
        /*else both 0 check */
   /* }while(type && typePrev);
  	printBoard(game);
    return 1;*/

}
int redoHelp(Game *game){
	int x,y,z, prev, flag = 1;
	/*game->ops->head = game->ops->head->next;*/
	x = game->ops->head->x;
	y = game->ops->head->y;
	z = game->ops->head->currZ;
	game->board[x][y].value = z;
	prev = game->ops->head->prevZ;
	updateErroneous(game);
	if(prev != 0 && z != 0){
		printf(	"Redo %d,%d: from %d to %d\n", x + 1, y + 1,prev, z);
	}
	else if(prev == 0 && z!= 0){
		printf("Redo %d,%d: from _to %d\n", x + 1, y + 1,z);
	}
	else if(prev != 0){  /*in this case z == 0 because the first condition was curr != 0 && z != 0*/
		printf("Redo %d,%d: from %d to _\n", x + 1, y + 1,prev);
	}
	if(game->ops->head->next != NULL){
		game->ops->head = game->ops->head->next;
	}
	else{flag = 0;}
	return flag;
}


int redo(Game *game){
    if(game->ops->next == NULL){
    	printf("Error: no moves to redo\n");
    	return 0;
    }

    game->ops = game->ops->next;
    while(redoHelp(game)){}

    printBoard(game);
    return 1;


    /*do{
    	if(game->ops->next == NULL){
    		break;
    	}
    	game->ops = game->ops->next;
    	type = game->ops->type;

    	x = game->ops->x;
    	y = game->ops->y;
    	z = game->ops->currZ;
    	game->board[x][y].value = z;
    	prev = game->ops->prevZ;
    	updateErroneous(game);
    	if(prev != 0 && z != 0){
    		printf(""
    				""
    				"Redo %d,%d: from %d to %d\n", x + 1, y + 1,prev, z);
    	}
    	else if(prev == 0 && z!= 0){
    		printf("Redo %d,%d: from _to %d\n", x + 1, y + 1,z);
    	}
    	else if(prev != 0){  in this case z == 0 because the first condition was curr != 0 && z != 0
    		printf("Redo %d,%d: from %d to _\n", x + 1, y + 1,prev);
    	}
    	if(game->ops->next != NULL){
    		typeNext = game->ops->next->type;
    	}
    }while(type && typeNext); */
    /*else both 0 check
    printBoard(game);
    return 1; */
}



int saveToFile(Game *game,char *fileName){
    FILE *fp = fopen(fileName, "w");
    int i,j;
    if(fp == NULL){
        printf("Error: File cannot be created or modified\n");
        return 0;
    }
    fprintf(fp,"%d %d\n", game->rowsInBlock, game->colsInBlock);
    for(i=0; i<game->size; i++){
        for(j=0; j<game->size; j++){
            if(game->board[i][j].fixed || game->mode == 2){
                fprintf(fp,"%d. ", game->board[i][j].value);
            }
            else{
                fprintf(fp,"%d ", game->board[i][j].value);
            }
        }
        fprintf(fp,"\n");
    }
    printf("Saved to: %s\n", fileName);
    if(fclose(fp) == EOF){
        fileError();
    }
    return 1;
}

int save(Game *game, char *fileName){
    int check;
    if(game->mode == 2){
        if(isErroneous(game)){
            return erroneous();
        }
        else{
            check = validate(game);
            if(!check){
                printf("Error: board validation failed\n");
                return 0;
            }
        }
    }
    return saveToFile(game,fileName);
}

int hint(Game *game, int x, int y){
    if(isErroneous(game)){
        printf("Error: board contains erroneous values\n");
        return 0;
    }
    if(game->board[x][y].fixed){
        printf("Error: cell is fixed\n");
        return 0;
    }
    if(game->board[x][y].value != 0){ /* the cell is not fixed and its value != 0, i.e cell is not empty */
        printf("Error: cell already contains a value\n");
        return 0;
    }
    /*if(ILP(game) != 1){
        printf("Error: board is unsolvable\n");
        return 0;
    }*/
    printf("Hint: set cell to %d\n", game->solved[x][y].value); /* the ILP function updates the solved board */
    return 1;
}

int numSolutions(Game *game){
    int numSol;
    if(isErroneous(game)){
        return erroneous();
    }
    numSol = eBacktracking(game);
    printf("Number of solutions: %d", numSol);
    if(numSol == 1){
        printf("This is a good board!\n");
    }
    else if(numSol > 1){
        printf("The puzzle has more than 1 solution, try to edit it further\n");
    }
    return 1;
}

int autofill(Game *game){
    if(isErroneous(game)){
        return erroneous();
    }
    markChanges(game);
    setChanges(game);
    if(!isFull(game)){
    	printBoard(game);
    }
    return 1;
}

int reset(Game *game){
	Link *temp;
    while(undo(game,0)){}
    if(game->ops->next != NULL){
    	temp = game->ops->next;
    }
    else{
    	temp = NULL;
    }

    if(temp != NULL){
    	freeList(temp);
    }
    game->ops->next = NULL;
    printf("Board reset\n");
    return 1;
}

int exitGame(Game *game){
    printf("Exiting...\n");
    freeGame(game);
    return -1;
}

