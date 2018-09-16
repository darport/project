#include <stdio.h>
#include <stdlib.h>
#include "Game.h"
#include "LinkedList.h"
#include "ErrorHandler.h"

void freeListNode(Node *head){
	/*recursive release of sources from the end to the beggining of every move's list*/
    if(head->next != NULL){
        freeListNode(head->next);
    }
    free(head);
}

void freeList(Link *head){
	/*recursive release of sources from the end to the beggining of the entire redo undo list*/
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

int addNode(Node **head, int x, int y, int currZ, int prevZ){
    Node *newNode = (Node *) malloc(sizeof(Node));
    if(newNode == NULL){
        return memoryError();
    } /* newNode is not null */
    /* initializing the new node*/
    newNode->x = x;
    newNode->y = y;
    newNode->currZ = currZ;
    newNode->prevZ = prevZ;
    newNode->next = NULL;
    newNode->prev = NULL;
    if(*head == NULL){
    	*head = newNode;
    	return 1;
    } /*if it is the first change then make the new node the head, o/w append to the list*/
    newNode->prev = *head;
    (*head)->next = newNode;
    *head = newNode;
    return 1;
}


int addLink(Link **ops, int x, int y, int currZ, int prevZ){
	Link *newLink = (Link *)malloc(sizeof(Link));

	if(newLink == NULL){
		return memoryError();
    }
	/*initializing the new link*/
	newLink->head = NULL;
	newLink->next = NULL;
    newLink->prev = NULL;
    if(addNode(&(newLink->head), x, y, currZ,prevZ) == -1){
        return -1;
    } /*updating the link's head, i.e the new link now has a list of changes*/
    if(*ops == NULL){
    	*ops = newLink;
    	return 1;
    }
    /*appending a new list to the redo/undo list*/
    newLink->prev = *ops;
    (*ops)->next = newLink;
    *ops = newLink;
    return 1;
}

