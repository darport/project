#include <stdio.h>
#include <stdlib.h>
#include "mainAux.h"
#include "Game.h"

void freeList(Node *head){
    while(head != NULL){
        freeList(head->next);
        free(head);
    }
}

void addNode(Node *tail, int x, int y, int currZ, int prevZ){
    Node *newNode = (Node *) malloc(sizeof(Node));
    if(newNode == NULL){
        memoryError();
    }
    newNode->x = x;
    newNode->y = y;
    newNode->currZ = currZ;
    newNode->prevZ = prevZ;
    newNode->next = NULL;
    newNode->prev = *tail;
    (*tail)->next = newNode;
    *tail = newNode;
}
