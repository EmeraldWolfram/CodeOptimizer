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

#define TEST_ASSERT_EQUAL_NODE(expectedExpression, expectedRank, expectedParent, actualNode);                   \
          customTestAssertNode(expectedExpression, expectedRank, expectedParent, actualNode, __LINE__);
//TEST_ASSERT_EQUAL_NODE used to test the value inside the node, does not necessarily to be the same node

#define TEST_ASSERT_EQUAL_CHAIN(expectedChain, actualChain);                   \
          customTestAssertChain(expectedChain, actualChain, __LINE__);
//TEST_ASSERT_EQUAL_CHAIN used to test the addresses of the chain and the addresses stored in the chain

void customTestAssertNode(char* expectedExpression, int expectedRank, Node* expectedParent, Node* actualNode, int lineNumber);
void customTestAssertChain(Node* expectedChain, Node* actualChain, int lineNumber);

#endif // customAssertion_H
