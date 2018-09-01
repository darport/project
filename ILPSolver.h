#include "gurobi_c.h"

#ifndef ILPSOLVER_H_
#define ILPSOLVER_H_

void updateSolved(Game *game,double *sol);
void freeILP(double *sol, int *ind, double *val,double *obj, char *vtype, GRBmodel *model, GRBenv *env);
int ILP(Game *game);

#endif /* ILPSOLVER_H_ */