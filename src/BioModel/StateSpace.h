#ifndef STATESPACE_H_
#define STATESPACE_H_

/* CHOOSE ONE LENGTH SETUP */
/* State.h, StateSpace.h */
#include "StateType.h"

using namespace std;

class State;
class HashTable;

class StateSpace
{
	unsigned int  n_species;
	unsigned long  n_states;
	string*       SpeciesID;
	HashTable*	  ssHashTable;
	STATETYPE*    States;
	//State*        States;
	
public:
	StateSpace();
	~StateSpace();
	
	int readStateSpace(char* ssfilename);
	int writeStateSpace(char* ssfilename);
	unsigned int getValue(unsigned int i, unsigned int j);
	State* getState(unsigned int i);
	HashTable* getHashTable ();
	unsigned int getNumSpecies ();
	int setNumSpecies(unsigned int n);
	unsigned long getNumStates ();
	int setNumStates(unsigned long n);
	string* getSpeciesID();
	int setSpeciesID(string* speciesID, unsigned int n_species);
	const char*   getSpeciesIDValue(unsigned int i);
	STATETYPE* getStates();
	int setStates(STATETYPE* states);
	int findState(State* astate);
};





#endif /*STATESPACE_H_*/

