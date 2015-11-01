#include "unity.h"
#include "NodeChain.h"
#include "ErrorObject.h"
#include "customAssertion.h"
#include "CException.h"
// TEST_ASSERT_NODE_DATA(char* expectExpress, int expectRank, Node* expectParent, Node* actualNode)
// TEST_ASSERT_NODE_DATA use to test internal data of the Node, not necessary to be the same node

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
  TEST_ASSERT_NODE_DATA("3 * 4", 0, NULL, testNode); 
}

void test_findUnion_given_NULL_Node_should_Throw_ERR_NULL_NODE(void){
  ErrorObject* err;
  Try{
    Node* testNode = findUnion(NULL, createNode("3 + 4", 0));
    TEST_FAIL_MESSAGE("Expecting ERR_NULL_NODE but none thrown.");
  }Catch(err){
    TEST_ASSERT_EQUAL(ERR_NULL_NODE, err->errorCode);
    TEST_ASSERT_EQUAL_STRING("Input node cannot be NULL!",  err->errorMsg);
    freeError(err);
  }
}

void test_findUnion_given_root_should_Throw_ERR_NO_PARENT(void){
  ErrorObject* err;
  Node* parentNode  = createNode("3 + 4", 0);
  Node* childNode   = createNode("2 * 5", 1);
  childNode->parent = parentNode;
  Try{
    Node* testNode = findUnion(childNode, parentNode);
    TEST_FAIL_MESSAGE("Expecting ERR_NO_PARENT but none thrown.");
  }Catch(err){
    TEST_ASSERT_EQUAL(ERR_NO_PARENT, err->errorCode);
    TEST_ASSERT_EQUAL_STRING("Input node does not have a parent!",  err->errorMsg);
    freeError(err);
  }
}

/**
 *  fingUnion should find the Union Node that split the two node
 *
 *        [p]
 *        / \
 *      [a] [b]
 */
void test_findUnion_given_two_rank_1_Node_should_return_their_union_node(void){
  Node* parentNode = createNode("p", 0);
  Node* childNodeA = createNode("a", 1);
  Node* childNodeB = createNode("b", 1);
  childNodeA->parent = parentNode;
  childNodeB->parent = parentNode;
  
  Node* testNode  = findUnion(childNodeA, childNodeB);
  TEST_ASSERT_NODE_ADDRESS(parentNode, testNode);
}

/**
 *  fingUnion should find the Union Node that split the two node
 *
 *        [p]
 *        / \
 *      [a] [b]
 *           |
 *          [c]
 */
void test_findUnion_given_two_different_rank_Node_should_return_their_union_node(void){
  Node* parentNode = createNode("p", 0);
  Node* childNodeA = createNode("a", 1);
  Node* childNodeB = createNode("b", 1);
  Node* childNodeC = createNode("c", 2);
  childNodeA->parent = parentNode;
  childNodeB->parent = parentNode;
  childNodeC->parent = childNodeB;
  
  Node* testNode  = findUnion(childNodeA, childNodeC);
  TEST_ASSERT_NODE_ADDRESS(parentNode, testNode);
}
