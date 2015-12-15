#ifndef ExpressionBlock_H
#define ExpressionBlock_H

#include "LinkedList.h"
#include "ErrorObject.h"
typedef struct Expression_t Expression;
#include "ExpressionBlock.h"
#include "NodeChain.h"

typedef enum{
  PHI_FUNC,
  GREATER_THAN,
  SMALLER_THAN,
  EQUAL_TO,
  NOT_EQUAL_TO,
  NORMAL_OPERATOR,
  ASSIGN,
  IF_STATEMENT
}Operator;

struct Expression_t{
  char id;
  int subscrpt;
  Operator opr;
  int operand1;
  int operand2;
  int condition;
};
Expression* getCondition(Node* imdDomNode);

Expression* createExpression(char thisID, int subs, Operator oprt,  \
                             int oprd1, int oprd2, int condt);

#endif // ExpressionBlock_H
