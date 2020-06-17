#ifndef QPROBING_CPP
#define QPROBING_CPP

#include "qProbing.h"
#include "hashfunctions.cpp"

int words = 0;

HashL::HashL(){
    tableSize = 1000; // you cant change this
    count = 0; //no entries in the table initially
    hashTable = new block*[tableSize]; //array of block*, all NULL initially
    for (int i = 0; i < tableSize; i++)
    	hashTable[i] = NULL;
}

HashL::~HashL(){
    for (int i = 0; i < tableSize; i++){
    	delete hashTable[i]; //deallocate every block*
    	count--;
    }

    delete[] hashTable; //deallocate ptr memory to block* array
}

unsigned long HashL :: hash(string value){
	return madCompression(bitHash(value), tableSize);
}

void HashL::resizeTable(){
	//create new_hashTable for double the old size, copy contents (ptrs only) to it, delete old table, point to this new table in memory
	int old_tableSize = tableSize;
	tableSize *= 2;
	block** new_hashTable = new block*[tableSize];
	for (int i = 0; i < tableSize; i++)
		new_hashTable[i] = NULL;

	for (int i = 0; i < old_tableSize; i++){ //copying ptrs
		if (hashTable[i] != NULL){ //some value found to rehash
			unsigned long key = hash(hashTable[i]->value); //new key hashed according to new tableSize

			unsigned new_index;
			for (unsigned j = 0; j < tableSize; j++){
				new_index = (key+j*j)%tableSize;
				//until end of probing (NULL found)
				if (new_hashTable[new_index] == NULL)
					break;
			}
			//until empty space found to insert (free block)
			new_hashTable[new_index] = new block(new_index, hashTable[i]->value); //allocated block memory, assigned given key & value
			delete hashTable[i];
		}
	}
	delete hashTable; //only deletes pointer to the old bucket array, not the memory of individual blocks
	hashTable = new_hashTable; //points to new table
}

void HashL::insert(string value){
	unsigned long key = hash(value);
	
	unsigned new_index;
	for (unsigned i = 0; i < tableSize; i++){
		new_index = (key+i*i)%tableSize;
		if (hashTable[new_index] == NULL) //until end of probing (NULL found)
			break;
	}
	//until empty space found to insert (free block)
	hashTable[new_index] = new block(new_index, value); //allocated block memory, assigned given key & value
	count++; //count incremented, since new block inserted
	
  	//check LF for need to resize
  	if (count/(float)tableSize >= 0.5f) //count/tableSize is the LF alpha here
  		resizeTable();	
}

void HashL::deleteWord(string value){
	unsigned long key = hash(value);
	unsigned new_index;
	for (unsigned i = 0; i < tableSize; i++){
		new_index = (key+i*i)%tableSize;
		if(hashTable[new_index] == NULL)
			return;

		if(hashTable[new_index]->value == value){ //value found
			delete hashTable[new_index];
			hashTable[new_index] = NULL;
			count--;
		}
	}
}

block* HashL::lookup(string value){
	unsigned long key = hash(value);
	unsigned new_index;
	for (unsigned i = 0; i < tableSize; i++){
		new_index = (key+i*i)%tableSize;
		if(hashTable[new_index] == NULL)
			return NULL;

		if(hashTable[new_index]->value == value) //value found
			return hashTable[new_index];
	}
}
#endif
