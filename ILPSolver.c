#include "gurobi_c.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Game.h"
#include "ErrorHandler.h"



void freeILP(double *sol, int *ind, int *ind2, double *val, double *val2, double *lb, char *vtype, GRBenv *env,
             GRBmodel *model) {
    free(sol);
    free(ind);
    free(ind2);
    free(val);
    free(val2);
    free(lb);
    free(vtype);
    GRBfreemodel(model);
    GRBfreeenv(env);
}

void createEmptyModel(Game *game, double *lb, char* vtype, int N){
    int i, j, v;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            for (v = 0; v < N; v++) {
                if (game->board[i][j].value == v + 1)
                    lb[i * N * N + j * N + v] = 1;
                else
                    lb[i * N * N + j * N + v] = 0;
                vtype[i * N * N + j * N + v] = GRB_BINARY;

            }
        }
    }
}
/*
int createEnvironment(double *sol, int *ind, int *ind2, double *val, double *val2, double *lb, char *vtype, GRBenv *env,
                    GRBmodel *model){
    int error;
    error = GRBloadenv(&env, "sudoku.log");
    if (error) {
        printf("ERROR: %s\n", GRBgeterrormsg(env));
        freeILP(sol, ind, ind2, val, val2, lb, vtype, env, model);
        return 0;
    }
    return 1;
}

int createNewModel(double *sol, int *ind, int *ind2, double *val, double *val2, double *lb, char *vtype, GRBenv *env,
                   GRBmodel *model, int N){
    int error;
    error = GRBnewmodel(env, &model, "sudoku", N * N * N, NULL, lb, NULL,
                        vtype, NULL);
    if (error) {
        printf("ERROR: %s\n", GRBgeterrormsg(env));
        freeILP(sol, ind, ind2, val, val2, lb, vtype, env, model);
        return 0;
    }
    return 1;
}

int firstCellConstrain(double *sol, int *ind, int *ind2, double *val, double *val2, double *lb, char *vtype, GRBenv *env,
                       GRBmodel *model, int N){
    int i,j,v,error;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            for (v = 0; v < N; v++) {
                ind[v] = i * N * N + j * N + v;
                val[v] = 1.0;
            }
            error = GRBaddconstr(model, N, ind, val, GRB_EQUAL, 1.0, NULL);
            if (error) {
                printf("ERROR %d GRBaddconstr1(): ALLFULL %s\n", error, GRBgeterrormsg(env));
                freeILP(sol, ind, ind2, val, val2, lb, vtype, env, model);
                return 0;
            }
        }
    }
    return 1;
}
int secondCellConstrain(Game *game, double *sol, int *ind, int *ind2, double *val, double *val2, double *lb, char *vtype, GRBenv *env,
                        GRBmodel *model, int N){
    int i,j,v,error;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if (game->board[i][j].value != 0) {
                ind2[0] = i * N * N + j * N + (game->board[i][j].value - 1);
                val2[0] = 1;
                error = GRBaddconstr(model, 1, ind2, val2, GRB_EQUAL, 1.0, NULL);
                if (error) {
                    printf("ERROR %d GRBaddconstr2():UNTOUCHED %s\n", error, GRBgeterrormsg(env));
                    freeILP(sol, ind, ind2, val, val2, lb, vtype, env, model);
                    return 0;
                }
            }
        }
    }
    return 1;
}

int rowConstrain(double *sol, int *ind, int *ind2, double *val, double *val2, double *lb, char *vtype, GRBenv *env,
                 GRBmodel *model, int N){
    int i,v,j,error;
    for (v = 0; v < N; v++) {
        for (j = 0; j < N; j++) {
            for (i = 0; i < N; i++) {
                ind[i] = i * N * N + j * N + v;
                val[i] = 1.0;
            }

            error = GRBaddconstr(model, N, ind, val, GRB_EQUAL, 1.0, NULL);
            if (error) {
                printf("ERROR %d GRBaddconstr3(): ROW  %s\n", error, GRBgeterrormsg(env));
                freeILP(sol, ind, ind2, val, val2, lb, vtype, env, model);
                return 0;
            }
        }
    }
    return 1;
}

int colConstrain(double *sol, int *ind, int *ind2, double *val, double *val2, double *lb, char *vtype, GRBenv *env,
                 GRBmodel *model, int N){
    int i,j,v,error;
    for (v = 0; v < N; v++) {
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                ind[j] = i * N * N + j * N + v;
                val[j] = 1.0;
            }

            error = GRBaddconstr(model, N, ind, val, GRB_EQUAL, 1.0, NULL);
            if (error) {
                printf("ERROR %d GRBaddconstr4(): COL %s\n", error, GRBgeterrormsg(env));
                freeILP(sol, ind, ind2, val, val2, lb, vtype, env, model);
                return 0;
            }
        }
    }
    return 1;
}
int blockConstrain(double *sol, int *ind, int *ind2, double *val, double *val2, double *lb, char *vtype, GRBenv *env,
                   GRBmodel *model, int N, int m, int n){
    int i,j,v,ig,jg,count,error;
    for (v = 0; v < N; v++) {
        defines what block are we at
        for (ig = 0; ig < n; ig++) {
             number of blocks vertically (rows)
            for (jg = 0; jg < m; jg++) {
                 number of block horizontally (cols)
                count = 0;
                iterates over the cells in that block
                for (i = ig * m; i < (ig + 1) * m; i++) {
                    for (j = jg * n; j < (jg + 1) * n; j++) {
                        ind[count] = i * N * N + j * N + v;
                        val[count] = 1.0;
                        count++;
                    }
                }
                error = GRBaddconstr(model, N, ind, val, GRB_EQUAL, 1.0, NULL);
                if (error) {
                    printf("ERROR %d GRBaddconstr5(): BLOCK  %s\n", error, GRBgeterrormsg(env));
                    freeILP(sol, ind, ind2, val, val2, lb, vtype, env, model);
                    return 0;
                }
            }
        }
    }
    return 1;
}
*/
void updateSolved(double *sol, Game *game) {
    int i, j, k, N = game->size;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            for (k = 0; k < N; k++) {
                if (sol[i * N * N + j * N + k] == 1) {
                    game->solved[i][j].value = (k + 1);
                }
            }
        }
    }
}


