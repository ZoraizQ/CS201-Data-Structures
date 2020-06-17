#ifndef __DICTIONARY_CPP
#define __DICTIONARY_CPP
#include "dictionary.h"
#include <time.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

/*
words.txt contains words in random order (lower-case), >= <= == overloaded for strings in c++,
so words to be used directly as keys (lexical order)
*/

Dictionary::Dictionary() {
	wordsTree = NULL; //empty dictionary created
}

Dictionary::~Dictionary(){
    //destructor for tree called automatically
}

void Dictionary::initialize(string wordsFile) {
	ifstream words_inFile;
    words_inFile.open(wordsFile.c_str()); //open wordsFile for reading
    if (words_inFile.fail())
        return;

    string curr_word = "";
    int count = 0;
    wordsTree = new BST<string>; //string type BST object created in heap
    clock_t t; 
    t = clock();
    while(words_inFile >> curr_word){ //read current line to curr_word string
        cout << count << ": " << curr_word << endl;
        count++;
        insertWord(curr_word);

        //if (count == 30000)        
        //    break;
    }
    t = clock() - t; 
    cout << "Time to insert " << count << " words: " << ((double)t) / CLOCKS_PER_SEC << "seconds \n";
}

bool Dictionary::deleteWord(string word) {
    wordsTree->delete_node(word);
	return true;
}
bool Dictionary::editWord(string oldWord, string newWord) {
    //get old word's node, change its key to new word
    node<string>* old_word_node = findWord(oldWord);
    if (old_word_node == NULL) //not found
        return false;

    old_word_node->key = newWord;
    return true;
}
bool Dictionary::insertWord(string word) {
    wordsTree->insert(word, word);
	return true;
}
node<string>* Dictionary::findWord(string word) {
	clock_t t; 
    t = clock();
    // TODO your code here
    node<string>* found_word = wordsTree->search(word);
    t = clock() - t; 
    double time_taken = ((double)t) / CLOCKS_PER_SEC; // in seconds 
    cout << "findWord() took " << time_taken << "sec to search for " << word << endl;  
    return found_word; // TODO
}

#endif

int main() {
    Dictionary random_dictionary;

    random_dictionary.initialize("words.txt");
    cout << "Ready to find.\n";

    //select first 26 words starting from distinct alphabets
    string curr_word = "";
    char letters[26] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
    string words_chosen[26];
    int word_num = 0;
    ifstream words_inFile;
    words_inFile.open("words.txt");
    if (words_inFile.fail())
        return 0;

    while(words_inFile >> curr_word){
        if(curr_word[0] == letters[word_num]){ //if current word's first
        // letter matches the current starting letter for the next word we are looking to choose
            words_chosen[word_num] = curr_word; //the word curr_word e.g. "asterion" becomes word number word_num e.g. 0
            word_num++;

            if (word_num == 26) //break after all 26 words chosen
                break; 
        }
    }

    //attempt to find these words again that were chosen
    for (int i = 0; i < word_num; i++){
        random_dictionary.findWord(words_chosen[i]);    
    }


	return 0;
}
