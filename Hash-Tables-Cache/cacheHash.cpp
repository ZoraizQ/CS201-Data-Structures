#ifndef CACHEHASH_CPP
#define CACHEHASH_CPP
#include "cacheHash.h"
#include "hashfunctions.cpp"
#include <sstream>

HashL::HashL(){
    tableSize = 1000; //initially by default, adjust cache size here as well
    hashTable = new block*[tableSize]; //array of block*, all NULL initially
    for (int i = 0; i < tableSize; i++){
    	hashTable[i] = NULL;
    }
}

HashL::~HashL(){
	for (int i = 0; i < tableSize; i++){
    	delete hashTable[i]; //deallocate every block*
    }

    delete[] hashTable; //deallocate ptr memory to block* array
}

void HashL::insert(string code, string word){
	unsigned long key = hash(code);
	
	unsigned new_index;
	for (unsigned i = 0; i < tableSize; i++){
		new_index = (key+i)%tableSize;
		if (hashTable[new_index] == NULL) //until end of probing (NULL found)
			break;
	}
	//until empty space found to insert (free block)
	hashTable[new_index] = new block(code, word, 0); //allocated block memory, assigned given code of the word & word & 0 used by default
}

void HashL::deleteWord(string code){
	unsigned long key = hash(code);
	unsigned long new_index;
	for (unsigned i = 0; i < tableSize; i++){
		new_index = (key+i)%tableSize;
		if(hashTable[new_index] == NULL)
			return;

		if(hashTable[new_index]->code == code){ //value found
			delete hashTable[new_index];
			hashTable[new_index] = NULL;
		}
	}
}


block* HashL::lookup(string code){
	unsigned long key = hash(code);
	unsigned long new_index;
	for (unsigned i = 0; i < tableSize; i++){
		new_index = (key+i)%tableSize;
		if(hashTable[new_index] == NULL)
			return NULL;

		if(hashTable[new_index]->code== code) //value found
			return hashTable[new_index];
	}
}

void HashL::deleteLeastUsed(){
	int l = -1;
	int l_used = -1;

	for (int i = 0; i < tableSize; i++){ //to get first non-NULL block
		if (hashTable[i] != NULL){
			l = i;
			l_used = hashTable[i]->used;
			break;
		}
	}

	for (int i = l+1; i < tableSize; i++){ 
		if (hashTable[i] == NULL)
			continue;

		int currUsed = hashTable[i]->used;

		if (currUsed == 0){
			l = i;
			break;
		}

		if (currUsed < l_used){
			l_used = currUsed;
			l = i;
		}
	}

	if (l == -1)
	 	return;
	
	delete hashTable[l];
	hashTable[l] = NULL;
}

unsigned long HashL :: hash(string code){
	return madCompression(bitHash(code), tableSize);
}


#endif