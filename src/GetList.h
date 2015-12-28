#ifndef GetList_H
#define GetList_H

#include "LinkedList.h"
#include "ErrorObject.h"
#include "ExpressionBlock.h"
#include "NodeChain.h"
#include "ErrorObject.h"

Subscript* getLargestIndex(LinkedList* exprList, Subscript* subsName);

LinkedList* getSubsList(LinkedList* expression);
LinkedList* getModifiedList(Node* inputNode);

void updateList(Node* inputNode, LinkedList* prevList);


#endif // GetList_H
