#include "unity.h"
#include "GetList.h"
#include "ExpressionBlock.h"
#include "LinkedList.h"
#include "customAssertion.h"
#include "NodeChain.h"
#include "ErrorObject.h"
#include "CException.h"
#include <stdlib.h>

int w = 119;
int x = 120;
int y = 121;
int z = 122;
int a = 97;
int b = 98;
int c = 99;
int d = 100;

void setUp(void){}

void tearDown(void){}


/**
 *  getSubsList
 *  should arrange the input list of expression into a LinkedList
 *  
 *  inputList:
 *  The sequence to be assemble into the list
 *  3rd   1st   2nd
 *   x  =  y  +  z
 *   y  =  z  +  x
 *   z  =  x  +  y
 *  
 *  tail
 *      \_________________________________
 *                                        \
 *  head                                   \
 *      \-[y]-[z]-[x]-[z]-[x]-[y]-[x]-[y]-[z]
 *                                         |
 *                                        GND
 *
 *
 *  This is to test that getSubsList will assemble all the 
 *  variable in every expressions into a LinkedList in the format
 *  of 
 *
 *  operand A => operand B => result => operand A => operandB => ... => GND
 *
 *  where ... is the subscript set of the next expressions and so on.
 *  
 **************************************************************/
void test_getSubsList_should_arrange_the_subscript_list(void){
  Expression* exp1 = createExpression(x, ADDITION, y, z, 0);
  Expression* exp2 = createExpression(y, ADDITION, z, x, 0);
  Expression* exp3 = createExpression(z, ADDITION, x, y, 0);
  
  Node* testNode = createNode(0);
  addListLast(testNode->block, exp1);
  addListLast(testNode->block, exp2);
  addListLast(testNode->block, exp3);
  
  LinkedList* testList = getSubsList(testNode->block);
  ListElement* testPtr = testList->head;
  
  TEST_ASSERT_EQUAL(9, testList->length);
  TEST_ASSERT_EQUAL_PTR(&exp1->oprdA, testPtr->node);
  TEST_ASSERT_EQUAL_PTR(&exp1->oprdB, testPtr->next->node);
  TEST_ASSERT_EQUAL_PTR(&exp1->id,    testPtr->next->next->node);
  
  testPtr = testPtr->next->next->next;
  TEST_ASSERT_EQUAL_PTR(&exp2->oprdA, testPtr->node);
  TEST_ASSERT_EQUAL_PTR(&exp2->oprdB, testPtr->next->node);
  TEST_ASSERT_EQUAL_PTR(&exp2->id,    testPtr->next->next->node);
  
  testPtr = testPtr->next->next->next;
  TEST_ASSERT_EQUAL_PTR(&exp3->oprdA, testPtr->node);
  TEST_ASSERT_EQUAL_PTR(&exp3->oprdB, testPtr->next->node);
  TEST_ASSERT_EQUAL_PTR(&exp3->id,    testPtr->next->next->node);
}
/**
 *  getSubsList
 *
 *  when getSubsList receive NULL input
 *  ERR_NULL_LIST shall be thrown
 *
 *****************************************************************************/
void test_getSubsList_should_Throw_ERR_NULL_LIST(void){
  ErrorObject* err;
  Try{
    LinkedList* testList = getSubsList(NULL);
    TEST_FAIL_MESSAGE("Expected ERR_NULL_LIST to be thrown, but nothing happen");
  } Catch(err){
    TEST_ASSERT_EQUAL(ERR_NULL_LIST, err->errorCode);
    TEST_ASSERT_EQUAL_STRING("Input LinkedList to function getSubsList is NULL", err->errorMsg);
  }
}

/**
 *  getLargestIndex
 *
 *  x0 = y0 + z0
 *  x1 = z0 + x0
 *  y1 = z0 + x1
 *  y2 = z0 + y1
 *  x2 = x1 + y2
 *
 *  getLargestIndex(exprList, x) should return 2 as x2 is the largest in the list
 *
 *************************************************************************/
