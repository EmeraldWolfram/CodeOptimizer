#include "unity.h"
#include "NodeChain.h"
#include "ExpressionBlock.h"
#include "ErrorObject.h"
#include "customAssertion.h"
#include "LinkedList.h"
#include "GetList.h"
#include "CException.h"
#include <stdlib.h>
// TEST_ASSERT_NODE_DATA(char* expectExpress, int expectRank, Node* expectParent, Node* actualNode)
// TEST_ASSERT_NODE_DATA use to test internal data of the Node, not necessary to be the same node

void setUp(void){}

void tearDown(void){}

void test_createNode(void){
	Node* testNode = createNode(0);

  TEST_ASSERT_NOT_NULL(testNode);
  TEST_ASSERT_EQUAL(0, testNode->rank);
  TEST_ASSERT_EQUAL(0, testNode->visitFlag);
  TEST_ASSERT_EQUAL(0, testNode->numOfChild);
  TEST_ASSERT_EQUAL(0, testNode->block->length);
  TEST_ASSERT_EQUAL(0, testNode->directDom->length);
  TEST_ASSERT_NULL(testNode->parent);
  TEST_ASSERT_NULL(testNode->lastBrhDom);
  TEST_ASSERT_NULL(testNode->imdDom);
  TEST_ASSERT_NULL(testNode->children);
  TEST_ASSERT_NULL(testNode->domFrontiers);
}

/**
 *  addChild
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

/**
 *  addChild
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

/**
 *  addChild
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
 *  assembleList
 *  This function should be able to add all the node in a Node Tree into
 *  a LinkedList without repetition of Node
 *
 *  Eg.
 *      TREE X:   |
 *         [A]    |   ListX:
 *        /   \   |   tail
 *     [B]    [C] |        \________________________________________
 *     / \     |  |   head                                          \
 *   [D] [E]  [F] |       \                                          \
 *  /  \ /    |   |        \-[A]-[B]-[C]-[D]-[E]-[F]-[G]-[H]-[I]-[J]-[K]
 *[G]->[H]<--[I]  |
 * |    ^         |
 * |    ^         |
 *[J]--[K]        |
 ************************************************************************/
void test_assembleList_given_node_tree_X_above_should_move_into_ListX(void){
  Node* nodeA  = createNode(0);
  Node* nodeB  = createNode(1);
  Node* nodeC  = createNode(1);
  Node* nodeD  = createNode(2);
  Node* nodeE  = createNode(2);
  Node* nodeF  = createNode(2);
  Node* nodeG  = createNode(3);
  Node* nodeH  = createNode(3);
  Node* nodeI  = createNode(3);
  Node* nodeJ  = createNode(4);
  Node* nodeK  = createNode(5);

  addChild(&nodeA, &nodeB);
  addChild(&nodeA, &nodeC);
  addChild(&nodeB, &nodeD);
  addChild(&nodeB, &nodeE);
  addChild(&nodeC, &nodeF);
  addChild(&nodeD, &nodeG);
  addChild(&nodeD, &nodeH);
  addChild(&nodeE, &nodeH);
  addChild(&nodeF, &nodeI);
  addChild(&nodeI, &nodeH);
  addChild(&nodeG, &nodeH);
  addChild(&nodeG, &nodeJ);
  addChild(&nodeJ, &nodeK);
  addChild(&nodeK, &nodeH);
  
  LinkedList* testList  = assembleList(&nodeA);
  ListElement* testPtr  = testList->head;
  
  TEST_ASSERT_EQUAL_PTR(nodeA, testPtr->node);
  TEST_ASSERT_EQUAL_PTR(nodeB, testPtr->next->node);
  TEST_ASSERT_EQUAL_PTR(nodeC, testPtr->next->next->node);
  
  testPtr = testPtr->next->next->next;
  TEST_ASSERT_EQUAL_PTR(nodeD, testPtr->node);
  TEST_ASSERT_EQUAL_PTR(nodeE, testPtr->next->node);
  TEST_ASSERT_EQUAL_PTR(nodeF, testPtr->next->next->node);
  
  testPtr = testPtr->next->next->next;
  TEST_ASSERT_EQUAL_PTR(nodeG, testPtr->node);
  TEST_ASSERT_EQUAL_PTR(nodeH, testPtr->next->node);
  TEST_ASSERT_EQUAL_PTR(nodeI, testPtr->next->next->node);
  
  testPtr = testPtr->next->next->next;
  TEST_ASSERT_EQUAL_PTR(nodeJ, testPtr->node);
  TEST_ASSERT_EQUAL_PTR(nodeK, testPtr->next->node);
}


