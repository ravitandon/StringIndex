#include "header.h"

// Constructs a new node object
nodePtr getNode(void){
  nodePtr newNode = new(struct node);
  newNode->prefixLeftChild = NULL;
  newNode->prefixRightChild = NULL;
  newNode->prefixParentNode = NULL;
  newNode->suffixLeftChild = NULL;
  newNode->suffixRightChild = NULL;
  newNode->suffixParentNode = NULL;
  newNode->instanceCount=0;
  newNode->str = "";
  sem_init(&(newNode->lock), 0, 1);
  return newNode;
};

// Inserts a string into the tree and returns the node, in which the string has been inserted into.
nodePtr insertIntoTree (nodePtr root, string stringArgument, nodePtr suffixRoot){
    nodePtr stringNode = insertPrefixTreeString (root, NULL, stringArgument);
    assert (stringNode != NULL);
    updateSuffixParent (suffixRoot, stringNode);
    return stringNode;
}

// Inserts a string into the prefix tree.
nodePtr insertPrefixTreeString (nodePtr currentPrefixNode, nodePtr currentPrefixParent, string stringArgument){
	if (currentPrefixNode == NULL){		
		nodePtr newNode = getNode();
        assert (newNode != NULL);
		newNode->str = stringArgument;
		newNode->prefixParentNode = currentPrefixParent;    
        if (currentPrefixParent != NULL){    
            if (lexicographicalCompareString(stringArgument, currentPrefixParent->str)){
                currentPrefixParent->prefixLeftChild = newNode;
                newNode->prefixParentNode = currentPrefixParent;
            }
            else{
                assert (stringArgument.compare(currentPrefixParent->str) != 0);
                currentPrefixParent->prefixRightChild = newNode;
                newNode->prefixParentNode = currentPrefixParent;
            }
        }            
        newNode->instanceCount=1;        
		return newNode;
	}else if ((currentPrefixNode->str).compare(stringArgument) == 0){
		currentPrefixNode->instanceCount++;
        return currentPrefixNode;
	}else{
        if (lexicographicalCompareString(stringArgument, currentPrefixNode->str)) 
            return (insertPrefixTreeString(currentPrefixNode->prefixLeftChild, currentPrefixNode, stringArgument));
        else
            return (insertPrefixTreeString(currentPrefixNode->prefixRightChild, currentPrefixNode, stringArgument));
    }
}

// Updates the suffix parent of the node in which the string has been inserted.
void updateSuffixParent (nodePtr currentSuffixNode, nodePtr stringNode){
    assert(stringNode != NULL);
    string stringArgument = stringNode->str;
    if (currentSuffixNode == NULL){        
        currentSuffixNode = stringNode; // Case when current suffix node is the un-initialized root.
        stringNode->suffixParentNode = NULL;        
        return;
    }else if ((currentSuffixNode->str).compare(stringArgument) == 0){        
        return;
    }else{
        string currentNodeReverseString = reverseString(currentSuffixNode->str);
        string stringArgumentReverseString = reverseString(stringArgument);
        if (lexicographicalCompareString(stringArgumentReverseString, currentNodeReverseString)) {
            if (currentSuffixNode->suffixLeftChild == NULL){
                currentSuffixNode->suffixLeftChild = stringNode;
                stringNode->suffixParentNode = currentSuffixNode;
                return;
            }else{
                return (updateSuffixParent(currentSuffixNode->suffixLeftChild, stringNode));
            }
        }else if (currentSuffixNode->suffixRightChild == NULL){
            currentSuffixNode->suffixRightChild = stringNode;
            stringNode->suffixParentNode = currentSuffixNode;
            return;
        }else
            return (updateSuffixParent(currentSuffixNode->suffixRightChild, stringNode));
    }
}

// This method returns the pointer to the node in the prefix tree which contains the string Argument or the 
// string which is lexicographically next to the given string.
nodePtr findStringPrefix (nodePtr currentNode, string stringArgument){
    assert(currentNode != NULL);
    if ((currentNode->str).compare(stringArgument) == 0){
        return currentNode;
    }else{        
        if (lexicographicalCompareString(stringArgument, currentNode->str)){
            if (currentNode->prefixLeftChild){
                return (findStringPrefix(currentNode->prefixLeftChild, stringArgument));
            }else{
                return currentNode;
            }    
        }else{
            if (currentNode->prefixRightChild){
                return (findStringPrefix(currentNode->prefixRightChild, stringArgument));
            }else{
                return currentNode->prefixParentNode;
            }
        }
    }
}

