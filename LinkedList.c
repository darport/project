#include <stdio.h>
#include <stdlib.h>
#include "Game.h"
#include "LinkedList.h"
#include "MainAux.h"

void freeListNode(Node *head){
    if(head->next != NULL){
        freeListNode(head->next);
    }
    free(head);
}

void freeList(Link *head){
	if(head->next != NULL){
		freeList(head->next);
	}
	if(head->head != NULL){
		freeListNode(head->head);
	}
	free(head);
}

void addNode(Node **head, int x, int y, int currZ, int prevZ, int type){
    Node *newNode = (Node *) malloc(sizeof(Node));
    if(newNode == NULL){
        memoryError();
    } /* newNode is not null */
    newNode->x = x;
    newNode->y = y;
    newNode->currZ = currZ;
    newNode->prevZ = prevZ;
    newNode->type = type;
    newNode->next = NULL;
    newNode->prev = NULL;
    if(*head == NULL){
    	*head = newNode;
    	return;
    }
    newNode->prev = *head;
    (*head)->next = newNode;
    *head = newNode;
}


void addLink(Link **ops, int x, int y, int currZ, int prevZ, int type){
	Link *newLink = (Link *)malloc(sizeof(Link));
	if(newLink == NULL){
		memoryError();
    }
	newLink->head = NULL;
	newLink->next = NULL;
    newLink->prev = NULL;
    addNode(&(newLink->head), x, y, currZ,prevZ,type);
    if(*ops == NULL){
    	*ops = newLink;
    	return;
    }
    newLink->prev = *ops;
    (*ops)->next = newLink;
    *ops = newLink;
}



