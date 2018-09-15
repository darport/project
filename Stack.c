#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "ErrorHandler.h"
#include "Stack.h"

int push(Stack *stk, int val){
    Elem *p;
	p = (Elem *)malloc(sizeof(Elem));
	if(p == NULL){
		return memoryError();
	}
	p->val = val;
	p->next = stk->top;
	stk->top = p;
	stk->size++;
	return 1;
}

int pop(Stack *stk){
    Elem *p;
    int val;
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

int initialize(Stack *stk){
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