// This method returns the pointer to the node in the suffix tree which contains the string Argument or the 
// string which is lexicographically next to the given string.
nodePtr findStringSuffix (nodePtr currentNode, string stringArgument){
    assert(currentNode != NULL);    
    if ((currentNode->str).compare(stringArgument) == 0){
        return currentNode;
    }else{        
        string currentNodeReverseString = reverseString(currentNode->str);
        string stringArgumentReverseString = reverseString(stringArgument);        
        if (lexicographicalCompareString(stringArgumentReverseString, currentNodeReverseString)){

            if (currentNode->suffixLeftChild){
                return (findStringSuffix(currentNode->suffixLeftChild, stringArgument));
            }else{
                return currentNode;
            }    
        }else{
            if (currentNode->suffixRightChild){
                return (findStringSuffix(currentNode->suffixRightChild, stringArgument));
            }else{
                return currentNode->suffixParentNode;
            }
        }
    }
}


// Method which reads the tree in lexicographical order into a container of strings
vector <nodePtr>  readStringsPrefixLexicographicOrder (vector <nodePtr> stringContainer, 
                            nodePtr currentNode, string stringArgument){
    
    if (currentNode == NULL)
        return stringContainer;


    if (lexicographicalCompareString(stringArgument, currentNode->str)){
        if (isPrefix(stringArgument, currentNode->str)){
            stringContainer.push_back(currentNode);
            stringContainer = readStringsPrefixLexicographicOrder(stringContainer, currentNode->prefixRightChild, stringArgument);
            stringContainer = readStringsPrefixLexicographicOrder(stringContainer, currentNode->prefixParentNode, stringArgument);
        }
    }else if (stringArgument.compare(currentNode->str) == 0){
            stringContainer.push_back(currentNode);           
    }
    return stringContainer;    
}


// Method which reads the tree in lexicographical order into a container of strings
vector <nodePtr> readStringsSuffixLexicographicOrder (vector <nodePtr> stringContainer, 
                            nodePtr currentNode, string stringArgument){
    
    if (currentNode == NULL)
        return stringContainer;

    if (lexicographicalCompareString(reverseString(stringArgument), reverseString(currentNode->str))){        
        if (isSuffix(stringArgument, currentNode->str)){
            stringContainer.push_back(currentNode);
            stringContainer = readStringsSuffixLexicographicOrder(stringContainer, currentNode->suffixRightChild, stringArgument);
            stringContainer = readStringsSuffixLexicographicOrder(stringContainer, currentNode->suffixParentNode, stringArgument);
        }
    }else if (stringArgument.compare(currentNode->str) == 0){
            stringContainer.push_back(currentNode);            
    }
    return stringContainer;    
}

// This method gets the strings from the prefix tree.
std::vector<nodePtr> getPrefixStrings (nodePtr rootNode, string stringArgument){
    vector <nodePtr> stringContainer;
    nodePtr currentNode = findStringPrefix (rootNode, stringArgument);
    if (currentNode != NULL){
         stringContainer = readStringsPrefixLexicographicOrder (stringContainer, currentNode, stringArgument);
     }
    return stringContainer;
};

// This method gets the strings from the prefix tree.
std::vector<nodePtr> getSuffixStrings (nodePtr rootNode, string stringArgument){
    vector <nodePtr> stringContainer;
    nodePtr currentNode = findStringSuffix (rootNode, stringArgument);    
    if (currentNode != NULL){
        stringContainer = readStringsSuffixLexicographicOrder (stringContainer, currentNode, stringArgument);        
    }
    return stringContainer;
};

// Inserts a node in the prefix tree.
void insertNodePrefixOrder (nodePtr currentNode, nodePtr nodeToBeInserted){
    assert (currentNode != NULL);
    if (lexicographicalCompareString(nodeToBeInserted->str, currentNode->str)){
        if (currentNode->prefixLeftChild){
            insertNodePrefixOrder(currentNode->prefixLeftChild, nodeToBeInserted);
        }else{
            currentNode->prefixLeftChild = nodeToBeInserted;
            nodeToBeInserted->prefixParentNode = currentNode;
        }
    }else{
        if ((nodeToBeInserted->str).compare(currentNode->str) == 0){
            assert(false);
        }else{
            if (currentNode->prefixRightChild){
                insertNodePrefixOrder(currentNode->prefixRightChild, nodeToBeInserted);
            }else{
                currentNode->prefixRightChild = nodeToBeInserted;
                nodeToBeInserted->prefixParentNode = currentNode;
            }   
        }
    }
}

