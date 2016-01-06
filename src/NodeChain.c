#include "NodeChain.h"
#include <stdlib.h>
#include <stdio.h>

Node* createNode(int thisRank){
  Node* newNode = malloc(sizeof(Node));

  newNode->rank         = thisRank;
  newNode->visitFlag    = 0;
  newNode->block        = createLinkedList();
  newNode->parent       = NULL;
  newNode->lastBrhDom   = NULL;
  newNode->imdDom       = NULL;
  newNode->numOfChild   = 0;
  newNode->children     = NULL;
  newNode->domFrontiers = NULL;
  newNode->directDom    = createLinkedList();

  return newNode;
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
  (*parentNode)->children = (Node**)realloc((*parentNode)->children, \
                            (sizeof(Node*) * ((*parentNode)->numOfChild)));

  (*parentNode)->children[((*parentNode)->numOfChild) - 1] = *childNode;

  /***************************************************
  *  Link Parent to childNode                       *
  ***************************************************/
  (*childNode)->parent = *parentNode;

}

void setLastBrhDom(Node** rootNode){
  if(*rootNode == NULL)
    ThrowError(ERR_NULL_NODE, "Empty Tree input detected!");

  LinkedList* tempList      = assembleList(rootNode);
  ListElement* tempElement  = tempList->head;
  Node* tempNode            = tempElement->node;
  int i, k;
  Node *highRankNode, *testRankNode_1, *testRankNode_2;

/****************************************************
 *  With the LinkedList, find and assign lastBrhDom *
 *  of each node in the Node tree                   *
 ****************************************************/
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
        testRankNode_1 = tempNode->lastBrhDom;
        testRankNode_2 = tempNode->children[i]->lastBrhDom;
        FIND_SAME_NODE(testRankNode_1, testRankNode_2);
        tempNode->children[i]->lastBrhDom = testRankNode_1;
      }
    }
    tempElement = tempElement->next;
  }
}

void getImdDom(Node* nodeA){
  if(nodeA->rank == 0)
    nodeA->imdDom = NULL;
  else{
    int i;
    Node *boudariesNode       = nodeA->lastBrhDom;
    LinkedList* tempList      = assembleList(&boudariesNode);
    ListElement* tempElement  = tempList->head;
    Node *tempNode;
    /*************************************************
    * Find the imdDominator and return it           *
    *************************************************/
    while(tempElement != NULL){
      tempNode = tempElement->node;
      for(i = 0; i < tempNode->numOfChild; i++){
        if(tempNode->children[i] == nodeA){
          if(nodeA->imdDom == NULL)
            nodeA->imdDom = tempNode;
          else{
            if(tempNode != nodeA)
              nodeA->imdDom = nodeA->lastBrhDom;
          }
        }
      }
      tempElement = tempElement->next;
    }
  }
}

void setAllImdDom(Node** rootNode){
  LinkedList* tempList      = assembleList(rootNode);
  ListElement* tempElement  = tempList->head;
  Node *tempNode;
  /*************************************************
  * Find the imdDominator and return it           *
  *************************************************/
  while(tempElement != NULL){
    tempNode = tempElement->node;
    getImdDom(tempNode);
    tempElement = tempElement->next;
  }
}


/*
 * brief @ To get the dominatorFrontiers of a node.
 * Example:
 *
 *          | Entry                 DF(A) = { A }
 *         \/                       DF(B) = { D }
 *        [ A ]<<<<                 DF(C) = { D }
 *       |   |    /\                DF(D) = { A }
 *      \/  \/    /\
 *     [B]  [C]   /\
 *      |    |    /\
 *     \/   \/    /\
 *       [D] >>>>>>>
 *
 *
 * brief @ There is three condition to be a dominatorFrontier of the node:
 *          1) the node(or the children of the node) must be an immediate predeccessor of domFrontier.
 *          2) (if any) to get domFrontiers from grandchild(child of child), the child must be dominated by the node
 *          3)domFrontier is not strictly dominated by the node
 *
 * param @ Node* node  - The node that is going to use this function to find the domFrontiers of it.
 *
 * retval@ LinkedList* - The list of domFrontiers of the 'Node* node' is going to return.
 */
LinkedList* getNodeDomFrontiers(Node* node){

  LinkedList* domFrontiers = createLinkedList();
  LinkedList* checklist = createLinkedList();
  ListElement* tempCLElement = NULL;
  ListElement* tempHeadCL = NULL;
  Node *tempImdDom = NULL;
  int i = 0;

  if(!node->numOfChild)
    return domFrontiers;

  addListLast(checklist, node);
  tempHeadCL = checklist->head;

  while(tempHeadCL){

    for( i = 0; i < ((Node*)tempHeadCL->node)->numOfChild; i++){

      tempImdDom = ((Node*)tempHeadCL->node)->children[i]->imdDom;

      //checking the imdDom of the child isn't the parentNode; imdDom of the parentNode is NULL
      if(tempImdDom){
        while(tempImdDom->rank > node->rank){
          tempImdDom = tempImdDom->imdDom;
        }
      }

      //the child is not strictly dominated by the node
      if(tempImdDom != node || !tempImdDom){
        addListLast(domFrontiers, ((Node*)tempHeadCL->node)->children[i]);
        continue; //any grandchild need to be dominated by node, otherwise the child of grandchild cannot be DF of node
      }

      tempCLElement = checklist->head;

      //checking is the children already put in the checklist
      while(tempCLElement){

        if( ((Node*)tempHeadCL->node)->children[i] == tempCLElement->node )
          break;

        tempCLElement = tempCLElement->next;
      }

     if(!tempCLElement)
        addListLast(checklist, ((Node*)tempHeadCL->node)->children[i]);
    }

    tempHeadCL = tempHeadCL->next;

  }

  return domFrontiers;
}

