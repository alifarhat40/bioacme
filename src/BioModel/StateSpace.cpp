
#include <stdio.h>
#include <iostream>
#include <string>
#include "State.h"
#include "StateSpace.h"
#include "HashTable.h"

using namespace std;

StateSpace::StateSpace()
{
	this->n_species = 0;
	this->n_states = 0;
	this->States = NULL;
	this->SpeciesID = 0;
	this->ssHashTable = new HashTable();
}

StateSpace::~StateSpace()
{
}

int StateSpace::readStateSpace(char* ssfilename)
{
	FILE* fp = 0;
	char* str = 0;
	char  ch = ' ';
	unsigned int s = 0;
	
	fp = fopen(ssfilename, "r");
	
	fscanf (fp, "%d %ld\n", &this->n_species, &this->n_states);
	
	this->SpeciesID = new string[this->n_species];
	
	str = new char[8];
	
	for (unsigned int i = 0; i < this->n_species; i++)
	{
		fscanf (fp, "%s", str);
		this->SpeciesID[i] = str;
		fscanf (fp, "%c", &ch);
	}
		
	this->States = new STATETYPE[this->n_states * this->n_species];
	
	// read lines
	State* ps = 0;
	for (unsigned int i = 0; i < this->n_states; i++)
	{
		// build a new null state first.
		ps = new State(n_species);
		
		// read columns, and put values in States.
		for (unsigned int j = 0; j < this->n_species; j++)
		{
			fscanf (fp, "%d", &s);
			this->States[i * this->n_species + j] = s;
			fscanf (fp, "%c", &ch);
			
			// set the value state ps.
			ps->setStateValue(j, (STATETYPE)s);
		}
		
		// insert new state into ssHashTable.
		ps->setSN((unsigned long)i);
		ssHashTable->Insert(ps);
	}
	
	fclose (fp);
	
	return 0;
}


int StateSpace::writeStateSpace(char* ssfilename)
{
	FILE* fp = 0;
	
	fp = fopen(ssfilename, "w");
	if (!fp)
	{
		printf("Error: cannot creat file: %s\n", ssfilename);
		return -1;
	}
	
	/* the first line */
	fprintf (fp, "%d %ld\n", this->n_species, this->n_states);
	
	// for debug
	//printf (">>>>>>>>>>>>---%d---%d\n", this->n_species, this->n_states);
	
	/* the second line */
	for (unsigned int i = 0; i < this->n_species; i++)
	{
		fprintf (fp, "%s", this->SpeciesID[i].data());
		if (i < this->n_species - 1)
		{
			fprintf (fp, " ");
		}
	}
	fprintf (fp, "\n");
	

	for (unsigned int i = 0; i < this->n_states; i++)
	{
		for (unsigned int j = 0; j < this->n_species; j++)
		{
			fprintf (fp, "%d", this->getValue(i, j));
			if (j < this->n_species - 1)
			{
				fprintf (fp, " ");
			}
		}
		fprintf (fp, "\n");
		// for debug
		//printf ("\n");
	}
	
	// for debug
	//printf (">>>>>>>---OK\n");
	
	fclose (fp);
	
	return 1;
}


unsigned int StateSpace::getValue(unsigned int i, unsigned int j)
{
	return this->States[i * this->n_species + j];
}

State* StateSpace::getState(unsigned int i)
{
	State* s = new State(this->n_species);
	for (unsigned int j=0; j<this->n_species; j++)
	{
		s->setStateValue(j, this->getValue(i,j));
	}
	return s;
}

HashTable* StateSpace::getHashTable ()
{
	return ssHashTable;
}

unsigned int StateSpace::getNumSpecies ()
{
	return this->n_species;
}

int StateSpace::setNumSpecies(unsigned int n)
{
	this->n_species = n;
	return 1;
}

unsigned long StateSpace::getNumStates ()
{
	return this->n_states;
}

int StateSpace::setNumStates(unsigned long n)
{
	this->n_states = n;
	return 1;
}

string* StateSpace::getSpeciesID()
{
	return this->SpeciesID;
}

int StateSpace::setSpeciesID(string* speciesID, unsigned int n_species)
{
	if (n_species != this->n_species)
	{
		printf ("Error parameters in arg2 of setSpecies().\n");
		return -1;
	}
	this->SpeciesID = new string[n_species];
	for (unsigned int i = 0; i < n_species; i++)
	{
		this->SpeciesID[i] = speciesID[i];
	}
	return 1;
}

const char* StateSpace::getSpeciesIDValue(unsigned int i)
{
	//string str = SpeciesID[i];
	const char* str = 0;
	str = this->SpeciesID[i].data();
	
	return str;
}



STATETYPE* StateSpace::getStates()
{
	return this->States;
}

int StateSpace::setStates(STATETYPE* states)
{
	this->States = states;
	return 1;
}

int StateSpace::findState(State* astate)
{
	return ssHashTable->Find(astate);
	/*
	unsigned int i = 0, k = 0;
	bool found = false;
	while (!found && k < this->n_states)
	{
		for (i = 0; i < this->n_species; i++)
		{
			if (getValue(k, i) != astate[i])
				break;
		}

		if (i == this->n_species)
		{
			found = true;
			break;
		}

		if (k < this->n_states)
			k++;
	}

	if (found)
	{
		return k;
	}
	else
	{
		return -1;
	}
	*/
}


/* EOF */

