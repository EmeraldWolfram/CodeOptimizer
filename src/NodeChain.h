#ifndef NodeChain_H
#define NodeChain_H

#include "LinkedList.h"
#include "ErrorObject.h"
typedef struct Node_t Node;

typedef struct {
  char* string;
  int data;
}Block;

struct Node_t{
  int rank;
  Block* block;
  Node* lastBrhDom;
  Node* imdDom;
  int numOfParent;
  Node* parent;
  int numOfDom;
  Node** doms;
  int numOfChild;
  Node** children;
};

#define FIND_SAME_NODE(testRankNode_1, testRankNode_2)  while(testRankNode_1 != testRankNode_2){          \
                                                          if(testRankNode_1->rank < testRankNode_2->rank) \
                                                            testRankNode_2 = testRankNode_2->lastBrhDom;  \
                                                          else                                            \
                                                            testRankNode_1 = testRankNode_1->lastBrhDom;  \
                                                        }

Node* createNode(int thisRank);
Block* createBlock(char* string, int data);
void addChild(Node** parentNode, Node** childNode);
void setLastBrhDom(Node** rootNode);

Node* getImdDom(Node* nodeA);

LinkedList* getAllDomFrontiers(Node** root);
LinkedList* getNodeDomFrontiers(Node* node);

#endif // NodeChain_H
