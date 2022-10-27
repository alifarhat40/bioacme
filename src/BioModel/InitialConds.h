#ifndef INITIALCONDS_H_
#define INITIALCONDS_H_

using namespace std;

class State;

class InitialConds
{
	unsigned int  num_initialstates;
	State*        initialstates; /* array of initial states */
	double*       pdv;
	
public:
	InitialConds();
	~InitialConds();
	
	int readInitialConds (char* pdvfile);
	unsigned int getNumInitialstates();
	State* getInitialState(unsigned int i) const;
	double getInitialPdv(unsigned int i);
};

#endif /*INITIALCONDS_H_*/
