  #include "header.h"

nodePtr rootPrefixNode; // pointer to the root of the prefix tree
nodePtr rootSuffixNode; // pointer to the root of the suffix tree
map <string,int> prefixLockMap; //lock map for the prefix tree 
map <string,int> suffixLockMap; // lock map for the suffix tree
sem_t lock_prefixMap; // semaphore for accessing the critical section of the prefix lock map tree
sem_t lock_suffixMap; // semaphore for accessing the critical section of the suuffix lock map tree

// The result class
class Result {
    public:
     int size;
     int stringsRemoved;
     vector <nodePtr> stringContainer;

    
      Result(){
        size=0;
        stringsRemoved=0;
    }

     int Size(void){
       return size;
    };

// Function that removes the strings from string Index in the collection.
     int remove(void){
       nodePtr tempNode;
       vector<nodePtr>::iterator it;
       for (it=stringContainer.begin() ; it < stringContainer.end(); it++ ){
         tempNode = *it;          
         if (tempNode != NULL){
          // Checking whether we can delete this node 
          while (!(isPrefixNotLocked (tempNode->str, prefixLockMap)));
          while (!(isSuffixNotLocked (reverseString(tempNode->str), suffixLockMap)));
           sem_wait(&(tempNode->lock)); // Checking whether no other thread is deleting or writing on the map.                       
                 //debug(tempNode);
                 stringsRemoved = stringsRemoved + tempNode->instanceCount;                 
                 tempNode->instanceCount=0;
                 if (tempNode->instanceCount>0){
                   deleteNodePrefixTree (tempNode, rootPrefixNode);
                   deleteNodeSuffixTree (tempNode, rootSuffixNode);                                          
                 }
            sem_post(&(tempNode->lock));           
         }          
       } 
       return stringsRemoved;
    };

    // Helper methods
     void incrementSize(void){
       size++;
    };

     void incrementStringsRemoved(void){
       stringsRemoved++;
    };

    vector <nodePtr> getStringContainer(void){
        return (stringContainer);
    };
   
   void pushString (nodePtr node){
    stringContainer.push_back(node);
   };

   void setStringContainer(vector<nodePtr> s){
      stringContainer = s;
   }
 // Fnction that sets the size of the numebr of strings searched.
   void setSize(void){
      nodePtr tempNode;
      vector<nodePtr>::iterator it;
      for (it=stringContainer.begin() ; it < stringContainer.end(); it++ ){
        tempNode = *it; 
        if (tempNode != NULL) // The tempNode might have been deleted in the mean time. 
          size = size + tempNode->instanceCount;
      }        
   }



};

class StringIndex{
  //private:
    //nodePtr rootNode; // The root node of the tree.

  public:
    StringIndex(){
      rootPrefixNode = NULL; // Initializing the rootNode of the tree
      rootSuffixNode = NULL;
      sem_init (&(lock_prefixMap), 0, 1);
      sem_init (&(lock_suffixMap), 0, 1);
    };

   int insert(string s){
     if (rootPrefixNode == NULL){
      assert (rootSuffixNode == NULL);
      nodePtr node = insertIntoTree (rootPrefixNode, s, rootSuffixNode); 
      assert (node != NULL);
      rootSuffixNode = node;
      rootPrefixNode = node; 
      return node->instanceCount-1;     
     }else{
        nodePtr node = insertIntoTree (rootPrefixNode, s, rootSuffixNode); 
        return node->instanceCount-1;
     }

};

// Finds the set of strings from the StringIndex and stores them in a result set
   Result stringsWithPrefix(string s){
     Result result;

     // Inserting lock on the prefix search string. 
     sem_wait(&(lock_prefixMap));    
     std::map<string, int> :: iterator mapIterator;
     if (prefixLockMap.count(s) == 0){
      prefixLockMap.insert ( pair<string,int>(s,1));      
      //cout << "After Insertion, Iterator=1:pid=" << getpid()  << endl;
     }else{
        mapIterator = prefixLockMap.find(s);
        mapIterator->second=mapIterator->second+1;              
        
     }     
     
     fflush(stdout);
     sem_post(&(lock_prefixMap));
     
     fflush(stdout);

     result.setStringContainer(getPrefixStrings(rootPrefixNode, s));     
     result.setSize();
  
     // Release Lock on Prefix Search For String s
     sem_wait(&(lock_prefixMap));
     mapIterator=prefixLockMap.find(s);
     if (mapIterator == prefixLockMap.end()){
        ;// assert(false);
     }else{
      //cout <<"Deletion" << endl;
        mapIterator->second=mapIterator->second-1;
        if (mapIterator->second == 0){
          prefixLockMap.erase(s);
        }
     }
     //cout << "After Deletion, Iterator=" << mapIterator->second << endl;
     fflush(stdout);
     sem_post(&(lock_prefixMap));
  
     return result;
  };

