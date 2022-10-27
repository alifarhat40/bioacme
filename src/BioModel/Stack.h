#ifndef STACK_H_
#define STACK_H_

/* a stack of states. */

#define USEEXMEM true
#define STACKBLOCKSIZE 100000

class State;
class SLinkedList;

using namespace std;


class Stack
{
	// Use external memory to store stack
	bool USEExMem;
	// number of states in each stack block
	unsigned int SizeofStackBlock;
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
	SLinkedList* theStack;
	
public:	
	// directory name generated randomly for saving the temporary files of stack blocks.
	char* DirName;
	
public:
	Stack();
	virtual ~Stack();
	
	void push(State* astate);
	State* pop();
	bool isEmpty();
	void setNumSpecies(unsigned int n);
};



#endif /*STACK_H_*/
