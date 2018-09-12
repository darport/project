#include "gurobi_c.h"
#include "Game.h"

#ifndef ILPSOLVER_H_
#define ILPSOLVER_H_

void updateSolved(double *sol,Game *game);
void freeILP(double *sol, int *ind, int *ind2, double *val, double *val2, double *lb, char *vtype, GRBenv *env,
        GRBmodel *model);
int ILP(Game *game,int command);

#endif /* ILPSOLVER_H_ */
