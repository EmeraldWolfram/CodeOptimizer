#ifndef NodeChain_H
#define NodeChain_H

#include "ErrorObject.h"

typedef struct {
  char* string;
  int data;
}Block;

typedef struct Node_t Node;
struct Node_t{
  int rank;
  Block* block;
  Node* trueParent;
  int numberOfParent;
  Node** imdParent;
  int numberOfChild;
  Node** imdChild;
};

Node* createNode(int thisRank);
Block* createBlock(char* string, int data);
void addChild(Node** parentNode, Node** childNode);
// void findParent(Node** rootNode);


Node* findUnion(Node* nodeA, Node* nodeB);


#endif // NodeChain_H
