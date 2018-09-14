

#include "gurobi_c.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "MainAux.h"


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


/* Solves sudoku using ILP
 * res will hold the solved board values */
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

    /* Each value must appear once in each subgrid */
    for (v = 0; v < N; v++) {
        /*defines what block are we at*/
        for (ig = 0; ig < n; ig++) {
            /* number of blocks vertically (rows)*/
            for (jg = 0; jg < m; jg++) {
                /* number of block horizontally (cols)*/
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





/*
int **fromCellMatToIntMat(cell ***src, int N) {

    int **dst, i, j;
    dst = allocateIntMatrix(N);
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            dst[i][j] = src[i][j]->value;
        }
    }

    return dst;
}


cell ***fromIntMatToCellMat(int **src, int N) {

    int i, j;
    cell ***dst;
    dst = allocateCellMatrix(N);
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {

            dst[i][j]->value = src[i][j];
        }
    }

    return dst;
}
*/

