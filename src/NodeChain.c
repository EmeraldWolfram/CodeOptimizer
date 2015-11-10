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


/*
 * brief @ Add a new child to a node as shown below.
 * Example:
 *              Before      [b]         After
 *           [a] rank 0    ---->       [a] rank 0
 *                                      |
 *                                     [b] rank 1
 *
 * brief @ The imdChild of the node(Node a in example) will assign to the added child(Node b in example).
 * brief @ The imdParent of the added child(Node b in example) will assign to the node(Node a in th example).
 *
 * param @ parentNode - The node that is going to add a new child(imdChild), it also will be the imdParent
 *                       of the child.
 * param @ childNode - The node that is going to be added to a node(imdParent), it also will be the imdChild
 *                       of the node.
 * retval@ void  - Nothing to return.
 */
void addChild(Node** parentNode, Node** childNode){
  int k = 0;
  if(*parentNode == NULL)
    ThrowError(ERR_NULL_NODE, "Input parent node is NULL!");
  
  if(*childNode == NULL)
    ThrowError(ERR_NULL_NODE, "Input child node is NULL!");
  
  /*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
  Hello,I reduced codes below by using realloc(reallocate), by Louise 

  int i = (*parentNode)->numberOfChild;  
  Node** childArr;
  childArr = malloc(sizeof(Node*) * i);
    
  for(k = 0; k < (i-1); k++){
    childArr[k] = (*parentNode)->imdChild[k];
  }
    
  free((*parentNode)->imdChild);
  (*parentNode)->imdChild = childArr;
  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
  */

  /***************************************************
  *  Link Child to parentNode                       *
  ***************************************************/
 
 (*parentNode)->numberOfChild++;
  
  // $$$$$$$$$$$$ expand array with one more item $$$$$$$$$$$$$$$
  //array U want to expand =(type cast)realloc (data within old array, new Size( sizeof(size u want to expand))) ;
  (*parentNode)->imdChild = (Node**)realloc((*parentNode)->imdChild, (sizeof(Node*) * ((*parentNode)->numberOfChild)));
  (*parentNode)->imdChild[((*parentNode)->numberOfChild) - 1] = *childNode;
    
  /*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
  int j = (*childNode)->numberOfParent;
  Node** parentArr;
  parentArr = malloc(sizeof(Node*) * j);

  for(k = 0; k < (j-1); k++){
    parentArr[k] = (*childNode)->imdParent[k];
  } 
    
  free((*childNode)->imdParent);
  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
  */
/***************************************************
 *  Link Parent to childNode                       *
 ***************************************************/
  (*childNode)->numberOfParent++;
  (*childNode)->imdParent = (Node**)realloc((*childNode)->imdParent, (sizeof(Node*) * ((*childNode)->numberOfParent)));
  (*childNode)->imdParent[((*childNode)->numberOfParent) - 1] = *parentNode;
  
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




