#include "unity.h"
#include "ExpressionBlock.h"
#include "LinkedList.h"
#include "ErrorObject.h"
#include "NodeChain.h"
#include <stdlib.h>

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
  Expression* expr1 = createExpression('c', 0, EQUAL_TO, 12, 12, 0);
  Expression* expr2 = createExpression('c', 0, IF_STATEMENT,  3, 7, 0);
  addListLast(nodeA->block, expr1);
  addListLast(nodeA->block, expr2);
  
  Expression* testExpr = getCondition(nodeA);
  
  TEST_ASSERT_EQUAL_PTR(expr1, testExpr);
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
  Expression* expr1 = createExpression('x', 0, ASSIGN, 12, 0, 0);
  Expression* expr2 = createExpression('y', 0, ASSIGN,  1, 0, 0);
  Expression* expr3 = createExpression('z', 0, ASSIGN,  2, 0, 0);
  Expression* expr4 = createExpression('x', 0, NORMAL_OPERATOR, 3, 7, 0);
  Expression* expr5 = createExpression('c', 0, GREATER_THAN, 12, 3, 0);
  Expression* expr6 = createExpression('c', 0, IF_STATEMENT,  3, 7, 0);
  addListLast(nodeA->block, expr1);
  addListLast(nodeA->block, expr2);
  addListLast(nodeA->block, expr3);
  addListLast(nodeA->block, expr4);
  addListLast(nodeA->block, expr5);
  addListLast(nodeA->block, expr6);
  
  Expression* testExpr = getCondition(nodeA);
  
  TEST_ASSERT_EQUAL_PTR(expr5, testExpr);
}







