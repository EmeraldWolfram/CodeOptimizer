#include "PhiFunction.h"
#include <stdlib.h>
#include <stdio.h>


Expression* getPhiFunction(){
  Expression* phiFunction = NULL;
  
  //Argument brought in InputNode
  //Get the condition from the imdDom
  //Get the imdDom
  //TRAVELING TASK: Check modified Id
  //TRAVELING TASK: Get the oprd1 & oprd2
  
  // LinkedList* idList       = getModifiedId(Node* domNode, Node* checkNode);
  // ListElement* idElement   = idList->head;
  // while(idList != NULL){
    // char modID = idElement->node;
    
    
    //                            modID
    //                              |
    phiFunction = createExpression('x', 3, PHI_FUNC, 1, 2, 1);
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
      addListFirst(domFNode->block->expression, phiFunction);
    
    domFElement = domFElement->next;
  }
}
