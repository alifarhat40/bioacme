
#include <string>
#include <iostream>
//#include "global.h"
#include "Network.h"
#include "SparseMatrix.h"
#include "MatrixElement.h"
#include <sbml/SBMLTypes.h>
#include "Stoichiometry.h"
#include "State.h"
#include "cmath"


using namespace std;

Network::Network()
{
	d = 0;
	m = 0;
	States = 0;
	stoichiometry = 0;
	TransMatrix = 0;
	matrixEle = 0;
	CompartmentsID = 0;
	SpeciesID = 0;
	RulesID = 0;
	ParametersID = 0;
	ReactionsID = 0;
	LocalParamIDs = 0;
	n_states = 0;
	n_compartments = 0;
	n_species = 0;
	n_reactions = 0;
	n_rules = 0;
	n_parameters = 0;
	n_realStates = 0;
	ReactionAttribute = 0;
	n_assignmentRules = 0;

}

/*
 * The implementations of other functions here
 */
 
int Network::getSpeciesIndex(string Id)
{
	int index = -1;
	unsigned int i = 0;
	
	for (i = 0; i < n_species; i++)
	{
		if (SpeciesID[i] == Id)
		{
			index = i;
			break;
		}
	}
	
	return index;
}

int Network::getCompartmentIndex(string Id)
{
	int index = -1;
	unsigned int i = 0;
	
	for (i = 0; i < n_compartments; i++)
	{
		if (CompartmentsID[i] == Id)
		{
			index = i;
			break;
		}
	}
	
	return index;
}

int Network::getRateRuleIndex(string Id)
{
	int index = -1;
	unsigned int i = 0;
	
	for (i = 0; i < n_assignmentRules; i++)
	{
		if (RulesID[i] == Id)
		{
			index = i;
			break;
		}
	}
	
	return index;
}

string Network::getReactionID(unsigned int sindex)
{
	return ReactionsID[sindex];
}

/*
*/


double Network::getReactionRate(const KineticLaw* klaw, unsigned int sindex, State* curstate)
{
	// for debug
	//if (clpara->verbose)
	//printf (">>>>>getReactionRate: %d\n", sindex);
	
	double rate = 0;
	
	//const ASTNode* p_math = 0; /* The content of p_math cannot be altered, but can be used. */
	
	//p_math = klaw->getMath();
	
	/*
	 * Parse the math formula and compute the reaction rate value.
	 * This function is actully a dispatcher or controller.
	 */
	//cout << "TEST@getReactionRate " << endl;
	if ( stoichiometry->getReactionAttribute(sindex) < 0 )
	{
		rate = -1 * computeASTNodeValue(klaw->getMath(), sindex, curstate);
		//rate = 1;
	}
	else
	{
		rate = computeASTNodeValue(klaw->getMath(), sindex, curstate);
		//rate = 1;
	}
	// for debug
	//if (clpara->verbose)
	//printf ("<<<<<getReactionRate: %f\n\n", rate);
	
	//delete p_math;
	
	//rate = 1;

	return rate;
}


