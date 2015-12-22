#include "unity.h"
#include "ExpressionBlock.h"
#include "LinkedList.h"
#include "ErrorObject.h"
#include "customAssertion.h"
#include "NodeChain.h"
#include <stdlib.h>

void setUp(void){}

void tearDown(void){}

/**
 *  createExpression('x',ADDITION, 'y', 'z', 0)
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
  Expression* addExpress = createExpression('x',ADDITION, 'y', 'z', 0);
  
  TEST_ASSERT_SUBSCRIPT('x', 0, &addExpress->id);
  TEST_ASSERT_EQUAL(ADDITION, addExpress->opr);
  TEST_ASSERT_SUBSCRIPT('y', 0, &addExpress->oprdA);
  TEST_ASSERT_SUBSCRIPT('z', 0, &addExpress->oprdB);
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
  Expression* exp1 = createExpression('x', ADDITION, 'y', 'z', 0);
  Expression* exp2 = createExpression('y', ADDITION, 'z', 'x', 0);
  Expression* exp3 = createExpression('z', ADDITION, 'x', 'y', 0);
  
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
  Expression* exp1 = createExpression('x', ADDITION, 'x', 'x', 0);
  
  Node* testNode = createNode(0);
  addListLast(testNode->block, exp1);

  arrangeSSA(testNode);
  
  ListElement* elemPtr = testNode->block->head;
  TEST_ASSERT_SUBSCRIPT('x', 1, &((Expression*)elemPtr->node)->id);
  TEST_ASSERT_SUBSCRIPT('x', 0, &((Expression*)elemPtr->node)->oprdA);
  TEST_ASSERT_SUBSCRIPT('x', 0, &((Expression*)elemPtr->node)->oprdA);
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
  Expression* exp1 = createExpression('x', ADDITION, 'y', 'z', 0);
  Expression* exp2 = createExpression('x', ADDITION, 'x', 'a', 0);
  Expression* exp3 = createExpression('z', ADDITION, 'z', 'x', 0);
  Expression* exp4 = createExpression('z', ADDITION, 'z', 'x', 0);

  
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
void test_getCondition_with_the_Block0_above_should_return_expr1(void){
  Node* nodeA = createNode(0);
  Node* nodeB = createNode(1);
  Expression* expr1 = createExpression('c', EQUAL_TO, 'x', 'y', 0);
  Expression* expr2 = createExpression(0, IF_STATEMENT, 'c', ((int)&nodeB), 0);
  addListLast(nodeA->block, expr1);
  addListLast(nodeA->block, expr2);
  
  Subscript* testExpr = getCondition(nodeA);
  
  TEST_ASSERT_EQUAL_PTR(&(expr2->oprdA), testExpr);
}

/**
 *  
 *
 *  Block1:
 *  x0 = 12;
 *  y0 = 1;
 *  z0 = 2
 *  x1 = 3 + 7;
 *  c0 = (12 > 3)
 *
 *
 *****************************************************************************/
void test_getCondition_with_the_Block1_above_should_return_expr5(void){
  Node* nodeA = createNode(0);
  Expression* expr1 = createExpression('x', ASSIGN, 12, 0, 0);
  Expression* expr2 = createExpression('y', ASSIGN,  1, 0, 0);
  Expression* expr3 = createExpression('z', ASSIGN,  2, 0, 0);
  Expression* expr4 = createExpression('x', ADDITION, 'x', 'y', 0);
  Expression* expr5 = createExpression('c', GREATER_THAN, 'z', 'y', 0);
  Expression* expr6 = createExpression(0, IF_STATEMENT,  'c', (int)&nodeA, 0);
  addListLast(nodeA->block, expr1);
  addListLast(nodeA->block, expr2);
  addListLast(nodeA->block, expr3);
  addListLast(nodeA->block, expr4);
  addListLast(nodeA->block, expr5);
  addListLast(nodeA->block, expr6);
  
  Subscript* testExpr = getCondition(nodeA);
  
  TEST_ASSERT_EQUAL_PTR(&expr6->oprdA, testExpr);
}







