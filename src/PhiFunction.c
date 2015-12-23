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


/**
 *  This function will compare the liveness list from two parent and 
 *
 *
 */
Expression* getPhiFunction(){
  Expression* phiFunction = NULL;
  
  //Argument brought in InputNode
  //Get the imdDom
  //Get the condition from the imdDom
  //TRAVELING TASK: Check modified Id(liveliness)
  //TRAVELING TASK: Get the oprd1 & oprd2
  
  // LinkedList* idList       = getModifiedId(Node* domNode, Node* checkNode);
  // ListElement* idElement   = idList->head;
  // while(idList != NULL){
    // char modID = idElement->node;
    
    
    //                            modID
    //                              |
    phiFunction = createExpression('x', PHI_FUNC, 1, 2, 1);
    // idElement = idElement->next;
  // }
  
  
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
    phiFunction = getPhiFunction();
    
    if(phiFunction != NULL)
      addListFirst(domFNode->block, phiFunction);
    
    domFElement = domFElement->next;
  }
}