   Result stringsWithSuffix(string s){
     Result result;

     // Inserting lock on the suffix search string. 
     sem_wait(&(lock_suffixMap));
     std::map<string, int> :: iterator mapIterator;
     mapIterator=suffixLockMap.find(s);
     if (mapIterator == suffixLockMap.end()){
      suffixLockMap[s]=1;
     }else{
        mapIterator->second=mapIterator->second+1;
     }
     sem_post(&(lock_suffixMap));

     result.setStringContainer(getSuffixStrings(rootSuffixNode, s));
     result.setSize();
 
     // Release Lock on suffix Search For String s
     sem_wait(&(lock_suffixMap));
     mapIterator=suffixLockMap.find(s);
     if (mapIterator == suffixLockMap.end()){
         assert(false);
     }else{
        mapIterator->second=mapIterator->second-1;
        if (mapIterator->second == 0){
          suffixLockMap.erase(s);
        }
     }
     sem_post(&(lock_suffixMap));
  

     return result;
  };

     nodePtr getPrefixRoot (){
    return rootPrefixNode;
   };

   nodePtr getSuffixRoot (){
    return rootSuffixNode;
   };

};

StringIndex *stringIndex;
// Test I - Test that the binary search tree gets instantiated correctly 
int main (){
  stringIndex = new StringIndex();
  test1();
  pthread_t thread_id_1, thread_id_2;
  pthread_create (&thread_id_1, NULL, test4, (void *)NULL); 
  pthread_create (&thread_id_2, NULL, test4, (void *)NULL); 
  while(1);
//  test2();
//  test3();
//  test4();
  return 0;
}


void test1(){
    string stringArray[8] = {"ravi", "rav", "ra", "fsfsfs", "ff", "ff", "dsds", "abc"};    
    int i;
    for (i=0; i<8; i++){
       stringIndex->insert(stringArray[i]);     
  }
  //printInOrderPrefix(stringIndex->getPrefixRoot());
  //printInOrderSuffix(stringIndex->getSuffixRoot());
}

void test2(){
  Result result = stringIndex->stringsWithPrefix(string("f"));
  //cout << result.Size() << endl;
  cout << result.remove() << endl;
  //printInOrderPrefix(stringIndex->getPrefixRoot());
}

void test3(){
  Result result = stringIndex->stringsWithSuffix(string("s"));
}

void * test4(void *args){
  Result result = stringIndex->stringsWithPrefix(string("f"));
  cout << "Removal Count=" << result.remove() << endl;
  fflush(stdout);
  fflush(stdout);
}

bool isPrefixNotLocked (string s1, map<string, int> lockMap){
 int i=0;
 map <string, int>:: iterator it;
 sem_wait(&(lock_prefixMap));
 for (; i<s1.length(); i++){
    it = lockMap.find(s1.substr(0, i));
    if (it != lockMap.end()){      
      // If lock for a string is present in the lock map then the number of locks must be > 0.
      sem_post(&(lock_prefixMap));
      return false;
    }
 }
 sem_post(&(lock_prefixMap));
 return true;
}

bool isSuffixNotLocked (string s1, map<string, int> lockMap){
 int i=0;
 map <string, int>:: iterator it;
 sem_wait(&(lock_suffixMap));
 for (; i<s1.length(); i++){
    it = lockMap.find(s1.substr(0, i));
    if (it != lockMap.end()){
      assert(it->second > 0); 
      // If lock for a string is present in the lock map then the number of locks must be > 0.
      sem_post(&(lock_suffixMap));
      return false;
    }
 }
 sem_post(&(lock_suffixMap));
 return true;
}

