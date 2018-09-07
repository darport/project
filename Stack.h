#ifndef STACK_H_
#define STACK_H_

typedef struct elem{
    int val;
    struct elem *next;
}Elem;

typedef struct stack{
    int size;
    Elem *top;
}Stack;

int push(Stack *stk,int val);
int pop(Stack *stk);
int top(Stack *stk);
int initialize(Stack *stk);
void freeStk(Stack *stk);

#endif /* STACK_H_ */
