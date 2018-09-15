#include <stdio.h>
#include <stdlib.h>
#include "Game.h"
#include "LinkedList.h"
#include "ErrorHandler.h"

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
        while(head->head->prev != NULL){
            head->head = head->head->prev;
        }
		freeListNode(head->head);
	}
	free(head);
}

int addNode(Node **head, int x, int y, int currZ, int prevZ, int type){
    Node *newNode = (Node *) malloc(sizeof(Node));
    if(newNode == NULL){
        return memoryError();
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
    	return 1;
    }
    newNode->prev = *head;
    (*head)->next = newNode;
    *head = newNode;
    return 1;
}


int addLink(Link **ops, int x, int y, int currZ, int prevZ, int type){
	Link *newLink = (Link *)malloc(sizeof(Link));

	if(newLink == NULL){
		return memoryError();
    }
	newLink->head = NULL;
	newLink->next = NULL;
    newLink->prev = NULL;
    if(addNode(&(newLink->head), x, y, currZ,prevZ,type) == -1){
        return -1;
    }
    if(*ops == NULL){
    	*ops = newLink;
    	return 1;
    }
    newLink->prev = *ops;
    (*ops)->next = newLink;
    *ops = newLink;
    return 1;
}



