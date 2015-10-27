#include "unity.h"
#include "NodeChain.h"
#include "customAssertion.h"

// TEST_ASSERT_EQUAL_NODE(char* expectExpress, int expectRank, Node* expectParent, Node* actualNode)
// TEST_ASSERT_EQUAL_NODE use to test internal data of the Node, not necessary to be the same node

void setUp(void){}

void tearDown(void){}

void test_createNodeChain(void){
	NodeChain* testChain = createNodeChain();
  
  TEST_ASSERT_NOT_NULL(testChain);
  TEST_ASSERT_EQUAL(0, testChain->length);
  TEST_ASSERT_NULL(testChain->hook);
  TEST_ASSERT_NULL(testChain->tail);
}

void test_createNode(void){
	Node* testNode = createNode("3 * 4", 0);
  
  TEST_ASSERT_NOT_NULL(testNode);
  TEST_ASSERT_EQUAL_NODE("3 * 4", 0, NULL, testNode); 
}