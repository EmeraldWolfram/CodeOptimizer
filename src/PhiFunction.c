#include "PhiFunction.h"
#include <stdlib.h>
#include <stdio.h>


/*
 *  getCondition function return the condition subscript
 *  Eg.
 *    condition = x > 10
 *    if( condition )  goto nodeX
 *  
 *  getCondition should return the condition expression
 *
 */
Subscript* getCondition(Node* imdDomNode){
  ListElement* ptrToCondition = imdDomNode->block->head;
  
  while(ptrToCondition->next->next != NULL){
    ptrToCondition = ptrToCondition->next;
  }
  
  return (&((Expression*)ptrToCondition->next->node)->oprdA);
}


/*
 *  This function will compare the liveness list from two parent and 
 *
 *
 */
Expression* getPhiFunction(LinkedList* listA, LinkedList* listB, Subscript* subs){
  Subscript *subsA  = NULL;
  Subscript *subsB  = NULL;
  
  ListElement *ptrA, *ptrB;
  ptrA = listA->head;
  ptrB = listB->head;
  
  int subsName = subs->name;
  
  while(ptrA != NULL && ((Subscript*)ptrA->node)->name != subsName){
    ptrA = ptrA->next;
  }

  if(ptrA != NULL)
    subsA = ptrA->node;
  else
    ThrowError(ERR_UNDECLARE_VARIABLE, "Undefine reference to Subscript %c", subsName);
  
  while(ptrB != NULL && ((Subscript*)ptrB->node)->name != subsName){
    ptrB = ptrB->next;
  }
  if(ptrB != NULL)
    subsB = ptrB->node;
  else
    ThrowError(ERR_UNDECLARE_VARIABLE, "Undefine reference to Subscript %c", subsName);
    
  Expression* phiFunction   = createExpression(subs->name, PHI_FUNC, subsA->name, subsB->name, 0);
  phiFunction->id.index     = subs->index;
  phiFunction->oprdA.index  = subsA->index;
  phiFunction->oprdB.index  = subsB->index;
  
  return phiFunction;
}

void allocPhiFunc(Node* entryNode){
  setAllImdDom(&entryNode);
  Expression* phiFunction;
  
  LinkedList* tempList     = getAllDomFrontiers(&entryNode);
  ListElement* domFElement = tempList->head;
  Node* domFNode           = (Node*)domFElement->node;
  
  while(domFElement != NULL){
    domFNode  = domFElement->node;
    // phiFunction = getPhiFunction(listA, listB, x3);
    phiFunction = getPhiFunction(createLinkedList(), createLinkedList(), NULL);
    
    if(phiFunction != NULL)
      addListFirst(domFNode->block, phiFunction);
    
    domFElement = domFElement->next;
  }
}
