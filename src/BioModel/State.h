#ifndef STATE_H_
#define STATE_H_

/* CHOOSE ONE LENGTH SETUP */
/* State.h, StateSpace.h */
#include "StateType.h"

//using namespace std;


class State
{
public:
	unsigned long sn; // serial number in state space, 0-based.
	unsigned int n_species;
	STATETYPE* state;
private:
	State*        prev;
	State*        next;
	
public:
	State();
	State(State* astate);
	State(unsigned int n_species);
	~State();
	
	int CopyfromState(State* astate);
	int PrintState();
	STATETYPE getStateValue(unsigned int i); // zero-based
	int setStateValue(unsigned int i, STATETYPE value);
	int setState(STATETYPE* astate, unsigned int n_species);
	State* getPrev();
	State* getNext();
	int setPrev(State* pt);
	int setNext(State* pt);
	int setSN (unsigned long index);
	unsigned long getSN ();
	int initialize ();
	
	State& operator= (const State &pt);
	bool operator== (const State &other);

	//void delete();
};



#endif /*STATE_H_*/
