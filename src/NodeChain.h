#ifndef NodeChain_H
#define NodeChain_H

#include "LinkedList.h"
#include "ErrorObject.h"
typedef struct Node_t Node;

/**
 * expression[0]  = id
 * expression[1]  = resulting value
 * expression[2]  = operator
 * expression[3]  = operand 1
 * expression[4]  = operand 2
 *************************************/
typedef enum{
  GREATER_THAN,
  SMALLER_THAN,
  EQUAL_TO,
  NOT_EQUAL_TO,
  NORMAL_OPERATOR,
  ASSIGN,
  IF_STATEMENT
}Operator;
 
typedef struct {
  LinkedList* expression;
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

#define FIND_SAME_NODE(testRankNode_1, testRankNode_2)      \
          while(testRankNode_1 != testRankNode_2){          \
            if(testRankNode_1->rank < testRankNode_2->rank) \
              testRankNode_2 = testRankNode_2->lastBrhDom;  \
            else                                            \
              testRankNode_1 = testRankNode_1->lastBrhDom;  \
          }

Node* createNode(int thisRank);
Block* createBlock(LinkedList* expList);
void addChild(Node** parentNode, Node** childNode);
void setLastBrhDom(Node** rootNode);

Node* getImdDom(Node* nodeA);

LinkedList* getNodeDomFrontiers(Node* node);
LinkedList* getAllDomFrontiers(Node** root);

#endif // NodeChain_H
