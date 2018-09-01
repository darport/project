#include <stdio.h>
#include <stdlib.h>
#include "gurobi_c.h"
#include "Game.h"

void updateSolved(Game *game, double *sol){
    int i,j,k, size = game->size;
    for(i = 0; i<size; i++){
        for(j = 0; j<size; j++){
            for(k = 0; k<size; k++){
                if(sol[i*size*size + j*size +k] == 1){
                    game->solved[i][j].value = k + 1;
                }
            }
        }
    }
}

void freeILP(double *sol, int *ind, double *val,double *obj, char *vtype, GRBmodel *model, GRBenv *env){
    free(sol);
    free(ind);
    free(val);
    free(obj);
    free(vtype);
    GRBfreemodel(model);
    GRBfreeenv(env);
}

int ILP(Game *game){
    int size = game->size, flag = 0;
    GRBenv   *env   = NULL;
    GRBmodel *model = NULL;
    double *sol = (double *)malloc(size * size * size * sizeof(double));
    int    *ind = (int *)malloc(size * sizeof(int));
    double *val = (double *)malloc(size *  sizeof(double));
    double *obj = (double *)malloc(size* size * size * sizeof(double));
    char   *vtype = (char *)malloc(size * size * size * sizeof(double));
    int       optimstatus;
    double    objval;

    int error = 0, i, j, k, l, temp, cols = game->rowsInBlock, rows = game->colsInBlock;

    /* Create environment - log file is sudoku.log */
    error = GRBloadenv(&env, "sudoku.log");
    if(error){
        printf("ERROR %d GRBloadenv(): %s\n", error, GRBgeterrormsg(env));
        free(sol);
        free(ind);
        free(val);
        free(obj);
        free(vtype);
        return -1;
    }

    GRBsetintparam(env,GRB_INT_PAR_LOGTOCONSOLE,0);

    /* Create an empty model named "sudoku" */
    error = GRBnewmodel(env, &model, "sudoku", 0, NULL, NULL, NULL, NULL, NULL);
    if (error) {
        printf("ERROR %d GRBnewmodel(): %s\n", error, GRBgeterrormsg(env));
        free(sol);
        free(ind);
        free(val);
        free(obj);
        free(vtype);
        GRBfreeenv(env);
        return 1;
    }
    for(i = 0; i<size*size*size; i++){
        /*no obj init*/
        vtype[i] = GRB_BINARY;
    }
    error = GRBaddvars(model, size*size*size, 0, NULL, NULL, NULL, obj, NULL, NULL, vtype, NULL);
    if (error) {
        printf("ERROR %d GRBaddvars(): %s\n", error, GRBgeterrormsg(env));
        freeILP(sol,ind,val,obj,vtype, model, env);
        return -1;
    }
    error = GRBupdatemodel(model);
    if (error) {
        printf("ERROR %d GRBupdatemodel(): %s\n", error, GRBgeterrormsg(env));
        freeILP(sol,ind,val,obj,vtype, model, env);
        return -1;
    }

    /*constrains for cell no 2 values for 1 cell*/
    for(i = 0; i<size; i++){
        for(j = 0; j<size; j++){
            for(k = 0; k<size; k++){
                ind[k] = i*size*size + j*size + k;
                val[k] = 1;
            }
            error = GRBaddconstr(model, size, ind, val, GRB_EQUAL, 1.0, NULL);
            if (error) {
                printf("ERROR %d 2 values for 1 cell GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
                freeILP(sol,ind,val,obj,vtype, model, env);
                return -1;
            }
        }
    }

    /* constrain that if value != 0 don't change it*/
    for(i = 0; i<size; i++){
        for(j = 0; j<size; j++){
            temp = game->board[i][j].value;
            if(temp != 0){
                for(k = 0; k<size; k++){
                    if(temp == k+1){
                        ind[k] = i*size*size + j*size + k;
                        val[k] = 1;
                    }
                }
                error = GRBaddconstr(model, size, ind, val, GRB_EQUAL, 1.0, NULL);
                if (error) {
                    printf("ERROR %d value should not change GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
                    freeILP(sol,ind,val,obj,vtype, model, env);
                    return -1;
                }
            }
        }
    }

    /* row constrains */
    for(i =0; i<size; i++){
        for(k=0; k<size; k++){
            for(j=0; j<size; j++){
                ind[j] = i*size*size + j*size + k;
                val[j] = 1;
            }
            error = GRBaddconstr(model, size, ind, val, GRB_EQUAL, 1.0, NULL);
            if (error) {
                printf("ERROR %d row GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
                freeILP(sol,ind,val,obj,vtype, model, env);
                return -1;
            }
        }
    }

    /* col constrains */
    for(j = 0; j<size; j++){
        for(k=0; j<size; j++){
            for(i=0; i<size; i++){
                ind[i] = i*size*size + j*size + k;
                val[i]=1;
            }
            error = GRBaddconstr(model, size, ind, val, GRB_EQUAL, 1.0, NULL);
            if (error) {
                printf("ERROR %d col GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
                freeILP(sol,ind,val,obj,vtype, model, env);
                return -1;
            }
        }
    }

    /*block constrains - creating block grid*/
    for(i = 0; i<rows; i++){
        for(j = 0; j<cols; j++){
            temp = 0;
            for(k = 0; k<game->rowsInBlock; k++){
                for(l = 0; l<game->colsInBlock; l++){
                    ind[temp] = i*game->rowsInBlock + k;
                    val[temp] = 1;
                    temp++;
                }
            }
            error = GRBaddconstr(model, size, ind, val, GRB_EQUAL, 1.0, NULL);
            if (error) {
                printf("ERROR %d block GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
                freeILP(sol,ind,val,obj,vtype, model, env);
                return -1;
            }
        }
    }

    /* Optimize model - need to call this before calculation */
    error = GRBoptimize(model);
    if (error) {
        printf("ERROR %d GRBoptimize(): %s\n", error, GRBgeterrormsg(env));
        freeILP(sol,ind,val,obj,vtype, model, env);
        return -1;
    }

    /* Write model to 'sudoku.lp' - this is not necessary but very helpful */
    error = GRBwrite(model, "sudoku.lp");
    if (error) {
        printf("ERROR %d GRBwrite(): %s\n", error, GRBgeterrormsg(env));
        freeILP(sol,ind,val,obj,vtype, model, env);
        return -1;
    }

    /* Get solution information */
    error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
    if (error) {
        printf("ERROR %d GRBgetintattr(): %s\n", error, GRBgeterrormsg(env));
        freeILP(sol,ind,val,obj,vtype, model, env);
        return -1;
    }

    /* get the objective -- the optimal result of the function */
    error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL, &objval);
    if (error) {
        printf("ERROR %d GRBgettdblattr(): %s\n", error, GRBgeterrormsg(env));
        freeILP(sol,ind,val,obj,vtype, model, env);
        return -1;
    }

    /* get the solution - the assignment to each variable */
    error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, size*size*size, sol);
    if (error) {
        printf("ERROR %d GRBgetdblattrarray(): %s\n", error, GRBgeterrormsg(env));
        freeILP(sol,ind,val,obj,vtype, model, env);
        return -1;
    }

    /* if there is a solution, update the board */
    if(optimstatus == GRB_OPTIMAL){
        updateSolved(game, sol);
        flag = 1;
    }

    freeILP(sol,ind,val,obj,vtype, model, env);
    return flag;
}