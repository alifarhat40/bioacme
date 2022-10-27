#ifndef HASHTABLE_H_
#define HASHTABLE_H_

//#include "sbml/util/List.h"
//#include "sbml/util/ListNode.h"
//#include "array.h"
//#include "list.h"
//#include "link.h"
//#include "iterator.h"
#include "SLinkedList.h"
#include "State.h"

#define M_SLOTS 7000001
//#define M_SLOTS 10001
#define BUCKETSIZEDIST 50

class HashTable
{
protected:
	// record total number of items in the hashtable
	unsigned long numTotalItems;

	// number of buckets; represents size of the hash table
	unsigned long numBuckets;
	
	// the hash table is an array of linked lists
	SLinkedList* hashTable;
	
public:
	HashTable();
	HashTable(unsigned long nbuckets);
	virtual ~HashTable();

	// Data access methods
	unsigned long getNumTotalItems();
	unsigned long getNumBuckets();
	unsigned int getSizeofBucket(unsigned long i);
	State* getHeadofBucket(unsigned int i);

	// hash function
	unsigned long hashfunc(State* key) const;
	
	// list handling methods
	int Insert (State* item);
	int  Find (State* item) const;
	void Delete (State* item);
	
	// output method: write to file
	//int WriteToFile(char* ssfilename);
};

#endif /*HASHTABLE_H_*/
