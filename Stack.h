#ifndef STACK_H_
#define STACK_H_

/* ELEM: a structure for element in the stack. the properties:
 * val: the value of the element - an integer
 * next: pointer to the next element in the stack */
typedef struct elem {
    int val;
    struct elem *next;
}Elem;

/* STACK: a structure for the stack that is used in the exhaustive backtracking algorithm. the properties:
 * size: the stack size
 * top: the element in the top of the stack */
typedef struct stack {
    int size;
    Elem *top;
}Stack;

/* the function gets a stack and a value, creates a new element and pushes it to the top of the stack.
 * the function returns 1 if the push was successful and handles memory allocation error if needed (returns -1) */
int push(Stack *stk, int val);

/* the function gets a stack, deletes the element at the top of the stack and returns its value */
int pop(Stack *stk);

/* the functions gets a stack and return the value of the element at the top of the stack */
int top(Stack *stk);

/* the gets a stack and initializes its properties */
int initialize(Stack *stk);

/* the function gets a stack and frees the memory that was allocated for the stack */
void freeStk(Stack *stk);

#endif /* STACK_H_ */
