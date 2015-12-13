#include "unity.h"
#include "PhiFunction.h"
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
  Expression* expA = createExpression('x', 0, ASSIGN, 12, 0, 0);
  Expression* expB = createExpression('x', 1, NORMAL_OPERATOR, 1, 5, 0);
  Expression* expC = createExpression('x', 2, NORMAL_OPERATOR, 2, 6, 0);
  Expression* expD = createExpression('x', 3, NORMAL_OPERATOR, 3, 7, 0);
  addListFirst(nodeA->block->expression, expA);
  addListFirst(nodeB->block->expression, expB);
  addListFirst(nodeC->block->expression, expC);
  addListFirst(nodeD->block->expression, expD);
  
  addChild(&nodeA, &nodeB);
  addChild(&nodeA, &nodeC);
  addChild(&nodeB, &nodeD);
  addChild(&nodeC, &nodeD);
  
  setLastBrhDom(&nodeA);

  allocPhiFunc(nodeA);
  TEST_ASSERT_EQUAL(2, nodeD->block->expression->length);
  TEST_ASSERT_EQUAL(1, nodeA->block->expression->length);
  TEST_ASSERT_EQUAL(1, nodeB->block->expression->length);
  TEST_ASSERT_EQUAL(1, nodeC->block->expression->length);

  Expression* testPhi = createExpression('x', 3, PHI_FUNC, 1, 2, 1);
  TEST_ASSERT_PHIFUNC(testPhi, &nodeD);
}
