#include "customAssertion.h"
#include "unity.h"
#include "NodeChain.h"
#include "PhiFunction.h"
#include <stdio.h>
#include <stdlib.h>

void customTestAssertPhiFunction(Expression* expectedPhiFunction, Node** nodeToTest, int lineNumber){
  Expression* actualPhiFunc = (Expression*)(*nodeToTest)->block->expression->head->node;
  
  if(actualPhiFunc == NULL || expectedPhiFunction == NULL){
    CUSTOM_TEST_FAIL(lineNumber, "NULL input detected");
  }
  else{
    if(actualPhiFunc->opr != PHI_FUNC)
      CUSTOM_TEST_FAIL(lineNumber, "Expected PhiFunction but actual was %d", \
                        actualPhiFunc->opr);
                        
    if(actualPhiFunc->id != expectedPhiFunction->id)
      CUSTOM_TEST_FAIL(lineNumber, "Expected id is %c but actual was %c",     \
                        expectedPhiFunction->id, actualPhiFunc->id);
                        
    if(actualPhiFunc->subscrpt != expectedPhiFunction->subscrpt)
      CUSTOM_TEST_FAIL(lineNumber, "Expected subscript %d but actual was %d", \
                        expectedPhiFunction->subscrpt, actualPhiFunc->subscrpt);
                        
    if(actualPhiFunc->operand1 != expectedPhiFunction->operand1)
      CUSTOM_TEST_FAIL(lineNumber, "Expected operand1 as %d but actual was %d", \
                        expectedPhiFunction->operand1, actualPhiFunc->operand1);
                        
    if(actualPhiFunc->operand2 != expectedPhiFunction->operand2)
      CUSTOM_TEST_FAIL(lineNumber, "Expected operand2 as %d but actual was %d", \
                        expectedPhiFunction->operand2, actualPhiFunc->operand2);
                        
    if(actualPhiFunc->condition != expectedPhiFunction->condition)
      CUSTOM_TEST_FAIL(lineNumber, "Expected condition is %d but actual was %d", \
                        expectedPhiFunction->condition, actualPhiFunc->condition);
  
  }
}

  // if(actualNode == NULL){
    // CUSTOM_TEST_FAIL(lineNumber, "NULL input detected");
  // }
  // else{
    // if(actualNode->expression != expectedExpression){
      // CUSTOM_TEST_FAIL(lineNumber,"Expected expression to be %s was %s", expectedExpression, actualNode->expression);
    // }
    // if(actualNode->rank != expectedRank){
      // CUSTOM_TEST_FAIL(lineNumber,"Expected rank to be %d was %d", expectedRank, actualNode->rank);
    // }
    // if(actualNode->parent != expectedParent){
      // CUSTOM_TEST_FAIL(lineNumber, "Expected parent is 0x%X was 0x%X", expectedParent, actualNode->parent);
    // }
  // }
// }

void customTestAssertNodeAddress(Node* expectedNode, Node* actualNode, int lineNumber){
  if(!actualNode && expectedNode)
    CUSTOM_TEST_FAIL(lineNumber, "Expected Node was 0x%X but actual was NULL", expectedNode);
  
  if(actualNode && !expectedNode)
    CUSTOM_TEST_FAIL(lineNumber, "Expected Node was NULL but actual was 0x%X", actualNode);
  
  if(actualNode != expectedNode)
    CUSTOM_TEST_FAIL(lineNumber, "Expected Node pointer point to 0x%X but actual was 0x%X", expectedNode, actualNode);
}

/**
 * compare the address of every list elements and the number of elements contained between two list.
 */
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