#ifndef STOICHIOMETRY_H_
#define STOICHIOMETRY_H_

using namespace std;

//class Model;
class State;
class ListOf;

class Stoichiometry
{
	unsigned int  n_reactions;
	unsigned int  n_species;
	string*       SpeciesID;
	double*       StoichiometricMatrix;
	//ListOf*       ReactionModifier;
	unsigned int* ReactionModifier;
	int*          ReactionAttribute;	// if reaction is irreversible then its value is positive, else negative.
						// the absolute value is the the serial number of the reaction, 1 based.
	
public:
	Stoichiometry();
	~Stoichiometry();

	double getValue(unsigned int species_i, unsigned int reactions_j);
	unsigned int getModifierValue(unsigned int species_i, unsigned int reactions_j);
	unsigned int getNumSpecies ();
	int setNumSpecies(unsigned int n);
	int getSpeciesIndex(string Id);
	unsigned int getNumReactions ();
	int setNumReactions(unsigned int n);
	double* getStoichiometricMatrix();
	int setStoichiometricMatrix(double* matrix);
	int genStoichiometricMatrix(class Model* m);
	int getReactionAttribute(unsigned int i);
	
	State* reactWithState(unsigned int i_reaction, State* astate);
	int    reactWithState(unsigned int i_reaction, State* astate, State* destState);
	int    updateStateWithReaction (unsigned int i_reaction, State* curstate);
	int    canReact (unsigned int i_reaction, State* curstate);
	
	int printStoichiometricMatrix();
};





#endif /*STOICHIOMETRY_H_*/

