#ifndef NodeChain_H
#define NodeChain_H

typedef struct Node_t Node;
struct Node_t{
  char* expression;
  int rank;
  Node* parent;
  Node* next[0];
};

typedef struct {
  Node* hook;
  Node* tail;
  int length;
}NodeChain;

Node* createNode(char* thisExpression, int thisRank);
NodeChain* createNodeChain();

#endif // NodeChain_H