void test_getLargestIndex_given_list_shown_above_should_return_2(void){
  Expression* exp1 = createExpression(x, ADDITION, y, z, 0);
  Expression* exp2 = createExpression(x, ADDITION, z, x, 0);
  Expression* exp3 = createExpression(y, ADDITION, z, x, 0);
  Expression* exp4 = createExpression(y, ADDITION, z, y, 0);
  Expression* exp5 = createExpression(x, ADDITION, x, y, 0);
  
  Node* testNode = createNode(0);
  addListLast(testNode->block, exp1);
  addListLast(testNode->block, exp2);
  addListLast(testNode->block, exp3);
  addListLast(testNode->block, exp4);
  addListLast(testNode->block, exp5);
  
  arrangeSSA(testNode);
  Subscript* xVariable = &((Expression*)testNode->block->head->node)->id;
  Subscript* yVariable = &((Expression*)testNode->block->head->next->next->node)->id;
  
  Subscript* testX = getLargestIndex(testNode->block, xVariable);
  Subscript* testY = getLargestIndex(testNode->block, yVariable);
  TEST_ASSERT_SUBSCRIPT(x, 2, testX);
  TEST_ASSERT_SUBSCRIPT(y, 2, testY);
}

/**
 *  getLargestIndex
 *
 *  when getLargestIndex receive NULL LinkedList
 *  ERR_NULL_LIST shall be thrown
 *
 *****************************************************************************/
void test_getLargestIndex_should_Throw_ERR_NULL_LIST(void){
  ErrorObject* err;
  Try{
    Subscript* emptySubs;
    Subscript* testList = getLargestIndex(NULL, emptySubs);
    TEST_FAIL_MESSAGE("Expected ERR_NULL_LIST to be thrown, but nothing happen");
  } Catch(err){
    TEST_ASSERT_EQUAL(ERR_NULL_LIST, err->errorCode);
    TEST_ASSERT_EQUAL_STRING("Input LinkedList to function getLargestIndex is NULL", err->errorMsg);
  }
}

/**
 *  getLargestIndex
 *
 *  when getLargestIndex receive NULL Subscript
 *  ERR_NULL_SUBSCRIPT shall be thrown
 *
 *****************************************************************************/
void test_getLargestIndex_should_Throw_ERR_NULL_SUBSCRIPT(void){
  ErrorObject* err;
  Try{
    LinkedList* emptyList = createLinkedList();
    Subscript* testList   = getLargestIndex(emptyList, NULL);
    TEST_FAIL_MESSAGE("Expected ERR_NULL_SUBSCRIPT to be thrown, but nothing happen");
  } Catch(err){
    TEST_ASSERT_EQUAL(ERR_NULL_SUBSCRIPT, err->errorCode);
    TEST_ASSERT_EQUAL_STRING("Input Subscript to function getLargestIndex is NULL", err->errorMsg);
  }
}

/**
 * getModifiedList
 *
 *  x0 = y0 + z0
 *  x1 = z0 + x0
 *  y1 = z0 + x1
 *  y2 = z0 + y1
 *  x2 = x1 + y2
 *
 *  should return x0->y1
 *
 *************************************************************************/
void test_getModifiedList_should_return_x2_y2_when_the_expression_list_is_given(void){
  Expression* exp1 = createExpression(x, ADDITION, y, z, 0);
  Expression* exp2 = createExpression(x, ADDITION, z, x, 0);
  Expression* exp3 = createExpression(y, ADDITION, z, x, 0);
  Expression* exp4 = createExpression(y, ADDITION, z, y, 0);
  Expression* exp5 = createExpression(x, ADDITION, x, y, 0);
  
  Node* testNode = createNode(0);
  addListLast(testNode->block, exp1);
  addListLast(testNode->block, exp2);
  addListLast(testNode->block, exp3);
  addListLast(testNode->block, exp4);
  addListLast(testNode->block, exp5);
  
  arrangeSSA(testNode);
  
  LinkedList* testList = getModifiedList(testNode);

  TEST_ASSERT_EQUAL(2, testList->length);
  TEST_ASSERT_SUBSCRIPT(x, 0, testList->head->node);
  TEST_ASSERT_SUBSCRIPT(y, 1, testList->head->next->node);
}

