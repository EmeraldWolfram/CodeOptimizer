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
 *       ControlFlowGraph1
 *
 *        [ A ]
 *       |   |
 *      \/  \/
 *     [B]  [C]
 *      |    |
 *     \/   \/
 *       [D]
 *
 **/
void test_phiFunction_allocation(void){
	Node* nodeA = createNode(0);
  Node* nodeB = createNode(1);
  Node* nodeC = createNode(1);
  Node* nodeD = createNode(2);

  addChild(&nodeA, &nodeB);
  addChild(&nodeA, &nodeC);
  addChild(&nodeB, &nodeD);
  addChild(&nodeC, &nodeD);
  
  setLastBrhDom(&nodeA);

  allocPhiFunc(&nodeA);
}
