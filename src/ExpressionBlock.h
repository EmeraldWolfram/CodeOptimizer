#ifndef ExpressionBlock_H
#define ExpressionBlock_H

#include "LinkedList.h"
#include "ErrorObject.h"
typedef struct Expression_t Expression;
#include "ExpressionBlock.h"
#include "NodeChain.h"

#define CHANGE_ID_SUBSCRIPT(exprPtr, checkPtr, currentRank)                   \
                while(&(((Expression*)exprPtr->node)->id) != checkPtr->node){ \
                                                                              \
                  if(((Expression*)exprPtr->node)->id.name ==                 \
                      ((Subscript*)checkPtr->node)->name &&                   \
                    currentRank <= ((Subscript*)checkPtr->node)->subs){       \
                    currentRank = ((Subscript*)checkPtr->node)->subs + 1;     \
                  }                                                           \
                  checkPtr = checkPtr->next;                                  \
                }                                                             \
                ((Expression*)exprPtr->node)->id.subs = currentRank;

                
#define CHANGE_OPERAND_A(exprPtr, checkPtr, oprARank)                           \
                while(&((Expression*)exprPtr->node)->oprdA != checkPtr->node){  \
                  if(((Expression*)exprPtr->node)->oprdA.name ==                \
                    ((Subscript*)checkPtr->node)->name &&                       \
                    oprARank < ((Subscript*)checkPtr->node)->subs){             \
                    oprARank = ((Subscript*)checkPtr->node)->subs;              \
                  }                                                             \
                  checkPtr = checkPtr->next;                                    \
                }                                                               \
                ((Expression*)exprPtr->node)->oprdA.subs = oprARank;

                
#define CHANGE_OPERAND_B(exprPtr, checkPtr, oprBRank)                           \
                while(&((Expression*)exprPtr->node)->oprdB != checkPtr->node){  \
                  if(((Expression*)exprPtr->node)->oprdB.name ==                \
                    ((Subscript*)checkPtr->node)->name &&                       \
                    oprBRank < ((Subscript*)checkPtr->node)->subs){             \
                    oprBRank = ((Subscript*)checkPtr->node)->subs;              \
                  }                                                             \
                  checkPtr = checkPtr->next;                                    \
                }                                                               \
                ((Expression*)exprPtr->node)->oprdB.subs = oprBRank;
                
                
                
typedef enum{
  PHI_FUNC,
  GREATER_THAN,
  SMALLER_THAN,
  EQUAL_TO,
  NOT_EQUAL_TO,
  ADDITION,
  SUBTRACTION,
  MULITPLICATION,
  DIVISION,
  ASSIGN,
  COPY,
  IF_STATEMENT
}Operator;

typedef struct{
  int name;
  int subs;
}Subscript;

struct Expression_t{
  Subscript id;
  Operator opr;
  Subscript oprdA;
  Subscript oprdB;
  int condition;
};


Expression* createExpression(int thisID, Operator oprt,        \
                            int oprdA, int oprdB, int condt);
                            
LinkedList* getSubsList(LinkedList* expression);
Subscript* getLargestIndex(LinkedList* exprList, Subscript* subsName);

void arrangeSSA(Node* inputNode);
LinkedList* getLiveList(Node* inputNode, LinkedList* prevLiveList);

#endif // ExpressionBlock_H
