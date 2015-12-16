#include "customAssertion.h"
#include "unity.h"
#include "NodeChain.h"
#include "PhiFunction.h"
#include <stdio.h>
#include <stdlib.h>

void customTestAssertPhiFunction(Expression* expectedPhiFunction, Node** nodeToTest, int lineNumber){
  Expression* actualPhiFunc = (Expression*)(*nodeToTest)->block->head->node;
  
  if(actualPhiFunc == NULL || expectedPhiFunction == NULL){
    CUSTOM_TEST_FAIL(lineNumber, "NULL input detected");
  }
  else{
    if(actualPhiFunc->opr != PHI_FUNC)
      CUSTOM_TEST_FAIL(lineNumber, "Expected PhiFunction but actual was %d", \
                        actualPhiFunc->opr);
                        
    if(actualPhiFunc->id.name != expectedPhiFunction->id.name)
      CUSTOM_TEST_FAIL(lineNumber, "Expected id is %c but actual was %c",     \
                        expectedPhiFunction->id.name, actualPhiFunc->id.name);
                        
    if(actualPhiFunc->id.subs != expectedPhiFunction->id.subs)
      CUSTOM_TEST_FAIL(lineNumber, "Expected ID subscript %d but actual was %d", \
                        expectedPhiFunction->id.subs, actualPhiFunc->id.subs);
                        
    if(actualPhiFunc->oprdA.name != expectedPhiFunction->oprdA.name)
      CUSTOM_TEST_FAIL(lineNumber, "Expected operand A as %c but actual was %c", \
                        expectedPhiFunction->oprdA, actualPhiFunc->oprdA);
                        
    if(actualPhiFunc->oprdB.name != expectedPhiFunction->oprdB.name)
      CUSTOM_TEST_FAIL(lineNumber, "Expected operand B as %c but actual was %c", \
                        expectedPhiFunction->oprdB, actualPhiFunc->oprdB);
                        
    if(actualPhiFunc->condition != expectedPhiFunction->condition)
      CUSTOM_TEST_FAIL(lineNumber, "Expected condition is %d but actual was %d", \
                        expectedPhiFunction->condition, actualPhiFunc->condition);
  
  }
}
//**********************************************************************************************
//**********************************************************************************************
void customTestAssertNodeAddress(Node* expectedNode, Node* actualNode, int lineNumber){
  if(!actualNode && expectedNode)
    CUSTOM_TEST_FAIL(lineNumber, "Expected Node was 0x%X but actual was NULL", expectedNode);
  
  if(actualNode && !expectedNode)
    CUSTOM_TEST_FAIL(lineNumber, "Expected Node was NULL but actual was 0x%X", actualNode);
  
  if(actualNode != expectedNode)
    CUSTOM_TEST_FAIL(lineNumber, "Expected Node pointer point to 0x%X but actual was 0x%X", expectedNode, actualNode);
}

/************************************************************************************************
 * compare the address of every list elements and the number of elements contained between two list.
 ************************************************************************************************/
void customTestAssertLinkedList(LinkedList* expectedList, LinkedList* actualList, int lineNumber){
  
  int i = 1;
  if(!expectedList->head && !actualList->head)
    return;
  
  if(!expectedList->head && actualList->head)
    CUSTOM_TEST_FAIL(lineNumber, "Expected the list was NULL actual was 0x%X.", actualList );

  if(expectedList->head && !actualList->head)
    CUSTOM_TEST_FAIL(lineNumber, "Expected the list was 0x%X actual was NULL.", expectedList );

  if(expectedList->length != actualList->length)
    CUSTOM_TEST_FAIL(lineNumber, "Expected the number of elements in list was %d actual was %d.", expectedList->length, actualList->length);

  ListElement* tempExpectedElem = expectedList->head;
  ListElement* tempActualElem = actualList->head;
  
  while(tempExpectedElem && tempActualElem)
  {
    if(tempExpectedElem->node != tempActualElem->node)
      CUSTOM_TEST_FAIL(lineNumber, "Expected the [%d] elements was %d actual was %d.", i, tempExpectedElem, tempActualElem);

    tempExpectedElem = tempExpectedElem->next;
    tempActualElem = tempActualElem->next;
    
    i++;
  }
} 
//*******************************************************************************************
void customTestAssertSubscript(int expectName, int expectSub, Subscript* actual, int lineNumber){
  if(actual->name != expectName)
    CUSTOM_TEST_FAIL(lineNumber, "Expected ID %c but actual was %c", expectName, actual->name);
  
  if(actual->subs != expectSub)
    CUSTOM_TEST_FAIL(lineNumber, "Expected subscript no. %d was %d", expectSub, actual->subs);
  
  
  
}
