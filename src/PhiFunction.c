#include "PhiFunction.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>


/*
 *  getCondition function return the condition subscript
 *  Eg.
 *    condition = x > 10
 *    if( condition )  goto nodeX
 *  
 *  getCondition should return the condition expression
 *
 */
Subscript getCondition(Node* imdDomNode){
  assert(imdDomNode != NULL);
  
  ListElement* ptrToCondition = imdDomNode->block->head;
  
  while(ptrToCondition->next->next != NULL){
    ptrToCondition = ptrToCondition->next;
  }
  if(((Expression*)ptrToCondition->next->node)->opr != IF_STATEMENT)
    ThrowError(ERR_INVALID_BRANCH, "This operator does not branch");
  
  return ((Expression*)ptrToCondition->next->node)->oprdA;
}


/*  getPhiFunction
 *
 *  This function will compare the liveness list from two parent 
 *  and create a phifunction in SSA form without the condition
 *
 *  @listA  a list of subscripts that was living at parent A
 *  @listB  a list of subscripts that was living at parent B
 *  @subs   The subscript that required a phiFunction
 *
 */
Expression* getPhiFunction(LinkedList* listA, LinkedList* listB, Subscript* subs){
  Subscript *subsA  = NULL;
  Subscript *subsB  = NULL;
  
  ListElement *ptrA, *ptrB;
  ptrA = listA->head;
  ptrB = listB->head;
  
  int subsName = subs->name;
  /*****************************************************************
   *  Find the latest index for operand A and operand B at this point
   *****************************************************************/
  while(ptrA != NULL && ((Subscript*)ptrA->node)->name != subsName)
    ptrA = ptrA->next;
  if(ptrA != NULL)
    subsA = ptrA->node;
  else
    ThrowError(ERR_UNDECLARE_VARIABLE, "Undefine reference to Subscript %c", subsName);
  
  while(ptrB != NULL && ((Subscript*)ptrB->node)->name != subsName)
    ptrB = ptrB->next;
  if(ptrB != NULL)
    subsB = ptrB->node;
  else
    ThrowError(ERR_UNDECLARE_VARIABLE, "Undefine reference to Subscript %c", subsName);
  
  //Create the phifunction without the condition yet
  Expression* phiFunction   = createExpression(subs->name, PHI_FUNC, subsA->name, subsB->name, 0);
  phiFunction->id.index     = subs->index + 1;
  phiFunction->oprdA.index  = subsA->index;
  phiFunction->oprdB.index  = subsB->index;
  
  return phiFunction;
}
/**
 *  allocPhiFunc
 *
 *  This function recursively check the node that require more than
 *  a phiFunction and place the required phiFunction for these subscript
 *
 *****************************************************/
void allocPhiFunc(Node** thisNode){
  (*thisNode)->visitFlag |= 2;
  LinkedList* liveList    = getAllLiveList(thisNode, NULL);
  resetFlag(thisNode, 2);
  
  ListElement* livePtr    = liveList->head;
  LinkedList* listA, *listB;
  Node *nodeAPtr, *nodeBPtr, *rootPtr;
  Subscript* subsPtr, condtSubs;
  int i;
  if((*thisNode)->directDom->length == 2){
    nodeBPtr = (*thisNode)->directDom->head->node;
    nodeAPtr = (*thisNode)->directDom->head->next->node;
    listA = getListTillNode(nodeAPtr);
    listB = getListTillNode(nodeBPtr);
    Expression* phiFunction;
    while(livePtr != NULL){
      subsPtr     = getLargestIndex(listB, livePtr->node);
      phiFunction = getPhiFunction(listA, listB, subsPtr);
      condtSubs   = getCondition((*thisNode)->imdDom);
      phiFunction->condt = condtSubs;
      addListFirst((*thisNode)->block, phiFunction);
      
      livePtr = livePtr->next;
    }
  }
  
  for(i = 0; i < (*thisNode)->numOfChild; i++)
    if(((*thisNode)->children[i]->visitFlag & 2) == 0)
      allocPhiFunc(&(*thisNode)->children[i]);
}



void resetFlag(Node** anyNode, int bitNumber){
  Node **rootNode, *nodePtr;
  nodePtr = (*anyNode);
  while(nodePtr->rank != 0)
    nodePtr = nodePtr->parent;
  rootNode = &nodePtr;
  
  LinkedList* nodeList = assembleList(rootNode);
  ListElement* elmPtr = nodeList->head;
  while(elmPtr != NULL){
    ((Node*)elmPtr->node)->visitFlag &= ~(1 << bitNumber);
    elmPtr = elmPtr->next;
  }
}