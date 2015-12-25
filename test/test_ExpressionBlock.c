#include "unity.h"
#include "ExpressionBlock.h"
#include "GetList.h"
#include "LinkedList.h"
#include "ErrorObject.h"
#include "customAssertion.h"
#include "NodeChain.h"
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
 *  createExpression(x,ADDITION, y, z, 0)
 *  should be able to create the following SSA format expression
 *  
 *  [ x | 0 | ADDITION | y | 0 | z | 0 | 0 ]
 *  
 *  which represent the equation x0 = y0 + z0 in SSA format
 *  
 *  The last column will remain zero for all the operation except
 *  for condition.
 *  
 ****************************************************************************/
void test_createExpression(void){
  Expression* addExpress = createExpression(x,ADDITION, y, z, 0);
  
  TEST_ASSERT_SUBSCRIPT(x, 0, &addExpress->id);
  TEST_ASSERT_EQUAL(ADDITION, addExpress->opr);
  TEST_ASSERT_SUBSCRIPT(y, 0, &addExpress->oprdA);
  TEST_ASSERT_SUBSCRIPT(z, 0, &addExpress->oprdB);
  TEST_ASSERT_EQUAL(0, addExpress->condition);
}


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
 *  arrangeSSA
 *  should arrange the subscript to of the expression correctly
 *  as shown below. The input ListA should be able to transform
 *  into ListB after calling arrangeSSA function.
 *  
 *  ListA             ListB
 *  x0 = x0 + x0      x1 = x0 + x0
 *
 *********************************************************/
void test_arrangeSSA_given_ListA_should_change_result_x_to_subscript_1(void){
  Expression* exp1 = createExpression(x, ADDITION, x, x, 0);
  
  Node* testNode = createNode(0);
  addListLast(testNode->block, exp1);

  arrangeSSA(testNode);
  
  ListElement* elemPtr = testNode->block->head;
  TEST_ASSERT_SUBSCRIPT(x, 1, &((Expression*)elemPtr->node)->id);
  TEST_ASSERT_SUBSCRIPT(x, 0, &((Expression*)elemPtr->node)->oprdA);
  TEST_ASSERT_SUBSCRIPT(x, 0, &((Expression*)elemPtr->node)->oprdA);
}

/**
 *  arrangeSSA
 *  should arrange the subscript to of the expression correctly
 *  as shown below. The input ListA should be able to transform
 *  into ListB after calling arrangeSSA function.
 *
 *  ListA             ListB
 *  x0 = y0 + z0      x0 = y0 + z0
 *  x0 = x0 + a0      x1 = x0 + a0
 *  z0 = z0 + x0      z1 = z0 + x1
 *  z0 = z0 + x0      z2 = z1 + x1
 *
 *  This test use to test whether the function able to handle
 *  LHS of the equation when the RHS is listed before but not
 *  in the LHS before.
 *
 *********************************************************/
void test_arrangeSSA_given_ListA_should_modify_to_ListB(void){
  Expression* exp1 = createExpression(x, ADDITION, y, z, 0);
  Expression* exp2 = createExpression(x, ADDITION, x, a, 0);
  Expression* exp3 = createExpression(z, ADDITION, z, x, 0);
  Expression* exp4 = createExpression(z, ADDITION, z, x, 0);

  
  Node* testNode = createNode(0);
  addListLast(testNode->block, exp1);
  addListLast(testNode->block, exp2);
  addListLast(testNode->block, exp3);
  addListLast(testNode->block, exp4);
  
  arrangeSSA(testNode);
  
  ListElement* elemPtr = testNode->block->head;
  TEST_ASSERT_SUBSCRIPT(x, 0, &((Expression*)elemPtr->node)->id);
  TEST_ASSERT_SUBSCRIPT(y, 0, &((Expression*)elemPtr->node)->oprdA);
  TEST_ASSERT_SUBSCRIPT(z, 0, &((Expression*)elemPtr->node)->oprdB);
  
  elemPtr = elemPtr->next;
  TEST_ASSERT_SUBSCRIPT(x, 1, &((Expression*)elemPtr->node)->id);
  TEST_ASSERT_SUBSCRIPT(x, 0, &((Expression*)elemPtr->node)->oprdA);
  TEST_ASSERT_SUBSCRIPT(a, 0, &((Expression*)elemPtr->node)->oprdB);
  
  elemPtr = elemPtr->next;
  TEST_ASSERT_SUBSCRIPT(z, 1, &((Expression*)elemPtr->node)->id);
  TEST_ASSERT_SUBSCRIPT(z, 0, &((Expression*)elemPtr->node)->oprdA);
  TEST_ASSERT_SUBSCRIPT(x, 1, &((Expression*)elemPtr->node)->oprdB);

  elemPtr = elemPtr->next;
  TEST_ASSERT_SUBSCRIPT(z, 2, &((Expression*)elemPtr->node)->id);
  TEST_ASSERT_SUBSCRIPT(z, 1, &((Expression*)elemPtr->node)->oprdA);
  TEST_ASSERT_SUBSCRIPT(x, 1, &((Expression*)elemPtr->node)->oprdB);
}

