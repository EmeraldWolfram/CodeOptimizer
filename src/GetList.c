#include "GetList.h"
#include <stdlib.h>
#include <stdio.h>

/*
 *  getSubsList is a simple function that extract all the
 *  variable in all the expressions in the Node into a LinkedList.
 *
 *  The function take the input LinkedList* exprList that contain
 *  all the expression in the Node.
 */
LinkedList* getSubsList(LinkedList* exprList){
  if(exprList == NULL)
    ThrowError(ERR_NULL_LIST, "Input LinkedList to function getSubsList is NULL");
  
  LinkedList* subsList = createLinkedList();
  ListElement* exprPtr = exprList->head;
  
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
 *  in the subsList
 *
 */
Subscript* getLargestIndex(LinkedList* subsList, Subscript* subsName){
  if(subsList == NULL)
    ThrowError(ERR_NULL_LIST, "Input LinkedList to function getLargestIndex is NULL");
  
  if(subsName == NULL)
    ThrowError(ERR_NULL_SUBSCRIPT, "Input Subscript to function getLargestIndex is NULL");
  
  ListElement *checkPtr, *resultPtr;
  
  checkPtr  = subsList->head;
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
  if(inputNode == NULL)
    ThrowError(ERR_NULL_NODE, "Input Node to function getModifiedList is NULL");
  
  LinkedList* modifyList  = createLinkedList();
  LinkedList* expList     = inputNode->block;
  
  ListElement *newPtr, *exprPtr, *checkPtr;
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
    
    if(checkPtr == NULL && ((Expression*)exprPtr->node)->opr != IF_STATEMENT){
        if(exprPtr->next == NULL)
          addListLast(modifyList, &((Expression*)exprPtr->node)->id); 
        else if(((Expression*)exprPtr->next->node)->opr != IF_STATEMENT)
          addListLast(modifyList, &((Expression*)exprPtr->node)->id);
    }
    exprPtr = exprPtr->next;
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
  if(inputNode == NULL)
    ThrowError(ERR_NULL_NODE, "Input Node to function updateList is NULL");
  
  if(prevList == NULL)
    ThrowError(ERR_NULL_LIST, "Input LinkedList to function updateList is NULL");
  
  ListElement *newPtr, *checkPtr;

  LinkedList* modifiedList = getLatestList(inputNode);
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

/*
 *  getLiveList(Node* inputNode)
 *
 *  This function should be able to return all the living subscript in 
 *  the input Node. 
 *
 *  It first extract all the Sunscript in the RHS of every expression 
 *  within the node.
 *  This function then extract all non-repeat Subscript and return the List
 *  that have no repeat subscript.
 *  
 ******************************************************************************/
LinkedList* getLiveList(Node *inputNode){
  if(inputNode == NULL)
    ThrowError(ERR_NULL_NODE, "Input Node to function getLiveList is NULL");
  
  LinkedList* allLiveList  = createLinkedList();
  LinkedList* filterList  = createLinkedList();
  ListElement* checkPtr = inputNode->block->head;
  int checkOpr;
  
  while(checkPtr != NULL){
    checkOpr = ((Expression*)checkPtr->node)->opr;
    if(checkOpr != ASSIGN && checkOpr != IF_STATEMENT){
      addListLast(allLiveList, &((Expression*)checkPtr->node)->oprdA);
      if(checkOpr != COPY)
        addListLast(allLiveList, &((Expression*)checkPtr->node)->oprdB);
    }
    checkPtr = checkPtr->next;
  }
  
  ListElement*  livePtr = allLiveList->head;
  while(livePtr != NULL){
    checkPtr = filterList->head;
    while(checkPtr != NULL && ((Subscript*)livePtr->node)->name \
                        !=  ((Subscript*)checkPtr->node)->name){
      checkPtr = checkPtr->next;
    }
    if(checkPtr == NULL)
      addListLast(filterList, livePtr->node);
    livePtr = livePtr->next;
  }
  
  return filterList;
}

/*
 *  getLatestList(Node* inputNode)
 *
 *  This function return the latest modified value
 *
 ***********************************************************************/
LinkedList* getLatestList(Node* inputNode){
  if(inputNode == NULL)
    ThrowError(ERR_NULL_NODE, "Input Node to function getLatestList is NULL");
  
  LinkedList* modifyList  = getModifiedList(inputNode);
  ListElement*  newPtr = modifyList->head;
  while(newPtr != NULL){
    newPtr->node = getLargestIndex(inputNode->block, (Subscript*)newPtr->node);
    newPtr = newPtr->next;
  }
  
  return modifyList;
}












