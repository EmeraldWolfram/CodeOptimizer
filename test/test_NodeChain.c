#include "unity.h"
#include "NodeChain.h"
#include "ErrorObject.h"
#include "customAssertion.h"
#include "LinkedList.h"
#include "CException.h"
#include <stdlib.h>
// TEST_ASSERT_NODE_DATA(char* expectExpress, int expectRank, Node* expectParent, Node* actualNode)
// TEST_ASSERT_NODE_DATA use to test internal data of the Node, not necessary to be the same node

void setUp(void){}

void tearDown(void){}

void test_createExpression(void){
  Expression* addExpress = createExpression('x',3,NORMAL_OPERATOR,1, 2, 0);
  
  TEST_ASSERT_EQUAL('x', addExpress->id);
  TEST_ASSERT_EQUAL(3, addExpress->subscrpt);
  TEST_ASSERT_EQUAL(NORMAL_OPERATOR, addExpress->opr);
  TEST_ASSERT_EQUAL(1, addExpress->operand1);
  TEST_ASSERT_EQUAL(2, addExpress->operand2);
  TEST_ASSERT_EQUAL(0, addExpress->condition);
}

void test_createBlock(void){
  LinkedList* nullList = malloc(sizeof(LinkedList));
	Block* testBlock = createBlock(nullList);

  TEST_ASSERT_NOT_NULL(testBlock);
  TEST_ASSERT_EQUAL_PTR(nullList, testBlock->expression);
}

void test_createNode(void){
	Node* testNode = createNode(0);

  TEST_ASSERT_NOT_NULL(testNode);
  TEST_ASSERT_EQUAL(0, testNode->rank);
  TEST_ASSERT_EQUAL(0, testNode->numOfChild);
  TEST_ASSERT_EQUAL(0, testNode->numOfDom);
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


/**
 *  setParent will assign lastBrhDom to all the node in the tree
 *  This test test assign of lastBrhDom for the following tree
 *  TreeC
 *
 *        [A]
 *       /   \
 *    [B]    [C]
 *    / \     |
 *  [D] [E]  [F]
 *  /  \ /    |
 *[G]--[H]---[I]
 * |    ^
 * |    ^
 *[J]--[K]
 *
 **/
void test_setLastBrhDom_with_treeD_shown_above(void){
  Node* parentNode  = createNode(0);
  Node* childNodeB  = createNode(1);
  Node* childNodeC  = createNode(1);
  Node* childNodeD  = createNode(2);
  Node* childNodeE  = createNode(2);
  Node* childNodeF  = createNode(2);
  Node* childNodeG  = createNode(3);
  Node* childNodeH  = createNode(3);
  Node* childNodeI  = createNode(3);
  Node* childNodeJ  = createNode(4);
  Node* childNodeK  = createNode(5);

  addChild(&parentNode, &childNodeB);
  addChild(&parentNode, &childNodeC);
  addChild(&childNodeB, &childNodeE);
  addChild(&childNodeB, &childNodeD);
  addChild(&childNodeC, &childNodeF);
  addChild(&childNodeD, &childNodeG);
  addChild(&childNodeD, &childNodeH);
  addChild(&childNodeE, &childNodeH);
  addChild(&childNodeF, &childNodeI);
  addChild(&childNodeI, &childNodeH);
  addChild(&childNodeG, &childNodeH);
  addChild(&childNodeG, &childNodeJ);
  addChild(&childNodeJ, &childNodeK);
  addChild(&childNodeK, &childNodeH);

  setLastBrhDom(&parentNode);

  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeB->lastBrhDom);
  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeC->lastBrhDom);
  TEST_ASSERT_NODE_ADDRESS(childNodeB, childNodeD->lastBrhDom);
  TEST_ASSERT_NODE_ADDRESS(childNodeB, childNodeE->lastBrhDom);
  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeF->lastBrhDom);
  TEST_ASSERT_NODE_ADDRESS(childNodeD, childNodeG->lastBrhDom);
  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeH->lastBrhDom);
  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeI->lastBrhDom);
  TEST_ASSERT_NODE_ADDRESS(childNodeG, childNodeJ->lastBrhDom);
  TEST_ASSERT_NODE_ADDRESS(childNodeG, childNodeK->lastBrhDom);
}

/**       ControlFlowGraph1      s
 *
 *          | Entry
 *         \/
 *        [ A ]<<<<
 *       |   |    /\
 *      \/  \/    /\
 *     [B]  [C]   /\
 *      |    |    /\
 *     \/   \/    /\
 *       [D] >>>>>>>
 *
 **/
