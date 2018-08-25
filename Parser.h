#ifndef PARSER_H_
#define PARSER_H_

int getCommand(Game *game);
int size(Game *game);
int isEmpty(Game *game);
int inRange(Game *game, int number);
int isInt(char *num);
int callSet(Game *game);
int callSolve(Game *game);
int callEdit(Game *game);
int callGenerate(Game *game);
int callMarkErrors(Game *game);
int callSave(Game *game);
int callHint(Game *game);
int checkParams(char **params, int len);

#endif /* PARSER_H_ */
