#include "unity.h"
#include "PhiFunction.h"
#include "ExpressionBlock.h"
#include "GetList.h"
#include "NodeChain.h"
#include "ErrorObject.h"
#include "customAssertion.h"
#include "LinkedList.h"
#include "CException.h"
#include <stdlib.h>
#include <stdio.h>

void setUp(void){}

void tearDown(void){}

/**
 *  ControlFlowGraph1
 *
 *       [ A ]
 *       |   |
 *      \/  \/
 *     [B]  [C]
 *      |    |
 *     \/   \/
 *       [D]          <= PhiFunction(B,C)
 *
 *  PhiFunction should allocated in D
 *
 **/
void test_phiFunction_allocation(void){
	Node* nodeA = createNode(0);
  Node* nodeB = createNode(1);
  Node* nodeC = createNode(1);
  Node* nodeD = createNode(2);
  Expression* expA = createExpression('x', ASSIGN, 'z', 'z', 0);
  Expression* expB = createExpression('x', ADDITION, 'y', 'z', 0);
  Expression* expC = createExpression('x', ADDITION, 'x', 'y', 0);
  Expression* expD = createExpression('x', ADDITION, 'x', 'z', 0);
  addListFirst(nodeA->block, expA);
  addListFirst(nodeB->block, expB);
  addListFirst(nodeC->block, expC);
  addListFirst(nodeD->block, expD);
  
  addChild(&nodeA, &nodeB);
  addChild(&nodeA, &nodeC);
  addChild(&nodeB, &nodeD);
  addChild(&nodeC, &nodeD);
  
  setLastBrhDom(&nodeA);

  allocPhiFunc(nodeA);
  TEST_ASSERT_EQUAL(2, nodeD->block->length);
  TEST_ASSERT_EQUAL(1, nodeA->block->length);
  TEST_ASSERT_EQUAL(1, nodeB->block->length);
  TEST_ASSERT_EQUAL(1, nodeC->block->length);

  Expression* testPhi = createExpression('x', PHI_FUNC, 1, 2, 1);
  TEST_ASSERT_PHIFUNC(testPhi, &nodeD);
}

/**
 *  getCondition
 *  should return the subscript that represent the condition of it's immediate
 *  dominator.
 *
 *  Block0:
 *  tail
 *      \__________
 *                 \
 *  head            \
 *      \-[expr1]-[expr2]_
 *                        |
 *                       GND
 *
 *  Assume a preprogram will split the following expression into two expression
 *  
 *  if(i == 10) goto y     =>   condition = (i == 10)   (expr1)
 *                              if(condition) goto y    (expr2)
 *
 *  getCondition function should return condition
 *
 *****************************************************************************/
// void test_getCondition_with_the_Block0_above_should_return_expr1(void){
  // Node* nodeA = createNode(0);
  // Node* nodeB = createNode(1);
  // Expression* expr1 = createExpression('c', EQUAL_TO, 'x', 'y', 0);
  // Expression* expr2 = createExpression(0, IF_STATEMENT, 'c', ((int)&nodeB), 0);
  // addListLast(nodeA->block, expr1);
  // addListLast(nodeA->block, expr2);
  // arrangeSSA(nodeA);
  
  // Subscript* testExpr = getCondition(nodeA);
  
  // TEST_ASSERT_EQUAL_PTR(&(expr2->oprdA), testExpr);
// }

/**
 *  
 *
 *  Block1:
 *  x0 = 12;
 *  y0 = 1;
 *  z0 = 2
 *  x1 = x0 + y0;
 *  c0 = z0 > 70
 *  if(c0) goto nodeA
 *
 *****************************************************************************/
// void test_getCondition_with_the_Block1_above_should_return_expr5(void){
  // Node* nodeA = createNode(0);
  // Expression* expr1 = createExpression('x', ASSIGN, 12, 0, 0);
  // Expression* expr2 = createExpression('y', ASSIGN,  1, 0, 0);
  // Expression* expr3 = createExpression('z', ASSIGN,  2, 0, 0);
  // Expression* expr4 = createExpression('x', ADDITION, 'x', 'y', 0);
  // Expression* expr5 = createExpression('c', GREATER_THAN, 'z', 'y', 0);
  // Expression* expr6 = createExpression(0, IF_STATEMENT,  'c', (int)&nodeA, 0);
  // addListLast(nodeA->block, expr1);
  // addListLast(nodeA->block, expr2);
  // addListLast(nodeA->block, expr3);
  // addListLast(nodeA->block, expr4);
  // addListLast(nodeA->block, expr5);
  // addListLast(nodeA->block, expr6);
  // arrangeSSA(nodeA);
  
  // Subscript* testExpr = getCondition(nodeA);
  
  // TEST_ASSERT_EQUAL_PTR(&expr6->oprdA, testExpr);
// }