/**
 *  setLastBrhDom 
 *  This function will assign lastBrhDom to the node in the tree
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
 *  setLastBrhDom
 *  This test test assign of lastBrhDom when there are two child Node
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
 *  setLastBrhDom will assign lastBrhDom to all the node in the tree
 *  This test test assign of lastBrhDom for the following tree
 *  where Node D should have lastBrhDom pointing to parent
 *
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
 *  setLastBrhDom will assign lastBrhDom to all the node in the tree
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
 *  setLastBrhDom will assign lastBrhDom to all the node in the tree
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
 *  setLastBrhDom will assign lastBrhDom to all the node in the tree
 *  This test test assign of lastBrhDom when there are lower rank node
 *  pointing to a higher rank Node
 *
 *
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

/**       
 *  setLastBrhDom
 *  This is to test when there is node point back to the root Node.
 *  Check whether setLastBrhDom can handle this case.
 *
 *    ControlFlowGraph1
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

/**       
 *  setLastBrhDom
 *  This test is to check if the function can handle the case when the Node point to itself
 *
 *
 *  ControlFlowGraph2
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


//********************TEST for getImdDom ******************************

/**
 *  getImdDom will find imdDom of that node and assign to it
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
 *  getImdDom 
 *  This test check if NodeD can get the imdDom as parent using getImdDom function
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
 *  getImdDom
 *  This test check if the function can handle the case when there are
 *  many node pointing to the particular Node
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
 *  setAllImdDom 
 *  This test check if setAllImdDom funcion will call getImdDom function iteratively
 *  to assign all the imdDom to every Node in the Tree.
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

/**      
 *  setAllImdDom
 *  This test check setAllImdDom can handle the scenario when
 *  1. Self poiting Node (Node C)
 *  2. Lower rank too higher rank Node (Node H -> Node E)
 *
 *
 *      TREE A                     
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
void test_setAllImdDom_given_treeA_above_should_return_expected_imdDom_in_table(void){
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

/**
 *  setAllDirectDom
 *  
 *      (A)     DD(A) = {}
 *     /  \     DD(B) = {A}
 *   (B)  (C)   DD(C) = {A}
 *    \   /     DD{D} = {B, C}
 *     (D)
 *  
 *  
 ***************************************************/
void test_setAllDirectDom_given_node_tree_above_should_assign_directDom(void){
   ErrorObject *err;
   Node* nodeA = createNode(0);
   Node* nodeB = createNode(1);
   Node* nodeC = createNode(1);
   Node* nodeD = createNode(2);
   
   addChild(&nodeA, &nodeB);
   addChild(&nodeA, &nodeC);
   addChild(&nodeB, &nodeD);
   addChild(&nodeC, &nodeD);

   setAllDirectDom(&nodeA);
   ListElement* testPtr;
   TEST_ASSERT_EQUAL(0, nodeA->directDom->length);
   
   testPtr = nodeB->directDom->head;
   TEST_ASSERT_EQUAL(1, nodeB->directDom->length);
   TEST_ASSERT_NODE_ADDRESS(nodeA, testPtr->node);
   
   testPtr = nodeC->directDom->head;
   TEST_ASSERT_EQUAL(1, nodeC->directDom->length);
   TEST_ASSERT_NODE_ADDRESS(nodeA, testPtr->node);
   
   testPtr = nodeD->directDom->head;
   TEST_ASSERT_EQUAL(2, nodeD->directDom->length);
   TEST_ASSERT_NODE_ADDRESS(nodeB, testPtr->node);
   TEST_ASSERT_NODE_ADDRESS(nodeC, testPtr->next->node);   
}
 
 
 
 
 