/**
 *  getModifiedList
 *
 *  when getModifiedList receive NULL LinkedList
 *  ERR_NULL_NODE shall be thrown
 *
 *****************************************************************************/
void test_getModifiedList_should_Throw_ERR_NULL_NODE(void){
  ErrorObject* err;
  Try{
    LinkedList* testList = getModifiedList(NULL);
    TEST_FAIL_MESSAGE("Expected ERR_NULL_NODE to be thrown, but nothing happen");
  } Catch(err){
    TEST_ASSERT_EQUAL(ERR_NULL_NODE, err->errorCode);
    TEST_ASSERT_EQUAL_STRING("Input Node to function getModifiedList is NULL", err->errorMsg);
  }
}

/**
 *  updateList
 *
 *  empty prevList
 *
 *  x1 = y0 + x0
 *
 *  updateList should update the prevList to x1
 *
 *************************************************************************/
void test_updateList_to_check_if_empty_LinkedList_can_be_entered(void){
  Expression* exp1 = createExpression(x, ADDITION, y, x, 0);
  Node* testNode = createNode(1);
  addListLast(testNode->block, exp1);
  
  LinkedList* testList = createLinkedList();
  arrangeSSA(testNode);
  updateList(testNode, testList);
  
  TEST_ASSERT_EQUAL(1, testList->length);
  TEST_ASSERT_SUBSCRIPT(x, 1, testList->head->node);
}


/**
 *  updateList
 *
 *  prevList = w0->z0->x0
 *
 *  x1 = y0 + x0
 *  y1 = z0 + x1
 *  y2 = z0 + y1
 *  x2 = x1 + y2
 *
 *  updateList should update the prevList to w0->z0->x2->y2
 *
 *************************************************************************/
void test_updateList_should_return_prevList_and_add_on_x2_y2_when_the_expression_list_is_given(void){
  Expression* exp1 = createExpression(x, ADDITION, y, x, 0);
  Expression* exp2 = createExpression(y, ADDITION, z, x, 0);
  Expression* exp3 = createExpression(y, ADDITION, z, y, 0);
  Expression* exp4 = createExpression(x, ADDITION, x, y, 0);
  Expression* prev1 = createExpression(w, ASSIGN, 10, 0, 0);
  Expression* prev2 = createExpression(z, ASSIGN, 11, 0, 0);
  Expression* prev3 = createExpression(x, ASSIGN, 12, 0, 0);
  
  Node* testNode = createNode(1);
  addListLast(testNode->block, exp1);
  addListLast(testNode->block, exp2);
  addListLast(testNode->block, exp3);
  addListLast(testNode->block, exp4);
  
  arrangeSSA(testNode);
  
  Node* prevNode = createNode(0);
  addListLast(prevNode->block, prev1);
  addListLast(prevNode->block, prev2);
  addListLast(prevNode->block, prev3);
  
  LinkedList* prevList = getSubsList(prevNode->block);
  updateList(testNode, prevList);

  TEST_ASSERT_EQUAL(4, prevList->length);
  TEST_ASSERT_SUBSCRIPT(w, 0, prevList->head->node);
  TEST_ASSERT_SUBSCRIPT(z, 0, prevList->head->next->node);
  TEST_ASSERT_SUBSCRIPT(x, 2, prevList->head->next->next->node);
  TEST_ASSERT_SUBSCRIPT(y, 2, prevList->head->next->next->next->node);
}

/**
 *  updateList
 *
 *  when updateList receive NULL LinkedList
 *  ERR_NULL_NODE shall be thrown
 *
 *****************************************************************************/
void test_updateList_should_Throw_ERR_NULL_NODE(void){
  ErrorObject* err;
  Try{
    updateList(NULL, createLinkedList());
    TEST_FAIL_MESSAGE("Expected ERR_NULL_NODE to be thrown, but nothing happen");
  } Catch(err){
    TEST_ASSERT_EQUAL(ERR_NULL_NODE, err->errorCode);
    TEST_ASSERT_EQUAL_STRING("Input Node to function updateList is NULL", err->errorMsg);
  }
}

