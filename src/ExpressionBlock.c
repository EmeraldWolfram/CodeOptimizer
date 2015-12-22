#include "ExpressionBlock.h"
#include <stdlib.h>
#include <stdio.h>


Expression* createExpression(int thisID, Operator oprt, int oprdA,\
                            int oprdB, int condt){
  Expression* newExp = malloc(sizeof(Expression));
  
  newExp->id.name    = thisID;
  newExp->id.subs    = 0;
  newExp->opr        = oprt;
  newExp->oprdA.name = oprdA;
  newExp->oprdA.subs = 0;
  newExp->oprdB.name = oprdB;
  newExp->oprdB.subs = 0;
  newExp->condition  = condt;

  return newExp;            
}

LinkedList* getSubsList(LinkedList* expression){
  LinkedList* subsList = createLinkedList();
  ListElement* exprPtr = expression->head;
  
  while(exprPtr != NULL){
    if(((Expression*)exprPtr->node)->opr != IF_STATEMENT){
      if(((Expression*)exprPtr->node)->opr != ASSIGN){
        addListLast(subsList, &((Expression*)exprPtr->node)->oprdA);
        addListLast(subsList, &((Expression*)exprPtr->node)->oprdB); 
      }
      addListLast(subsList, &((Expression*)exprPtr->node)->id);
      exprPtr = exprPtr->next;
    }
  }
  
  return subsList;
}

void arrangeSSA(Node* inputNode){
  LinkedList* exprList  = inputNode->block;
  LinkedList* checkList = getSubsList(inputNode->block);
  ListElement* exprPtr  = exprList->head;
  ListElement* checkPtr;
  int currentRank;
  int oprARank;
  int oprBRank;
  
  while(exprPtr != NULL){
    currentRank = 0;
    oprARank = 0;
    oprBRank = 0;
    
    checkPtr = checkList->head;
    CHANGE_ID_SUBSCRIPT(exprPtr, checkPtr, currentRank);
    
    checkPtr = checkList->head;
    CHANGE_OPERAND_A(exprPtr, checkPtr, oprARank);

    checkPtr = checkList->head;
    CHANGE_OPERAND_B(exprPtr, checkPtr, oprBRank);
    
    exprPtr = exprPtr->next;
  }
}


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




