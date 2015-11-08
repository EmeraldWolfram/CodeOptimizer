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

// #define TEST_ASSERT_NODE_DATA(expectedExpression, expectedRank, expectedParent, actualNode);                   \
          // customTestAssertNodeData(expectedExpression, expectedRank, expectedParent, actualNode, __LINE__);
//TEST_ASSERT_EQUAL_NODE used to test the value inside the node, does not necessarily to be the same node

#define TEST_ASSERT_NODE_ADDRESS(expectedNode, actualNode);                   \
          customTestAssertNodeAddress(expectedNode, actualNode, __LINE__);
//TEST_ASSERT_EQUAL_CHAIN used to test the addresses of the chain and the addresses stored in the chain

// void customTestAssertNodeData(char* expectedExpression, int expectedRank, Node* expectedParent, Node* actualNode, int lineNumber);
void customTestAssertNodeAddress(Node* expectedNode, Node* actualNode, int lineNumber);

#endif // customAssertion_H
