#include "GetList.h"
#include <stdlib.h>
#include <stdio.h>


LinkedList* getModifiedList(Node* inputNode){
  LinkedList* modifyList = createLinkedList();
  LinkedList* expList     = inputNode->block;
  
  ListElement *newPtr, *exprPtr, *checkPtr;
  int validFlag;
  exprPtr   = expList->head;
  
  /***********************************************************
   * Form modifyList with the modified variable in this block
   ***********************************************************/
  while(exprPtr != NULL){
    checkPtr = modifyList->head;
    while(checkPtr != NULL && ((Subscript*)checkPtr->node)->name \
          != ((Expression*)exprPtr->node)->id.name){
      checkPtr = checkPtr->next;
    }
    
    if(checkPtr == NULL){
      if(((Expression*)exprPtr->node)->opr != IF_STATEMENT &&   \
        ((Expression*)exprPtr->next->node)->opr != IF_STATEMENT)
        addListLast(modifyList, &((Expression*)exprPtr->node)->id); 
    }
    exprPtr = exprPtr->next;
  }
  /******************************************************
   *  Find the correct index index and assign to it
   ******************************************************/
  newPtr = modifyList->head;
  while(newPtr != NULL){
    newPtr->node = getLargestIndex(expList, (Subscript*)newPtr->node);
    newPtr = newPtr->next;
  }
  
  return modifyList;
}