#include "ExpressionBlock.h"
#include <stdlib.h>


Expression* createExpression(char thisID, int subs, Operator oprt,  \
                             int oprd1, int oprd2, int condt){
  Expression* newExp = malloc(sizeof(Expression));
  
  newExp->id = thisID;
  newExp->subscrpt  = subs;
  newExp->opr = oprt;
  newExp->operand1  = oprd1;
  newExp->operand2  = oprd2;
  newExp->condition = condt;
               
  return newExp;            
}

/*
 *  getCondition function return the subscript of 
 *
 *  ???? SHOULD I CREATE A RESULTING VALUE OF THAT SUBSCRIPT  ?????
 *
 *
 */

Expression* getCondition(Node* imdDomNode){
  ListElement* ptrToCondition = imdDomNode->block->head;
  
  while(ptrToCondition->next->next != NULL){
    ptrToCondition = ptrToCondition->next;
  }
  
  //????????????????????HERE IS THE QUESTION! WHAT SHOULD I RETURN
  return (Expression*)(ptrToCondition->node);
}




