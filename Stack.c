#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "mainAux.h"
#include "Stack.h"

int push(Stack *stk, int val){
    if(stk == NULL){
        return INT_MIN;
    }
	Elem *p;
	p = (Elem *)malloc(sizeof(Elem));
	if(p == NULL){
		memoryError();
		return INT_MIN;
	}
	p->val = val;
	p->next = stk->top;
	stk->top = p;
	stk->size++;
	return 1;
}

int pop(Stack *stk){
    if(stk == NULL){
        return INT_MIN;
    }
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
    if(stk == NULL){
        return INT_MIN;
    }
	return stk->top->val;
}

int initialize(Stack *stk){
    if(stk == NULL){
        return memoryError();
    }
	stk->size = 0;
	stk->top = NULL;
	return 1;
}

void freeStk(Stack *stk){
    if(stk == NULL){
        return;
    }
    while(stk->size > 0){
        pop(stk); /* pop uses free() */
    }
    free(stk);
}
