#include "unity.h"
#include "ExpressionBlock.h"
#include "LinkedList.h"
#include "ErrorObject.h"
#include "customAssertion.h"
#include "NodeChain.h"
#include <stdlib.h>

void setUp(void){}

void tearDown(void){}

void test_createExpression(void){
  Expression* addExpress = createExpression('x',NORMAL_OPERATOR, 'y', 'z', 0);
  
  TEST_ASSERT_SUBSCRIPT('x', 0, &addExpress->id);
  TEST_ASSERT_EQUAL(NORMAL_OPERATOR, addExpress->opr);
  TEST_ASSERT_SUBSCRIPT('y', 0, &addExpress->oprdA);
  TEST_ASSERT_SUBSCRIPT('z', 0, &addExpress->oprdB);
  TEST_ASSERT_EQUAL(0, addExpress->condition);
}


/** inputList
 *  x = y + z
 *  y = z + x
 *  z = x + y
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
 *  subscript in every expressions into a LinkedList in the format
 *  of operand A => operand B => resulting => ... => GND
 *  where ... is the subscript set of the next expressions and so on.
 *  
 **************************************************************/
void test_getSubsList_should_arrange_the_subscript_list(void){
  Expression* exp1 = createExpression('x', NORMAL_OPERATOR, 'y', 'z', 0);
  Expression* exp2 = createExpression('y', NORMAL_OPERATOR, 'z', 'x', 0);
  Expression* exp3 = createExpression('z', NORMAL_OPERATOR, 'x', 'y', 0);
  
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
 *  ListA             ListB
 *  x0 = x0 + x0      x1 = x0 + x0
 *
 *********************************************************/
void test_arrangeSSA_given_ListA_should_change_result_x_to_subscript_1(void){
  Expression* exp1 = createExpression('x', NORMAL_OPERATOR, 'x', 'x', 0);
  
  Node* testNode = createNode(0);
  addListLast(testNode->block, exp1);

  arrangeSSA(testNode);
  
  ListElement* elemPtr = testNode->block->head;
  TEST_ASSERT_SUBSCRIPT('x', 1, &((Expression*)elemPtr->node)->id);
  TEST_ASSERT_SUBSCRIPT('x', 0, &((Expression*)elemPtr->node)->oprdA);
  TEST_ASSERT_SUBSCRIPT('x', 0, &((Expression*)elemPtr->node)->oprdA);
}

/**
 *  ListA             ListB
 *  x0 = y0 + z0      x0 = y0 + z0
 *  x0 = x0 + a0      x1 = x0 + a0
 *  z0 = z0 + x0      z1 = z0 + x1
 *  z0 = z0 + x0      z2 = z1 + x1
 *
 *********************************************************/
void test_arrangeSSA_given_ListA_should_modify_to_ListB(void){
  Expression* exp1 = createExpression('x', NORMAL_OPERATOR, 'y', 'z', 0);
  Expression* exp2 = createExpression('x', NORMAL_OPERATOR, 'x', 'a', 0);
  Expression* exp3 = createExpression('z', NORMAL_OPERATOR, 'z', 'x', 0);
  Expression* exp4 = createExpression('z', NORMAL_OPERATOR, 'z', 'x', 0);

  
  Node* testNode = createNode(0);
  addListLast(testNode->block, exp1);
  addListLast(testNode->block, exp2);
  addListLast(testNode->block, exp3);
  addListLast(testNode->block, exp4);
  
  arrangeSSA(testNode);
  
  ListElement* elemPtr = testNode->block->head;
  TEST_ASSERT_SUBSCRIPT('x', 0, &((Expression*)elemPtr->node)->id);
  TEST_ASSERT_SUBSCRIPT('y', 0, &((Expression*)elemPtr->node)->oprdA);
  TEST_ASSERT_SUBSCRIPT('z', 0, &((Expression*)elemPtr->node)->oprdB);
  
  elemPtr = elemPtr->next;
  TEST_ASSERT_SUBSCRIPT('x', 1, &((Expression*)elemPtr->node)->id);
  TEST_ASSERT_SUBSCRIPT('x', 0, &((Expression*)elemPtr->node)->oprdA);
  TEST_ASSERT_SUBSCRIPT('a', 0, &((Expression*)elemPtr->node)->oprdB);
  
  elemPtr = elemPtr->next;
  TEST_ASSERT_SUBSCRIPT('z', 1, &((Expression*)elemPtr->node)->id);
  TEST_ASSERT_SUBSCRIPT('z', 0, &((Expression*)elemPtr->node)->oprdA);
  TEST_ASSERT_SUBSCRIPT('x', 1, &((Expression*)elemPtr->node)->oprdB);

  elemPtr = elemPtr->next;
  TEST_ASSERT_SUBSCRIPT('z', 2, &((Expression*)elemPtr->node)->id);
  TEST_ASSERT_SUBSCRIPT('z', 1, &((Expression*)elemPtr->node)->oprdA);
  TEST_ASSERT_SUBSCRIPT('x', 1, &((Expression*)elemPtr->node)->oprdB);
}

/**
 *  Block0:
 *  tail
 *      \__________
 *                 \
 *  head            \
 *      \-[expr1]-[expr2]_
 *                        |
 *                       GND
 *****************************************************************************/
void test_getCondition_with_the_Block0_above_should_return_expr1(void){
  Node* nodeA = createNode(0);
  Expression* expr1 = createExpression('c', EQUAL_TO, 'x', 'y', 0);
  Expression* expr2 = createExpression(0, IF_STATEMENT, 'c', 'z', 0);
  addListLast(nodeA->block, expr1);
  addListLast(nodeA->block, expr2);
  
  Subscript* testExpr = getCondition(nodeA);
  
  TEST_ASSERT_EQUAL_PTR(&(expr1->id), testExpr);
}

/**
 *  Block1:
 *  tail
 *      \________________________________________
 *                                                \
 *  head                                           \
 *      \-[expr1]-[expr2]-[expr3]-[expr4]-[expr5]-[expr6]_
 *                                                        |
 *                                                       GND
 *****************************************************************************/
void test_getCondition_with_the_Block1_above_should_return_expr5(void){
  Node* nodeA = createNode(0);
  Expression* expr1 = createExpression('x', ASSIGN, 12, 0, 0);
  Expression* expr2 = createExpression('y', ASSIGN,  1, 0, 0);
  Expression* expr3 = createExpression('z', ASSIGN,  2, 0, 0);
  Expression* expr4 = createExpression('x', NORMAL_OPERATOR, 3, 7, 0);
  Expression* expr5 = createExpression('c', GREATER_THAN, 12, 3, 0);
  Expression* expr6 = createExpression('c', IF_STATEMENT,  3, 7, 0);
  addListLast(nodeA->block, expr1);
  addListLast(nodeA->block, expr2);
  addListLast(nodeA->block, expr3);
  addListLast(nodeA->block, expr4);
  addListLast(nodeA->block, expr5);
  addListLast(nodeA->block, expr6);
  
  Subscript* testExpr = getCondition(nodeA);
  
  TEST_ASSERT_EQUAL_PTR(&expr5->id, testExpr);
}







