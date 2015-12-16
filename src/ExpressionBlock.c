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
      addListLast(subsList, &((Expression*)exprPtr->node)->oprdA);
      addListLast(subsList, &((Expression*)exprPtr->node)->oprdB);
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
    while(&((Expression*)exprPtr->node)->oprdA != checkPtr->node){
      if(((Expression*)exprPtr->node)->oprdA.name == \
         ((Subscript*)checkPtr->node)->name &&       \
         oprARank < ((Subscript*)checkPtr->node)->subs){
        oprARank = ((Subscript*)checkPtr->node)->subs;
      }
      checkPtr = checkPtr->next;
    }
    ((Expression*)exprPtr->node)->oprdA.subs = oprARank;

    checkPtr = checkList->head;
    while(&((Expression*)exprPtr->node)->oprdB != checkPtr->node){
      if(((Expression*)exprPtr->node)->oprdB.name == \
         ((Subscript*)checkPtr->node)->name &&       \
         oprARank < ((Subscript*)checkPtr->node)->subs){
        oprARank = ((Subscript*)checkPtr->node)->subs;
      }
      checkPtr = checkPtr->next;
    }
    ((Expression*)exprPtr->node)->oprdB.subs = oprARank;
    
    
    
    exprPtr = exprPtr->next;
  }
}


/*
 *  getCondition function return the subscript of 
 *
 *  ???? SHOULD I CREATE A RESULTING VALUE OF THAT SUBSCRIPT  ?????
 *
 *
 */

Subscript* getCondition(Node* imdDomNode){
  ListElement* ptrToCondition = imdDomNode->block->head;
  
  while(ptrToCondition->next->next != NULL){
    ptrToCondition = ptrToCondition->next;
  }
  
  return (&((Expression*)ptrToCondition->node)->id);
}