/**
 *  updateList
 *
 *  when updateList receive NULL LinkedList
 *  ERR_NULL_LIST shall be thrown
 *
 *****************************************************************************/
void test_updateList_should_Throw_ERR_NULL_LIST(void){
  ErrorObject* err;
  Try{
    updateList(createNode(0), NULL);
    TEST_FAIL_MESSAGE("Expected ERR_NULL_LIST to be thrown, but nothing happen");
  } Catch(err){
    TEST_ASSERT_EQUAL(ERR_NULL_LIST, err->errorCode);
    TEST_ASSERT_EQUAL_STRING("Input LinkedList to function updateList is NULL", err->errorMsg);
  }
}

/**
 *  getLiveList(inputNode)
 *
 *  given input NodeA:  x = 0
 *                      y = 1
 *                      x = x + y
 *                      c = x > y
 *                      if(c) goto &NodeC
 *
 *  getLiveList should return x->y
 ************************************************************************************/
void test_getLiveList_given_Node_should_return_a_LinkedList_with_x_and_y(void){
  Node* nodeA = createNode(0);
  Node* nodeC = createNode(1);
  Expression* exp1 = createExpression(x, ASSIGN, 0, 0, 0);
  Expression* exp2 = createExpression(y, ASSIGN, 1, 0, 0);
  Expression* exp3 = createExpression(x, ADDITION, x, y, 0);
  Expression* exp4 = createExpression(c, GREATER_THAN, x, y, 0);
  Expression* exp5 = createExpression(c, IF_STATEMENT, x, (int)&nodeC, 0);

  addListLast(nodeA->block, exp1);
  addListLast(nodeA->block, exp2);
  addListLast(nodeA->block, exp3);
  addListLast(nodeA->block, exp4);
  addListLast(nodeA->block, exp5);
  
  LinkedList* testList = getLiveList(&nodeA);
  TEST_ASSERT_EQUAL(2, testList->length);
  TEST_ASSERT_SUBSCRIPT(x, 0, testList->head->node);
  TEST_ASSERT_SUBSCRIPT(y, 0, testList->head->next->node);  
}

/**
 *  getLiveList
 *
 *  when getLiveList receive NULL LinkedList
 *  ERR_NULL_NODE shall be thrown
 *
 *****************************************************************************/
void test_getLiveList_should_Throw_ERR_NULL_NODE(void){
  ErrorObject* err;
  Try{
    LinkedList* testList = getLiveList(NULL);
    TEST_FAIL_MESSAGE("Expected ERR_NULL_NODE to be thrown, but nothing happen");
  } Catch(err){
    TEST_ASSERT_EQUAL(ERR_NULL_NODE, err->errorCode);
    TEST_ASSERT_EQUAL_STRING("Input Node to function getLiveList is NULL", err->errorMsg);
  }
}

/**
 *  getLatestList
 *
 *  NodeA:
 *  x = 3         x0 = 3
 *  x = y + x     x1 = y0 + x0
 *  x = x * x     x2 = x1 * z0
 *
 *  return x2
 *************************************************************************************/
void test_getLatestList_given_NodeA_above_should_return_x2(void){
  Node* nodeA = createNode(0);
  Expression* exp1 = createExpression(x, ASSIGN, 3, 0, 0);
  Expression* exp2 = createExpression(x, ADDITION, y, x, 0);
  Expression* exp3 = createExpression(x, MULTIPLICATION, x, z, 0);
  
  addListLast(nodeA->block, exp1);
  addListLast(nodeA->block, exp2);
  addListLast(nodeA->block, exp3);

  arrangeSSA(nodeA);
  
  LinkedList* testList = getLatestList(nodeA, createLinkedList());
  TEST_ASSERT_EQUAL(1, testList->length);
  TEST_ASSERT_SUBSCRIPT(x, 2, testList->head->node);  
}

/**
 *  getLatestList
 *
 *  NodeA:
 *  x0 = 3
 *  x1 = y0 + x0
 *  x2 = x1 * z0
 *
 *  NodeB:        
 *  w0 = x2 + y0
 *  
 *  prevList: x2
 *  getLatestList(NodeB, prevList)
 *  return x2->w0
 *************************************************************************************/
