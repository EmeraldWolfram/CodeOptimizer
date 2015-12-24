#include "ExpressionBlock.h"
#include <stdlib.h>
#include <stdio.h>


Expression* createExpression(int thisID, Operator oprt, int oprdA,\
                            int oprdB, int condt){
  Expression* newExp = malloc(sizeof(Expression));
  
  newExp->id.name    = thisID;
  newExp->id.index    = 0;
  newExp->opr        = oprt;
  newExp->oprdA.name = oprdA;
  newExp->oprdA.index = 0;
  newExp->oprdB.name = oprdB;
  newExp->oprdB.index = 0;
  newExp->condition  = condt;

  return newExp;            
}


/*
 *  getSubsList is a simple function that extract all the
 *  variable in all the expressions in the Node into a LinkedList.
 *
 *  The function take the input LinkedList* expression that contain
 *  all the expression in the Node.
 */
LinkedList* getSubsList(LinkedList* expression){
  LinkedList* subsList = createLinkedList();
  ListElement* exprPtr = expression->head;
  
  while(exprPtr != NULL){
    if(((Expression*)exprPtr->node)->opr != IF_STATEMENT){
      if(((Expression*)exprPtr->node)->opr != ASSIGN){
        addListLast(subsList, &((Expression*)exprPtr->node)->oprdA);
        if(((Expression*)exprPtr->node)->opr != COPY)
        addListLast(subsList, &((Expression*)exprPtr->node)->oprdB); 
      }
      addListLast(subsList, &((Expression*)exprPtr->node)->id);
    }
    exprPtr = exprPtr->next;
  }
  
  return subsList;
}

/*
 *  getLargestIndex
 *  
 *  find the largest number of the input argument subscript
 *  in the exprList
 *
 */
Subscript* getLargestIndex(LinkedList* exprList, Subscript* subsName){
  ListElement *checkPtr, *resultPtr;
  
  checkPtr  = exprList->head;
  while(checkPtr != NULL){
    if(((Subscript*)checkPtr->node)->name == subsName->name)
        resultPtr = checkPtr;
    checkPtr = checkPtr->next;
  }
  
  if(resultPtr == NULL)
    return 0;
  
  return &((Expression*)resultPtr->node)->id;
}

/*
 *  arrangeSSA take in the inputNode and arrange all the expression
 *  in the Node to the correct subscript.
 *  Eg.
 *    x0 = x0 + x0
 *  
 *  arrangeSSA should arrange the equation above to
 *  x1 = x0 + x0
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


LinkedList* getLiveList(Node* inputNode, LinkedList* prevLiveList){
  LinkedList* newLiveList = createLinkedList();
  LinkedList* checkList   = createLinkedList();
  LinkedList* expList     = inputNode->block;
  
  ListElement *newPtr, *exprPtr, *checkPtr;
  int validFlag;
  exprPtr   = expList->head;
  
  /***********************************************************
   * Form newLiveList with the living variable in this block
   ***********************************************************/
  while(exprPtr != NULL){
    checkPtr = newLiveList->head;
    while(checkPtr != NULL && ((Subscript*)checkPtr->node)->name \
          != ((Expression*)exprPtr->node)->id.name){
      checkPtr = checkPtr->next;
    }
    if(checkPtr != NULL)
      exprPtr = exprPtr->next;
    else{
      addListLast(newLiveList, &((Expression*)exprPtr->node)->id);
    }
  }
  /******************************************************
   *  Find the correct subscript index and assign to it
   ******************************************************/
  newPtr = newLiveList->head;
  while(newPtr != NULL){
    newPtr->node = getLargestIndex(expList, (Subscript*)newPtr->node);
    newPtr = newPtr->next;
  }
  
  /******************************************************
   *  Update the prevLiveList with the newLiveList
   ******************************************************/
  newPtr    = newLiveList->head;
  while(newPtr != NULL){
    checkPtr  = prevLiveList->head;
    while(checkPtr != NULL && ((Subscript*)checkPtr->node)->name != ((Subscript*)newPtr->node)->name)
      checkPtr = checkPtr->next;
  
    if(checkPtr == NULL)
      addListLast(prevLiveList, (Subscript*)newPtr->node);
    else{
      checkPtr->node = newPtr->node;
    }
    
    newPtr = newPtr->next;
  }
  
  return prevLiveList;
}


/*  Traverse down every single node to make sure all the node
 *  does not have the same subscript index.
 *
 *  assignAllNodeSSA should change all the subscript in the node
 *  to a higher index when the same subscript was found in
 *  another group.
 *
 **********************************************************/
void assignAllNodeSSA(Node* inputNode, LinkedList* liveList){
  inputNode->visitFlag = 1;
  ListElement* livePtr = liveList->head;
  int subsName, i;
  Expression* newExpr;
  
  while(livePtr != NULL){
    subsName  = ((Subscript*)livePtr->node)->name;
    newExpr   = createExpression(subsName, COPY, subsName, 0, 0);
    newExpr->oprdA  = *(Subscript*)livePtr->node;

    addListFirst(inputNode->block, newExpr);
    livePtr   = livePtr->next;
  }
  
  arrangeSSA(inputNode);
  LinkedList* myLiveList = getLiveList(inputNode, liveList);

  for(i=0; i < inputNode->numOfChild; i++){
    if(inputNode->children[i]->visitFlag != 1)
      assignAllNodeSSA(inputNode->children[i], myLiveList);
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