// Inserts a node in the suffix tree in the correct location.
void insertNodeSuffixOrder (nodePtr currentNode, nodePtr nodeToBeInserted){
    assert (currentNode != NULL);
    if (lexicographicalCompareString(reverseString(nodeToBeInserted->str), reverseString(currentNode->str))){
        if (currentNode->suffixLeftChild){
            insertNodeSuffixOrder(currentNode->suffixLeftChild, nodeToBeInserted);
        }else{
            currentNode->suffixLeftChild = nodeToBeInserted;
            nodeToBeInserted->prefixParentNode = currentNode;
        }
    }else{
        if ((nodeToBeInserted->str).compare(currentNode->str) == 0){
            assert(false);
        }else{
            if (currentNode->suffixRightChild){
                insertNodeSuffixOrder(currentNode->suffixRightChild, nodeToBeInserted);
            }else{
                currentNode->suffixRightChild = nodeToBeInserted;
                nodeToBeInserted->prefixParentNode = currentNode;
            }   
        }
    }
}


// deletion should update the suffix tree also.
void deleteNodePrefixTree (nodePtr currentNode, nodePtr rootNode){
    if (currentNode == NULL)
        return;
     nodePtr parentNode = currentNode->prefixParentNode;
     if (parentNode != NULL){          
        if (lexicographicalCompareString(currentNode->str, parentNode->str)){
           if (currentNode->prefixLeftChild){            
                parentNode->prefixLeftChild = currentNode->prefixLeftChild;
                (currentNode->prefixLeftChild)->prefixParentNode = currentNode->prefixParentNode;
               // debug(parentNode);
                if (currentNode->prefixRightChild != NULL) 
                    insertNodePrefixOrder (currentNode->prefixLeftChild, currentNode->prefixRightChild);
            }else{
                    parentNode->prefixLeftChild = NULL;
            }
        }else{
            if (currentNode->prefixLeftChild){
                parentNode->prefixRightChild = currentNode->prefixLeftChild;
                (currentNode->prefixRightChild)->prefixParentNode = currentNode->prefixParentNode;
                if (currentNode->prefixRightChild)
                    insertNodePrefixOrder (currentNode->prefixLeftChild, currentNode->prefixRightChild);
            }else{
                    parentNode->prefixRightChild = NULL;
            }
        }
     }else{
        assert(currentNode == rootNode);
        nodePtr tempRootNode;
        if (currentNode->prefixLeftChild){
            tempRootNode = currentNode->prefixLeftChild;
            if (currentNode->prefixRightChild)
                insertNodePrefixOrder (tempRootNode, currentNode->prefixRightChild);
            rootNode = tempRootNode;
        }
     }
     //free (currentNode); the node will be freed - After deletin from the suffix tree
}


// deletion should update the suffix tree also.
void deleteNodeSuffixTree (nodePtr currentNode, nodePtr rootNode){
    if (currentNode == NULL)
        return;
     nodePtr parentNode = currentNode->suffixParentNode;
    // debug(currentNode);
    /// debug(parentNode);
     if (parentNode != NULL){
        if (lexicographicalCompareString(reverseString(currentNode->str), reverseString(parentNode->str))){
           if (currentNode->suffixLeftChild){
                parentNode->suffixLeftChild = currentNode->suffixLeftChild;
                (currentNode->suffixLeftChild)->prefixParentNode = currentNode->suffixParentNode;
                if (currentNode->suffixRightChild)
                    insertNodeSuffixOrder (currentNode->suffixLeftChild, currentNode->suffixRightChild);
            }else{
                    parentNode->suffixLeftChild = currentNode->suffixRightChild;
            }
        }else{
            if (currentNode->suffixLeftChild){
                parentNode->suffixRightChild = currentNode->suffixLeftChild;
                (currentNode->suffixLeftChild)->suffixParentNode = currentNode->suffixParentNode;
                if (currentNode->suffixRightChild)
                    insertNodeSuffixOrder (currentNode->suffixLeftChild, currentNode->suffixRightChild);
            }else{
                    parentNode->suffixRightChild = currentNode->suffixRightChild;
            }
        }
     }else{
        assert(currentNode == rootNode);
        nodePtr tempRootNode;
        if (currentNode->suffixLeftChild){
            tempRootNode = currentNode->suffixLeftChild;
            if (currentNode->suffixRightChild)
                insertNodeSuffixOrder (tempRootNode, currentNode->suffixRightChild);
            rootNode = tempRootNode;
        }
     }
}

void printInOrderPrefix (nodePtr node){
  if (node != NULL){
    printInOrderPrefix(node->prefixLeftChild);    
    cout << node->str << endl;    
    printInOrderPrefix(node->prefixRightChild);
  }
}

void printInOrderSuffix (nodePtr node){
  if (node != NULL){
    printInOrderSuffix(node->suffixLeftChild);
    cout << node->str << endl;
    printInOrderSuffix(node->suffixRightChild);
  }
}

void debug (nodePtr node){
    if (node != NULL){
        cout << "Debug----" << node->str << endl; 
    }
}


