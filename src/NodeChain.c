#include "NodeChain.h"
#include <stdlib.h>
#include <stdio.h>

Node* createNode(int thisRank){
  Node* newNode = malloc(sizeof(Node));
  
  newNode->rank   = thisRank;
  newNode->block  = NULL;
  newNode->parent = NULL;
  newNode->lastBrhDom   = NULL;
  newNode->imdDom       = NULL;
  newNode->numOfParent  = 0;
  newNode->numOfDom     = 0;
  newNode->doms         = NULL;
  newNode->numOfChild   = 0;
  newNode->children     = NULL;
  
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
  if(*parentNode == NULL)
    ThrowError(ERR_NULL_NODE, "Input parent node is NULL!");
  
  if(*childNode == NULL)
    ThrowError(ERR_NULL_NODE, "Input child node is NULL!");

  /***************************************************
  *  Link Child to parentNode                       *
  ***************************************************/
  (*parentNode)->numOfChild++;
  (*parentNode)->children = (Node**)realloc((*parentNode)->children, (sizeof(Node*) * ((*parentNode)->numOfChild)));
  (*parentNode)->children[((*parentNode)->numOfChild) - 1] = *childNode;
/***************************************************
 *  Link Parent to childNode                       *
 ***************************************************/
  (*childNode)->parent = *parentNode;
  (*childNode)->numOfParent++;
  // (*childNode)->imdParent = (Node**)realloc((*childNode)->imdParent, (sizeof(Node*) * ((*childNode)->numberOfParent)));
  // (*childNode)->imdParent[((*childNode)->numberOfParent) - 1] = *parentNode;
  
}

void setLastBrhDom(Node** rootNode){
  if(*rootNode == NULL)
    ThrowError(ERR_NULL_NODE, "Empty Tree input detected!");
  
  LinkedList* tempList = createLinkedList();
  addListLast(tempList, *rootNode);
  ListElement* tempElement = tempList->head;
  Node *highRankNode, *testRankNode_1, *testRankNode_2;
  Node* tempNode = tempElement->node;
  int i, k;
/**************************************************
 *  Assemble the tree into a LinkedList           *
 **************************************************/
  while(tempElement != NULL){
    int j = tempNode->numOfChild;
    for(i = 0; i < j; i++){
      if(tempList->tail->node != tempNode->children[i] &&\
         tempNode->rank < tempNode->children[i]->rank)
        addListLast(tempList, tempNode->children[i]);
    }
    tempElement = tempElement->next;
    if(tempElement != NULL)
      tempNode = tempElement->node;
  }
/****************************************************
 *  With the LinkedList, find and assign lastBrhDom *
 *  of each node in the Node tree                   *
 ****************************************************/
  tempElement = tempList->head;
  while(tempElement != NULL){
    tempNode = tempElement->node;
    for(i = 0; i < tempNode->numOfChild; i++){
      //handle ROOT Node
      if(tempNode->parent == NULL){
        tempNode->lastBrhDom = NULL;
      }//handle child with 1 parent only (no assign to lastBrhDom yet)
      if(tempNode->children[i]->lastBrhDom == NULL){
        if(tempNode->numOfChild > 1 || tempNode->parent == NULL)
          tempNode->children[i]->lastBrhDom = tempNode;
        else if(tempNode->lastBrhDom != NULL)
          tempNode->children[i]->lastBrhDom = tempNode->lastBrhDom;
      }//handle child with more than 1 parent (lastBrhDom assigned by prev parents)
      else{
        highRankNode = tempNode->children[i]->lastBrhDom;
        if(tempNode->numOfChild > 1){
          if(tempNode->rank < highRankNode->rank){
            testRankNode_1 = tempNode;
            testRankNode_2 = highRankNode->lastBrhDom;
            FIND_SAME_NODE(testRankNode_1, testRankNode_2);
            tempNode->children[i]->lastBrhDom = testRankNode_1;
          }
        }
        else{
          testRankNode_1 = tempNode->lastBrhDom;
          testRankNode_2 = highRankNode;
          FIND_SAME_NODE(testRankNode_1, testRankNode_2);
          tempNode->children[i]->lastBrhDom = testRankNode_1;
        } 
      }
    }
    //**************************************************************************************************************
    //***********************IF numOfParent are used, the following code is enuf************************************
    /**************************************************************************************************************
      // tempNode->lastBrhDom = NULL;            //For ROOT, last Branch is NULL
    // else if((tempNode->parent->numOfChild > 1 && tempNode->numOfParent < 2) || tempNode->parent->parent == NULL)
      // tempNode->lastBrhDom = tempNode->parent;
    // else
      // tempNode->lastBrhDom = tempNode->parent->lastBrhDom;
    //*************************************************************************************************************/
    //*************************************************************************************************************
    //*************************************************************************************************************
    tempElement = tempElement->next;
  }
}

Node* getImdDom(Node* nodeA){
  Node *boudariesNode, *tempNode;
  boudariesNode = nodeA->lastBrhDom;
  int i;
  LinkedList* tempList = createLinkedList();
  addListLast(tempList, boudariesNode);
  ListElement* tempElement = tempList->head;
  tempNode = tempElement->node;
  /**************************************************
   *  Assemble the tree into a LinkedList           *
   **************************************************/
  while(tempElement != NULL){
    for(i = 0; i < tempNode->numOfChild; i++){
      if(tempList->tail->node != tempNode->children[i] &&\
         tempNode->rank < tempNode->children[i]->rank)
        addListLast(tempList, tempNode->children[i]);
    }
    tempElement = tempElement->next;
    if(tempElement != NULL)
      tempNode = tempElement->node;
  }
  // printf("%d", tempList->length);
  /*************************************************
   * Find the imdDominator and return it           *
   *************************************************/
  tempElement = tempList->head;
  while(tempElement != NULL){
    tempNode = tempElement->node;
        // printf("I entered");
    for(i = 0; i < tempNode->numOfChild; i++){
      if(tempNode->children[i] == nodeA){
        if(nodeA->imdDom == NULL)
          nodeA->imdDom = tempNode;
        else
          nodeA->imdDom = nodeA->lastBrhDom;
      }
    }
    tempElement = tempElement->next;
  }
  return nodeA->imdDom;
}

// struct Node_t{
  // int rank;
  // Block* block;
  // Node* lastBrhDom;
  // Node* imdDom;
  // int numOfParent;
  // Node* parent;
  // int numOfDom;
  // Node** doms;
  // int numOfChild;
  // Node** children;
  // LinkedList* domFrontiers;
// };

// LinkedList* getNodeDomFrontiers(Node* node)
// {
  
// }