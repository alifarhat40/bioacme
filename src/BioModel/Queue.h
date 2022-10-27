#ifndef QUEUE_H_
#define QUEUE_H_

/* a stack of states. */

#define USEEXMEM true
#define QUEUEBLOCKSIZE 100000
//#define QUEUEBLOCKSIZE 1000

class State;
class SLinkedList;

using namespace std;


class Queue
{
	// Use external memory to store stack
	bool USEExMem;
	// number of states in each stack block
	unsigned int SizeofQueueBlock;
	// depth of current block
	unsigned int DepthofCurrentBlock;
	// total blocks resided in external memory
	unsigned int TotalBlocks;
	// current full path
	char* path;
	// depth of the stack
	unsigned int depth;
	/* stacklist, the stack top */
	unsigned int n_species;
	//State* top;
	//State* bottom;
	SLinkedList* theQueue;
	
public:	
	// directory name generated randomly for saving the temporary files of stack blocks.
	char* DirName;
	
public:
	Queue();
	virtual ~Queue();
	
	void push(State* astate);
	State* pop();
	bool isEmpty();
	void setNumSpecies(unsigned int n);
};



#endif /*QUEUE_H_*/
