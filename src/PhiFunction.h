#ifndef PhiFunction_H
#define PhiFunction_H

#include "LinkedList.h"
#include "ErrorObject.h"
#include "ExpressionBlock.h"
#include "NodeChain.h"

void allocPhiFunc(Node* entryNode);
Expression* getPhiFunction();
Subscript* getCondition(Node* imdDomNode);



#endif // PhiFunction_H