void test_setLastBrhDom_give_CFG1_should_assign_lastBranchDominator_of_each_node_respectively(void){
  Node* nodeA = createNode(0);
  Node* nodeB = createNode(1);
  Node* nodeC = createNode(1);
  Node* nodeD = createNode(2);

  addChild(&nodeA, &nodeB);
  addChild(&nodeA, &nodeC);
  addChild(&nodeB, &nodeD);
  addChild(&nodeC, &nodeD);
  addChild(&nodeD, &nodeA);

  setLastBrhDom(&nodeA);

  TEST_ASSERT_NODE_ADDRESS(nodeA, nodeB->lastBrhDom);
  TEST_ASSERT_NODE_ADDRESS(nodeA, nodeC->lastBrhDom);
  TEST_ASSERT_NODE_ADDRESS(nodeA, nodeD->lastBrhDom);
}

/**       ControlFlowGraph2
 *
 *          | Entry
 *         \/
 *        [ A ]
 *        / \
 *     [B]  [C]
 *      |    |
 * >>>>\/   \/
 * |   [D] [E]
 * <<<< \  /
 *       \/
 *      [F]
 *
 **/
void test_setLastBrhDom_give_CFG2_should_assign_lastBranchDominator_of_each_node_respectively(void){
  Node* nodeA = createNode(0);
  Node* nodeB = createNode(1);
  Node* nodeC = createNode(1);
  Node* nodeD = createNode(2);
  Node* nodeE = createNode(2);
  Node* nodeF = createNode(3);

  addChild(&nodeA, &nodeB);
  addChild(&nodeA, &nodeC);
  addChild(&nodeB, &nodeD);
  addChild(&nodeC, &nodeE);
  addChild(&nodeD, &nodeD);
  addChild(&nodeD, &nodeF);
  addChild(&nodeE, &nodeF);

  setLastBrhDom(&nodeA);

  TEST_ASSERT_NODE_ADDRESS(nodeA, nodeB->lastBrhDom);
  TEST_ASSERT_NODE_ADDRESS(nodeA, nodeC->lastBrhDom);
  TEST_ASSERT_NODE_ADDRESS(nodeA, nodeD->lastBrhDom);
  TEST_ASSERT_NODE_ADDRESS(nodeA, nodeE->lastBrhDom);
  TEST_ASSERT_NODE_ADDRESS(nodeA, nodeF->lastBrhDom);
}

//*********************************************************************
//********************TEST for getImdDom ******************************
//*********************************************************************
/**
 *  getImdDom will find imdDom of that node and return
 *
 *       TREE A:  ImdDom
 *      [parent]  {parent}
 *         |
 *        [B]     {parent}
 *         |
 *        [C]     {B}
 *
 **/
void test_getImdDom_given_nodeC_should_return_nodeB(void){
  Node* parentNode  = createNode(0);
  Node* childNodeB  = createNode(1);
  Node* childNodeC  = createNode(2);

  addChild(&parentNode, &childNodeB);
  addChild(&childNodeB, &childNodeC);
  setLastBrhDom(&parentNode);

  getImdDom(childNodeC);

  TEST_ASSERT_NODE_ADDRESS(childNodeB, childNodeC->imdDom);
}


/**
 *  getImdDom will find imdDom of that node and return
 *
 *       TREE A:  ImdDom
 *      [parent]  {parent}
 *       /    \
 *     [B]    [C] {parent}
 *       \   /
 *        [D]
 *
 *
 **/
void test_getImdDom_given_nodeD_should_return_parentNode(void){
  Node* parentNode  = createNode(0);
  Node* childNodeB  = createNode(1);
  Node* childNodeC  = createNode(2);
  Node* childNodeD  = createNode(3);

  addChild(&parentNode, &childNodeB);
  addChild(&parentNode, &childNodeC);
  addChild(&childNodeB, &childNodeD);
  addChild(&childNodeC, &childNodeD);
  setLastBrhDom(&parentNode);

  getImdDom(childNodeD);

  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeD->imdDom);
}

/**
 *  getImdDom will find imdDom of that node and return
 *
 *         [A]
 *        /   \
 *     [B]    [C]
 *     / \     |
 *   [D] [E]  [F]
 *  /  \ /    |
 *[G]->[H]<--[I]
 * |    ^
 * |    ^
 *[J]--[K]
 *
 **/
