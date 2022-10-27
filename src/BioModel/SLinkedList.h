#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

/*
 * LinkedList class specifically designed for State node.
 * Maybe it also can be used as a generic linked list class
 * after some revision.
 */

#include "State.h"

class SLinkedList
{
private:
	unsigned long 	number;
	State* 	head;
	State* 	tail;

public:
	SLinkedList();
	virtual ~SLinkedList();
	
	State* AddtoHead (State* node);
	State* AddtoTail (State* node);
	State* HeadDel ();	// return the pointer of the removed node
	State* TailDel ();  // return the pointer of the removed node
	State* Find (State* item) const;
	unsigned long getSize() const;
	State* getHead() const;
	State* getTail() const;
	
	int WriteList (char* filepath);
	int WriteListQ (char* filepath);
	int WriteListA(char* filepath);
	int ReadList (char* filepath, int n_species, unsigned int num_states);
	int ReadListQ (char* filepath, int n_species, unsigned int num_states);
};

#endif /*LINKEDLIST_H_*/

