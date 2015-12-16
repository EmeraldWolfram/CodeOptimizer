#ifndef customAssertion_H
#define customAssertion_H

#include "unity.h"
#include "NodeChain.h"

// ... can represent any arguement.
#define CUSTOM_TEST_FAIL(lineNo, msg, ...)                                                        \
                {                                                                                 \
                char buffer[256];                                                                 \
                sprintf(buffer, msg, ## __VA_ARGS__);                                             \
                UNITY_TEST_FAIL(lineNo,buffer);                                                   \
                }

#define TEST_ASSERT_PHIFUNC(expectedPhiFunction, actualNodeToTest);           \
          customTestAssertPhiFunction(expectedPhiFunction, actualNodeToTest, __LINE__);

#define TEST_ASSERT_NODE_ADDRESS(expectedNode, actualNode);                   \
          customTestAssertNodeAddress(expectedNode, actualNode, __LINE__);
          
#define TEST_ASSERT_LINKED_LIST(expectedList, actualList);                    \
          customTestAssertLinkedList(expectedList, actualList, __LINE__);
          
#define TEST_ASSERT_SUBSCRIPT(expectName, expectSub, actualSubscript);        \
          customTestAssertSubscript(expectName, expectSub, actualSubscript, __LINE__);
//TEST_ASSERT_EQUAL_CHAIN used to test the addresses of the chain and the addresses stored in the chain

void customTestAssertPhiFunction(Expression* expectedPhiFunction, Node** nodeToTest, int lineNumber);
void customTestAssertNodeAddress(Node* expectedNode, Node* actualNode, int lineNumber);
void customTestAssertLinkedList(LinkedList* expectedList, LinkedList* actualList, int lineNumber);
void customTestAssertSubscript(int expectName, int expectSub, Subscript* actual, int lineNumber);

#endif // customAssertion_H