void test_getImdDom_given_nodeJ_and_nodeH_should_return_nodeG_and_node_A_respectively(void){
  Node* parentNode  = createNode(0);
  Node* childNodeB  = createNode(1);
  Node* childNodeC  = createNode(1);
  Node* childNodeD  = createNode(2);
  Node* childNodeE  = createNode(2);
  Node* childNodeF  = createNode(2);
  Node* childNodeG  = createNode(3);
  Node* childNodeH  = createNode(3);
  Node* childNodeI  = createNode(3);
  Node* childNodeJ  = createNode(4);
  Node* childNodeK  = createNode(5);

  addChild(&parentNode, &childNodeB);
  addChild(&parentNode, &childNodeC);
  addChild(&childNodeB, &childNodeE);
  addChild(&childNodeB, &childNodeD);
  addChild(&childNodeC, &childNodeF);
  addChild(&childNodeD, &childNodeG);
  addChild(&childNodeD, &childNodeH);
  addChild(&childNodeE, &childNodeH);
  addChild(&childNodeF, &childNodeI);
  addChild(&childNodeI, &childNodeH);
  addChild(&childNodeG, &childNodeH);
  addChild(&childNodeG, &childNodeJ);
  addChild(&childNodeJ, &childNodeK);
  addChild(&childNodeK, &childNodeH);
  setLastBrhDom(&parentNode);

  getImdDom(childNodeJ);
  getImdDom(childNodeH);

  TEST_ASSERT_NODE_ADDRESS(childNodeG, childNodeJ->imdDom);
  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeH->imdDom);
}

/**
 *  setAllImdDom will set all imdDom by iterative calling getImdDom function
 *
 *        [A]
 *       /   \
 *    [B]    [C]
 *    / \     |
 *  [D] [E]  [F]
 *  /  \ /    |
 *[G]--[H]---[I]
 * |    ^
 * |    ^
 *[J]--[K]
 *
 **/
void test_setAllImdDom_will_set_all_imdDom_to_every_Node(void){
  Node* parentNode  = createNode(0);
  Node* childNodeB  = createNode(1);
  Node* childNodeC  = createNode(1);
  Node* childNodeD  = createNode(2);
  Node* childNodeE  = createNode(2);
  Node* childNodeF  = createNode(2);
  Node* childNodeG  = createNode(3);
  Node* childNodeH  = createNode(3);
  Node* childNodeI  = createNode(3);
  Node* childNodeJ  = createNode(4);
  Node* childNodeK  = createNode(5);

  addChild(&parentNode, &childNodeB);
  addChild(&parentNode, &childNodeC);
  addChild(&childNodeB, &childNodeE);
  addChild(&childNodeB, &childNodeD);
  addChild(&childNodeC, &childNodeF);
  addChild(&childNodeD, &childNodeG);
  addChild(&childNodeD, &childNodeH);
  addChild(&childNodeE, &childNodeH);
  addChild(&childNodeF, &childNodeI);
  addChild(&childNodeI, &childNodeH);
  addChild(&childNodeG, &childNodeH);
  addChild(&childNodeG, &childNodeJ);
  addChild(&childNodeJ, &childNodeK);
  addChild(&childNodeK, &childNodeH);
  setLastBrhDom(&parentNode);
  setAllImdDom(&parentNode);
  
  TEST_ASSERT_NULL(parentNode->imdDom);
  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeB->imdDom);
  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeC->imdDom);
  TEST_ASSERT_NODE_ADDRESS(childNodeB, childNodeD->imdDom);
  TEST_ASSERT_NODE_ADDRESS(childNodeB, childNodeE->imdDom);
  TEST_ASSERT_NODE_ADDRESS(childNodeC, childNodeF->imdDom);
  TEST_ASSERT_NODE_ADDRESS(childNodeD, childNodeG->imdDom);
  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeH->imdDom);
  TEST_ASSERT_NODE_ADDRESS(childNodeF, childNodeI->imdDom);
  TEST_ASSERT_NODE_ADDRESS(childNodeG, childNodeJ->imdDom);
  TEST_ASSERT_NODE_ADDRESS(childNodeJ, childNodeK->imdDom);
}

/**      TREE A                     
 *
 *          | Entry                 imdD(A) = {NULL}
 *         \/                       imdD(B) = { A }
 *       [  A  ]                    imdD(C) = { B }
 *        |    |                    imdD(D) = { A }  
 *       \/   \/                    imdD(E) = { A }
 *     [B]   [ E ]<<<<<             imdD(F) = { E }
 *      |     |   |   /\            imdD(G) = { E }
 * >>>>\/    \/  \/   /\            imdD(H) = { E }
 * |  [C]   [F]  [G]  /\            imdD(I) = { A }
 * <<< |  /  \   |    /\
 *    \/\/   \/ \/    /\
 *    [D]   [ H ] >>>>>
 *     \      |
 *      \    |
 *      \|  \/
 *      [ I ]
 *
 **/
