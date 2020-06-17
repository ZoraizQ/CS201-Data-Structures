//include any header files as required
#include <fstream>
#include "cacheHash.cpp"
#include <chrono>

#define RST  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KBLU  "\x1B[34m"
#define FRED(x) KRED x RST
#define FGRN(x) KGRN x RST
#define FBLU(x) KBLU x RST

class CacheManager
{
	private:
		HashL cache;
		long wordsInCache;
		long cacheSize;
		//long dynamic_cap;
		string dictionary_file;
		string decodeWord_Direct(string code);
		string decodeWord_Cache(string code);
        //void optimizeByLFU();
	public:
		//methods
		CacheManager();
		~CacheManager();
		void setDictionary(string fn); //setter for dictionary file name, so that user can modify it on runtime
		void decodeWithCacheMode(bool withCache, string codesFile, string decodeFile, int maxWords);
};

CacheManager::CacheManager(){
	//cache HashD constructor initialised
	cacheSize = 1000;
	wordsInCache = 0;
	//dynamic_cap = 20; //initial value
}

CacheManager::~CacheManager(){
	//empty, since the cache's (HashD) destructor deallocates its memory automatically
}

void CacheManager::setDictionary(string fn){
	dictionary_file = fn;
}

/*
void CacheManager::optimizeByLFU(){
	for (int i = 0; i < dynamic_cap/4; i++){ //delete a fraction e.g. 5 from 25 inserted words, where 25 is the current dynamic cap (will be == to wordsInCache)
		string minWord_code = cache.getMinimumUsedCode(); //note that these words are the ones that are least used
		cache.deleteWord(minWord_code);
		cout << "Deleted " << minWord_code << endl;
		wordsInCache--;
	}

	if (dynamic_cap/(float)cacheSize < 0.9f)
		dynamic_cap += 20; //the reason its a dynamic cap, increases every time, except when cacheSize reached
}
*/

void CacheManager::decodeWithCacheMode(bool withCache, string codesFile, string decodeFile, int maxWords){
	using namespace std::chrono;
	ifstream inFile;
	inFile.open(codesFile.c_str());
	cout << "Reading from " << codesFile << " and writing decoded words to " << decodeFile << endl;
	ofstream outFile;
	outFile.open(decodeFile.c_str());

	high_resolution_clock::time_point timeStart = high_resolution_clock::now();;
	int wordsRead = 0;
	if (inFile.is_open())
	{
		string currCode;
		while (inFile >> currCode)
		{
			wordsRead++;
			if (currCode[currCode.length()-1] == ',')
				currCode = currCode.substr(0, currCode.length()-1); //truncating the last character which is a ,
			
			if (withCache == true){
				outFile << decodeWord_Cache(currCode) << " "; //decode using cache
			}
			else{
				outFile << decodeWord_Direct(currCode) << " "; //decode directly
			}

			if (wordsRead == maxWords)
				break;
		}
		inFile.close();
	}
	high_resolution_clock::time_point timeEnd = high_resolution_clock::now();;
    duration<double> totalTime = duration_cast<duration<double>>(timeEnd - timeStart);
    cout << FBLU("DECODE TIME: ") << totalTime.count() << " SECONDS."<<endl;
    outFile << "\n\nDECODE TIME: " << totalTime.count() << " SECONDS.";
    outFile.close();
}

string CacheManager::decodeWord_Direct(string code){
	ifstream inFile;
	inFile.open(dictionary_file.c_str());
	string currCode;
	string currWord;	
	if (inFile.is_open())
	{	
		stringstream converter;
		converter << code;
		int linesToSkip;
		converter >> linesToSkip; //if code is 1 then 1 line to skip since it will be at the second line
		for (int i = 0; i < linesToSkip; i++)
			inFile.ignore(67,'\n'); //longest line in dictionary is 66 characters
	
		inFile >> currCode;
		inFile >> currWord;
		if (currCode == code)
			return currWord;

		inFile.close();
	}

	return ""; //when code not found in dictionary
}



string CacheManager::decodeWord_Cache(string code){
	block* blockFound = cache.lookup(code); //code for the word is key in the cache hash table
	if (blockFound != NULL){ //if the word's block is found in the cache
		//std::cout << FGRN("From cache.") << endl;
		blockFound->used += 1;
		return blockFound->word; //return word
	}
	
	//if no entry for the word exists in the cache, then find from file and insert it into cache before returning
	ifstream inFile;
	inFile.open(dictionary_file.c_str());
	string currCode;
	string currWord;
	if (inFile.is_open())
	{
		stringstream converter;
		converter << code;
		int linesToSkip;
		converter >> linesToSkip; //if code is 1 then 1 line to skip since it will be at the second line
		for (int i = 0; i < linesToSkip; i++)
			inFile.ignore(67,'\n'); //longest line in dictionary is 66 characters
	
		inFile >> currCode;
		inFile >> currWord;
		//std::cout << FRED("New word inserted.") << endl;
		cache.insert(currCode, currWord);
		wordsInCache++;
		/*
		if (wordsInCache == dynamic_cap){ //max allowed words currently
			optimizeByLFU();
		}
		*/

		if (wordsInCache == cacheSize){
			cache.deleteLeastUsed(); //LFU
			//std::cout <<FBLU("\nTime to optimize.\n");
			wordsInCache--;
		}

		return currWord;

		inFile.close();
	}

	return ""; //when code not found in dictionary
}


int main(){
	CacheManager CM1; //max size of cache of default constructor is 1000
	CM1.setDictionary("dictionary.txt");

	int maxWords = 10000;
	cout << "Enter max number of words to decode: ";
	cin >> maxWords;

	cout << FRED("\nDecoding secret1.txt directly: ") << endl;
	CM1.decodeWithCacheMode(false, "secret1.txt", "direct-s1-decoded.txt", maxWords); //decode without cache
	
	cout << FGRN("\nDecoding secret1.txt with cache: ") << endl;
	CM1.decodeWithCacheMode(true, "secret1.txt", "cache-s1-decoded.txt", maxWords); //decode with cache
	
	CacheManager CM2; //max size of cache of default constructor is 1000
	CM2.setDictionary("dictionary.txt");

	cout << FRED("\nDecoding secret2.txt directly: ") << endl;
	CM2.decodeWithCacheMode(false, "secret2.txt", "direct-s2-decoded.txt", maxWords); //decode without cache

	cout << FGRN("\nDecoding secret2.txt with cache: ") << endl;
	CM2.decodeWithCacheMode(true, "secret2.txt", "cache-s2-decoded.txt", maxWords); //decode with cache

	CacheManager CM3; //max size of cache of default constructor is 1000
	CM3.setDictionary("dictionary.txt");

	cout << FRED("\nDecoding secret3.txt directly: ") << endl;
	CM3.decodeWithCacheMode(false, "secret3.txt", "direct-s3-decoded.txt", maxWords); //decode without cache

	cout << FGRN("\nDecoding secret3.txt with cache: ") << endl;
	CM3.decodeWithCacheMode(true, "secret3.txt", "cache-s3-decoded.txt", maxWords); //decode with cache
	

	return 0;
}