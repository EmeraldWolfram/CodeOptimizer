#include "PhiFunction.h"
#include <stdlib.h>
#include <stdio.h>


Expression* getPhiFunction(){
  Expression* phiFunction = NULL;
  
  phiFunction = createExpression('x', 3, PHI_FUNC, 1, 2, 1);
  
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
