#include "customAssertion.h"
#include "unity.h"
#include "NodeChain.h"
#include <stdio.h>
#include <stdlib.h>

void customTestAssertNode(char* expectedExpression, int expectedRank, Node* expectedParent, Node* actualNode, int lineNumber){

  if(actualNode == NULL){
    CUSTOM_TEST_FAIL(lineNumber, "NULL input detected");
  }
  else{
    if(actualNode->expression != expectedExpression){
      CUSTOM_TEST_FAIL(lineNumber,"Expected expression to be %s was %s", expectedExpression, actualNode->expression);
    }
    if(actualNode->rank != expectedRank){
      CUSTOM_TEST_FAIL(lineNumber,"Expected rank to be %d was %d", expectedRank, actualNode->rank);
    }
    if(actualNode->parent != expectedParent){
      CUSTOM_TEST_FAIL(lineNumber, "Expected parent is 0x%X was 0x%X", expectedParent, actualNode->parent);
    }
  }
}

void customTestAssertChain(Node* expectedChain, Node* actualChain, int lineNumber){
  if(actualChain == NULL || expectedChain == NULL){
    CUSTOM_TEST_FAIL(lineNumber, "NULL input detected");
  }
  else{
    if(actualChain != expectedChain)
      CUSTOM_TEST_FAIL(lineNumber, "Expected Node pointer point to 0x%X was 0x%X", expectedChain, actualChain);
  }
}
