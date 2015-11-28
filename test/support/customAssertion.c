#include "customAssertion.h"
#include "unity.h"
#include "NodeChain.h"
#include <stdio.h>
#include <stdlib.h>

// void customTestAssertNodeData(int expRank, Block* expBlock, , Node* expParent, Node* actualNode, int lineNumber){

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
      CUSTOM_TEST_FAIL(lineNumber, "Expected the list elements was %d actual was %d.", tempExpectedElem, tempActualElem);

    tempExpectedElem = tempExpectedElem->next;
    tempActualElem = tempActualElem->next;
  }
} 