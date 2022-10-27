
#include <stdio.h>
#include <iostream>
#include <string>
#include "InitialConds.h"
#include "State.h"

using namespace std;


InitialConds::InitialConds()
{
	num_initialstates = 0;
	initialstates = 0; /* array of initial states */
	pdv = 0;
}

InitialConds::~InitialConds()
{
}


int InitialConds::readInitialConds (char* pdvfile)
{
	unsigned int  n_species = 0;
	unsigned int  n_nonzerop = 0;
	char  ch = 0;
	unsigned int  d = 0;
	STATETYPE* astate = 0;
	unsigned int  i, j;

	float        f = 0;
	//int* nonzero_i = 0;
	double*       nonzero_p = 0;

	FILE* fp = 0;
		
		
	// for debug
	//printf (">>>>>>>>>>>>test<<<<<<<<<<<<\n");
		
		
	fp = fopen (pdvfile, "r");
	if (!fp)
	{
		printf ("cannot open file: %s\n", pdvfile);
		return -1;
	}
	
	if (fscanf (fp, "%d %d\n", &n_species, &n_nonzerop) < 2)
	{
		return -1;
	}
	//nonzero_i = new int(n_nonzerop);
	nonzero_p = new double[n_nonzerop];
	
	/* Initialization... */
	num_initialstates = n_nonzerop;
	initialstates = new State[num_initialstates];
	pdv = new double[num_initialstates];
	
	astate = new STATETYPE[n_species];

	// for debug
	//printf ("%d %d\n", n_species, n_nonzerop);
		
	for (i = 0; i < n_nonzerop; i++)
	{
		for (j = 0; j < n_species; j++)
		{
			fscanf (fp, "%d", &d);
			astate[j] = d;
			fscanf (fp, "%c", &ch);
			
			// for debug
			//printf (">>>>>--%d-----%c----\n", d, ch);
		}
		fscanf (fp, "%c", &ch);
		fscanf (fp, "%f", &f);
		nonzero_p[i] = (double)f;
		if (!feof(fp))
		{
			fscanf (fp, "%c", &ch);
		}
		
		initialstates[i].setState(astate, n_species);
		pdv[i] = nonzero_p[i];
	}
		
	return 1;
	
}



unsigned int InitialConds::getNumInitialstates()
{
	return this->num_initialstates;
}

State* InitialConds::getInitialState(unsigned int i) const
{
	return &initialstates[i];
}

double InitialConds::getInitialPdv(unsigned int i)
{
	return this->pdv[i];
}



/* EOF */


