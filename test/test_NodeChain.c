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
  TEST_ASSERT_EQUAL(0, testNode->numOfChild);
  TEST_ASSERT_EQUAL(0, testNode->numOfDom);
  TEST_ASSERT_EQUAL(0, testNode->numOfParent);
  TEST_ASSERT_NULL(testNode->parent);
  TEST_ASSERT_NULL(testNode->lastBrhDom);
  TEST_ASSERT_NULL(testNode->imdDom);
  TEST_ASSERT_NULL(testNode->children);
  TEST_ASSERT_NULL(testNode->doms);
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

  TEST_ASSERT_EQUAL(1, parentNode->numOfChild);
  TEST_ASSERT_NODE_ADDRESS(childNode, parentNode->children[0]);
  TEST_ASSERT_EQUAL(1, childNode->numOfParent);
  TEST_ASSERT_NODE_ADDRESS(parentNode, childNode->parent);
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

  addChild(&parentNode, &childNodeB);
  addChild(&parentNode, &childNodeC);

  TEST_ASSERT_EQUAL(2, parentNode->numOfChild);
  TEST_ASSERT_NODE_ADDRESS(childNodeB, parentNode->children[0]);
  TEST_ASSERT_NODE_ADDRESS(childNodeC, parentNode->children[1]);
  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeB->parent);
  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeC->parent);
}

/***************************************
 *  Forming the following tree with addChild function only
 *
 *           [parent]
 *            /   \
 *          [B]   [C]
 *            \   /
 *             [D]
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

  TEST_ASSERT_EQUAL(2, parentNode->numOfChild);
  TEST_ASSERT_NODE_ADDRESS(childNodeB, parentNode->children[0]);
  TEST_ASSERT_NODE_ADDRESS(childNodeC, parentNode->children[1]);
  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeB->parent);
  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeC->parent);
  TEST_ASSERT_EQUAL(1, childNodeB->numOfChild);
  TEST_ASSERT_NODE_ADDRESS(childNodeD, childNodeB->children[0]);
  TEST_ASSERT_EQUAL(1, childNodeC->numOfChild);
  TEST_ASSERT_NODE_ADDRESS(childNodeD, childNodeC->children[0]);
  TEST_ASSERT_EQUAL(2, childNodeD->numOfParent);
  TEST_ASSERT_NODE_ADDRESS(childNodeC, childNodeD->parent);
}

/**
 *  setParent will assign lastBrhDom to all the node in the tree
 *  This test test assign of true parent for the following tree
 *
 *      [parent]
 *         |
 *        [B]
 *
 **/
void test_setLastBrhDom_with_one_child_only(void){
  Node* parentNode  = createNode(0);
  Node* childNodeB  = createNode(1);

  addChild(&parentNode, &childNodeB);

  setLastBrhDom(&parentNode);

  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeB->lastBrhDom);
}

/**
 *  setParent will assign lastBrhDom to all the node in the tree
 *  This test test assign of lastBrhDom for the following tree
 *
 *      [parent]
 *        /  \
 *      [B]  [C]
 *
 **/
void test_setLastBrhDom_with_two_child_only(void){
  Node* parentNode  = createNode(0);
  Node* childNodeB  = createNode(1);
  Node* childNodeC  = createNode(1);

  addChild(&parentNode, &childNodeB);
  addChild(&parentNode, &childNodeC);

  setLastBrhDom(&parentNode);

  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeB->lastBrhDom);
  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeC->lastBrhDom);
}

/**
 *  setParent will assign lastBrhDom to all the node in the tree
 *  This test test assign of lastBrhDom for the following tree
 *  TreeA
 *
 *     [parent]
 *       /  \
 *     [B]  [C]
 *       \  /
 *       [D]
 *
 **/
void test_setLastBrhDom_with_treeA_shown_above(void){
  Node* parentNode  = createNode(0);
  Node* childNodeB  = createNode(1);
  Node* childNodeC  = createNode(1);
  Node* childNodeD  = createNode(2);

  addChild(&parentNode, &childNodeB);
  addChild(&parentNode, &childNodeC);
  addChild(&childNodeB, &childNodeD);
  addChild(&childNodeC, &childNodeD);

  setLastBrhDom(&parentNode);

  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeB->lastBrhDom);
  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeC->lastBrhDom);
  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeD->lastBrhDom);
}

/**
 *  setParent will assign lastBrhDom to all the node in the tree
 *  This test test assign of lastBrhDom for the following tree
 *  TreeB
 *
 *     [parent]
 *      /   \
 *    [B]   [C]
 *      \   / \
 *       [D]  [E]
 *
 **/
void test_setLastBrhDom_with_treeB_shown_above(void){
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

  setLastBrhDom(&parentNode);

  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeB->lastBrhDom);
  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeC->lastBrhDom);
  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeD->lastBrhDom);
  TEST_ASSERT_NODE_ADDRESS(childNodeC, childNodeE->lastBrhDom);
}

/**
 *  setParent will assign lastBrhDom to all the node in the tree
 *  This test test assign of lastBrhDom for the following tree
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
void test_setLastBrhDom_with_treeC_shown_above(void){
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

  setLastBrhDom(&parentNode);

  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeB->lastBrhDom);
  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeC->lastBrhDom);
  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeD->lastBrhDom);
  TEST_ASSERT_NODE_ADDRESS(childNodeB, childNodeE->lastBrhDom);
  TEST_ASSERT_NODE_ADDRESS(childNodeB, childNodeF->lastBrhDom);
  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeG->lastBrhDom);
  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeH->lastBrhDom);
}



