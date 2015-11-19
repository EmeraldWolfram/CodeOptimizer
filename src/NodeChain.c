#include "NodeChain.h"
#include <stdlib.h>

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
  addListLast(tempList, createListElement(*rootNode));
  ListElement* tempElement = tempList->head;
  Node *highRankNode, *testRankNode_1, *testRankNode_2;
  Node* tempNode = tempElement->node;
  int i, k;
/**************************************************
 *  Assemble the tree into a LinkedList           *
 **************************************************/
  while(tempNode->numOfChild != 0 && tempElement != NULL){
    int j = tempNode->numOfChild;
    for(i = 0; i < j; i++){
      if(tempList->tail->node != tempNode->children[i])
        addListLast(tempList, createListElement(tempNode->children[i]));
    }
    tempElement = tempElement->next;
    tempNode = tempElement->node;
  }
/****************************************************
 *  With the LinkedList, find and assign lastBrhDom *
 *  of each node in the Node tree                   *
 ****************************************************/
  tempElement = tempList->head;
  while(tempElement != NULL){
    tempNode = tempElement->node;
    k = tempNode->numOfChild;
    for(i = 0; i < k; i++){
      //This is to handle ROOT Node
      if(tempNode->parent == NULL){
        tempNode->lastBrhDom = NULL;
      }//Following use to handle if the child only have 1 parent (no assign to lastBrhDom yet)
      if(tempNode->children[i]->lastBrhDom == NULL){
        if(tempNode->numOfChild > 1 || tempNode->parent == NULL)
          tempNode->children[i]->lastBrhDom = tempNode;
        else if(tempNode->lastBrhDom != NULL)
          tempNode->children[i]->lastBrhDom = tempNode->lastBrhDom;
      }//Following use to handle if the child have more than 1 parent (lastBrhDom assigned during prev parent)
      else{
        highRankNode = tempNode->children[i]->lastBrhDom;
        if(tempNode->numOfChild > 1){
          if(tempNode->rank < highRankNode->rank){
            tempNode->children[i]->lastBrhDom = tempNode;
          }
        }
        else if(tempNode->lastBrhDom != NULL){
          if(tempNode->lastBrhDom->rank < highRankNode->rank){
            if(highRankNode->lastBrhDom == tempNode->lastBrhDom)
              tempNode->children[i]->lastBrhDom = tempNode->lastBrhDom;
            else{
              //LOOP until 2 lastBrhDom is the same
              tempNode->children[i]->lastBrhDom = tempNode->lastBrhDom;
            }
          }
          else if(tempNode->lastBrhDom->rank == highRankNode->rank){
            testRankNode_1 = tempNode->lastBrhDom;
            testRankNode_2 = highRankNode;
            while(testRankNode_1 != testRankNode_2){
              testRankNode_1 = testRankNode_1->lastBrhDom;
              testRankNode_2 = testRankNode_2->lastBrhDom;
            }
            tempNode->children[i]->lastBrhDom = testRankNode_1;
          }
        } 
      }
    }
    //*********************************************************
    
      // tempNode->lastBrhDom = NULL;            //For ROOT, last Branch is NULL
    // else if((tempNode->parent->numOfChild > 1 && tempNode->numOfParent < 2) || tempNode->parent->parent == NULL)
      // tempNode->lastBrhDom = tempNode->parent;
    // else
      // tempNode->lastBrhDom = tempNode->parent->lastBrhDom;

    tempElement = tempElement->next;
  }
}

// Node* findUnion(Node* nodeA, Node* nodeB){
  // return nodeA;
// }




