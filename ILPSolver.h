#ifndef ILPSOLVER_H_
#define ILPSOLVER_H_

void updateSolved(Game *game,int *sol);
void freeILP(int *sol, int *ind, int *val, int *obj, int *vtype, GRBenv *env, GRBmodel *model);
int ILP(Game *game);



#endif /* ILPSOLVER_H_ */