double Network::computeASTNodeValue(const ASTNode* node, unsigned int sindex, State* curstate)
{
	// for debug
	//printf ("computeASTNodeValue: %d\n", sindex);
	
	unsigned int num_children = 0;
	unsigned int nodetype = 0;
	int in = 0;
	
	string nodeName("");

	double nodeValue = 0;
	
	// for Debug
	//cout << "TEST@computeASTNodeValue " << node->getType() << " " << sindex << " " << node->getNumChildren() << endl;
	
	// for debug
	/*cout << (ASTNodeType_t)node->getType() << "\t";
	cout << node->getNumChildren()<< "\t";
	
	if(node->getNumChildren() == 0)
	{
		if(node->isName())
		{
			cout << node->getName() << endl;
		}
		else
		{
			cout << endl;
		}
	}
	else
	{
		cout << endl; 
	}*/
	////
	
	
	
	num_children = node->getNumChildren();
	
	if (num_children == 0)
	{
		//cout << "    No children." << endl;
		
		nodetype = node->getType();
		switch(nodetype)
		{
			case AST_NAME:  // 260
				/*  */
				nodeValue = getValueOfParameter (node->getName(), sindex, curstate);
				//if (nodeValue)
				// for debug
				//cout << "       AST_NAME: " << node->getName() << " = " << nodeValue << endl;
				////
				//nodeValue = 1;
				return nodeValue;
				break;
			case AST_INTEGER:
				in = node->getInteger();
				// debug
				//printf ("\n%d\n", in);
				////
				return in;
				break;
			case AST_REAL:
				nodeValue = node->getReal();
				return nodeValue;
				break;
			case AST_CONSTANT_E:
				nodeValue = E;
				return nodeValue;
				break; 	
			case AST_CONSTANT_FALSE:
				nodeValue = 0;
				return nodeValue;
				break;
			case AST_CONSTANT_PI:
				nodeValue = PI;
				return nodeValue;
				break;
			case AST_CONSTANT_TRUE:
				nodeValue = node->getReal();
				return nodeValue;
				break;
			case AST_LAMBDA:
				nodeValue = 0;
				return nodeValue;
				break;
			default:
				nodeValue = 0;
				return nodeValue;
				break;
		}
	}
	else if (num_children != 0)
	{
		nodetype = node->getType();
		
		switch(nodetype)
		{
			case AST_PLUS:
				nodeValue = computeASTNodeValue (node->getLeftChild(), sindex, curstate) + computeASTNodeValue (node->getRightChild(), sindex, curstate);
				return nodeValue;
				break;
			case AST_MINUS:
				nodeValue = computeASTNodeValue (node->getLeftChild(), sindex, curstate) - computeASTNodeValue (node->getRightChild(), sindex, curstate);
				return nodeValue;
				break;
			case AST_TIMES:
				nodeValue = 1.0;
				for (unsigned int i = 0; i < num_children; i++)
				{
					//double v = computeASTNodeValue (node->getChild(i), sindex, curstate);
					//printf ("%d - child %d = %f  %d\n", num_children, i, v, node->getType());
					nodeValue = nodeValue * computeASTNodeValue (node->getChild(i), sindex, curstate);
				}
				//double leftvalue = computeASTNodeValue (node->getLeftChild(), sindex, curstate);
				//double rightvalue = computeASTNodeValue (node->getRightChild(), sindex, curstate);
				/*
				if (leftvalue == 0 && rightvalue == 0)
				{
					return 0;
				}
				else if (leftvalue == 0)
				{
					leftvalue = 1;
				}
				else if (rightvalue == 0)
				{
					rightvalue = 1;
				}*/
				//nodeValue = leftvalue * rightvalue;
				return nodeValue;
				break;
			case AST_DIVIDE:
				nodeValue = computeASTNodeValue (node->getLeftChild(), sindex, curstate) / computeASTNodeValue (node->getRightChild(), sindex, curstate);
				// for debug
				//printf ("\n%f\n", nodeValue);
				////
				return nodeValue;
				break;
			case AST_POWER:
				// ATTENTION:
				// The handling of mark <power/> in SBML is in AST_FUNCTION_POWER, instead of here. 
				//nodeValue = pow(computeASTNodeValue (node->getLeftChild(), sindex, curstate), computeASTNodeValue (node->getRightChild(), sindex, curstate));
				// for debug
				//printf ("\n%f\n", nodeValue);
				////
				//return nodeValue;
				break;
			case AST_FUNCTION:
			case AST_FUNCTION_ABS:
				nodeValue = abs(computeASTNodeValue (node->getChild(0), sindex, curstate));
				return nodeValue;
				break;
			case AST_FUNCTION_ARCCOS:
			case AST_FUNCTION_ARCCOSH:
				nodeValue = acos(computeASTNodeValue (node->getChild(0), sindex, curstate));
				return nodeValue;
				break;
			case AST_FUNCTION_ARCCOT:
			case AST_FUNCTION_ARCCOTH:
			case AST_FUNCTION_ARCCSC:
			case AST_FUNCTION_ARCCSCH:
			case AST_FUNCTION_ARCSEC:
			case AST_FUNCTION_ARCSECH:
				break;
			case AST_FUNCTION_ARCSIN:
			case AST_FUNCTION_ARCSINH:
				nodeValue = asin(computeASTNodeValue (node->getChild(0), sindex, curstate));
				return nodeValue;
				break;
			case AST_FUNCTION_ARCTAN:
			case AST_FUNCTION_ARCTANH:
				nodeValue = atan(computeASTNodeValue (node->getChild(0), sindex, curstate));
				return nodeValue;
				break;
			case AST_FUNCTION_CEILING:
				nodeValue = ceil (computeASTNodeValue (node->getChild(0), sindex, curstate));
				return nodeValue;
				break;
			case AST_FUNCTION_COS:
			case AST_FUNCTION_COSH:
				nodeValue = cos (computeASTNodeValue (node->getChild(0), sindex, curstate));
				return nodeValue;
				break;
			case AST_FUNCTION_COT:
			case AST_FUNCTION_COTH:
			case AST_FUNCTION_CSC:
			case AST_FUNCTION_CSCH:
			case AST_FUNCTION_DELAY:
				break;
			case AST_FUNCTION_EXP:
				nodeValue = exp (computeASTNodeValue (node->getChild(0), sindex, curstate));
				return nodeValue;
				break;
			case AST_FUNCTION_FACTORIAL:
				break;
			case AST_FUNCTION_FLOOR:
				nodeValue = floor (computeASTNodeValue (node->getChild(0), sindex, curstate));
				return nodeValue;
				break;
			case AST_FUNCTION_LN:
				nodeValue = log (computeASTNodeValue (node->getChild(0), sindex, curstate));
				return nodeValue;
				break;
			case AST_FUNCTION_LOG:
				nodeValue = log10 (computeASTNodeValue (node->getChild(0), sindex, curstate));
				return nodeValue;
				break;
			case AST_FUNCTION_PIECEWISE:
			case AST_FUNCTION_POWER:
				nodeValue = pow(computeASTNodeValue (node->getLeftChild(), sindex, curstate), computeASTNodeValue (node->getRightChild(), sindex, curstate));
				// for debug
				//printf ("\n%f\n", nodeValue);
				////
				return nodeValue;
				break;
			case AST_FUNCTION_ROOT:
				nodeValue = sqrt (computeASTNodeValue (node->getChild(0), sindex, curstate));
				return nodeValue;
				break;
			case AST_FUNCTION_SEC:
			case AST_FUNCTION_SECH:
				break;
			case AST_FUNCTION_SIN:
			case AST_FUNCTION_SINH:
				nodeValue = cos (computeASTNodeValue (node->getChild(0), sindex, curstate));
				return nodeValue;
				break;
			case AST_FUNCTION_TAN:
			case AST_FUNCTION_TANH:
				nodeValue = tan (computeASTNodeValue (node->getChild(0), sindex, curstate));
				return nodeValue;
				break;
			default:
				return 0;
				break;
		}
		
	}
	
	return 0;
}

