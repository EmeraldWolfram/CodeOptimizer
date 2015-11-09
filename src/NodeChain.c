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
    Node** childArr;
    childArr = malloc(sizeof(Node*) * i);
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
    Node** parentArr;
    parentArr = malloc(sizeof(Node*) * j);

    for(k = 0; k < (j-1); k++){
      parentArr[k] = (*childNode)->imdParent[k];
    } 
    free((*childNode)->imdParent);
    (*childNode)->imdParent = parentArr;
    (*childNode)->imdParent[j-1] = *parentNode;
  }
}

void setParent(Node** rootNode){
  LinkedList* tempList = createLinkedList();
  addListLast(tempList, createListElement(*rootNode));
  ListElement* tempElement = tempList->head;
  Node* tempNode = tempElement->node;
  int i, k;
/**************************************************
 *  Assemble the tree into a LinkedList           *
 **************************************************/
  while(tempNode->numberOfChild != 0 && tempElement != NULL){
    int j = tempNode->numberOfChild;
    for(i = 0; i < j; i++){
      addListLast(tempList, createListElement(tempNode->imdChild[i]));
    }
    tempElement = tempElement->next;
    tempNode = tempElement->node;
  }
/****************************************************
 *  With the LinkedList, find and assign trueParent *
 *  of each node in the Node tree                   *
 ****************************************************/
  tempElement = tempList->head->next;
  while(tempElement != NULL){
    tempNode = tempElement->node;
    if(tempNode->numberOfParent == 1){
      if(tempNode->imdParent[0]->numberOfChild   > 1 || \
         tempNode->imdParent[0]->numberOfParent == 0){
        tempNode->trueParent = tempNode->imdParent[0];
      }
      else
        tempNode->trueParent = tempNode->imdParent[0]->trueParent;
    }
    else{
      i = tempNode->numberOfParent;
      Node* highRankParent = tempNode->imdParent[0]->trueParent;
      for(k = 0; k < i; k++){
        if((tempNode->imdParent[k]->trueParent->rank) < (highRankParent->rank))
          highRankParent = tempNode->imdParent[k]->trueParent;
      }
      tempNode->trueParent = highRankParent;
    }
    tempElement = tempElement->next;
  }   
}

Node* findUnion(Node* nodeA, Node* nodeB){
  return nodeA;
}




