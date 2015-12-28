#include "GetList.h"
#include <stdlib.h>
#include <stdio.h>

/*
 *  getSubsList is a simple function that extract all the
 *  variable in all the expressions in the Node into a LinkedList.
 *
 *  The function take the input LinkedList* expression that contain
 *  all the expression in the Node.
 */
LinkedList* getSubsList(LinkedList* expression){
  LinkedList* subsList = createLinkedList();
  ListElement* exprPtr = expression->head;
  
  while(exprPtr != NULL){
    if(((Expression*)exprPtr->node)->opr != IF_STATEMENT){
      if(((Expression*)exprPtr->node)->opr != ASSIGN){
        addListLast(subsList, &((Expression*)exprPtr->node)->oprdA);
        if(((Expression*)exprPtr->node)->opr != COPY)
        addListLast(subsList, &((Expression*)exprPtr->node)->oprdB); 
      }
      addListLast(subsList, &((Expression*)exprPtr->node)->id);
    }
    exprPtr = exprPtr->next;
  }
  
  return subsList;
}

/*
 *  getLargestIndex
 *  
 *  find the largest number of the input argument subscript
 *  in the exprList
 *
 */
Subscript* getLargestIndex(LinkedList* exprList, Subscript* subsName){
  ListElement *checkPtr, *resultPtr;
  
  checkPtr  = exprList->head;
  while(checkPtr != NULL){
    if(((Subscript*)checkPtr->node)->name == subsName->name)
        resultPtr = checkPtr;
    checkPtr = checkPtr->next;
  }
  
  if(resultPtr == NULL)
    return 0;
  
  return &((Expression*)resultPtr->node)->id;
}

/*
 *  getModifiedList(Node* inputNode)
 *
 *  this function extract all the Expression in the inputNode
 *  and put all the subscript at the LHS of the expression into
 *  a LinkedList. If same subscript name was found, the function
 *  will remain the subscript with the largest index.
 *
 *  Eq.  if the input node have LHS : x2->y3->z0->x3->z1
 *       the function will return   : x3->y3->z1
 *
 */
LinkedList* getModifiedList(Node* inputNode){
  LinkedList* modifyList  = createLinkedList();
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
/*
 *  updateList(Node* inputNode, LinkedList* prevList)
 *
 *  This function called the getModifiedList to extract the LHS assigned
 *  subscript in the inputNode and take update the prevList with the
 *  modifiedList.
 *
 ********************************************************************/
void updateList(Node* inputNode, LinkedList* prevList){
  ListElement *newPtr, *checkPtr;

  LinkedList* modifiedList = getModifiedList(inputNode);
  /******************************************************
   *  Update the prevList with the modifiedList
   ******************************************************/
  newPtr    = modifiedList->head;
  while(newPtr != NULL){
    checkPtr  = prevList->head;
    while(checkPtr != NULL && ((Subscript*)checkPtr->node)->name \
                           != ((Subscript*)newPtr->node)->name){
      checkPtr = checkPtr->next;
    }
  
    if(checkPtr == NULL)
      addListLast(prevList, (Subscript*)newPtr->node);
    else
      checkPtr->node = newPtr->node;
    
    newPtr = newPtr->next;
  }
}

