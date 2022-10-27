#ifndef NETWORK_H_
#define NETWORK_H_

/* CHOOSE ONE LENGTH SETUP */
/* global.h */
#include "StateType.h"

#include <string>

using namespace std;


#define E 2.7182818
#define PI 3.1415926535897932

class SparseMatrix;
class MatrixElement;
class CLParameters;
class KineticLaw;
class ASTNode;
class SBMLDocument;
class Model;
class Stoichiometry;
class State;


class Network
{
public:
	SBMLDocument*        d;
	Model*               m;
	STATETYPE*        States;
	Stoichiometry*       stoichiometry;
	SparseMatrix*        TransMatrix;
	MatrixElement*       matrixEle;
	string*		     CompartmentsID;
	string*              SpeciesID;
	string*              RulesID;
	string*		     ParametersID;
	string*              ReactionsID;
	string* 	     LocalParamIDs;
	unsigned long        n_states;
	unsigned int         n_compartments;
	unsigned int         n_species;
	unsigned int         n_reactions;
	unsigned int         n_rules;
	unsigned int	     n_parameters;
	unsigned int         n_realStates;
	int*                 ReactionAttribute;
	unsigned int         n_assignmentRules;

	// Constructors here.
	Network();


	/*
	 * Define a useful function here
	 */
	int getSpeciesIndex(string Id);

	int getCompartmentIndex(string Id);

	int getRateRuleIndex(string Id);

	string getReactionID(unsigned int sindex);

	//int validateTransMatrix(MatrixElement* TMatrix, unsigned int n_states);

	//int writeStateSpace();

	//int writeTransMatrix();

	/*
	 * reaction index: sindex - stoichiometry matrix index
	 */
	double getReactionRate(const KineticLaw* klaw, unsigned int sindex, State* curstate);

	double computeASTNodeValue(const ASTNode* node, unsigned int sindex, State* curstate);

	double getValueOfParameter(const char* parameter_id, unsigned int rindex, State* curstate);

	//int genStoichiometricMatrix();
	

};

#endif /*NETWORK_H_*/

