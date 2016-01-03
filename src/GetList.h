#ifndef GetList_H
#define GetList_H

#include "LinkedList.h"
#include "ErrorObject.h"
#include "ExpressionBlock.h"
#include "NodeChain.h"
#include "ErrorObject.h"

//********** Function that check only the input Node ******************
LinkedList* getModifiedList(Node* inputNode);
LinkedList* getLiveList(Node** inputNode);
LinkedList* getLatestList(Node* inputNode, LinkedList* prevList);
LinkedList* getPathToNode(Node** rootNode, Node* stopNode);
Subscript* getLargestIndex(LinkedList* subsList, Subscript* subsName);
LinkedList* getSubsList(LinkedList* expression);
void updateList(Node* inputNode, LinkedList* prevList);

//************ Function that will traverse the input Node **************
LinkedList* getAllLiveList(Node** inputNode, LinkedList* prevLiveList);
LinkedList* getListTillNode(Node* stopNode);



#endif // GetList_H
