#ifndef PhiFunction_H
#define PhiFunction_H

#include "LinkedList.h"
#include "ErrorObject.h"
#include "ExpressionBlock.h"
#include "NodeChain.h"

void allocPhiFunc(Node* entryNode);
Expression* getPhiFunction(LinkedList* listA, LinkedList* listB, Subscript* subs);
Subscript getCondition(Node* imdDomNode);

#endif // PhiFunction_H
