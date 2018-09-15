#include "gurobi_c.h"
#include "Game.h"

#ifndef ILPSOLVER_H_
#define ILPSOLVER_H_

void updateSolved(double *sol,Game *game);
void freeILP(double *sol, int *ind, int *ind2, double *val, double *val2, double *lb, char *vtype, GRBenv *env,
        GRBmodel *model);
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
