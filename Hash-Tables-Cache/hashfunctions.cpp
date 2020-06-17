#ifndef HASHFUNCTIONS_CPP
#define HASHFUNCTIONS_CPP
#include <string>
#include <iostream>
#include <cmath>

using namespace std;
// this takes in a string and returns a 64bit hash.
unsigned long polyHash(std::string value,int a = 9){
	// a == non-zero constant, configurable
	// co-efficient = value[i]
	int k = value.length();
	int poly_hash = 0;
	for (int i = k-1; i >= 0; i--){ //last value of i is 0
		poly_hash = poly_hash * a + (int)value[i]; //better formula from the Book
	}

	return poly_hash;
}

//does some bitwise operation and generates a 64bit hash.
unsigned long bitHash(std::string value){
	/*
	Initialize bitwise_hash = 0
	For every s_i in str
	bitwise_hash ^= (bitwise_hash << 5) + (bitwise_hash >> 2) + s_i
	*/
	unsigned long bitwise_hash = 0;
	for (unsigned int i = 0; i < value.length(); i++){
		bitwise_hash ^= (bitwise_hash << 5) + (bitwise_hash >> 2) + value[i];
	}

	return bitwise_hash;
}
// Size is the size of array maintained by the hashtable.
unsigned long divCompression(unsigned long hash,long size){
	//h(k) = |k| mod N, where k is the hash and N is the array size -- should be a prime number
	return hash % size;
}
// multiplication addition and division compression. 
unsigned long madCompression(unsigned long hash,long size,int m = 1993,int a = 1637){
	//h(k) = |ak+m| mod N, k = hash, N is a prime number (array size), a and m = non-negative integers randomly chosen
	return (a * hash + m) % size;
}
#endif
// you may write your own program to test these functions.