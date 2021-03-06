#include "LinkedList.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <assert.h>

/**
 * createLinkedList
 *	Calling the function will generate an empty Linked List and return it
 *
 *	As the linked list created was empty, both head and tail point to NULL
 *
 * Words:
 *	1. newList		a new linked list object that is empty
 *
 *
 * createListElement
 *
 *	Calling the function will generate a ListElement's object and return it.
 *	ListElement type object contain:
 *		1. an integer
 *		2. a pointer to the next ListElement object
 *
 *	ListElement type object is basically a node that can extend to store more value
 *
 * Words:
 *	1. inValue		integer argument brought in to store in the listElement object
 *	2. newNode		new created node that store the "inValue"
 */


LinkedList* createLinkedList(){
	LinkedList* newList = malloc(sizeof(LinkedList));
	assert(newList != NULL);

	newList-> head	= NULL;
	newList-> tail	= NULL;
  newList->length = 0;

	return newList;
}

ListElement* createListElement(void *data){
	ListElement* newElement = malloc(sizeof(ListElement));

	newElement->node = data;
	newElement->next = NULL;

	return newElement;
}

void addListLast(LinkedList* currentList, void *data){

  if(currentList == NULL){
    ThrowError(ERR_NULL_LIST, "ERROR: Link List cannot be NULL!");
  }

  if(data == NULL){
    ThrowError(ERR_NULL_ELEMENT, "ERROR: Element cannot be NULL!");
  }

  ListElement* newElement = createListElement(data);
  ListElement* tempNode = currentList->head;
  //check if the linked list empty. If empty, create a Node that the head point to
  if(currentList->head == NULL){
    currentList->head = newElement;
  }
  //if the list is not empty, find the last node and add a node as new last node
  else{
    while(tempNode->next != NULL){
      tempNode = tempNode->next;
    }
    tempNode->next = newElement;
  }
  //No matter what, the tail always point to last node
  currentList->tail = newElement;
  currentList->length ++;

}

void addListFirst(LinkedList* currentList, void *data){
  if(data == NULL)
    ThrowError(ERR_NULL_ELEMENT, "ERROR: Element cannot be NULL!");

  if(currentList == NULL)
    ThrowError(ERR_NULL_LIST, "ERROR: Link List cannot be NULL!");

  ListElement* prevHead   = currentList->head;
  ListElement* newElement = createListElement(data);
  currentList->head = newElement; //No matter what, the head always point to the new added Element
  
  if(currentList->length == 0)
    currentList->tail = newElement;
  
  currentList->head->next = prevHead;
  currentList->tail->next = NULL; //Tail->next permanently pointing to NULL
  (currentList->length)++;
}

ListElement* listRemoveFirst(LinkedList* link){
  ListElement* removedElement;

  if(link == NULL){
    removedElement = NULL;
    ThrowError(ERR_NULL_LIST, "ERROR: Link List cannot be NULL!");
  }

  if(link->head == NULL){
    removedElement = NULL;
    ThrowError(ERR_EMPTY_LIST, "Nothing to remove, empty list here! Return empty element");
  }

  removedElement = malloc(sizeof(ListElement));
  removedElement = link->head;
  if(link->head->next != NULL)
    link->head = link->head->next;
  else{
    link->head = NULL;
    link->tail = NULL;
  }
  removedElement->next = NULL;
  link->length --;

  return removedElement;
}

ListElement* listRemoveLast(LinkedList* link){
  ListElement* removedElement;
  ListElement* tempNode = malloc(sizeof(ListElement));

  if(link == NULL){
    removedElement = NULL;
    ThrowError(ERR_NULL_LIST, "ERROR: Link List cannot be NULL!");
  }
  if(link->head == NULL){
    removedElement = NULL;
    ThrowError(ERR_EMPTY_LIST, "Nothing to remove, empty list here! Return empty element");
  }

  tempNode = link->head;
  removedElement = malloc(sizeof(ListElement));
  removedElement = link->tail;
  if(tempNode->next != NULL){
    while(tempNode->next != link->tail){
      tempNode = tempNode->next;
    }
    link->tail = tempNode;
    tempNode->next = NULL;
  }
  else{
    link->head = NULL;
    link->tail = NULL;
  }
  link->length --;

  return removedElement;
}










