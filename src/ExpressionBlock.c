#include "ExpressionBlock.h"
#include <stdlib.h>
#include <stdio.h>


Expression* createExpression(int thisID, Operator oprt, int oprdA,\
                            int oprdB, int condt){
  Expression* newExp = malloc(sizeof(Expression));
  
  newExp->id.name     = thisID;
  newExp->id.index    = 0;
  newExp->opr         = oprt;
  newExp->oprdA.name  = oprdA;
  newExp->oprdA.index = 0;
  newExp->oprdB.name  = oprdB;
  newExp->oprdB.index = 0;
  newExp->condition   = condt;

  return newExp;            
}


/*
 *  arrangeSSA take in the inputNode and arrange all the expression
 *  in the Node to the correct subscript.
 *  Eg.
 *    x0 = x0 + x0
 *  
 *  arrangeSSA should arrange the equation above to
 *    x1 = x0 + x0
 *
 ********************************************************************/
void arrangeSSA(Node* inputNode){
  LinkedList* exprList  = inputNode->block;
  LinkedList* checkList = getSubsList(inputNode->block);
  ListElement* exprPtr  = exprList->head;
  ListElement* checkPtr;
  
  int currentRank, oprARank, oprBRank;
  
  while(exprPtr != NULL){
    currentRank = ((Expression*)exprPtr->node)->id.index;
    oprARank = ((Expression*)exprPtr->node)->oprdA.index;
    oprBRank = ((Expression*)exprPtr->node)->oprdB.index;
    
    if(((Expression*)exprPtr->node)->opr != IF_STATEMENT){
      checkPtr = checkList->head;
      CHANGE_ID_SUBSCRIPT(exprPtr, checkPtr, currentRank);  
                
      if(((Expression*)exprPtr->node)->opr != ASSIGN){
        checkPtr = checkList->head;
        CHANGE_OPERAND_A(exprPtr, checkPtr, oprARank);
                
        if(((Expression*)exprPtr->node)->opr != COPY){
          checkPtr = checkList->head;
          CHANGE_OPERAND_B(exprPtr, checkPtr, oprBRank);
        }
      }
    }
    exprPtr = exprPtr->next;
  }
}


/*  Traverse down every single node to make sure all the node
 *  does not have the same subscript index.
 *
 *  assignAllNodeSSA should change all the subscript in the node
 *  to a higher index when the same subscript was found in
 *  another group.
 *
 **********************************************************/
void assignAllNodeSSA(Node* inputNode, LinkedList* liveList, LinkedList* prevList){
  inputNode->visitFlag = 1;
  ListElement* livePtr = liveList->head;
  ListElement* prevPtr = prevList->head;
  int subsName, i;
  Expression* newExpr;
  
  while(livePtr != NULL){
    subsName  = ((Subscript*)livePtr->node)->name;
    newExpr   = createExpression(subsName, COPY, subsName, 0, 0);
    
    while(((Subscript*)prevPtr->node)->name != subsName)
      prevPtr = prevPtr->next;
    
    newExpr->oprdA    = *(Subscript*)prevPtr->node;
    newExpr->id.index = ((Subscript*)livePtr->node)->index + 1;
    
    if(inputNode->parent != inputNode->imdDom){
      newExpr->oprdA.index ++;
      newExpr->id.index ++;
    }

    addListFirst(inputNode->block, newExpr);
    livePtr   = livePtr->next;
  }
  
  arrangeSSA(inputNode);
  updateList(inputNode, liveList);
  LinkedList* curList     = getModifiedList(inputNode);

  for(i=0; i < inputNode->numOfChild; i++){
    if(inputNode->children[i]->visitFlag != 1)
      assignAllNodeSSA(inputNode->children[i], liveList, curList);
  }
}


//TO PRINT ELEMENT IN A LINKEDLIST
  // ListElement* printPtr = inputNode->block->head;
  // Subscript* printScr;
  // printf("\n");
  // while(printPtr != NULL){
    // printScr   = &((Expression*)printPtr->node)->oprdA;
    // printf("%c%d->", printScr->name, printScr->index);
    // printPtr = printPtr->next;
  // }


