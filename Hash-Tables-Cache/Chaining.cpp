#ifndef CHAINING_CPP
#define CHAINING_CPP
#include "Chaining.h"
#include "hashfunctions.cpp"


HashC::HashC(int size){
	tableSize = long(size); //tableSize from constructor
	hashTable = new LinkedList<string>[tableSize]; //hashTable points to an array of LL<string> of size tableSize now in memory
}

HashC::~HashC(){
	// only dynamic memory is the hashtable LL of type string*
	// which deallocates itself with its own destructor
}

unsigned long HashC :: hash(string input){ //return the hash for a given string 
	//poly hash with mad compression used	
	return madCompression(polyHash(input), tableSize); //3
}

void HashC::insert(string word){
	//get key by hashing the word, and at that index, insertAtHead at the LL, if there's a collision, the linked list length increases (more than 1 listitems)
	hashTable[hash(word)].insertAtHead(word);
} 

ListItem<string>* HashC :: lookup(string word){
	//hash word to get key to check the LL, and then search for the word in the corresponding LL
	return hashTable[hash(word)].searchFor(word);
}

void HashC :: deleteWord(string word){
	//hash word to get key for the index of the LL array, and then delete element from that LL
	hashTable[hash(word)].deleteElement(word);
}


#endif