#include "gurobi_c.h"
#include "Game.h"

#ifndef ILPSOLVER_H_
#define ILPSOLVER_H_
/*the function updates the solved board that the struct game keaps, using to sol array that the ilp function returns,
 *  in order to use it for vary functions*/
void updateSolved(double *sol,Game *game);
/*the function releases all the resources used in the ilp function*/
void freeILP(double *sol, int *ind, int *ind2, double *val, double *val2, double *lb, char *vtype, GRBenv *env,
        GRBmodel *model);
/*the function uses integer linear programming in order to find a solution for the game board,
 * returns 1 if a solution was found and 0 o/w*/
int ILP(Game *game,int command);
/*
int createEnvironment(double *sol, int *ind, int *ind2, double *val, double *val2, double *lb, char *vtype, GRBenv *env,
                      GRBmodel *model);
int createNewModel(double *sol, int *ind, int *ind2, double *val, double *val2, double *lb, char *vtype, GRBenv *env,
                   GRBmodel *model, int N);
int firstCellConstrain(double *sol, int *ind, int *ind2, double *val, double *val2, double *lb, char *vtype, GRBenv *env,
                       GRBmodel *model, int N);
int secondCellConstrain(Game *game, double *sol, int *ind, int *ind2, double *val, double *val2, double *lb, char *vtype, GRBenv *env,
                        GRBmodel *model, int N);
int rowConstrain(double *sol, int *ind, int *ind2, double *val, double *val2, double *lb, char *vtype, GRBenv *env,
                 GRBmodel *model, int N);
int colConstrain(double *sol, int *ind, int *ind2, double *val, double *val2, double *lb, char *vtype, GRBenv *env,
                 GRBmodel *model, int N);
int blockConstrain(double *sol, int *ind, int *ind2, double *val, double *val2, double *lb, char *vtype, GRBenv *env,
                   GRBmodel *model, int N, int m, int n);
                   */

#endif /* ILPSOLVER_H_ */