/**
 * brief @ To get all the dominatorFrontiers of a tree by using the getNodeDomFrontiers() function.
 * Example:
 *        ControlFlowGraph1         Union of DomFrontiers
 *
 *                | Entry                 DF(A) = { A }
 *               \/                       DF(B) = { D }
 *              [ A ]<<<<                 DF(C) = { D }
 *             |   |    /\                DF(D) = { A }
 *            \/  \/    /\                UDF = { A , D}
 *           [B]  [C]   /\
 *            |    |    /\
 *           \/   \/    /\
 *             [D] >>>>>>>
 *
 * param @ Node* node  - The tree that is going to use this function to find all the domFrontiers of it.
 *
 * retval@ LinkedList* - The union of domFrontiers of the input argument, 'Node** root' is going to return.
 **/
LinkedList* getAllDomFrontiers(Node** root){

  LinkedList* domFrontiers = createLinkedList();
  LinkedList* checklist = assembleList(root);
  ListElement* tempCheckList = NULL;
  ListElement* tempDFList = NULL;
  ListElement* tempCLElement = NULL;
  ListElement* tempHeadDF = NULL;
  int i;

  tempCheckList = checklist->head;

  //get domFs of each node and add in the list, compare between DFlist and nodeDFlist, if already inside DFlist, just skip
  while(tempCheckList){
    ((Node*)tempCheckList->node)->domFrontiers = getNodeDomFrontiers(tempCheckList->node);
    tempHeadDF = ((Node*)tempCheckList->node)->domFrontiers->head;

    while(tempHeadDF){

      tempDFList = domFrontiers->head;
      while(tempDFList){

          if(tempDFList->node == tempHeadDF->node )
            break;

          tempDFList = tempDFList->next;
        }

      if(!tempDFList)
        addListLast(domFrontiers, tempHeadDF->node);

      tempHeadDF = tempHeadDF->next;
    }

    tempCheckList = tempCheckList->next;
  }

  return domFrontiers;

}

//ASSEMBLE ALL THE NODE THAT HAVE A RANK EQUAL OR LOWER THAN THE ROOT INTO A LINKEDLIST
LinkedList* assembleList(Node **rootNode){
    LinkedList* tempList      = createLinkedList();
    addListLast(tempList, *rootNode);
    int i;
    ListElement* tempElement  = tempList->head;
    Node *tempNode            = tempElement->node;

    while(tempElement != NULL){
      for(i = 0; i < tempNode->numOfChild; i++){
        if(tempList->tail->node != tempNode->children[i] \
        && tempNode->rank < tempNode->children[i]->rank)
          addListLast(tempList, tempNode->children[i]);
      }
      tempElement = tempElement->next;

      if(tempElement != NULL)
        tempNode = tempElement->node;
    }
    return tempList;
}

//This function assign directDominator to the node
void setAllDirectDom(Node** rootNode){
  (*rootNode)->visitFlag |= 8;
  Node *childPtr;
  int i;

  for(i = 0; i < (*rootNode)->numOfChild; i++){
    childPtr = (*rootNode)->children[i];
    addListLast(childPtr->directDom, (*rootNode));
    if((childPtr->visitFlag & 8) == 0)
      setAllDirectDom(&childPtr);
  }
}

/**
 *  This function should make sure none of the node in the node
 *  tree have more than 2 parents. If case Tree A happens,
 *  splitNode shall create NodeG and form Tree B.
 *

 *
 *
/**
 * brief @ To split the node that had more than 2 parents.
 * Example:
 *               TREE A:           TREE B:
 *                 (A)              (A)
 *                /   \            /   \
 *              (B)   (C)   =>   (B)   (C)
 *              / \    |    =>   / \     \
 *            (D) (E)  |    => (D)  (E)  |
 *              \  |  /         \    |   |
 *  split Node>> (F)            (F)<-+----    * C is pointing to F while E is pointing to (newNode)
 *                                 \ |
 *                                  (newNode) << Expression of F passed to newNode
 *
 * brief @ The expression block in NodeF will be moved to newNode
 * brief @ The expression block in NodeF become empty then
 *
 * param @ Node* node  - The tree that is going to use this function to find all the domFrontiers of it.
 *
 * retval@ LinkedList* - The union of domFrontiers of the input argument, 'Node** root' is going to return.
 **/
