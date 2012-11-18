#ifndef __HEADER_H__
#define __HEADER_H__

#include <arpa/inet.h>
#include <math.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <cctype>
#include <assert.h>

using namespace std;

#define null NULL

struct node {
  string str ;
  struct node* prefixParentNode; 
  struct node* prefixLeftChild; 
  struct node* prefixRightChild;
  struct node* suffixParentNode;
  struct node* suffixLeftChild;
  struct node* suffixRightChild;
  int instanceCount; 
  sem_t lock;
};
typedef struct node* nodePtr;

// Constructs a new node.
nodePtr getNode(void);
// Inserts a string into the prefix tree.
nodePtr insertPrefixTreeString (nodePtr , nodePtr , string);
// Updates the suffix parent of the node in which the string has been inserted.
void updateSuffixParent (nodePtr, nodePtr); 
// Lexicograhic comparison of two strings
bool lexicographicalCompareString(string str1, string str2);
// Reverses a given string.
string reverseString (string stringArgument);
// Inserts a string into the tree and returns the node, in which the string has been inserted into.
nodePtr insertIntoTree (nodePtr root, string stringArgument, nodePtr);
// This method returns the pointer in the prefix tree to the node which contains the string Argument or the 
// string which is lexicographically next to the given string.
nodePtr findStringPrefix (nodePtr, string);
// This method returns the pointer in the suffix tree to the node which contains the string Argument or the 
// string which is lexicographically next to the given string.
nodePtr findStringSuffix (nodePtr, string);
// This method gets the strings from the prefix tree.
std::vector<nodePtr> getPrefixStrings (nodePtr, string);
// This method gets the strings from the prefix tree.
std::vector<nodePtr> getSuffixStrings (nodePtr, string);
// Checks if string s1 is a prefix of string s2.
bool isPrefix (string s1, string s2);
// Checks if string s1 is a suffix of string s2.
bool isSuffix (string s1, string s2);
// Method which reads the tree in lexicographical order from the prefix tree into a container of strings
vector <nodePtr>  readStringsPrefixLexicographicOrder (vector <nodePtr> stringContainer, 
                            nodePtr currentNode, string stringArgument);
// Method which reads the tree in lexicographical order from the suffix tree into a container of strings
vector <nodePtr>  readStringsSuffixLexicographicOrder (vector <nodePtr> stringContainer, 
                            nodePtr currentNode, string stringArgument);
// Inserts a node in the prefix tree.
void insertNodePrefixOrder (nodePtr currentNode, nodePtr nodeToBeInserted);
// Inserts a node in the suffix tree in the correct location.
void insertNodeSuffixOrder (nodePtr currentNode, nodePtr nodeToBeInserted);
// Deletes a node from the prefix tree.
void deleteNodePrefixTree (nodePtr currentNode, nodePtr);
// Deletes a node from the suffix tree.
void deleteNodeSuffixTree (nodePtr currentNode, nodePtr);
// Function which checks whether a read lock is acquired on any prefix of a given string or not. 
// This is a check for ensuring that during a read operation any string that has a prefix s does not get deleted.
// Here s is the string on which a prefix search is being performed. 
bool isPrefixNotLocked (string s1, map<string, int> lockMap);
bool isSuffixNotLocked (string s1, map<string, int> lockMap);
void printInOrderPrefix (nodePtr node);
void printInOrderSuffix (nodePtr node);
void test1(void);
void test2(void);
void test3(void);
void *test4(void *args);
void debug (nodePtr node);

#endif