void test_getImdDom_given_treeA_above_should_return_expected_imdDom_in_table(void){
  Node* nodeA = createNode(0);
  Node* nodeB = createNode(1);
  Node* nodeE = createNode(1);
  Node* nodeC = createNode(2);
  Node* nodeF = createNode(2);
  Node* nodeG = createNode(2);
  Node* nodeD = createNode(3);
  Node* nodeH = createNode(3);
  Node* nodeI = createNode(4);

  addChild(&nodeA, &nodeB);
  addChild(&nodeA, &nodeE);
  addChild(&nodeB, &nodeC);
  addChild(&nodeE, &nodeF);
  addChild(&nodeE, &nodeG);
  addChild(&nodeC, &nodeC);
  addChild(&nodeC, &nodeD);
  addChild(&nodeF, &nodeD);
  addChild(&nodeF, &nodeH);
  addChild(&nodeG, &nodeH);
  addChild(&nodeD, &nodeI);
  addChild(&nodeH, &nodeI);
  addChild(&nodeH, &nodeE);

  setLastBrhDom(&nodeA);
  setAllImdDom(&nodeA);

  TEST_ASSERT_NULL(nodeA->imdDom);
  TEST_ASSERT_EQUAL_PTR(nodeA, nodeB->imdDom);
  TEST_ASSERT_EQUAL_PTR(nodeA, nodeE->imdDom);
  TEST_ASSERT_EQUAL_PTR(nodeB, nodeC->imdDom);
  TEST_ASSERT_EQUAL_PTR(nodeE, nodeF->imdDom);
  TEST_ASSERT_EQUAL_PTR(nodeE, nodeG->imdDom);
  TEST_ASSERT_EQUAL_PTR(nodeE, nodeH->imdDom);
  TEST_ASSERT_EQUAL_PTR(nodeA, nodeD->imdDom);
  TEST_ASSERT_EQUAL_PTR(nodeA, nodeI->imdDom);
}

/**       ControlFlowGraph1           DomFrontiers
 *
 *          | Entry                 DF(A) = { A }
 *         \/                       DF(B) = { D }
 *        [ A ]<<<<                 DF(C) = { D }
 *       |   |    /\                DF(D) = { A }
 *      \/  \/    /\
 *     [B]  [C]   /\
 *      |    |    /\
 *     \/   \/    /\
 *       [D] >>>>>>>
 *
 **/
void test_getNodeDomFrontiers_given_CFG1_then_find_domFrontiers_of_each_node_should_return_domFrontiers_respectively(void){
  Node* nodeA = createNode(0);
  Node* nodeB = createNode(1);
  Node* nodeC = createNode(1);
  Node* nodeD = createNode(2);

  addChild(&nodeA, &nodeB);
  addChild(&nodeA, &nodeC);
  addChild(&nodeB, &nodeD);
  addChild(&nodeC, &nodeD);
  addChild(&nodeD, &nodeA);

  setLastBrhDom(&nodeA);
  setAllImdDom(&nodeA);

  LinkedList* domFrontiersA = createLinkedList();
  LinkedList* domFrontiersB = createLinkedList();
  LinkedList* domFrontiersC = createLinkedList();
  LinkedList* domFrontiersD = createLinkedList();
  addListLast(domFrontiersA, nodeA);
  addListLast(domFrontiersB, nodeD);
  addListLast(domFrontiersC, nodeD);
  addListLast(domFrontiersD, nodeA);

  nodeA->domFrontiers = getNodeDomFrontiers(nodeA);
  nodeB->domFrontiers = getNodeDomFrontiers(nodeB);
  nodeC->domFrontiers = getNodeDomFrontiers(nodeC);
  nodeD->domFrontiers = getNodeDomFrontiers(nodeD);

  TEST_ASSERT_LINKED_LIST(domFrontiersA, nodeA->domFrontiers);
  TEST_ASSERT_LINKED_LIST(domFrontiersB, nodeB->domFrontiers);
  TEST_ASSERT_LINKED_LIST(domFrontiersC, nodeC->domFrontiers);
  TEST_ASSERT_LINKED_LIST(domFrontiersD, nodeD->domFrontiers);
}