void splitNode(Node** rootNode){
  LinkedList*   splitList = createLinkedList();
  LinkedList*   nodeList  = assembleList(rootNode);
  ListElement*  checkListHead = NULL;
  ListElement*  tempNode   = NULL;
  int i, count, rank, positionOfSplitNode;

  /* build a list for the children those have more than 1 parent */
  ListElement*  tempHead  = nodeList->head;
  while(tempHead){
    for(i = 0; i < ((Node*)tempHead->node)->numOfChild; i++){
      if(((Node*) tempHead->node)->children[i]->parent != tempHead->node && \
          ((Node*) tempHead->node)->children[i]->imdDom != tempHead->node)
        addListLast(splitList, ((Node*) tempHead->node)->children[i]);
    }
    tempHead = tempHead->next;
  }

  tempHead = splitList->head;

  /* check each element inside the split list,
     if one of the elements appear twice, that means it had more than 2 parents.
  */
  while(tempHead){
    count = 0;
    checkListHead = tempHead;
    tempNode = tempHead->node; // store the current pointing node

    while(checkListHead){
      if(checkListHead->node == tempNode)
        count++;

      //more than 2 parent for (checkListHead->node), so need to separate the parents out
      if(count == 2)
        break;

      checkListHead = checkListHead->next;
    }

    if(count == 2)
      break;

    tempHead = tempHead->next;
  }

  if(!tempHead)
    return;

  /* compare the rank of both of the nodes that is going to be the parents of newNode */
  if(((Node*)checkListHead->node)->rank > ((Node*)checkListHead->node)->parent->rank)
    rank = ((Node*)checkListHead->node)->parent->rank + 1;
  else
    rank = ((Node*)checkListHead->node)->rank + 1;

  Node* newNode = createNode(rank);

  positionOfSplitNode = 0;

  /*  find the place of the Node(that is more than 2 parent & going to be splitted) in its parent
      its parent may own more than 1 child
   */
  while(positionOfSplitNode < ((Node*)checkListHead->node)->parent->numOfChild && \
        ((Node*)checkListHead->node)->parent->children[positionOfSplitNode] != ((Node*)checkListHead->node))
    positionOfSplitNode++;

  if(!((Node*)checkListHead->node)->parent->children[positionOfSplitNode])
    ThrowError(ERR_UNHANDLE_ERROR, "There was a unhandled exception error");

  /*  in case the splitNode(had >2 parent at the first) had any children
      link the newNode with the children of splitNode
  */  
  for(i = 0; i < ((Node*)checkListHead->node)->numOfChild; i++)
    addChild(&newNode, &((Node*)checkListHead->node)->children[i]);

  /*  break the children of the splitNode */
  if(((Node*)checkListHead->node)->numOfChild){
    ((Node*)checkListHead->node)->numOfChild   = 0;
    ((Node*)checkListHead->node)->children     = NULL;
  }

  /* link the splitNode to the newNode */
  addChild(((Node**)&checkListHead->node), &newNode);
  
  /*  pass the expression block of splitNode to newNode  */
  newNode->block = ((Node*)checkListHead->node)->block;
  ((Node*)checkListHead->node)->block = NULL;
  
  /* add the newNode created in the children list of the splitNode(had >2 parent at the first) and its parent */
  ((Node*)checkListHead->node)->parent->children[positionOfSplitNode] = newNode;

  /* re-assign parent */
  nodeList  = assembleList(rootNode);
  tempHead  = nodeList->head;
  while(tempHead){
    for(i = 0; i < ((Node*)tempHead->node)->numOfChild; i++){
      if(((Node*)tempHead->node)->children[i]->parent != tempHead->node)
        ((Node*)tempHead->node)->children[i]->parent = tempHead->node;
    }
    tempHead = tempHead->next;
  }
  
  //check is any node's parent >2
  nodeList  = assembleList(rootNode);
  tempHead  = nodeList->head;
  while(tempHead){
    for(i = 0; i < ((Node*)tempHead->node)->numOfChild; i++){
      if(((Node*) tempHead->node)->children[i]->parent != tempHead->node && \
          ((Node*) tempHead->node)->children[i]->imdDom != tempHead->node)
        addListLast(splitList, ((Node*) tempHead->node)->children[i]);
    }
    tempHead = tempHead->next;
  }

  tempHead = splitList->head;
  while(tempHead){
    count = 0;
    checkListHead = tempHead;
    tempNode = tempHead->node; // store the current pointing node

    while(checkListHead){
      if(checkListHead->node == tempNode)
        count++;

      //more than 2 parent for (checkListHead->node), so need to separate the parents out
      if(count == 2)
        break;

      checkListHead = checkListHead->next;
    }

    if(count == 2)
      break;
    
    tempHead = tempHead->next;
  }
  
  if(!tempHead)
    return;
  
  //recursive
  splitNode(rootNode);

}