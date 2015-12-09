#include "PhiFunction.h"
#include <stdlib.h>
#include <stdio.h>


int phiFunction(){
  return 1;
}

void allocPhiFunc(Node* entryNode){
  LinkedList* tempList     = getAllDomFrontiers(&entryNode);
  ListElement* domFElement = tempList->head;
  Node* domFNode           = (Node*)domFElement->node;

  while(domFElement != NULL){
    domFNode  = domFElement->node;
    addListFirst(domFNode->block->expression, phiFunction);
    domFElement = domFElement->next;
  }
}