/**    ControlFlowGraph2         DomFrontiers
 *
 *          | Entry
 *         \/
 *      [[[A]]]                DF(A) = { }
 *      |  |  \                DF(B) = { E }
 *     \/ \/  \/               DF(C) = { E }
 *    [B] [C]  [D]             DF(D) = { F }
 *     |   |    |              DF(E) = { F }
 *    \/  \/   |               DF(F) = { }
 *     [E]    |
 *       \   |
 *       \/ \/
 *        [F]
 **/
void test_getNodeDomFrontiers_given_CFG2_then_find_domFrontiers_of_each_node_should_return_domFrontiers_respectively(void){
  Node* nodeA = createNode(0);
  Node* nodeB = createNode(1);
  Node* nodeC = createNode(1);
  Node* nodeD = createNode(1);
  Node* nodeE = createNode(2);
  Node* nodeF = createNode(2);

  addChild(&nodeA, &nodeB);
  addChild(&nodeA, &nodeC);
  addChild(&nodeA, &nodeD);
  addChild(&nodeB, &nodeE);
  addChild(&nodeC, &nodeE);
  addChild(&nodeD, &nodeF);
  addChild(&nodeE, &nodeF);

  setLastBrhDom(&nodeA);
  setAllImdDom(&nodeA);


  LinkedList* domFrontiersA = createLinkedList();
  LinkedList* domFrontiersB = createLinkedList();
  LinkedList* domFrontiersC = createLinkedList();
  LinkedList* domFrontiersD = createLinkedList();
  LinkedList* domFrontiersE = createLinkedList();
  LinkedList* domFrontiersF = createLinkedList();
  addListLast(domFrontiersB, nodeE);
  addListLast(domFrontiersC, nodeE);
  addListLast(domFrontiersD, nodeF);
  addListLast(domFrontiersE, nodeF);

  nodeA->domFrontiers = getNodeDomFrontiers(nodeA);
  nodeB->domFrontiers = getNodeDomFrontiers(nodeB);
  nodeC->domFrontiers = getNodeDomFrontiers(nodeC);
  nodeD->domFrontiers = getNodeDomFrontiers(nodeD);
  nodeE->domFrontiers = getNodeDomFrontiers(nodeE);
  nodeF->domFrontiers = getNodeDomFrontiers(nodeF);

  TEST_ASSERT_LINKED_LIST(domFrontiersA, nodeA->domFrontiers);
  TEST_ASSERT_LINKED_LIST(domFrontiersB, nodeB->domFrontiers);
  TEST_ASSERT_LINKED_LIST(domFrontiersC, nodeC->domFrontiers);
  TEST_ASSERT_LINKED_LIST(domFrontiersD, nodeD->domFrontiers);
  TEST_ASSERT_LINKED_LIST(domFrontiersE, nodeE->domFrontiers);
  TEST_ASSERT_LINKED_LIST(domFrontiersF, nodeF->domFrontiers);
}

/**       ControlFlowGraph3           DomFrontiers
 *
 *          | Entry                 DF(A) = { }
 *         \/                       DF(B) = { F }
 *        [ A ]                     DF(C) = { E }
 *          |  \                    DF(D) = { E }
 *         \/   \                   DF(E) = { F }
 *       [ B ]   \                  DF(F) = { }
 *       /   \    \
 *     \/    \/   |
 *    [C]   [D]   |
 *     \     /    |
 *     \/  \/    /
 *     [ E ]    /
 *       |    /
 *      \/  \/
 *     [ F ]
 *
 **/
void test_getNodeDomFrontiers_given_CFG3_then_find_domFrontiers_of_each_node_should_return_domFrontiers_respectively(void){
  Node* nodeA = createNode(0);
  Node* nodeB = createNode(1);
  Node* nodeC = createNode(2);
  Node* nodeD = createNode(2);
  Node* nodeE = createNode(3);
  Node* nodeF = createNode(1);

  addChild(&nodeA, &nodeB);
  addChild(&nodeA, &nodeF);
  addChild(&nodeB, &nodeC);
  addChild(&nodeB, &nodeD);
  addChild(&nodeC, &nodeE);
  addChild(&nodeD, &nodeE);
  addChild(&nodeE, &nodeF);

  setLastBrhDom(&nodeA);
  setAllImdDom(&nodeA);


  LinkedList* domFrontiersA = createLinkedList();
  LinkedList* domFrontiersB = createLinkedList();
  LinkedList* domFrontiersC = createLinkedList();
  LinkedList* domFrontiersD = createLinkedList();
  LinkedList* domFrontiersE = createLinkedList();
  LinkedList* domFrontiersF = createLinkedList();
  addListLast(domFrontiersB, nodeF);
  addListLast(domFrontiersC, nodeE);
  addListLast(domFrontiersD, nodeE);
  addListLast(domFrontiersE, nodeF);

  nodeA->domFrontiers = getNodeDomFrontiers(nodeA);
  nodeB->domFrontiers = getNodeDomFrontiers(nodeB);
  nodeC->domFrontiers = getNodeDomFrontiers(nodeC);
  nodeD->domFrontiers = getNodeDomFrontiers(nodeD);
  nodeE->domFrontiers = getNodeDomFrontiers(nodeE);
  nodeF->domFrontiers = getNodeDomFrontiers(nodeF);

  TEST_ASSERT_LINKED_LIST(domFrontiersA, nodeA->domFrontiers);
  TEST_ASSERT_LINKED_LIST(domFrontiersB, nodeB->domFrontiers);
  TEST_ASSERT_LINKED_LIST(domFrontiersC, nodeC->domFrontiers);
  TEST_ASSERT_LINKED_LIST(domFrontiersD, nodeD->domFrontiers);
  TEST_ASSERT_LINKED_LIST(domFrontiersE, nodeE->domFrontiers);
  TEST_ASSERT_LINKED_LIST(domFrontiersF, nodeF->domFrontiers);
}

