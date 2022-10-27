
#include <stdio.h>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <sys/stat.h>
#include "Queue.h"
#include "State.h"
#include "SLinkedList.h"

#if defined(_WIN32)
#include <direct.h>
#endif
using namespace std;


Queue::Queue()
{
	USEExMem = USEEXMEM;
	SizeofQueueBlock = QUEUEBLOCKSIZE;
	DepthofCurrentBlock = 0;
	depth = 0;
	TotalBlocks = 0;
	DirName = NULL;
	path = NULL;
	n_species = 0;
	theQueue = new SLinkedList();
	//this->top = new State();
}

Queue::~Queue()
{
}

void Queue::push(State* astate)
{
	// for debug
	//cout << "It's here." << endl;
	
	//cout << "  Before. " << theQueue << " " << astate << endl;
	theQueue->AddtoTail(astate);
	//cout << "  After." << endl;
	depth ++;
	if (USEExMem)
	{
		DepthofCurrentBlock ++;
	}
	
	// if the DepthofCurrentBlock is >= SizeofQueueBlock,
	// then transfer excess data of stack bottom to external memory(hard disk files).
	if (USEExMem)
	{
		if (DepthofCurrentBlock >= SizeofQueueBlock)
		{
			//cout << "It's here. Save it to HD." << endl;
			// Save current Queue block to external file.
			// First, generate a directory name randomly, and build the directory.
			if (DirName == NULL)
			{
				srand((unsigned long)time(0));
				unsigned long rn = rand();
				DirName = new char[64];
				sprintf (DirName, "/tmp/_tmp%lx", rn);

				#if defined(_WIN32)
					_mkdir (DirName);
				#else	
					mkdir (DirName, 0755);	// ATTENTION: THIS FUNCTION IS OS-DEPENDENT! //
				#endif	
			}

			// Second, put data in the stack block to a file named with the value of TotalBlocks
			// in the built directory.
			path = new char[80];
			sprintf (path, "%s/%x", DirName, TotalBlocks);
			
			/*unsigned int count = */theQueue->WriteListQ(path);
			//cout << "It's out here. Saved it to HD." << endl;
			
			// delete current block from memory space.
			
			// revise the value of DepthofCurrentBlock, TotalBlocks
			DepthofCurrentBlock = DepthofCurrentBlock - SizeofQueueBlock;
			TotalBlocks ++;
		}
	}
	
	
	//printf (">>>>push to stack.\n");
	//State* s = new State(astate->n_species);
	//printf ("species number: %d\n", astate->n_species);
	//for (unsigned int i = 0; i < astate->n_species; i++)
	//{
		//s->state[i] = astate->state[i];
		//printf (">>%d\n", s->state[i]);
	//}
	
	////this->top->n_species ++; /* counter */
	
	//printf ("%X\n", top->getNext());
	
	////astate->setNext(this->top->getNext());
	////this->top->setNext(astate);
	//s->setNext(this->top->getNext());
	//this->top->setNext(s);
	
	//printf ("%X\n", top->getNext());
	//printf ("%d\n", top->n_species);
	//for (unsigned int i = 0; i < 6; i++)
	//{
	//	printf ("a%d\n", top->getNext()->state[i]);
	//	printf ("b%d\n", s->state[i]);
	//}
	////return this->top;
	
	//return theQueue;
}


State* Queue::pop()
{
	if (USEExMem)
	{
		if (DepthofCurrentBlock <= 0 && depth > 0 && TotalBlocks >= 0)
		{
			TotalBlocks --;
			path = new char[64];
			sprintf (path, "%s/%x", DirName, TotalBlocks);
			
			theQueue->ReadListQ(path, n_species, SizeofQueueBlock);
			
			// revise the value of DepthofCurrentBlock, TotalBlocks
			DepthofCurrentBlock = DepthofCurrentBlock + SizeofQueueBlock;

			delete[] path;
		}
	}
	
	State* p = theQueue->HeadDel();
	depth --;
	//cout << " POP " << endl;
	if (USEExMem)
	{
		DepthofCurrentBlock --;
	}
	
	return p;
	
	//printf ("<<<<pop out stack.\n");
	//for (unsigned int i = 0; i < 6; i++)
	//{
	//	printf ("t%d\n", (unsigned int)top->getNext()->state[i]);
	//}
	//printf ("%X\n", top->getNext());
	////State* p = this->top->getNext();
	//for (unsigned int i = 0; i < 6; i++)
	//{
	//	printf ("p%d\n", p.state[i]);
	//}
	////if (!isEmpty())
	////{
	////	this->top->setNext(this->top->getNext()->getNext());
	////}
	////p->setNext(0);
	//for (unsigned int i = 0; i < 6; i++)
	//{
	//	printf ("p%d\n", p.state[i]);
	//}
	//printf ("%X\n", top->getNext());
	////return p;
}


bool Queue::isEmpty()
{
	if (depth == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Queue::setNumSpecies(unsigned int n)
{
	n_species = n;
}

