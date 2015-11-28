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
  int numOfParent; //X
  Node* parent;
  int numOfDom; //X
  Node** doms; //LINKEDLIST
  int numOfChild; //X
  Node** children; //LINKEDLIST
  LinkedList* domFrontiers;
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

LinkedList* getNodeDomFrontiers(Node* node);
LinkedList* getAllDomFrontiers(Node** root);

#endif // NodeChain_H
