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

int w = 119;
int x = 120;
int y = 121;
int z = 122;
int a = 97;
int b = 98;
int c = 99;
int d = 100;

void setExpression(Expression* expression, int idIndex, int aIndex, int bIndex){
  expression->id.index    = idIndex;
  expression->oprdA.index = aIndex;
  expression->oprdB.index = bIndex;
}

Subscript* createSubscript(int name, int index){
  Subscript* new = malloc(sizeof(Subscript));
  new->name   = name;
  new->index  = index;
  return new;
}

void setUp(void){}

void tearDown(void){}


/**
 *  getPhiFunction
 *  
 *  ListA : x4->y3->z1
 *  ListB : w0->y2->x1
 *
 *  getPhiFunction(ListA, ListB, x2) should return 
 *  x3 = Phi(x4, x1, 0)
 *  y4 = Phi(y3, y2, 0)
 *  
 *************************************************************/
void test_getPhiFunction_given_ListA_and_ListB_shoudl_return_PhiFunction_of_x(void){
  Node* nodeA = createNode(0);
  Node* nodeB = createNode(1);
  Expression* exp1 = createExpression(z, ADDITION, x, y, 0);
  setExpression(exp1, 1, 4, 3);
  Expression* exp2 = createExpression(x, ADDITION, w, y, 0);
  setExpression(exp2, 1, 0, 2);
  
  addListLast(nodeA->block, exp1);
  addListLast(nodeB->block, exp2);

  LinkedList* listA = getSubsList(nodeA->block);
  LinkedList* listB = getSubsList(nodeB->block);
  
  Subscript* subs = createSubscript(x, 2);
  Expression* phiFunction = getPhiFunction(listA, listB, subs);
  TEST_ASSERT_SUBSCRIPT(x, 3, &phiFunction->id);
  TEST_ASSERT_SUBSCRIPT(x, 4, &phiFunction->oprdA);
  TEST_ASSERT_SUBSCRIPT(x, 1, &phiFunction->oprdB);
  
  subs = createSubscript(y, 3);
  phiFunction = getPhiFunction(listA, listB, subs);
  TEST_ASSERT_SUBSCRIPT(y, 4, &phiFunction->id);
  TEST_ASSERT_SUBSCRIPT(y, 3, &phiFunction->oprdA);
  TEST_ASSERT_SUBSCRIPT(y, 2, &phiFunction->oprdB);
}

/**
 *  getPhiFunction
 *  
 *  ListA : x4->y3->z1
 *  ListB : w0->y2->x1
 *
 *  getPhiFunction(ListA, ListB, w3) should Throw error 
 *  ERR_UNDECLARE_VARIABLE
 *************************************************************/
