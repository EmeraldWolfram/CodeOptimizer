#include "unity.h"
#include "NodeChain.h"
#include "ErrorObject.h"
#include "customAssertion.h"
#include "LinkedList.h"
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

/**
 *  setParent will assign trueParent to all the node in the tree
 *  This test test assign of true parent for the following tree
 *
 *      [parent]
 *         |
 *        [B]
 *
 **/
void test_setParent_with_one_child_only(void){
  Node* parentNode  = createNode(0);
  Node* childNodeB  = createNode(1);

  addChild(&parentNode, &childNodeB);
  
  setParent(&parentNode);
  
  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeB->trueParent);
}

/**
 *  setParent will assign trueParent to all the node in the tree
 *  This test test assign of true parent for the following tree
 *
 *      [parent]
 *        /  \
 *      [B]  [C]
 *
 **/
void test_setParent_with_two_child_only(void){
  Node* parentNode  = createNode(0);
  Node* childNodeB  = createNode(1);
  Node* childNodeC  = createNode(1);

  addChild(&parentNode, &childNodeB);
  addChild(&parentNode, &childNodeC);
  
  setParent(&parentNode);
  
  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeB->trueParent);
  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeC->trueParent);
}

/**
 *  setParent will assign trueParent to all the node in the tree
 *  This test test assign of true parent for the following tree
 *  TreeA
 *
 *     [parent]
 *       /  \
 *     [B]  [C]
 *       \  / 
 *       [D] 
 *
 **/
void test_setParent_with_treeA_shown_above(void){
  Node* parentNode  = createNode(0);
  Node* childNodeB  = createNode(1);
  Node* childNodeC  = createNode(1);
  Node* childNodeD  = createNode(2);

  addChild(&parentNode, &childNodeB);
  addChild(&parentNode, &childNodeC);
  addChild(&childNodeB, &childNodeD);
  addChild(&childNodeC, &childNodeD);
  
  setParent(&parentNode);
  
  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeB->trueParent);
  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeC->trueParent);
  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeD->trueParent);
}

/**
 *  setParent will assign trueParent to all the node in the tree
 *  This test test assign of true parent for the following tree
 *  TreeB
 *
 *     [parent]
 *      /   \
 *    [B]   [C]
 *      \   / \
 *       [D]  [E]
 *
 **/
void test_setParent_with_treeB_shown_above(void){
  Node* parentNode  = createNode(0);
  Node* childNodeB  = createNode(1);
  Node* childNodeC  = createNode(1);
  Node* childNodeD  = createNode(2);
  Node* childNodeE  = createNode(2);

  addChild(&parentNode, &childNodeB);
  addChild(&parentNode, &childNodeC);
  addChild(&childNodeB, &childNodeD);
  addChild(&childNodeC, &childNodeD);
  addChild(&childNodeC, &childNodeE);
  
  setParent(&parentNode);
  
  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeB->trueParent);
  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeC->trueParent);
  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeD->trueParent);
  TEST_ASSERT_NODE_ADDRESS(childNodeC, childNodeE->trueParent);
}

/**
 *  setParent will assign trueParent to all the node in the tree
 *  This test test assign of true parent for the following tree
 *  TreeC
 *
 *     [parent]
 *      /   \
 *    [B]   [C]
 *    / \   /
 *  [E]  [D]
 *   |    |
 *  [F]  [G]
 *   \   /
 *    [H]
 **/
void test_setParent_with_treeC_shown_above(void){
  Node* parentNode  = createNode(0);
  Node* childNodeB  = createNode(1);
  Node* childNodeC  = createNode(1);
  Node* childNodeD  = createNode(2);
  Node* childNodeE  = createNode(2);
  Node* childNodeF  = createNode(3);
  Node* childNodeG  = createNode(3);
  Node* childNodeH  = createNode(4);

  addChild(&parentNode, &childNodeB);
  addChild(&parentNode, &childNodeC);
  addChild(&childNodeB, &childNodeE);
  addChild(&childNodeB, &childNodeD);
  addChild(&childNodeC, &childNodeD);
  addChild(&childNodeE, &childNodeF);
  addChild(&childNodeD, &childNodeG);
  addChild(&childNodeF, &childNodeH);
  addChild(&childNodeG, &childNodeH);
  
  setParent(&parentNode);
  
  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeB->trueParent);
  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeC->trueParent);
  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeD->trueParent);
  TEST_ASSERT_NODE_ADDRESS(childNodeB, childNodeE->trueParent);
  TEST_ASSERT_NODE_ADDRESS(childNodeB, childNodeF->trueParent);
  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeG->trueParent);
  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeH->trueParent);
}