/**       ControlFlowGraph4          DomFrontiers
 *
 *          | Entry                 DF(A) = { }
 *         \/                       DF(B) = { D }
 *       [  A  ]                    DF(C) = { C, D }
 *        |    |                    DF(D) = { I }  
 *       \/   \/                    DF(E) = { D, I, E }
 *     [B]   [ E ]<<<<<             DF(F) = { D, H }
 *      |     |   |   /\            DF(G) = { H }
 * >>>>\/    \/  \/   /\            DF(H) = { I, E }
 * |  [C]   [F]  [G]  /\            DF(I) = { }
 * <<< |  /  \   |    /\
 *    \/\/   \/ \/    /\
 *    [D]   [ H ] >>>>>
 *     \      |
 *      \    |
 *      \|  \/
 *      [ I ]
 *
 **/
void test_getNodeDomFrontiers_given_CFG4_then_find_domFrontiers_of_each_node_should_return_domFrontiers_respectively(void){
  Node* nodeA = createNode(0);
  Node* nodeB = createNode(1);
  Node* nodeE = createNode(1);
  Node* nodeC = createNode(2);
  Node* nodeF = createNode(2);
  Node* nodeG = createNode(2);
  Node* nodeD = createNode(3);
  Node* nodeH = createNode(3);
  Node* nodeI = createNode(4);

  addChild(&nodeA, &nodeB);
  addChild(&nodeA, &nodeE);
  addChild(&nodeB, &nodeC);
  addChild(&nodeE, &nodeF);
  addChild(&nodeE, &nodeG);
  addChild(&nodeC, &nodeC);
  addChild(&nodeC, &nodeD);
  addChild(&nodeF, &nodeD);
  addChild(&nodeF, &nodeH);
  addChild(&nodeG, &nodeH);
  addChild(&nodeD, &nodeI);
  addChild(&nodeH, &nodeI);
  addChild(&nodeH, &nodeE);

  setLastBrhDom(&nodeA);
  setAllImdDom(&nodeA);



  LinkedList* domFrontiersA = createLinkedList();
  LinkedList* domFrontiersB = createLinkedList();
  LinkedList* domFrontiersC = createLinkedList();
  LinkedList* domFrontiersD = createLinkedList();
  LinkedList* domFrontiersE = createLinkedList();
  LinkedList* domFrontiersF = createLinkedList();
  LinkedList* domFrontiersG = createLinkedList();
  LinkedList* domFrontiersH = createLinkedList();
  LinkedList* domFrontiersI = createLinkedList();
  
  addListLast(domFrontiersB, nodeD);
  addListLast(domFrontiersC, nodeC);
  addListLast(domFrontiersC, nodeD);
  addListLast(domFrontiersD, nodeI);
  addListLast(domFrontiersE, nodeD);
  addListLast(domFrontiersE, nodeI);
  addListLast(domFrontiersE, nodeE);
  addListLast(domFrontiersF, nodeD);
  addListLast(domFrontiersF, nodeH);
  addListLast(domFrontiersG, nodeH);
  addListLast(domFrontiersH, nodeI);
  addListLast(domFrontiersH, nodeE);

  nodeA->domFrontiers = getNodeDomFrontiers(nodeA);
  nodeB->domFrontiers = getNodeDomFrontiers(nodeB);
  nodeC->domFrontiers = getNodeDomFrontiers(nodeC);
  nodeD->domFrontiers = getNodeDomFrontiers(nodeD);
  nodeE->domFrontiers = getNodeDomFrontiers(nodeE);
  nodeF->domFrontiers = getNodeDomFrontiers(nodeF);
  nodeG->domFrontiers = getNodeDomFrontiers(nodeG);
  nodeH->domFrontiers = getNodeDomFrontiers(nodeH);
  nodeI->domFrontiers = getNodeDomFrontiers(nodeI);
  
  TEST_ASSERT_LINKED_LIST(domFrontiersA, nodeA->domFrontiers);
  TEST_ASSERT_LINKED_LIST(domFrontiersB, nodeB->domFrontiers);
  TEST_ASSERT_LINKED_LIST(domFrontiersC, nodeC->domFrontiers);
  TEST_ASSERT_LINKED_LIST(domFrontiersD, nodeD->domFrontiers);
  TEST_ASSERT_LINKED_LIST(domFrontiersE, nodeE->domFrontiers);
  TEST_ASSERT_LINKED_LIST(domFrontiersF, nodeF->domFrontiers);
  TEST_ASSERT_LINKED_LIST(domFrontiersG, nodeG->domFrontiers);
  TEST_ASSERT_LINKED_LIST(domFrontiersH, nodeH->domFrontiers);
  TEST_ASSERT_LINKED_LIST(domFrontiersI, nodeI->domFrontiers);
}

