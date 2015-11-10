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
