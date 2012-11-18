#include "header.h"

bool lexicographicalCompareString(string str1, string str2){
	return (lexicographical_compare(str1.c_str(), str1.c_str()+str1.length(), str2.c_str(), str2.c_str()+str2.length()));
};

string reverseString (string stringArgument){
   string tempString;
   int i=0, length = stringArgument.length();
   for (; i<length; i++){
   	tempString.push_back(stringArgument[length-1-i]);
   }
   return tempString;
};

// Checks if string s1 is a prefix of string s2.
bool isPrefix (string s1, string s2){
	
	if(s1.length() > s2.length())
		 return false;
  return ((s2.substr(0, s1.length())).compare(s1) == 0);
}

// Checks if string s1 is a suffix of string s2.
bool isSuffix (string s1, string s2){
	if(s1.length() > s2.length())
		 return false;
  	return ((s2.substr(s2.length()-s1.length(), s1.length())).compare(s1) == 0);
}