/**       ControlFlowGraph1         Union of DomFrontiers
 *
 *          | Entry                 DF(A) = { A }
 *         \/                       DF(B) = { D }
 *        [ A ]<<<<                 DF(C) = { D }
 *       |   |    /\                DF(D) = { A }
 *      \/  \/    /\                UDF = { A , D}
 *     [B]  [C]   /\
 *      |    |    /\
 *     \/   \/    /\
 *       [D] >>>>>>>
 *
 **/
void test_getAllDomFrontiers_given_CFG1_should_give_a_union_of_dominatorFrontiers_list(void){
  Node* nodeA = createNode(0);
  Node* nodeB = createNode(1);
  Node* nodeC = createNode(1);
  Node* nodeD = createNode(2);

  addChild(&nodeA, &nodeB);
  addChild(&nodeA, &nodeC);
  addChild(&nodeB, &nodeD);
  addChild(&nodeC, &nodeD);
  addChild(&nodeD, &nodeA);

  setLastBrhDom(&nodeA);
  setAllImdDom(&nodeA);

  LinkedList* unionDomFrontiers = createLinkedList();
  LinkedList* expectUnionDomFrontiers = createLinkedList();

  addListLast(expectUnionDomFrontiers, nodeA);
  addListLast(expectUnionDomFrontiers, nodeD);
  unionDomFrontiers = getAllDomFrontiers(&nodeA);

  TEST_ASSERT_LINKED_LIST(expectUnionDomFrontiers, unionDomFrontiers);

}

/**    ControlFlowGraph2     Union of DomFrontiers
 *
 *          | Entry
 *         \/
 *      [[[A]]]                DF(A) = { }
 *      |  |  \                DF(B) = { E }
 *     \/ \/  \/               DF(C) = { E }
 *    [B] [C]  [D]             DF(D) = { F }
 *     |   |    |              DF(E) = { F }
 *    \/  \/   |               DF(F) = { }
 *     [E]    |                UDF = { E , F}
 *       \   |
 *       \/ \/
 *        [F]
 **/
void test_getAllDomFrontiers_given_CFG2_should_give_a_union_of_dominatorFrontiers_list(void){
  Node* nodeA = createNode(0);
  Node* nodeB = createNode(1);
  Node* nodeC = createNode(1);
  Node* nodeD = createNode(1);
  Node* nodeE = createNode(2);
  Node* nodeF = createNode(2);

  addChild(&nodeA, &nodeB);
  addChild(&nodeA, &nodeC);
  addChild(&nodeA, &nodeD);
  addChild(&nodeB, &nodeE);
  addChild(&nodeC, &nodeE);
  addChild(&nodeD, &nodeF);
  addChild(&nodeE, &nodeF);

  setLastBrhDom(&nodeA);
  setAllImdDom(&nodeA);

  LinkedList* unionDomFrontiers = createLinkedList();
  LinkedList* expectUnionDomFrontiers = createLinkedList();

  addListLast(expectUnionDomFrontiers, nodeE);
  addListLast(expectUnionDomFrontiers, nodeF);
  unionDomFrontiers = getAllDomFrontiers(&nodeA);

  TEST_ASSERT_LINKED_LIST(expectUnionDomFrontiers, unionDomFrontiers);

}

