#include "NodeChain.h"
#include <stdlib.h>

Node* createNode(char* thisExpression, int thisRank){
  Node* newNode = malloc(sizeof(Node) + sizeof(Node*));
  
  newNode->expression = thisExpression;
  newNode->rank       = thisRank;
  newNode->parent     = NULL;
  newNode->next[0]    = NULL;
  
  return newNode;
}


NodeChain* createNodeChain(){
  NodeChain* newChain = malloc(sizeof(NodeChain));
  
  newChain->hook    = NULL;
  newChain->tail    = NULL;
  newChain->length  = 0;
  
  return newChain;
}