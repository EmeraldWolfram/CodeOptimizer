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

Node* findUnion(Node* nodeA, Node* nodeB){
  if(nodeA == NULL | nodeB == NULL)
    ThrowError(ERR_NULL_NODE, "Input node cannot be NULL!");
  else if(nodeA->parent == NULL | nodeB->parent == NULL)
    ThrowError(ERR_NO_PARENT, "Input node does not have a parent!");
  else{
    Node* parentA = nodeA->parent;
    Node* parentB = nodeB->parent;
    while(parentA != parentB){
      if(parentA->rank > parentB->rank)
        parentA = parentA->parent;
      else
        parentB = parentB->parent;
    }
    return parentA;
  }
}




