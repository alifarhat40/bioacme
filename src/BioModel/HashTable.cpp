#include "HashTable.h"
#include <iostream>
#include <cmath>

HashTable::HashTable()
{
	numTotalItems = 0;
	numBuckets = M_SLOTS;
	hashTable = new SLinkedList[numBuckets];
}

HashTable::HashTable(unsigned long nbuckets)
{
	numTotalItems = 0;
	numBuckets = nbuckets;
	hashTable = new SLinkedList[nbuckets];
}

HashTable::~HashTable()
{
}

unsigned long HashTable::getNumTotalItems()
{
	return numTotalItems;
}

unsigned long HashTable::getNumBuckets()
{
	return numBuckets;
}

unsigned int HashTable::getSizeofBucket(unsigned long i)
{
	return hashTable[i].getSize();
}

State* HashTable::getHeadofBucket(unsigned int i)
{
	return hashTable[i].getHead();
}
	
unsigned long HashTable::hashfunc(State* key) const
{
	// division method
	const unsigned int BaseNumber = 13; // use this base number to calculate an integar from any state.
	// AFTER SEVERAL TESTS:
	// BASE 2 IS THE WORST CHOICE, 3 IS BETTER THAN 2, 5 IS BETTER THAN 3. 
	// 7 IS BETTER THAN 5. 13 IS BETTER THAN 7. 
	// CONCLUSION: THE BIGGER THE BETTER?
	// NO. 17 AND 19 ARE WORSE THAN 13. OK, I CHOOSE 13 AS THE BASE NUMBER NOW.

	unsigned long theNumber = 0;

	//debug
	//cout << "state: ";
	
	for (unsigned int i=0; i<key->n_species; i++)
	{
		// debug
		//cout << key->getStateValue(i) << " ";
		theNumber = theNumber + key->getStateValue(i) * (unsigned long)pow((double)BaseNumber, (double)i);
	}

	// debug
	//cout << "theNumber: " << theNumber << ";  bucket: " << theNumber % numBuckets << endl;
	
	return theNumber % numBuckets;
}

// Actually, Insert method has involved a search method that
// can make sure states will not be inserted repeatedly.
// So, main program can use only Insert method directly to 
// insert new states to hash table, don't need to search them
// in hash table.
int HashTable::Insert(State* item)
{
	unsigned int hashslot = hashfunc(item);
	unsigned int lllength = hashTable[hashslot].getSize();
	// for debug
	//cout << "<<<<<<HashTable Insert.>>>>>>>" << endl;

	if (lllength == 0)
	{
		hashTable[hashslot].AddtoTail(item);
		numTotalItems ++;
		return 1;
	}
	else
	{
		if (hashTable[hashslot].Find(item) == 0)
		{
			hashTable[hashslot].AddtoTail(item);
			numTotalItems ++;
			return 1;
		}
		else
		{
			return -1;
		}
	}
	
	// if item is in current hash table, return -1.
	return -1;
}

int HashTable::Find(State* item) const
{
	unsigned int hashslot = hashfunc(item);
	unsigned int lllength = hashTable[hashslot].getSize();
	if (lllength == 0)
	{
		return -1;
	}
	else
	{
		State* p = hashTable[hashslot].Find(item);
		if (p == 0)
		{
			return -1;
		}
		else
		{
			return p->getSN(); // item is found in current hash table, and return SN.
		}
	}
}

void HashTable::Delete(State* item)
{
}