/**
 *  arrangeSSA
 *
 *  This test check if the function can handle the case when there are
 *  ASSIGN and IF_STATEMENT without bad memory access
 *
 ************************************************************************/
void test_arrangeSSA_with_IF_STATEMENT_and_ASSIGN(void){
  Node* nodeA = createNode(0);
  Node* nodeB = createNode(1);
  
  Expression* expr0 = createExpression(x, ASSIGN, 12, 0, 0);
  Expression* expr1 = createExpression(c, EQUAL_TO, x, y, 0);
  Expression* expr2 = createExpression(0, IF_STATEMENT, c, ((int)&nodeB), 0);
  
  addListLast(nodeA->block, expr0);
  addListLast(nodeA->block, expr1);
  addListLast(nodeA->block, expr2);
  
  arrangeSSA(nodeA);
  
  TEST_ASSERT_SUBSCRIPT(c, 0, &expr2->oprdA);
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
 *  getLiveList
 *
 *  x0 = y0 + z0
 *  x1 = z0 + x0
 *  y1 = z0 + x1
 *  y2 = z0 + y1
 *  x2 = x1 + y2
 *
 *  should return x2->y2
 *
 *************************************************************************/
void test_getLiveList_should_return_x2_y2_when_the_expression_list_is_given(void){
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
  
  LinkedList* testList = getLiveList(testNode, createLinkedList());

  TEST_ASSERT_EQUAL(2, testList->length);
  TEST_ASSERT_SUBSCRIPT(x, 2, testList->head->node);
  TEST_ASSERT_SUBSCRIPT(y, 2, testList->head->next->node);
}

/**
 *  getLiveList
 *
 *  prevList = w0->z0->x0
 *
 *  x1 = y0 + x0
 *  y1 = z0 + x1
 *  y2 = z0 + y1
 *  x2 = x1 + y2
 *
 *  getLiveList should update the prevList and return w0->z0->x2->y2
 *
 *************************************************************************/
void test_getLiveList_should_return_prevList_and_add_on_x2_y2_when_the_expression_list_is_given(void){
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
  LinkedList* testList = getLiveList(testNode, prevList);

  TEST_ASSERT_EQUAL(4, testList->length);
  TEST_ASSERT_SUBSCRIPT(w, 0, testList->head->node);
  TEST_ASSERT_SUBSCRIPT(z, 0, testList->head->next->node);
  TEST_ASSERT_SUBSCRIPT(x, 2, testList->head->next->next->node);
  TEST_ASSERT_SUBSCRIPT(y, 2, testList->head->next->next->next->node);
}

/**
 *  assignAllNodeSSA
 *
 *  BEFORE                  AFTER
 *
 *    NodeA:                NodeA:
 *    x0 = 3;               x0 = 3;
 *    x0 = x0 + x0          x1 = x0 + x0
 *    goto NodeB            goto NodeB
 *
 *    NodeB:                NodeB:
 *    x0 = x0 * x0          x2 = x1
 *                          x3 = x2 * x2;
 *
 *************************************************************************/
void test_assignAllNodeSSA_(void){
  Expression* exp1 = createExpression(x, ASSIGN, 3, 0, 0);
  Expression* exp2 = createExpression(x, ADDITION, x, x, 0);
  Expression* exp3 = createExpression(x, MULITPLICATION, x, x, 0);
  
  Node* nodeA = createNode(0);
  Node* nodeB = createNode(1);
  addChild(&nodeA, &nodeB);
  
  addListLast(nodeA->block, exp1);
  addListLast(nodeA->block, exp2);
  addListLast(nodeB->block, exp3);

  setLastBrhDom(&nodeA);
  setAllImdDom(&nodeA);
  
  assignAllNodeSSA(nodeA, createLinkedList(), createLinkedList());
  ListElement* testExp = nodeA->block->head;
  
  TEST_ASSERT_SUBSCRIPT(x, 0, &((Expression*)testExp->node)->id);
  TEST_ASSERT_SUBSCRIPT(x, 1, &((Expression*)testExp->next->node)->id);
  TEST_ASSERT_SUBSCRIPT(x, 0, &((Expression*)testExp->next->node)->oprdA);
  TEST_ASSERT_SUBSCRIPT(x, 0, &((Expression*)testExp->next->node)->oprdB);
  
  testExp = nodeB->block->head;
  TEST_ASSERT_SUBSCRIPT(x, 2, &((Expression*)testExp->node)->id);
  TEST_ASSERT_SUBSCRIPT(x, 1, &((Expression*)testExp->node)->oprdA);
  TEST_ASSERT_SUBSCRIPT(x, 3, &((Expression*)testExp->next->node)->id);
  TEST_ASSERT_SUBSCRIPT(x, 2, &((Expression*)testExp->next->node)->oprdA);
  TEST_ASSERT_SUBSCRIPT(x, 2, &((Expression*)testExp->next->node)->oprdB);
}


/**
 *  assignAllNodeSSA
 *
 *  BEFORE                              AFTER
 *
 *    NodeA:                            NodeA:
 *    x0 = 3                            x0 = 3;
 *    y0 = 5                            y0 = 5;
 *    z0 = 7                            z0 = 7
 *    x0 = y0 + x0                      x1 = y0 + x0
 *    goto NodeB                        goto NodeB
 *
 *    NodeB:                            NodeB:
 *    z0 = y0 * x0                      z1 = z0
 *                                      y1 = y0
 *                                      x2 = x1
 *                                      z2 = y1 * x2;
 *
 *************************************************************************/
void test_assignAllNodeSSA_with_muliple_variable(void){
  Expression* exp1 = createExpression(x, ASSIGN, 3, 0, 0);
  Expression* exp2 = createExpression(y, ASSIGN, 5, 0, 0);
  Expression* exp3 = createExpression(z, ASSIGN, 7, 0, 0);
  Expression* exp4 = createExpression(x, ADDITION, y, x, 0);
  Expression* exp5 = createExpression(z, MULITPLICATION, y, x, 0);
  
  Node* nodeA = createNode(0);
  Node* nodeB = createNode(1);
  addChild(&nodeA, &nodeB);
  
  addListLast(nodeA->block, exp1);
  addListLast(nodeA->block, exp2);
  addListLast(nodeA->block, exp3);
  addListLast(nodeA->block, exp4);
  addListLast(nodeB->block, exp5);

  setLastBrhDom(&nodeA);
  setAllImdDom(&nodeA);
  
  assignAllNodeSSA(nodeA, createLinkedList(), createLinkedList());
  ListElement* testExp = nodeA->block->head;
  TEST_ASSERT_SUBSCRIPT(x, 0, &((Expression*)testExp->node)->id);
  TEST_ASSERT_SUBSCRIPT(y, 0, &((Expression*)testExp->next->node)->id);
  TEST_ASSERT_SUBSCRIPT(z, 0, &((Expression*)testExp->next->next->node)->id);
  Expression* testExpr = (Expression*)testExp->next->next->next->node;
  TEST_ASSERT_SUBSCRIPT(x, 1, &testExpr->id);
  TEST_ASSERT_SUBSCRIPT(y, 0, &testExpr->oprdA);
  TEST_ASSERT_SUBSCRIPT(x, 0, &testExpr->oprdB);
  
  testExp = nodeB->block->head;
  TEST_ASSERT_SUBSCRIPT(z, 1, &((Expression*)testExp->node)->id);
  TEST_ASSERT_SUBSCRIPT(z, 0, &((Expression*)testExp->node)->oprdA);
  TEST_ASSERT_SUBSCRIPT(y, 1, &((Expression*)testExp->next->node)->id);
  TEST_ASSERT_SUBSCRIPT(y, 0, &((Expression*)testExp->next->node)->oprdA);
  TEST_ASSERT_SUBSCRIPT(x, 2, &((Expression*)testExp->next->next->node)->id);
  TEST_ASSERT_SUBSCRIPT(x, 1, &((Expression*)testExp->next->next->node)->oprdA);
  testExpr = (Expression*)testExp->next->next->next->node;
  TEST_ASSERT_SUBSCRIPT(z, 2, &testExpr->id);
  TEST_ASSERT_SUBSCRIPT(y, 1, &testExpr->oprdA);
  TEST_ASSERT_SUBSCRIPT(x, 2, &testExpr->oprdB);
}


/**
 *  assignAllNodeSSA
 *
 *  BEFORE                                    AFTER
 *
 *          NodeA:                            NodeA:
 *          x = 4                             x0 = 4
 *          c = (x != x)                      c0 = (x0 != x0)
 *          if(c0) goto Node C                if(c0) goto NodeC
 *          /         \                      /                  \
 *    NodeB:          NodeC:            NodeB:                  NodeC:
 *    x = x * x       x = x + x         x1 = x0                 x6 = x0
 *           \        /                 x2 = x1 * x1            x7 = x6 + x6
 *            NodeD:                            \             /
 *            x = x + x                             NodeD:
 *                                                x4 = x3         (reserved x3 for phiFunction)
 *                                                x5 = x4 + x4
 *
 *************************************************************************/
void test_assignAllNodeSSA_with_muliple_Node(void){
  Node* nodeA = createNode(0);
  Node* nodeB = createNode(1);
  Node* nodeC = createNode(1);
  Node* nodeD = createNode(2);
  Expression* exp1 = createExpression(x, ASSIGN, 4, 0, 0);
  Expression* exp2 = createExpression(c, NOT_EQUAL_TO, x, x, 0);
  Expression* exp3 = createExpression(0, IF_STATEMENT, c, (int)&nodeC, 0);
  Expression* exp4 = createExpression(x, MULITPLICATION, x, x, 0);
  Expression* exp5 = createExpression(x, ADDITION, x, x, 0);
  Expression* exp6 = createExpression(x, ADDITION, x, x, 0);
  
  addChild(&nodeA, &nodeB);
  addChild(&nodeA, &nodeC);
  addChild(&nodeB, &nodeD);
  addChild(&nodeC, &nodeD);
  
  addListLast(nodeA->block, exp1);
  addListLast(nodeA->block, exp2);
  addListLast(nodeA->block, exp3);
  addListLast(nodeB->block, exp4);
  addListLast(nodeC->block, exp5);
  addListLast(nodeD->block, exp6);

  setLastBrhDom(&nodeA);
  setAllImdDom(&nodeA);
  
  assignAllNodeSSA(nodeA, createLinkedList(), createLinkedList());
  ListElement* testExp = nodeA->block->head;
  TEST_ASSERT_SUBSCRIPT(x, 0, &((Expression*)testExp->node)->id);
  TEST_ASSERT_SUBSCRIPT(4, 0, &((Expression*)testExp->node)->oprdA);
  TEST_ASSERT_SUBSCRIPT(c, 0, &((Expression*)testExp->next->node)->id);
  TEST_ASSERT_SUBSCRIPT(x, 0, &((Expression*)testExp->next->node)->oprdA);
  TEST_ASSERT_SUBSCRIPT(x, 0, &((Expression*)testExp->next->node)->oprdB);
  
  testExp = nodeB->block->head;
  TEST_ASSERT_SUBSCRIPT(x, 1, &((Expression*)testExp->node)->id);
  TEST_ASSERT_SUBSCRIPT(x, 0, &((Expression*)testExp->node)->oprdA);
  TEST_ASSERT_SUBSCRIPT(x, 2, &((Expression*)testExp->next->node)->id);
  TEST_ASSERT_SUBSCRIPT(x, 1, &((Expression*)testExp->next->node)->oprdA);
  TEST_ASSERT_SUBSCRIPT(x, 1, &((Expression*)testExp->next->node)->oprdB);
  
  testExp = nodeD->block->head;
  TEST_ASSERT_SUBSCRIPT(x, 4, &((Expression*)testExp->node)->id);
  TEST_ASSERT_SUBSCRIPT(x, 3, &((Expression*)testExp->node)->oprdA);
  TEST_ASSERT_SUBSCRIPT(x, 5, &((Expression*)testExp->next->node)->id);
  TEST_ASSERT_SUBSCRIPT(x, 4, &((Expression*)testExp->next->node)->oprdA);
  TEST_ASSERT_SUBSCRIPT(x, 4, &((Expression*)testExp->next->node)->oprdB);
  
  testExp = nodeC->block->head;
  TEST_ASSERT_SUBSCRIPT(x, 6, &((Expression*)testExp->node)->id);
  TEST_ASSERT_SUBSCRIPT(x, 0, &((Expression*)testExp->node)->oprdA);
  TEST_ASSERT_SUBSCRIPT(x, 7, &((Expression*)testExp->next->node)->id);
  TEST_ASSERT_SUBSCRIPT(x, 6, &((Expression*)testExp->next->node)->oprdA);
  TEST_ASSERT_SUBSCRIPT(x, 6, &((Expression*)testExp->next->node)->oprdB);
}