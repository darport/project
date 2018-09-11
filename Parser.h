#include "Game.h"

#ifndef PARSER_H_
#define PARSER_H_

int isEmpty(Game *game);
int printInvalid();
int isInt(char *num);
int callSolve(Game *game);
int callEdit(Game *game);
int callMarkErrors(Game *game);
int inRange(Game *game, int num);
int checkParams(char **params, int len);
int callSet(Game *game);
int callGenerate(Game *game);
int callSave(Game *game);
int callHint(Game *game);
int getCommand(Game *game);
int inRangeGen(Game *game, int num);

#endif /* PARSER_H_ */
