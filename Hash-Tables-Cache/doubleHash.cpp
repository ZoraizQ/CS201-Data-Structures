#ifndef DOUBLEHASH_CPP
#define DOUBLEHASH_CPP
#include "doubleHash.h"
#include "hashfunctions.cpp"

HashD::HashD(){
    tableSize = 10000; // you cant change this
    count = 0; //no entries in the table initially
    hashTable = new block*[tableSize]; //array of block*, all NULL initially
    for (int i = 0; i < tableSize; i++)
    	hashTable[i] = NULL;
}

HashD::~HashD(){
	for (int i = 0; i < tableSize; i++){
    	delete hashTable[i]; //deallocate every block*
    	count--;
    }

    delete[] hashTable; //deallocate ptr memory to block* array
}

void HashD::resizeTable(){
	//create new_hashTable for double the old size, copy contents (ptrs only) to it, delete old table, point to this new table in memory
	int old_tableSize = tableSize;
	tableSize *= 2;
	block** new_hashTable = new block*[tableSize];
	for (int i = 0; i < tableSize; i++)
		new_hashTable[i] = NULL;

	for (int i = 0; i < old_tableSize; i++){ //copying ptrs
		if (hashTable[i] != NULL){ //some value found to rehash
			string v = hashTable[i]->value;
			unsigned long h = hash1(v);
			unsigned long d = hash2(v);

			unsigned new_index;
			for (unsigned i = 0; i < tableSize; i++){
				new_index = (h+i*d)%tableSize;
				//until end of probing (NULL found)
				if (new_hashTable[new_index] == NULL)
					break;
			}
			//until empty space found to insert (free block)
			new_hashTable[new_index] = new block(new_index, v); //allocated block memory, assigned given key & value
			delete hashTable[i];
		}
	}
	delete hashTable; //only deletes pointer to the old bucket array, not the memory of individual blocks
	hashTable = new_hashTable; //points to new table
}

void HashD::insert(string value){
	unsigned long h = hash1(value);
	unsigned long d = hash2(value);
	
	unsigned new_index;
	for (unsigned i = 0; i < tableSize; i++){
		new_index = (h+i*d)%tableSize;
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

void HashD::deleteWord(string value){
	unsigned long h = hash1(value);
	unsigned long d = hash2(value);

	unsigned new_index;
	for (unsigned i = 0; i < tableSize; i++){
		new_index = (h+i*d)%tableSize;
		if(hashTable[new_index] == NULL)
			return;

		if(hashTable[new_index]->value == value){ //value found
			delete hashTable[new_index];
			hashTable[new_index] = NULL;
			count--;
		}
	}	
}

block* HashD::lookup(string value){
	unsigned long h = hash1(value);
	unsigned long d = hash2(value);

	unsigned new_index;
	for (unsigned i = 0; i < tableSize; i++){ //keep probing by double hash
		new_index = (h+i*d)%tableSize;
		if(hashTable[new_index] == NULL)
			return NULL;

		if(hashTable[new_index]->value == value) //value found
			return hashTable[new_index];
	}
}

unsigned long HashD::hash1(string value){
	return madCompression(polyHash(value), tableSize); //bit-div
}

unsigned long HashD::hash2(string value){
	//h2(key) = q − (key % q), where key is the bitHash(value)
	unsigned long prime_q = 11;
	return prime_q - (polyHash(value) % prime_q);
}


#endif