#include "unity.h"
#include "PhiFunction.h"
#include "NodeChain.h"
#include "ErrorObject.h"
#include "customAssertion.h"
#include "LinkedList.h"
#include "CException.h"
#include <stdlib.h>

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
  Expression* exp = createExpression(0, 25, 3, ASSIGN, 10, 15);
  
  addChild(&nodeA, &nodeB);
  addChild(&nodeA, &nodeC);
  addChild(&nodeB, &nodeD);
  addChild(&nodeC, &nodeD);
  
  setLastBrhDom(&nodeA);
  TEST_ASSERT_NULL(nodeD->block->expression->head);
  TEST_ASSERT_NULL(nodeA->block->expression->head);
  TEST_ASSERT_NULL(nodeB->block->expression->head);
  TEST_ASSERT_NULL(nodeC->block->expression->head);

  allocPhiFunc(nodeA);
  
  TEST_ASSERT_NULL(nodeA->block->expression->head);
  // TEST_ASSERT_NULL(nodeB->block->expression->head);
  // TEST_ASSERT_NULL(nodeC->block->expression->head);
  TEST_ASSERT_NOT_NULL(nodeD->block->expression->head);
}
