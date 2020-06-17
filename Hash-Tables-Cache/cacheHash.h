#include "cacheHash.cpp"
#ifndef CACHEHASH_H
#define CACHEHASH_H

#include <string>
#include <iostream>

using namespace std;
class block{
    public:
        string code;
        string word;
        unsigned long used;
        block(string _code,string _word,unsigned long _used){
            this->code = _code;
            this->word = _word;
            this->used = _used;
        }
};

class HashL{
    private:
        block** hashTable;
        long tableSize; //same as cache size
        unsigned long hash(string code); 
    public:
        HashL();
        ~HashL();
        void insert(string code, string word);
		void deleteWord(string code);
		block* lookup(string code);
        void deleteLeastUsed(); //only use it for userData where string values store integers
};

#endif
