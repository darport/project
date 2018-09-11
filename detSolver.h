/*
 * detSolver.h
 *
 *  Created on: 11 בספט׳ 2018
 *      Author: Dell
 */

#ifndef DETSOLVER_H_
#define DETSOLVER_H_

int recDet(int x, int y, int ** cpy,int size,Game *game);
int detBacktracking(Game *game);
int validateChange(Game *game);
int recval(int x, int y,int size, Game *game);

#endif /* DETSOLVER_H_ */
