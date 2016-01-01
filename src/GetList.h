#ifndef GetList_H
#define GetList_H

#include "LinkedList.h"
#include "ErrorObject.h"
#include "ExpressionBlock.h"
#include "NodeChain.h"
#include "ErrorObject.h"




Subscript* getLargestIndex(LinkedList* subsList, Subscript* subsName);
LinkedList* getSubsList(LinkedList* expression);

LinkedList* getModifiedList(Node* inputNode);
LinkedList* getLiveList(Node** inputNode);
LinkedList* getAllLiveList(Node** inputNode, LinkedList* prevLiveList);


LinkedList* getLatestList(Node* inputNode, LinkedList* prevList);
LinkedList* getPathToNode(Node** rootNode, Node* stopNode);
LinkedList* getListTillNode(Node* stopNode);
void updateList(Node* inputNode, LinkedList* prevList);


#endif // GetList_H
