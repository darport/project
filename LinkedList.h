#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

typedef struct node_t {
    int x;
    int y;
    int prevZ;
    int currZ;
    int type; /* 0 - set, 1- autofill*/
    struct node_t *next;
    struct node_t *prev;
}Node;

typedef struct list_t{
	Node *head;
	struct list_t * next;
	struct list_t * prev;
}Link;

void freeListNode(Node *head);
void freeList(Link *head);
int addNode(Node **lst, int x, int y, int currZ, int prevZ, int type);
int addLink(Link **lst, int x, int y, int currZ, int prevZ, int type);
#endif /* LINKEDLIST_H_ */
