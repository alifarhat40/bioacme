
#include <stdio.h>
#include <iostream>
#include "State.h"
#include "SLinkedList.h"

using namespace std;

SLinkedList::SLinkedList()
{
	number = 0;
	head = 0;
	tail = 0;
}

SLinkedList::~SLinkedList()
{
}

State* SLinkedList::AddtoHead (State* node)
{
	if (node == 0)
	{
		return 0;
	}
	
	if (head != 0 && tail != 0 && number != 0)
	{
		node->setNext(head);
		head->setPrev(node);
		head = node;
		number ++;
	}
	else
	{
		head = tail = node;
		number = 1;
	}
	
	return head;
}

State* SLinkedList::AddtoTail (State* node)
{
	if (node == 0)
	{
		return 0;
	}
	
	if (number == 0)
	{
		head = node;
		tail = node;
		number = 1;
		node->setNext(NULL);
		node->setPrev(NULL);
	}
	else
	{
		tail->setNext(node);
		node->setPrev(tail);
		tail = node;
		number ++;
	}
	
	/*
	if (head != 0 && tail != 0 && size != 0)
	{
		tail->setNext(node);
		tail = node;
		size ++;
	}
	else
	{
		head = tail = node;
		size = 1;
	}
	*/
	//printf ("ad%d\n", head);
	//printf ("address\n");
	return head;
}

State* SLinkedList::HeadDel ()
{
	if (!head)
	{
		return NULL;
	}

	State* p = head;
	head = head->getNext();
	p->setNext(0);
	if (head)
	{
		head->setPrev(0);
	}
	number --;
	
	return p;
}

State* SLinkedList::TailDel ()
{
	if (!tail)
	{
		return NULL;
	}

	State* p = tail;
	tail = tail->getPrev();
	p->setPrev(0);
	if (tail)
	{
		tail->setNext(0);
	}
	number --;
	
	return p;
}


State* SLinkedList::Find (State* item) const
{
	State* p = 0;

	p = head;
	while (p != 0)
	{
		if(*item == *p)
		{
			return p;
		}
		p = p->getNext();
	}

	return 0;
}

unsigned long SLinkedList::getSize() const
{
	return number;
}

State* SLinkedList::getHead() const
{
	return head;
}

State* SLinkedList::getTail() const
{
	return tail;
}

int SLinkedList::WriteList(char* filepath)
{
	// for debug
	//printf ("push.\n");
	
	unsigned int counter = 0;
	
	FILE* fp = 0;
	fp = fopen (filepath, "w");
	if (!fp)
	{
		printf ("ERROR: Can not create temporary file: %s\n", filepath);
		return -1;
	}
	
	// for debug
	//printf ("size: %d\n", this->getSize());
	
	State* sp = 0;
	while (/*this->getSize() > 0*/this->getHead() != NULL)
	{
		sp = this->HeadDel();
		for (unsigned int i = 0; i < sp->n_species; i++)
		{
			fprintf (fp, "%d", sp->getStateValue(i));
			if (i < sp->n_species - 1)
			{
				fprintf (fp, " ");
			}
			else if(i == sp->n_species - 1)
			{
				fprintf (fp, "\n");
			}
		}
		counter ++;
		delete sp;
		//sp = 0;
		
		
		//this->HeadDel();
	}
	
	fflush (fp);
	fclose (fp);
	
	// for debug
	//printf ("write block finished.\n");
	
	return counter;
}

int SLinkedList::WriteListQ(char* filepath) // For Queue
{
	// for debug
	//printf ("push.\n");
	
	unsigned int counter = 0;
	
	FILE* fp = 0;
	fp = fopen (filepath, "w");
	if (!fp)
	{
		printf ("ERROR: Can not create temporary file: %s\n", filepath);
		return -1;
	}
	
	// for debug
	//printf ("size: %d\n", this->getSize());
	
	State* sp = 0;
	while (/*this->getSize() > 0*/this->getTail() != NULL)
	{
		sp = this->TailDel();
		for (unsigned int i = 0; i < sp->n_species; i++)
		{
			fprintf (fp, "%d", sp->getStateValue(i));
			if (i < sp->n_species - 1)
			{
				fprintf (fp, " ");
			}
			else if(i == sp->n_species - 1)
			{
				fprintf (fp, "\n");
			}
		}
		counter ++;
		delete sp;
		//sp = 0;
		
		
		//this->HeadDel();
	}
	
	fflush (fp);
	fclose (fp);
	
	// for debug
	//printf ("write block finished.\n");
	
	return counter;
}


int SLinkedList::WriteListA(char* filepath)
{
	unsigned int counter = 0;
	
	FILE* fp = 0;
	fp = fopen (filepath, "a");
	if (!fp)
	{
		printf ("ERROR: Can not open file: %s\n", filepath);
		return -1;
	}
	
	State* sp = 0;
	//sp = this->getHead();
	while (head)
	{
		sp = this->HeadDel();
		for (unsigned int i = 0; i < sp->n_species; i++)
		{
			fprintf (fp, "%d", sp->getStateValue(i));
			if (i < sp->n_species - 1)
			{
				fprintf (fp, " ");
			}
			else if(i == sp->n_species - 1)
			{
				fprintf (fp, "\n");
			}
		}
		counter ++;
	}
	
	fflush (fp);
	fclose (fp);
	
	return counter;
}


int SLinkedList::ReadList(char* filepath, int n_species, unsigned int num_states)
{
	unsigned int counter = 0;
	
	FILE* fp = 0;
	fp = fopen (filepath, "r");
	if (!fp)
	{
		printf ("ERROR: Can not open temporary file: %s\n", filepath);
		return -1;
	}
	
	int d;
	char ch;
	
	while(!feof(fp) && counter < num_states)
	{
		State* astate = new State(n_species);
		for (int j = 0; j < n_species; j++)
		{
			fscanf (fp, "%d", &d);
			astate->setStateValue(j, d);
			fscanf (fp, "%c", &ch);
		}
		
		this->AddtoTail(astate);
		counter ++;
	}
	
	fclose (fp);
	remove (filepath);
	
	return counter;
}

int SLinkedList::ReadListQ(char* filepath, int n_species, unsigned int num_states) // For Queue
{
	unsigned int counter = 0;
	
	FILE* fp = 0;
	fp = fopen (filepath, "r");
	if (!fp)
	{
		printf ("ERROR: Can not open temporary file: %s\n", filepath);
		return -1;
	}
	
	int d;
	char ch;
	
	while(!feof(fp) && counter < num_states)
	{
		State* astate = new State(n_species);
		for (int j = 0; j < n_species; j++)
		{
			fscanf (fp, "%d", &d);
			astate->setStateValue(j, d);
			fscanf (fp, "%c", &ch);
		}
		
		this->AddtoHead(astate);
		counter ++;
	}
	
	fclose (fp);
	remove (filepath);
	
	return counter;
}