void test_getPhiFunction_given_ListA_and_ListB_shoudl_throw_ERR_UNDECLARE_VARIABLE(void){
  ErrorObject* err;
  Try{
    Node* nodeA = createNode(0);
    Node* nodeB = createNode(1);
    Expression* exp1 = createExpression(z, ADDITION, x, y, 0);
    setExpression(exp1, 1, 4, 3);
    Expression* exp2 = createExpression(x, ADDITION, w, y, 0);
    setExpression(exp2, 1, 0, 2);
    
    addListLast(nodeA->block, exp1);
    addListLast(nodeB->block, exp2);
    
    LinkedList* listA = getSubsList(nodeA->block);
    LinkedList* listB = getSubsList(nodeB->block);
    
    Subscript* subs = createSubscript(w, 3);
    Expression* phiFunction = getPhiFunction(listA, listB, subs);
    TEST_FAIL_MESSAGE("Expected ERR_UNDECLARE_VARIABLE but no error thrown");
  } Catch(err){
    TEST_ASSERT_EQUAL(ERR_UNDECLARE_VARIABLE, err->errorCode);
    TEST_ASSERT_EQUAL_STRING("Undefine reference to Subscript w", err->errorMsg);
    free(err);
  }
  
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
void test_getCondition_with_the_Block0_above_should_return_operandA_of_expr2(void){
  Node* nodeA = createNode(0);
  Node* nodeB = createNode(1);
  Expression* expr1 = createExpression(c, EQUAL_TO, x, y, 0);
  Expression* expr2 = createExpression(0, IF_STATEMENT, c, ((int)&nodeB), 0);
  addListLast(nodeA->block, expr1);
  addListLast(nodeA->block, expr2);
  arrangeSSA(nodeA);
  
  Subscript testSubs = getCondition(nodeA);
  
  TEST_ASSERT_SUBSCRIPT(c, 0, &testSubs);
}

/**
 *  Block1:
 *  x0 = 12;
 *  y0 = 1;
 *  z0 = 2
 *  x1 = x0 + y0;
 *  c0 = z0 > 70
 *  if(c0) goto nodeA
 *
 *****************************************************************************/
void test_getCondition_with_the_Block1_above_should_travel_all_the_way_down(void){
  Node* nodeA = createNode(0);
  Expression* expr1 = createExpression(x, ASSIGN, 12, 0, 0);
  Expression* expr2 = createExpression(y, ASSIGN,  1, 0, 0);
  Expression* expr3 = createExpression(z, ASSIGN,  2, 0, 0);
  Expression* expr4 = createExpression(x, ADDITION, x, y, 0);
  Expression* expr5 = createExpression(c, GREATER_THAN, z, y, 0);
  Expression* expr6 = createExpression(0, IF_STATEMENT, c, (int)&nodeA, 0);
  addListLast(nodeA->block, expr1);
  addListLast(nodeA->block, expr2);
  addListLast(nodeA->block, expr3);
  addListLast(nodeA->block, expr4);
  addListLast(nodeA->block, expr5);
  addListLast(nodeA->block, expr6);
  arrangeSSA(nodeA);
  
  Subscript testSubs = getCondition(nodeA);
  
  TEST_ASSERT_SUBSCRIPT(c, 0, &testSubs);
}

/**
 *  Block2:
 *  x0 = 12;
 *  c0 = y0 == y0
 *  if(c0) goto nodeA
 *  x1 = x0 + y0;
 *  
 *****************************************************************************/
void test_getCondition_with_the_Block2_above_should_throw_ERR_INVALID_BRANCH(void){
  ErrorObject *err;
  Try{
    Node* nodeA = createNode(0);
    Expression* expr1 = createExpression(x, ASSIGN, 12, 0, 0);
    Expression* expr2 = createExpression(c, EQUAL_TO, y, y, 0);
    Expression* expr3 = createExpression(0, IF_STATEMENT, c, (int)&nodeA, 0);
    Expression* expr4 = createExpression(x, ADDITION, x, y, 0);
    addListLast(nodeA->block, expr1);
    addListLast(nodeA->block, expr2);
    addListLast(nodeA->block, expr3);
    addListLast(nodeA->block, expr4);
    arrangeSSA(nodeA);
  
    Subscript testSubs = getCondition(nodeA);
    TEST_FAIL_MESSAGE("Expected ERR_INVALID_BRANCH but none thrown");
  }Catch(err){
    TEST_ASSERT_EQUAL(ERR_INVALID_BRANCH, err->errorCode);
    TEST_ASSERT_EQUAL_STRING("This operator does not branch", err->errorMsg);
    free(err);
  }
}


/**
 *  ControlFlowGraph1
 *          
 *       [A](x0) 
 *      /   \
 *     \/   \/          
 *(x1)[B]   [C](x4)   
 *     |     |
 *     \/   \/
 *       [D]    <=  (x2) = PhiFunction(x4,x1,a)
 *        
 *  PhiFunction should allocated in D
 *
 **/
void test_allocPhiFunc_allocation_after_assignAllNodeSSA(void){
	Node* nodeA = createNode(0);
  Node* nodeB = createNode(1);
  Node* nodeC = createNode(1);
  Node* nodeD = createNode(2);
  Expression* expA1 = createExpression(x, ASSIGN, 14, 0, 0);
  Expression* expA2 = createExpression(a, EQUAL_TO, x, x, 0);
  Expression* expA3 = createExpression(0, IF_STATEMENT, a, (int)&nodeC, 0);
  Expression* expB  = createExpression(x, ADDITION, x, x, 0);
  Expression* expC  = createExpression(x, MULTIPLICATION, x, x, 0);
  Expression* expD  = createExpression(x, ADDITION, x, x, 0);
  addListLast(nodeA->block, expA1);
  addListLast(nodeA->block, expA2);
  addListLast(nodeA->block, expA3);
  addListLast(nodeB->block, expB);
  addListLast(nodeC->block, expC);
  addListLast(nodeD->block, expD);
  
  addChild(&nodeA, &nodeB);
  addChild(&nodeA, &nodeC);
  addChild(&nodeB, &nodeD);
  addChild(&nodeC, &nodeD);
  
  setLastBrhDom(&nodeA);
  setAllDirectDom(&nodeA);
  assignAllNodeSSA(nodeA, createLinkedList(), createLinkedList());
  ErrorObject* err;
  allocPhiFunc(&nodeA);

  TEST_ASSERT_EQUAL(3, nodeA->block->length);
  TEST_ASSERT_EQUAL(1, nodeB->block->length);
  TEST_ASSERT_EQUAL(1, nodeC->block->length);
  TEST_ASSERT_EQUAL(2, nodeD->block->length);


  Expression* testPhi = createExpression(x, PHI_FUNC, x, x, a);
  setExpression(testPhi, 2, 4, 1);
  TEST_ASSERT_PHIFUNC(testPhi, &nodeD);
}


/**
 *  ControlFlowGraph2
 *          
 *         [A](x0) 
 *        /   \          
 *  (x1)[B]   [C](x6)   
 *      / \    |
 *(x2)[D] [E]  |
 *      \  |   |         
 *       [F]   | 
 *          \ /
 *          [G]
 *
 *  PhiFunction should allocated in F and G
 *  Node F: x3 = PhiFunction(x2, x5)
 *  Node G: x5 = PhiFunction(x3, x6)
 *
 **/
void test_allocPhiFunc_allocation_after_assignAllNodeSSA_with_empty_nodeF(void){
	Node* nodeA = createNode(0);
  Node* nodeB = createNode(1);
  Node* nodeC = createNode(1);
  Node* nodeD = createNode(2);
  Node* nodeE = createNode(2);
  Node* nodeF = createNode(3);
  Node* nodeG = createNode(2);
  Expression* expA1 = createExpression(x, ASSIGN, 14, 0, 0);
  Expression* expA2 = createExpression(a, EQUAL_TO, x, x, 0);
  Expression* expA3 = createExpression(0, IF_STATEMENT, a, (int)&nodeC, 0);
  Expression* expB1 = createExpression(x, ADDITION, x, x, 0);
  Expression* expB2 = createExpression(b, EQUAL_TO, x, x, 0);
  Expression* expB3 = createExpression(0, IF_STATEMENT, b, (int)&nodeE, 0);
  Expression* expC  = createExpression(x, MULTIPLICATION, x, x, 0);
  Expression* expD  = createExpression(x, ADDITION, x, x, 0);
  Expression* expE  = createExpression(x, ADDITION, x, x, 0);
  Expression* expG  = createExpression(x, MULTIPLICATION, x, x, 0);
  addListLast(nodeA->block, expA1);
  addListLast(nodeA->block, expA2);
  addListLast(nodeA->block, expA3);
  addListLast(nodeB->block, expB1);
  addListLast(nodeB->block, expB2);
  addListLast(nodeB->block, expB3);
  addListLast(nodeC->block, expC);
  addListLast(nodeD->block, expD);
  addListLast(nodeE->block, expE);
  addListLast(nodeG->block, expG);
  
  addChild(&nodeA, &nodeB);
  addChild(&nodeA, &nodeC);
  addChild(&nodeB, &nodeD);
  addChild(&nodeB, &nodeE);
  addChild(&nodeD, &nodeF);
  addChild(&nodeE, &nodeF);
  addChild(&nodeF, &nodeG);
  addChild(&nodeC, &nodeG);
  
  setLastBrhDom(&nodeA);
  setAllDirectDom(&nodeA);
  
  assignAllNodeSSA(nodeA, createLinkedList(), createLinkedList());
  ErrorObject* err;
  allocPhiFunc(&nodeA);

  TEST_ASSERT_EQUAL(3, nodeA->block->length);
  TEST_ASSERT_EQUAL(3, nodeB->block->length);
  TEST_ASSERT_EQUAL(1, nodeC->block->length);
  TEST_ASSERT_EQUAL(1, nodeD->block->length);
  TEST_ASSERT_EQUAL(1, nodeE->block->length);
  TEST_ASSERT_EQUAL(1, nodeF->block->length);
  TEST_ASSERT_EQUAL(2, nodeG->block->length);

  Expression* testPhi = createExpression(x, PHI_FUNC, x, x, b);
  setExpression(testPhi, 3, 5, 2);
  TEST_ASSERT_PHIFUNC(testPhi, &nodeF);

  testPhi = createExpression(x, PHI_FUNC, x, x, a);
  setExpression(testPhi, 4, 6, 3);
  TEST_ASSERT_PHIFUNC(testPhi, &nodeG);
}