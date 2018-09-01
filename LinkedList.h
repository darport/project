#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

typedef struct node_t {
    int x;
    int y;
    int prevZ;
    int currZ;
    struct node_t *next;
    struct node_t *prev;
}Node;

void freeList(Node *head);
void addNode(Node **lst, int x, int y, int currZ, int prevZ);

#endif /* LINKEDLIST_H_ */