double Network::getValueOfParameter(const char* parameter_id, unsigned int rindex, State* curstate)
{
	//cout << "        - Get the value. " << endl;
	int speciesIndex = 0;
	int compartmentIndex = 0;
	int localparamIndex = -1;
	unsigned int n_localparam = 0, i = 0;
	//string* LocalParamIDs = 0;
	KineticLaw* kl = 0;
	
	string Id = parameter_id;
	//debug
	//cout << Id << " " << parameter_id << endl;
	
	/* 
	 * Read all of local parameters of current reaction.
	 */
	//kl = m->getReaction( abs(stoichiometry->getReactionAttribute(sindex)) )->getKineticLaw();
	kl = m->getReaction( getReactionID(rindex) )->getKineticLaw();
	n_localparam = kl->getNumParameters();
	//LocalParamIDs = new string[n_localparam];
	for (i = 0; i < n_localparam; i++)
	{
		LocalParamIDs[i] = kl->getParameter(i)->getId();
		// debug
		//cout << LocalParamIDs[i] << endl;
	}
	
	/*
	 * Whether parameter_id is in LocalParamIDs.
	 */
	// debug
	//cout << "n: " << n_localparam << endl;
	//cout << "l: " << kl->getFormula() << endl;
	//cout << "ID: " << parameter_id << endl;
	for (i = 0; i < n_localparam; i++)
	{
		//cout << "    " << LocalParamIDs[i] << endl;
		
		if (LocalParamIDs[i] == Id)
		{
			// debug
			//cout << LocalParamIDs[i] << "~~" << Id << endl;
			
			localparamIndex = i;
			break;
		}
	}
	// debug
	//cout << "index: " << localparamIndex << endl;
	
	
	
	/* Find in species ID list.
	 * If found, then this parameter is a species; then it's a parameter value.
	 */
	speciesIndex = getSpeciesIndex(Id);
	compartmentIndex = getCompartmentIndex(Id);
	
	// for debug
	//cout << "         " << Id << ": " << speciesIndex << endl;
	if (speciesIndex >= 0)
	{
		if ( stoichiometry->getValue(speciesIndex, rindex) <= 0 )	// for reactants and modifiers
		{
			return curstate->getStateValue(speciesIndex);
		}
		else if ( stoichiometry->getValue(speciesIndex, rindex) > 0 )	// for products
		{
			//return 0;  // Haaha! A huge bug, I find you finally!
						 // For some reactions, they need copy numbers of some products
						 // in the kinetic laws, but this bug ignored such situations.
			
			// CORRECT SOLUTION SO FAR
			if (stoichiometry->getModifierValue(speciesIndex, rindex) > 0 )
			{
				// debug
				//printf ("\nModifier: %d\n", stoichiometry->getModifierValue(speciesIndex, rindex));
				////
				return curstate->getStateValue(speciesIndex);
			}
			else
			{
				return 0;
			}
			// MAYBE STILL NEED TO BE IMPROVED.
		}
		/*
		cout << "%%%%%%%%DEBUG: " << speciesIndex << " " << n_realStates << endl;
		for (unsigned int i=0; i<n_species; i++)
		{
			cout << States[i * n_realStates + stateindex] << " ";
		}
		cout << endl;
		*/
		//return unsigned int v = States[speciesIndex * n_realStates + sindex];
		///double v = stoichiometry->getValue(speciesIndex, sindex);
		// for debug
		//printf ("--------getValueOfParameter: %s. %d,%d:%f\n", parameter_id, speciesIndex, sindex, v);
		///if (v < 0) /* for reactants */
		///{
		///	if ( stoichiometry->getReactionAttribute(sindex) > 0 ) /* for forward reactions */
		///	{
				// for debug
				//printf ("ReactionAttribute[%d] = %d\n", sindex, stoichiometry->getReactionAttribute(sindex));
		///		return -1 * v;
		///	}
		///	else if ( stoichiometry->getReactionAttribute(sindex) < 0 ) /* for reverse reactions */
		///	{
				// for debug
				//printf ("ReactionAttribute[%d] = %d\n", sindex, stoichiometry->getReactionAttribute(sindex));
		///		return v;
		///	}
		///}
		///else /* for products */
		///{
			// for debug
			//printf ("&&aaa\n");
		///	return 0;
		///}
		//cout << v << endl;
	}
	else if (compartmentIndex >= 0)
	{
		return m->getCompartment(compartmentIndex)->getSize();
	}
	else if (m->getParameter(Id) != NULL)
	{
		// debug
		//cout << "parameters here!" << endl;
		
		if (m->getParameter(Id)->getConstant())
		{
			return m->getParameter(Id)->getValue();
		}
		else
		{
			return computeASTNodeValue( ((RateRule*)m->getRule(getRateRuleIndex(Id)))->getMath(), rindex, curstate );
		}
	}
	else if (localparamIndex >= 0)
	{
		// debug
		//cout << localparamIndex << endl;
		//cout << kl->getParameter(localparamIndex)->getValue() << endl;
		
		return kl->getParameter(localparamIndex)->getValue();
	}
	else
	{
		printf ("ERROR: can not get value of parameter: %s\n", parameter_id);
		return 0;
	}
	
	//if (LocalParamIDs != 0)
	//{
	//	delete LocalParamIDs;
	//}
	
	//delete[] LocalParamIDs;
	delete   kl;

	return 0;
}





/*
 * Generate Stoichiometrix Matrix
 */
// This function has been moved to class Stoichiometry.

