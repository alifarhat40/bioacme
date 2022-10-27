
#include <iostream>
#include "State.h"

using namespace std;


State::State()
{
	this->sn = 0;
	this->n_species = 0;
	this->state = 0;
	this->prev = 0;
	this->next = 0;
}

State::State(State* astate)
{
	this->sn = astate->sn;
	this->n_species = astate->n_species;
	this->state = new STATETYPE[this->n_species];
	for (unsigned int i = 0; i < this->n_species; i++)
	{
		this->state[i] = astate->state[i];
	}
	this->prev = astate->prev;
	this->next = astate->next;
}

State::State(unsigned int n_species)
{
	this->sn = 0;
	this->n_species = n_species;
	state = new STATETYPE[this->n_species];
	for (unsigned int i = 0; i < this->n_species; i++)
	{
		this->state[i] = 0;
	}
	this->prev = 0;
	this->next = 0;
}


State::~State()
{
	delete[] state;
}

int State::CopyfromState(State* astate)
{
	this->sn = astate->sn;
	this->n_species = astate->n_species;
	this->state = new STATETYPE[this->n_species];
	for (unsigned int i = 0; i < this->n_species; i++)
	{
		this->state[i] = astate->state[i];
	}
	this->prev = astate->prev;
	this->next = astate->next;

	return 1;
}

int State::PrintState()
{
	for (unsigned int j=0; j<this->n_species; j++)
        {
		cout << this->getStateValue(j) << " ";
	}
	cout << endl;

	return 1;
}


STATETYPE State::getStateValue(unsigned int i)
{
	return this->state[i];
}


int State::setStateValue(unsigned int i, STATETYPE value)
{
	this->state[i] = value;
	return 0;
}


int State::setState(STATETYPE* astate, unsigned int n_species)
{
	this->sn = 0;
	this->n_species = n_species;
	state = new STATETYPE[this->n_species];
	for (unsigned int i = 0; i < this->n_species; i++)
	{
		this->state[i] = astate[i];
	}
	return 1;
}


State* State::getPrev()
{
	return this->prev;
}

State* State::getNext()
{
	return this->next;
}

int State::setPrev(State* pt)
{
	this->prev = pt;
	return 0;
}

int State::setNext(State* pt)
{
	this->next = pt;
	return 0;
}

int State::setSN (unsigned long index)
{
	this->sn = index;
	return 1;
}

unsigned long State::getSN ()
{
	return sn;
}

int State::initialize ()
{
	this->sn = 0;
	int ns = this->n_species;
	for (int i = 0; i < ns; i ++)
	{
		this->state[i] = 0;
	}
	this->prev = NULL;
	this->next = NULL;

	return 1;
}
	
State& State::operator= (const State &pt)
{
	this->sn = pt.sn;
	this->n_species = pt.n_species;
	for (unsigned int i = 0; i < this->n_species; i++)
	{
		this->state[i] = pt.state[i];
	}
	
	this->prev = pt.prev;
	this->next = pt.next;
	return *this;
}

bool State::operator== (const State &other)
{
	bool equal = true;
	if (this->n_species != other.n_species)
	{
		equal = false;
	}
	
	for (unsigned int i = 0; i < this->n_species; i++)
	{
		if (this->state[i] != other.state[i])
		{
			equal = false;
		}
	}
	
	return equal;
}

/*
void State::delete ()
{
	if (this == NULL) return;
	free (this->sn);
	free (this->n_species);
	free (this->next);
	free (this->state);
}
*/

/* EOF */