void test_getLatestList_given_NodeB_above_should_return_x2_and_z0(void){
  Node* nodeA = createNode(0);
  Node* nodeB = createNode(1);
  Expression* exp1 = createExpression(x, ASSIGN, 3, 0, 0);
  Expression* exp2 = createExpression(x, ADDITION, y, x, 0);
  Expression* exp3 = createExpression(x, MULTIPLICATION, x, z, 0);
  Expression* exp4 = createExpression(w, MULTIPLICATION, x, y, 0);
  
  addListLast(nodeA->block, exp1);
  addListLast(nodeA->block, exp2);
  addListLast(nodeA->block, exp3);
  addListLast(nodeB->block, exp4);
  
  arrangeSSA(nodeA);
  arrangeSSA(nodeB);
  
  LinkedList* prevList = getLatestList(nodeA, createLinkedList());
  LinkedList* testList = getLatestList(nodeB, prevList);
  TEST_ASSERT_EQUAL(2, testList->length);
  TEST_ASSERT_SUBSCRIPT(w, 0, testList->head->node);  
  TEST_ASSERT_SUBSCRIPT(x, 2, testList->head->next->node);  
}

/**
 *  getLatestList
 *
 *  when getLatestList receive NULL LinkedList
 *  ERR_NULL_NODE shall be thrown
 *
 *****************************************************************************/
void test_getLatestList_should_Throw_ERR_NULL_NODE(void){
  ErrorObject* err;
  Try{
    LinkedList* testList = getLatestList(NULL, createLinkedList());
    TEST_FAIL_MESSAGE("Expected ERR_NULL_NODE to be thrown, but nothing happen");
  } Catch(err){
    TEST_ASSERT_EQUAL(ERR_NULL_NODE, err->errorCode);
    TEST_ASSERT_EQUAL_STRING("Input Node to function getLatestList is NULL", err->errorMsg);
  }
}

/**
 *  getPathToNode(A, C)
 *    (A)
 *    / \
 *  (B) (C)
 *
 *  should return A->C
 *****************************************************************/
void test_getPathToNode_should_return_the_path_to_that_nodeC(void){
  Node* nodeA = createNode(0);
  Node* nodeB = createNode(1);
  Node* nodeC = createNode(1);
  
  addChild(&nodeA, &nodeB);
  addChild(&nodeA, &nodeC);
  LinkedList* checkList = createLinkedList();
  addListLast(checkList, nodeA);
  addListLast(checkList, nodeC);
  
  LinkedList* testList = getPathToNode(&nodeA, nodeC);
  TEST_ASSERT_LINKED_LIST(checkList, testList);
}


/**
 *  getPathToNode(A, D)
 *    (A)
 *    / \
 *  (B) (C)
 *       |
 *      (D)
 *
 *
 *  should return A->C->D without ending at nodeB
 *****************************************************************/
void test_getPathToNode_should_return_the_path_to_that_nodeD(void){
  Node* nodeA = createNode(0);
  Node* nodeB = createNode(1);
  Node* nodeC = createNode(1);
  Node* nodeD = createNode(2);
  
  addChild(&nodeA, &nodeB);
  addChild(&nodeA, &nodeC);
  addChild(&nodeC, &nodeD);
  LinkedList* checkList = createLinkedList();
  addListLast(checkList, nodeA);
  addListLast(checkList, nodeC);
  addListLast(checkList, nodeD);
  
  LinkedList* testList = getPathToNode(&nodeA, nodeD);
  TEST_ASSERT_LINKED_LIST(checkList, testList);
}

/**
 *  getPathToNode(A, D)
 *    (A)
 *    / \
 * >(B) (C)
 * <<    |
 *      (D)
 *
 *
 *  should return A->C->D without enter infinity loop at nodeB
 *****************************************************************/
