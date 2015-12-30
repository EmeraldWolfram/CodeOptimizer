#include "unity.h"
#include "ExpressionBlock.h"
#include "GetList.h"
#include "LinkedList.h"
#include "ErrorObject.h"
#include "customAssertion.h"
#include "NodeChain.h"
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
  TEST_ASSERT_EQUAL(0, addExpress->condt.name);
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
 *  arrangeSSA
 *
 *  arrangeSSA should throw ERR_NULL_NODE when NULL input node detected
 *
 ************************************************************************/
void test_arrangeSSA_should_throw_ERR_NULL_NODE(void){
  ErrorObject *err;
  Try{
    arrangeSSA(NULL);
    TEST_FAIL_MESSAGE("Expected ERR_NULL_NODE but no error thrown");
  } Catch (err){
    TEST_ASSERT_EQUAL(ERR_NULL_NODE, err->errorCode);
    TEST_ASSERT_EQUAL_STRING("NULL input detected in arrangeSSA", err->errorMsg);
  }
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
 *    x0 = x0 * x0          x2 = x1 * x1;
 *
 *************************************************************************/
void test_assignAllNodeSSA_with_only_x(void){
  Expression* exp1 = createExpression(x, ASSIGN, 3, 0, 0);
  Expression* exp2 = createExpression(x, ADDITION, x, x, 0);
  Expression* exp3 = createExpression(x, MULTIPLICATION, x, x, 0);
  
  Node* nodeA = createNode(0);
  Node* nodeB = createNode(1);
  addChild(&nodeA, &nodeB);
  
  addListLast(nodeA->block, exp1);
  addListLast(nodeA->block, exp2);
  addListLast(nodeB->block, exp3);

  setLastBrhDom(&nodeA);
  
  assignAllNodeSSA(nodeA, createLinkedList(), createLinkedList());
  ListElement* testExp = nodeA->block->head;
  
  TEST_ASSERT_SUBSCRIPT(x, 0, &((Expression*)testExp->node)->id);
  TEST_ASSERT_SUBSCRIPT(x, 1, &((Expression*)testExp->next->node)->id);
  TEST_ASSERT_SUBSCRIPT(x, 0, &((Expression*)testExp->next->node)->oprdA);
  TEST_ASSERT_SUBSCRIPT(x, 0, &((Expression*)testExp->next->node)->oprdB);
  
  testExp = nodeB->block->head;
  TEST_ASSERT_SUBSCRIPT(x, 2, &((Expression*)testExp->node)->id);
  TEST_ASSERT_SUBSCRIPT(x, 1, &((Expression*)testExp->node)->oprdA);
  TEST_ASSERT_SUBSCRIPT(x, 1, &((Expression*)testExp->node)->oprdB);
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
 *    z0 = y0 * x0                      z1 = y0 * x1;
 *
 *************************************************************************/
void test_assignAllNodeSSA_with_muliple_variable(void){
  Expression* exp1 = createExpression(x, ASSIGN, 3, 0, 0);
  Expression* exp2 = createExpression(y, ASSIGN, 5, 0, 0);
  Expression* exp3 = createExpression(z, ASSIGN, 7, 0, 0);
  Expression* exp4 = createExpression(x, ADDITION, y, x, 0);
  Expression* exp5 = createExpression(z, MULTIPLICATION, y, x, 0);
  
  Node* nodeA = createNode(0);
  Node* nodeB = createNode(1);
  addChild(&nodeA, &nodeB);
  
  addListLast(nodeA->block, exp1);
  addListLast(nodeA->block, exp2);
  addListLast(nodeA->block, exp3);
  addListLast(nodeA->block, exp4);
  addListLast(nodeB->block, exp5);

  setLastBrhDom(&nodeA);

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
  TEST_ASSERT_SUBSCRIPT(y, 0, &((Expression*)testExp->node)->oprdA);
  TEST_ASSERT_SUBSCRIPT(x, 1, &((Expression*)testExp->node)->oprdB);
}


/**
 *  assignAllNodeSSA
 *
 *  BEFORE                                    AFTER
 *
 *          NodeA:                            NodeA:
 *          x = 4                             x0 = 4
 *          c = (x != x)                      c0 = (x0 != x0)
 *          if(c0) goto NodeC                 if(c0) goto NodeC
 *          /         \                      /                  \
 *    NodeB:          NodeC:            NodeB:                  NodeC:
 *    x = x * x       x = x + x         x1 = x0 * x0            x4 = x0 + x0
 *           \        /                        \             /     
 *            NodeD:                              NodeD:
 *            x = x + x                           x3 = x2 + x2
 *                                              (reserved x2 for phiFunction)

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
  Expression* exp4 = createExpression(x, MULTIPLICATION, x, x, 0);
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
  TEST_ASSERT_SUBSCRIPT(x, 0, &((Expression*)testExp->node)->oprdB);
  
  testExp = nodeD->block->head;
  TEST_ASSERT_SUBSCRIPT(x, 3, &((Expression*)testExp->node)->id);
  TEST_ASSERT_SUBSCRIPT(x, 2, &((Expression*)testExp->node)->oprdA);
  TEST_ASSERT_SUBSCRIPT(x, 2, &((Expression*)testExp->node)->oprdB);
  
  testExp = nodeC->block->head;
  TEST_ASSERT_SUBSCRIPT(x, 4, &((Expression*)testExp->node)->id);
  TEST_ASSERT_SUBSCRIPT(x, 0, &((Expression*)testExp->node)->oprdA);
  TEST_ASSERT_SUBSCRIPT(x, 0, &((Expression*)testExp->node)->oprdB);
}

/**
 *  assignAllNodeSSA
 *
 *    NodeA:
 *    x0 = 3
 *    x0 = x0 + x0
 *    goto NodeB
 *
 *    NodeB:
 *    x0 = y0 * x0
 *
 *  As y0 is not declared anywhere, the function should throw ERR_UNDECLARE_VARIABLE
 *
 *************************************************************************/
void test_assignAllNodeSSA_should_Throw_ERR_UNDECLARE_VARIABLE(void){
  ErrorObject *err;
  Try{
    Expression* exp1 = createExpression(x, ASSIGN, 3, 0, 0);
    Expression* exp2 = createExpression(x, ADDITION, x, x, 0);
    Expression* exp3 = createExpression(x, MULTIPLICATION, y, x, 0);
    Node* nodeA = createNode(0);
    Node* nodeB = createNode(1);
    addChild(&nodeA, &nodeB);
    addListLast(nodeA->block, exp1);
    addListLast(nodeA->block, exp2);
    addListLast(nodeB->block, exp3);
    setLastBrhDom(&nodeA);
  
    assignAllNodeSSA(nodeA, createLinkedList(), createLinkedList());
    TEST_FAIL_MESSAGE("Expected ERR_UNDECLARE_VARIABLE but not error thrown")
  } Catch(err){
    TEST_ASSERT_EQUAL(ERR_UNDECLARE_VARIABLE, err->errorCode);
    TEST_ASSERT_EQUAL_STRING("Subscript y not define yet!", err->errorMsg);
  }
}

/**
 *  assignAllNodeSSA
 *
 *    NodeA:
 *    x0 = 3
 *    x1 = x0 + x0
 *    goto NodeB
 *
 *    NodeB:
 *    y0 = x1 * x1
 *
 *  If LHS is undeclare, nothing should happen
 *
 *************************************************************************/
void test_assignAllNodeSSA_should_not_change_thing_in_NodeB(void){
  Expression* exp1 = createExpression(x, ASSIGN, 3, 0, 0);
  Expression* exp2 = createExpression(x, ADDITION, x, x, 0);
  Expression* exp3 = createExpression(y, MULTIPLICATION, x, x, 0);
  Node* nodeA = createNode(0);
  Node* nodeB = createNode(1);
  addChild(&nodeA, &nodeB);
  addListLast(nodeA->block, exp1);
  addListLast(nodeA->block, exp2);
  addListLast(nodeB->block, exp3);
  setLastBrhDom(&nodeA); 
  assignAllNodeSSA(nodeA, createLinkedList(), createLinkedList());
  
  LinkedList* testList = nodeB->block;
  ListElement* testPtr = testList->head;
  TEST_ASSERT_SUBSCRIPT(y, 0, &((Expression*)testPtr->node)->id);
  TEST_ASSERT_SUBSCRIPT(x, 1, &((Expression*)testPtr->node)->oprdA);
  TEST_ASSERT_SUBSCRIPT(x, 1, &((Expression*)testPtr->node)->oprdB);
}