/* Solves the game with ILP */
int ILP(Game *game, int command) {

    int N, error, i, j, v, ig, jg, count, *ind, *ind2, optimstatus, result,m = game->rowsInBlock,n = game->colsInBlock;
    double *sol, *val, *val2, *lb, objval;
    char *vtype;
    GRBenv *env;
    GRBmodel *model = NULL;
    env = NULL;
    N = game->size;
    sol = (double *) malloc(N * N * N * sizeof(double));
    ind = (int *) malloc(N * sizeof(int));
    ind2 = (int *) malloc(N * sizeof(int));
    val = (double *) malloc(N * sizeof(double));
    val2 = (double *) malloc(N * sizeof(double));
    lb = (double *) malloc(N * N * N * sizeof(double));
    vtype = (char *) malloc(N * N * N * sizeof(char));

    if(sol == NULL || ind == NULL || ind2 == NULL || val == NULL || val2 == NULL || lb == NULL || vtype == NULL){
        return memoryError();
    }
    result = 0;

    /* Create an empty model */
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            for (v = 0; v < N; v++) {
                if (game->board[i][j].value == v + 1)
                    lb[i * N * N + j * N + v] = 1;
                else
                    lb[i * N * N + j * N + v] = 0;
                vtype[i * N * N + j * N + v] = GRB_BINARY;

            }
        }
    }

    /* Create environment */
    error = GRBloadenv(&env, "sudoku.log");
    if (error) {
        printf("ERROR: %s\n", GRBgeterrormsg(env));
        freeILP(sol, ind, ind2, val, val2, lb, vtype, env, model);
        return result;
    }

    GRBsetintparam(env, GRB_INT_PAR_LOGTOCONSOLE, 0); /* cancel prints */
    /* Create new model */


    error = GRBnewmodel(env, &model, "sudoku", N * N * N, NULL, lb, NULL,
                        vtype, NULL);
    if (error) {
        printf("ERROR: %s\n", GRBgeterrormsg(env));
        freeILP(sol, ind, ind2, val, val2, lb, vtype, env, model);
        return result;
    }



    /* Each cell gets a value */

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            for (v = 0; v < N; v++) {
                ind[v] = i * N * N + j * N + v;
                val[v] = 1.0;
            }
            error = GRBaddconstr(model, N, ind, val, GRB_EQUAL, 1.0, NULL);
            if (error) {
                printf("ERROR %d GRBaddconstr1(): ALLFULL %s\n", error, GRBgeterrormsg(env));
                freeILP(sol, ind, ind2, val, val2, lb, vtype, env, model);
                return result;
            }
        }
    }




    /* constrain that if cell had value != 0
     * it will hold the same value */

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if (game->board[i][j].value != 0) {
                ind2[0] = i * N * N + j * N + (game->board[i][j].value - 1);
                val2[0] = 1;
                error = GRBaddconstr(model, 1, ind2, val2, GRB_EQUAL, 1.0, NULL);
                if (error) {
                    printf("ERROR %d GRBaddconstr2():UNTOUCHED %s\n", error, GRBgeterrormsg(env));
                    freeILP(sol, ind, ind2, val, val2, lb, vtype, env, model);
                    return result;
                }
            }
        }
    }



    /* Each value must appear once in each row */

    for (v = 0; v < N; v++) {
        for (j = 0; j < N; j++) {
            for (i = 0; i < N; i++) {
                ind[i] = i * N * N + j * N + v;
                val[i] = 1.0;
            }

            error = GRBaddconstr(model, N, ind, val, GRB_EQUAL, 1.0, NULL);
            if (error) {
                printf("ERROR %d GRBaddconstr3(): ROW  %s\n", error, GRBgeterrormsg(env));
                freeILP(sol, ind, ind2, val, val2, lb, vtype, env, model);
                return result;
            }
        }
    }


    /* Each value must appear once in each column */

    for (v = 0; v < N; v++) {
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                ind[j] = i * N * N + j * N + v;
                val[j] = 1.0;
            }

            error = GRBaddconstr(model, N, ind, val, GRB_EQUAL, 1.0, NULL);
            if (error) {
                printf("ERROR %d GRBaddconstr4(): COL %s\n", error, GRBgeterrormsg(env));
                freeILP(sol, ind, ind2, val, val2, lb, vtype, env, model);
                return result;
            }
        }
    }


    /* Each value must appear once in each block */

    for (v = 0; v < N; v++) {
        /*defines what block are we at*/
        for (ig = 0; ig < n; ig++) {
             /*number of blocks vertically (rows)*/
            for (jg = 0; jg < m; jg++) {
                 /*number of block horizontally (cols)*/
                count = 0;
                /* iterates over the cells in that block*/
                for (i = ig * m; i < (ig + 1) * m; i++) {
                    for (j = jg * n; j < (jg + 1) * n; j++) {
                        ind[count] = i * N * N + j * N + v;
                        val[count] = 1.0;
                        count++;
                    }
                }
                error = GRBaddconstr(model, N, ind, val, GRB_EQUAL, 1.0, NULL);
                if (error) {
                    printf("ERROR %d GRBaddconstr5(): BLOCK  %s\n", error, GRBgeterrormsg(env));
                    freeILP(sol, ind, ind2, val, val2, lb, vtype, env, model);
                    return result;
                }
            }
        }
    }


    /* Optimize model */
    error = GRBoptimize(model);
    if (error) {
        printf("ERROR: %s\n", GRBgeterrormsg(env));
        freeILP(sol, ind, ind2, val, val2, lb, vtype, env, model);
        return result;
    }

    /* Write model to 'sudoku.lp' */
    error = GRBwrite(model, "sudoku.lp");
    if (error) {
        printf("ERROR: %s\n", GRBgeterrormsg(env));
        freeILP(sol, ind, ind2, val, val2, lb, vtype, env, model);
        return result;
    }

    /* Capture solution information */
    error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
    if (error) {
        printf("ERROR: %s\n", GRBgeterrormsg(env));
        freeILP(sol, ind, ind2, val, val2, lb, vtype, env, model);
        return result;
    }

    /* get the objective -- the optimal result of the function */
    error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL, &objval);
    if (error) {
        printf("ERROR: %s\n", GRBgeterrormsg(env));
        freeILP(sol, ind, ind2, val, val2, lb, vtype, env, model);
        return result;
    }

    /* get the solution - the assignment to each variable */
    error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, N * N * N, sol);
    if (error) {
        printf("ERROR: %s\n", GRBgeterrormsg(env));
        freeILP(sol, ind, ind2, val, val2, lb, vtype, env, model);
        return result;
    }

    /* board is solved */
    if (optimstatus == GRB_OPTIMAL) {
        if (command == 1) {
            updateSolved(sol, game);
        }
        result = 1;
    }

    freeILP(sol, ind, ind2, val, val2, lb, vtype, env, model);

    return result;
}