void test_getPathToNode_should_not_enter_infinity_loop(void){
  Node* nodeA = createNode(0);
  Node* nodeB = createNode(1);
  Node* nodeC = createNode(1);
  Node* nodeD = createNode(2);
  
  addChild(&nodeA, &nodeB);
  addChild(&nodeB, &nodeB);
  addChild(&nodeA, &nodeC);
  addChild(&nodeC, &nodeD);
  LinkedList* checkList = createLinkedList();
  addListLast(checkList, nodeA);
  addListLast(checkList, nodeC);
  addListLast(checkList, nodeD);
  
  LinkedList* testList = getPathToNode(&nodeA, nodeD);
  TEST_ASSERT_LINKED_LIST(checkList, testList);
}

/**
 *  getListTillNode(B)
 *
 *  NodeA:
 *  x = 3         x0 = 3
 *  y = 4         y0 = 4
 *
 *  NodeB:
 *  y = 6         y1 = x0 + y0
 *  
 *  should return a LinkedList with x0->y1
 *
 ***************************************************************/
void test_getListTillNode_given_NodeB_above_should_return_x2_and_y0(void){
  Node* nodeA = createNode(0);
  Node* nodeB = createNode(1);
  Expression* exp1 = createExpression(x, ASSIGN, 3, 0, 0);
  Expression* exp2 = createExpression(y, ASSIGN, 4, 0, 0);
  Expression* exp3 = createExpression(y, ADDITION, x, y, 0);
  
  addChild(&nodeA, &nodeB);
  
  addListLast(nodeA->block, exp1);
  addListLast(nodeA->block, exp2);
  addListLast(nodeA->block, exp3);
  setLastBrhDom(&nodeA);
  assignAllNodeSSA(nodeA, createLinkedList(), createLinkedList());
  
  LinkedList* testList = getListTillNode(nodeB);
  TEST_ASSERT_EQUAL(2, testList->length);
  TEST_ASSERT_SUBSCRIPT(x, 0, testList->head->node);  
  TEST_ASSERT_SUBSCRIPT(y, 1, testList->head->next->node);  
}

/**
 *          
 *     (A)        NodeA:                  NodeB:
 *     / \        x = 3                   x = x + x
 *   (B) (C)      z = 6                   y = 5
 *     \ /        c = x != x 
 *     (D)        if(c0) goto NodeC
 *        
 *                NodeC:                  NodeD:
 *                x = x * x               x = z + x
 *        
 *  getAllLiveList(NodeA, empty LinkedList) should return x->z
 *
 **/
void test_getAllLiveList_given_NodeD_that_read_z0_should_return_x_z(void){
  Node* nodeA = createNode(0);
  Node* nodeB = createNode(1);
  Node* nodeC = createNode(1);
  Node* nodeD = createNode(2);
  Expression* expA1 = createExpression(x, ASSIGN, 3, 0, 0);
  Expression* expA2 = createExpression(z, ASSIGN, 6, 0, 0);
  Expression* expA3 = createExpression(c, NOT_EQUAL_TO, x, x, 0);
  Expression* expA4 = createExpression(0, IF_STATEMENT, c, (int)&nodeC, 0);
  Expression* expB1 = createExpression(x, ADDITION, x, x, 0);
  Expression* expB2 = createExpression(y, ASSIGN, 5, 0, 0);
  Expression* expC  = createExpression(x, MULTIPLICATION, x, x, 0);
  Expression* expD  = createExpression(x, ADDITION, z, x, 0);
  addListLast(nodeA->block, expA1);
  addListLast(nodeA->block, expA2);
  addListLast(nodeA->block, expA3);
  addListLast(nodeA->block, expA4);
  addListLast(nodeB->block, expB1);
  addListLast(nodeB->block, expB2);
  addListLast(nodeC->block, expC);
  addListLast(nodeD->block, expD);
  
  addChild(&nodeA, &nodeB);
  addChild(&nodeA, &nodeC);
  addChild(&nodeB, &nodeD);
  addChild(&nodeC, &nodeD);
  
  setLastBrhDom(&nodeA);
  
  LinkedList* testList = getAllLiveList(&nodeA, NULL);
  TEST_ASSERT_EQUAL(2, testList->length);
  TEST_ASSERT_SUBSCRIPT(x, 0, testList->head->node);
  TEST_ASSERT_SUBSCRIPT(z, 0, testList->head->next->node);
}