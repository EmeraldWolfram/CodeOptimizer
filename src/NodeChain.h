#ifndef NodeChain_H
#define NodeChain_H

typedef struct Node_t Node;
#include "LinkedList.h"
#include "ErrorObject.h"
#include "ExpressionBlock.h"

struct Node_t{
  int rank;
  int visitFlag;
  LinkedList* block;
  Node* lastBrhDom;
  Node* imdDom;
  Node* parent;
  int numOfDom;
  Node** doms;
  int numOfChild;
  Node** children;
  LinkedList* domFrontiers;
};

#define FIND_SAME_NODE(testRankNode_1, testRankNode_2)      \
          while(testRankNode_1 != testRankNode_2){          \
            if(testRankNode_1->rank < testRankNode_2->rank) \
              testRankNode_2 = testRankNode_2->lastBrhDom;  \
            else                                            \
              testRankNode_1 = testRankNode_1->lastBrhDom;  \
          }

Node* createNode(int thisRank);

void addChild(Node** parentNode, Node** childNode);
void setLastBrhDom(Node** rootNode);

void getImdDom(Node* nodeA);
void setAllImdDom(Node** rootNode);

LinkedList* getNodeDomFrontiers(Node* node);
LinkedList* getAllDomFrontiers(Node** root);

LinkedList* assembleList(Node **rootNode);

#endif // NodeChain_H
