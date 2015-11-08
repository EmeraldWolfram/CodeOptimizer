#include "NodeChain.h"
#include <stdlib.h>

Node* createNode(int thisRank){
  Node* newNode = malloc(sizeof(Node));
  
  newNode->rank   = thisRank;
  newNode->block  = NULL;
  newNode->trueParent     = NULL;
  newNode->numberOfParent = 0;
  newNode->imdParent      = NULL;
  newNode->numberOfChild  = 0;
  newNode->imdChild       = NULL;

  return newNode;
}

Block* createBlock(char* thisString, int thisData){
  Block* newBlock = malloc(sizeof(Block));
  newBlock->string  = thisString;
  newBlock->data    = thisData;
  
  return newBlock;
}

void addChild(Node** parentNode, Node** childNode){
  int k = 0;
  if(*parentNode == NULL)
    ThrowError(ERR_NULL_NODE, "Input parent node is NULL!");
  else if(*childNode == NULL)
    ThrowError(ERR_NULL_NODE, "Input child node is NULL!");
  else{
/***************************************************
 *  Link Child to parentNode                       *
 ***************************************************/
    (*parentNode)->numberOfChild++;
    int i = (*parentNode)->numberOfChild;
    Node* childArr[i];
    for(k = 0; k < (i-1); k++){
      childArr[k] = (*parentNode)->imdChild[k];
    }
    free((*parentNode)->imdChild);
    (*parentNode)->imdChild = childArr;
    (*parentNode)->imdChild[i-1] = *childNode;
    
/***************************************************
 *  Link Parent to childNode                       *
 ***************************************************/
    (*childNode)->numberOfParent++;
    int j = (*childNode)->numberOfParent;
    Node* parentArr[j];
    for(k = 0; k < (j-1); k++){
      parentArr[k] = (*childNode)->imdParent[k];
    }
    free((*childNode)->imdParent);
    (*childNode)->imdParent = parentArr;
    (*childNode)->imdParent[j-1] = *parentNode;
  }
}

// Node* findUnion(Node* nodeA, Node* nodeB);
// NodeChain* createNodeChain(){
  // NodeChain* newChain = malloc(sizeof(NodeChain));
  
  // newChain->hook    = NULL;
  // newChain->tail    = NULL;
  // newChain->length  = 0;
  
  // return newChain;
// }

Node* findUnion(Node* nodeA, Node* nodeB){
  if(nodeA == NULL | nodeB == NULL)
    ThrowError(ERR_NULL_NODE, "Input node cannot be NULL!");
  else if(nodeA->imdParent == NULL | nodeB->imdParent == NULL)
    ThrowError(ERR_NO_PARENT, "Input node does not have a parent!");
  else{
    Node* parentA = (*nodeA->imdParent);
    // Node* parentB = nodeB->parent;
    // while(parentA != parentB){
      // if(parentA->rank > parentB->rank)
        // parentA = parentA->parent;
      // else
        // parentB = parentB->parent;
    return parentA;
  }
}




