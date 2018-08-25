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

void push(int val, Stack stk);
int top(Stack *stk);
int pop(Stack *stk);
void initialize(Stack *stk);

#endif /* STACK_H_ */