/**       ControlFlowGraph3     Union of DomFrontiers
 *
 *          | Entry                 DF(A) = { }
 *         \/                       DF(B) = { F }
 *        [ A ]                     DF(C) = { E }
 *          |  \                    DF(D) = { E }
 *         \/   \                   DF(E) = { F }
 *       [ B ]   \                  DF(F) = { }
 *       /   \    \
 *     \/    \/   |                UDF = { E , F}
 *    [C]   [D]   |
 *     \     /    |
 *     \/  \/    /
 *     [ E ]    /
 *       |    /
 *      \/  \/
 *     [ F ]
 *
 **/
void test_getAllDomFrontiers_given_CFG3_should_give_a_union_of_dominatorFrontiers_list(void){

  Node* nodeA = createNode(0);
  Node* nodeB = createNode(1);
  Node* nodeC = createNode(2);
  Node* nodeD = createNode(2);
  Node* nodeE = createNode(3);
  Node* nodeF = createNode(1);

  addChild(&nodeA, &nodeB);
  addChild(&nodeA, &nodeF);
  addChild(&nodeB, &nodeC);
  addChild(&nodeB, &nodeD);
  addChild(&nodeC, &nodeE);
  addChild(&nodeD, &nodeE);
  addChild(&nodeE, &nodeF);

  setLastBrhDom(&nodeA);
  setAllImdDom(&nodeA);

  LinkedList* unionDomFrontiers = createLinkedList();
  LinkedList* expectUnionDomFrontiers = createLinkedList();

  addListLast(expectUnionDomFrontiers, nodeF);
  addListLast(expectUnionDomFrontiers, nodeE);
  unionDomFrontiers = getAllDomFrontiers(&nodeA);

  TEST_ASSERT_LINKED_LIST(expectUnionDomFrontiers, unionDomFrontiers);
}

/**       ControlFlowGraph4          DomFrontiers
 *
 *          | Entry                 DF(A) = { }
 *         \/                       DF(B) = { D }
 *       [  A  ]                    DF(C) = { C, D }
 *        |    |                    DF(D) = { I }  
 *       \/   \/                    DF(E) = { D, I, E }
 *     [B]   [ E ]<<<<<             DF(F) = { D, H }
 *      |     |   |   /\            DF(G) = { H }
 * >>>>\/    \/  \/   /\            DF(H) = { I, E }
 * |  [C]   [F]  [G]  /\            DF(I) = { }
 * <<< |  /  \   |    /\            UDF = { D, I, E, C, H}
 *    \/\/   \/ \/    /\
 *    [D]   [ H ] >>>>>
 *     \      |
 *      \    |
 *      \|  \/
 *      [ I ]
 *
 **/
void test_getAllDomFrontiers_given_CFG4_should_give_a_union_of_dominatorFrontiers_list(void){
  Node* nodeA = createNode(0);
  Node* nodeB = createNode(1);
  Node* nodeE = createNode(1);
  Node* nodeC = createNode(2);
  Node* nodeF = createNode(2);
  Node* nodeG = createNode(2);
  Node* nodeD = createNode(3);
  Node* nodeH = createNode(3);
  Node* nodeI = createNode(4);

  addChild(&nodeA, &nodeB);
  addChild(&nodeA, &nodeE);
  addChild(&nodeB, &nodeC);
  addChild(&nodeE, &nodeF);
  addChild(&nodeE, &nodeG);
  addChild(&nodeC, &nodeC);
  addChild(&nodeC, &nodeD);
  addChild(&nodeF, &nodeD);
  addChild(&nodeF, &nodeH);
  addChild(&nodeG, &nodeH);
  addChild(&nodeD, &nodeI);
  addChild(&nodeH, &nodeI);
  addChild(&nodeH, &nodeE);

  setLastBrhDom(&nodeA);
  setAllImdDom(&nodeA);

    
  LinkedList* unionDomFrontiers = createLinkedList();
  LinkedList* expectUnionDomFrontiers = createLinkedList();

  addListLast(expectUnionDomFrontiers, nodeD);
  addListLast(expectUnionDomFrontiers, nodeI);
  addListLast(expectUnionDomFrontiers, nodeE);
  addListLast(expectUnionDomFrontiers, nodeC);
  addListLast(expectUnionDomFrontiers, nodeH);
  unionDomFrontiers = getAllDomFrontiers(&nodeA);

  TEST_ASSERT_LINKED_LIST(expectUnionDomFrontiers, unionDomFrontiers);
}

