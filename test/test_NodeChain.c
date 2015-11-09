#include "unity.h"
#include "NodeChain.h"
#include "ErrorObject.h"
#include "customAssertion.h"
#include "CException.h"
// TEST_ASSERT_NODE_DATA(char* expectExpress, int expectRank, Node* expectParent, Node* actualNode)
// TEST_ASSERT_NODE_DATA use to test internal data of the Node, not necessary to be the same node

void setUp(void){}

void tearDown(void){}

void test_createBlock(void){
	Block* testBlock = createBlock("Hello World", 3);
  
  TEST_ASSERT_NOT_NULL(testBlock);
  TEST_ASSERT_EQUAL(3, testBlock->data);
  TEST_ASSERT_EQUAL("Hello World", testBlock->string);
}

void test_createNode(void){
	Node* testNode = createNode(0);

  TEST_ASSERT_NOT_NULL(testNode);
  TEST_ASSERT_EQUAL(0, testNode->rank); 
  TEST_ASSERT_EQUAL(0, testNode->numberOfChild); 
  TEST_ASSERT_EQUAL(0, testNode->numberOfParent); 
  TEST_ASSERT_NULL(testNode->trueParent); 
  TEST_ASSERT_NULL(testNode->imdParent); 
  TEST_ASSERT_NULL(testNode->imdChild); 
}
/***************************************
 *  Given Node parentNode (a) and childNode (b)
 *  addChild should link them as shown below
 *
 *          [a] rank 0
 *           |
 *          [b] rank 1
 ***************************************/
void test_addChild_given_parentNode_and_childNode_should_link_it_as_tree_above(void){
  Node* parentNode  = createNode(0);
  Node* childNode   = createNode(1);
  
  addChild(&parentNode, &childNode);
  
  TEST_ASSERT_EQUAL(1, parentNode->numberOfChild);
  TEST_ASSERT_NODE_ADDRESS(childNode, parentNode->imdChild[0]);
  TEST_ASSERT_EQUAL(1, childNode->numberOfParent);
  TEST_ASSERT_NODE_ADDRESS(parentNode, childNode->imdParent[0]); 
}

/***************************************
 *  Given Node parentNode (a) with child (b) add childNode (c)
 *  addChild should link them as shown below
 *
 *    [a]      [a] rank 0
 *     |  >>   / \
 *    [b]    [b] [c] rank 1
 ***************************************/
void test_addChild_given_parentNode_with_childB_and_childNodeC_should_link_it_as_tree_above(void){
  Node* parentNode  = createNode(0);
  Node* childNodeB  = createNode(1);
  Node* childNodeC  = createNode(1);
  
  Node* childArr[1] = {childNodeB};
  Node* parentArr[1]= {parentNode};
  
  parentNode->numberOfChild = 1;
  parentNode->imdChild      = childArr;
  childNodeB->numberOfParent  = 1;
  childNodeB->imdParent       = parentArr;
  
  addChild(&parentNode, &childNodeC);
  
  TEST_ASSERT_EQUAL(2, parentNode->numberOfChild);
  TEST_ASSERT_NODE_ADDRESS(childNodeB, parentNode->imdChild[0]);
  TEST_ASSERT_NODE_ADDRESS(childNodeC, parentNode->imdChild[1]);
  TEST_ASSERT_EQUAL(1, childNodeC->numberOfParent);
  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeC->imdParent[0]);
}

/***************************************
 *  Forming the following tree with addChild function only
 *
 *           [parent]
 *            /    \
 *          [B]    [C]
 *            \    /
 *              [D]
 ***************************************/
void test_addChild_given_parentNode_add_3_childNode_should_link_it_as_tree_above(void){
  Node* parentNode  = createNode(0);
  Node* childNodeB  = createNode(1);
  Node* childNodeC  = createNode(1);
  Node* childNodeD  = createNode(2);
  
  addChild(&parentNode, &childNodeB);
  addChild(&parentNode, &childNodeC);
  addChild(&childNodeB, &childNodeD);
  addChild(&childNodeC, &childNodeD);
  
  TEST_ASSERT_EQUAL(2, parentNode->numberOfChild);
  TEST_ASSERT_NODE_ADDRESS(childNodeB, parentNode->imdChild[0]);
  TEST_ASSERT_NODE_ADDRESS(childNodeC, parentNode->imdChild[1]);
  TEST_ASSERT_EQUAL(1, childNodeB->numberOfParent);
  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeB->imdParent[0]); 
  TEST_ASSERT_EQUAL(1, childNodeC->numberOfParent);
  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeC->imdParent[0]); 
  TEST_ASSERT_EQUAL(2, childNodeD->numberOfParent);
  TEST_ASSERT_NODE_ADDRESS(childNodeB, childNodeD->imdParent[0]); 
  TEST_ASSERT_NODE_ADDRESS(childNodeC, childNodeD->imdParent[1]); 
  TEST_ASSERT_EQUAL(0, childNodeD->numberOfChild);
}

// void test_findUnion_given_NULL_Node_should_Throw_ERR_NULL_NODE(void){
  // ErrorObject* err;
  // Try{
    // Node* testNode = findUnion(NULL, createNode("3 + 4", 0));
    // TEST_FAIL_MESSAGE("Expecting ERR_NULL_NODE but none thrown.");
  // }Catch(err){
    // TEST_ASSERT_EQUAL(ERR_NULL_NODE, err->errorCode);
    // TEST_ASSERT_EQUAL_STRING("Input node cannot be NULL!",  err->errorMsg);
    // freeError(err);
  // }
// }

// void test_findUnion_given_root_should_Throw_ERR_NO_PARENT(void){
  // ErrorObject* err;
  // Node* parentNode  = createNode("3 + 4", 0);
  // Node* childNode   = createNode("2 * 5", 1);
  // childNode->parent = parentNode;
  // Try{
    // Node* testNode = findUnion(childNode, parentNode);
    // TEST_FAIL_MESSAGE("Expecting ERR_NO_PARENT but none thrown.");
  // }Catch(err){
    // TEST_ASSERT_EQUAL(ERR_NO_PARENT, err->errorCode);
    // TEST_ASSERT_EQUAL_STRING("Input node does not have a parent!",  err->errorMsg);
    // freeError(err);
  // }
// }

/**
 *  fingUnion should find the Union Node that split the two node
 *
 *        [p]
 *        / \
 *      [a] [b]
 */
// void test_findUnion_given_two_rank_1_Node_should_return_their_union_node(void){
  // Node* parentNode = createNode("p", 0);
  // Node* childNodeA = createNode("a", 1);
  // Node* childNodeB = createNode("b", 1);
  // childNodeA->parent = parentNode;
  // childNodeB->parent = parentNode;
  
  // Node* testNode  = findUnion(childNodeA, childNodeB);
  // TEST_ASSERT_NODE_ADDRESS(parentNode, testNode);
// }

/**
 *  fingUnion should find the Union Node that split the two node
 *
 *        [p]
 *        / \
 *      [a] [b]
 *           |
 *          [c]
 */
// void test_findUnion_given_two_different_rank_Node_should_return_their_union_node(void){
  // Node* parentNode = createNode("p", 0);
  // Node* childNodeA = createNode("a", 1);
  // Node* childNodeB = createNode("b", 1);
  // Node* childNodeC = createNode("c", 2);
  // childNodeA->parent = parentNode;
  // childNodeB->parent = parentNode;
  // childNodeC->parent = childNodeB;
  
  // Node* testNode  = findUnion(childNodeA, childNodeC);
  // TEST_ASSERT_NODE_ADDRESS(parentNode, testNode);
// }
