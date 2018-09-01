#include <stdio.h>
#include <stdlib.h>
#include "mainAux.h"
#include "Stack.h"

void push(Stack *stk, int val){
	Elem *p;
	p = (Elem *)malloc(sizeof(Elem));
	if(p == NULL){
		memoryError();
		return;
	}
	p->val = val;
	p->next = stk->top;
	stk->top = p;
	stk->size++;
}

int pop(Stack *stk){
	int val;
	Elem *p;
	val = stk->top->val;
	p = stk->top;
	stk->top = stk->top->next;
	stk->size--;
	free(p);
	return val;
}

int top(Stack *stk){
	return stk->top->val;
}

void initialize(Stack *stk){
	stk->size = 0;
	stk->top = NULL;
}

void freeStk(Stack *stk){

}