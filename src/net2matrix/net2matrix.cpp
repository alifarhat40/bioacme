/**
 * \file    net2matrix.cpp
 * \brief   compute a transition matrix from a biological network model.
 * \author
 *
 * 		Youfang Cao
 * 		Shanghai Center for Systems Biomedicine
 * 		Shanghai Jiao Tong University
 * 		Shanghai, China
 * 
 * 		mailto: yfcao@sjtu.edu.cn
 */


#include <sbml/SBMLTypes.h>
#include "../BioModel/SparseMatrix.h"
#include "../BioModel/MatrixElement.h"
#include "../BioModel/Network.h"
#include "../BioModel/Stoichiometry.h"
#include "../BioModel/StateSpace.h"
#include "../BioModel/State.h"
#include "../BioModel/Stack.h"
#include "../BioModel/HashTable.h"
#include "anyoption.h"
#include <iostream>
#include <string>
#include <cmath>
#include <vector>

#define  MAX_SCALE 8e6
#define  PRECISION 10


using namespace std;


/*
 * The main function.
 */

int main(int argc, char* argv[])
{
	/* Variable definitions */
	char*		sbmlfile = 0;        	// SBML filename.
	char*		statespacefile = 0;     // state space filename.
	char*		tmfile = 0;        	// output transition matrix filename.
	bool            verbose = false;
	bool            rlabel = false;
	int		s_reaction = -1;

	Network              nw;  // Store everything of the network here.
	StateSpace*          StatesC = 0;

	MatrixElement*       p_cur = 0;
	string               cl_opt("");

	string               modelid("");
	unsigned int         errors = 0;
	unsigned int         i, j, k, l, n;
	Compartment*		 cur_compartment = 0;
	Species*             cur_species = 0;
	Parameter*			 cur_parameter = 0;
	Reaction*            cur_reaction = 0;
	AssignmentRule*      cur_rule = 0;
	

        /* CREATE AN OBJECT TO HANDLE COMMAND LINE */
        AnyOption *opt = new AnyOption();

        /* SET THE USAGE/HELP   */
        opt->addUsage( "" );
        opt->addUsage( "Usage: " );
        opt->addUsage( "" );
        opt->addUsage( " -h  --help  			Prints this help " );
        opt->addUsage( " -m  --sbml <sbmlfile>		Input SBML file" );
        opt->addUsage( " -s  --statespace <ssfile>	State space file" );
        opt->addUsage( " -t  --omatrix <tmfile> 	Output transition matrix file" );
        opt->addUsage( " -r  --rlabel			Put reaction labels aside the matrix entries" );
        opt->addUsage( " -x  --reaction	<n>		About which single reaction you want the matrix to be" );
        opt->addUsage( " -v  --verbose 0  		Switch for more output " );
        opt->addUsage( "" );

        /* SET THE OPTION STRINGS/CHARACTERS */
        opt->setFlag(  "help", 'h' );
        opt->setOption(  "sbml", 'm' );
        opt->setOption(  "statespace", 's' );
        opt->setOption(  "tmatrix", 't' );
        opt->setOption(  "reaction", 'x' );
        opt->setFlag(  "rlabel", 'r' );
        opt->setFlag(  "verbose", 'v' );

	/* go through the command line and get the options  */
        opt->processCommandArgs( argc, argv );

        /* GET THE VALUES */
        if( opt->getFlag( "help" ) || opt->getFlag( 'h' ) ) 
	{
                opt->printUsage();
		delete opt;
		return 1;
	}
        if( opt->getFlag( "verbose" ) || opt->getFlag( 'v' ) ) 
	{
		verbose = 1;
	}
	else
	{
		verbose = 0;
	}
        if( opt->getFlag( "rlabel" ) || opt->getFlag( 'r' ) ) 
	{
		rlabel = 1;
	}
	else
	{
		rlabel = 0;
	}
	if( opt->getValue( 'm' ) != NULL  || opt->getValue( "sbml" ) != NULL  )
		sbmlfile = opt->getValue( 'm' );
	else
	{
		printf ("Please specify the SBML filename.\n\n");
		opt->printUsage();
		return 1;
	}
	if( opt->getValue( 's' ) != NULL  || opt->getValue( "statespace" ) != NULL  )
		statespacefile = opt->getValue( 's' );
	else
	{
		printf ("Please specify the state space filename.\n\n");
		opt->printUsage();
		return 1;
	}
	if( opt->getValue( 't' ) != NULL  || opt->getValue( "tmatrix" ) != NULL  )
		tmfile = opt->getValue( 't' );
	else
	{
		printf ("Please specify the output transition matrix filename.\n\n");
		opt->printUsage();
		return 1;
	}
	if( opt->getValue( 'x' ) != NULL  || opt->getValue( "reaction" ) != NULL  )
		s_reaction = atoi(opt->getValue( 'x' ));
	//cout << endl;


	/*
	 * Get pointer of the model.
	 */
	nw.d = readSBML(sbmlfile);
	nw.m = nw.d->getModel();

	/*
	 * Get some basic information of the model.
	 */
	errors = nw.d->getNumErrors();
	
	modelid = nw.m->getId();
	nw.n_compartments = nw.m->getNumCompartments();
	nw.n_species = nw.m->getNumSpecies();
	nw.n_rules = nw.m->getNumRules();
	nw.n_reactions = nw.m->getNumReactions();
	
	/* Count in reversible reactions. */
	int num_reversibleReaction = 0;
	for (i = 0; i < nw.n_reactions; i++)
	{
		if (nw.m->getReaction(i)->getReversible())
			num_reversibleReaction ++;
	}
	nw.n_reactions += num_reversibleReaction;
	
	/* record all reaction IDs */
	nw.ReactionsID = new string[nw.n_reactions];
	n = nw.m->getNumReactions();
	j = 0;
	for (i = 0; i < n; i++)
	{
		nw.ReactionsID[j] = nw.m->getReaction(i)->getId();
		j++;
		if (nw.m->getReaction(i)->getReversible())
		{
			nw.ReactionsID[j] = nw.m->getReaction(i)->getId();
			j++;
		}
	}


	if (verbose)
	{
		printf ("\n");
		printf ("        filename: %s\n", sbmlfile);
		printf ("         modelID: %s\n", modelid.data());
		printf ("        error(s): %d\n", errors);
		printf ("  compartment(s): %d\n", nw.n_compartments);
		printf ("      species(s): %d\n", nw.n_species);
		printf ("     reaction(s): %d\n", nw.n_reactions);
		printf ("         rule(s): %d\n", nw.n_rules);

		if (errors > 0)
		{
			//d->printWarnings(cout); // this function is removed for libSBML-3.0.0
			nw.d->printErrors  (cout);
			//d->printFatals  (cout); // this function is removed for libSBML-3.0.0
		}
	
		printf ("\n");
	}
	
	
	/*
	 * read all compartments, species and reactions
	 */
	
	/*
	 * store compartment IDs in CompartmentsID array.
	 * Initialize the array first here.
	 */
	nw.CompartmentsID = new string[nw.n_compartments];
	for (i = 0; i < nw.n_compartments; i++)
	{
		cur_compartment = nw.m->getCompartment(i);
		nw.CompartmentsID[i] = cur_compartment->getId();
	}

	
	/*
	 * store species Ids in SpeciesID array.
	 * Initialize the array here.
	 */
	nw.SpeciesID = new string[nw.n_species];
	
	if (verbose)
	{
		printf ("Species:\n");
		printf ("id\tinitAmt\tinitCon\tconst\tboundcond\n");
	}
	for(i = 0; i < nw.n_species; i++)
	{
		cur_species = nw.m->getSpecies(i);
		
		/* Store id data in the array. */
		nw.SpeciesID[i] = cur_species->getId();
		
		if (verbose)
		{
			printf ("%s\t", cur_species->getId().data());
			printf ("%f\t", cur_species->getInitialAmount());
			printf ("%f\t", cur_species->getInitialConcentration());
			printf ("%d\t", cur_species->getConstant());
			printf ("%d\n", cur_species->getBoundaryCondition());
		}
	}
	
	if (verbose)
	{
		printf ("Reactions:\n");
		printf ("id\tname\tnReact\tnProt\tnMod\tReversible\tformula\n");
	
		for(j = 0; j < nw.m->getNumReactions(); j++)
		{
			cur_reaction = nw.m->getReaction(j);
			printf ("%s\t", cur_reaction->getId().data());
			printf ("%s\t", cur_reaction->getName().data());
			printf ("%d\t", cur_reaction->getNumReactants());
			printf ("%d\t", cur_reaction->getNumProducts());
			printf ("%d\t", cur_reaction->getNumModifiers());
			printf ("%d\t", cur_reaction->getReversible());
			KineticLaw* kl = cur_reaction->getKineticLaw();
			printf ("%s\n", kl->getFormula().data());
		}
	}

	/*
	 * Get rules ID list.
	 */
	if(verbose)
	{
		printf ("Rules:\n");
	}
	nw.RulesID = new string[nw.n_rules];
	//cout << "Get Rate Rule IDs. " << endl;
	nw.n_assignmentRules = 0;
	for(i = 0; i < nw.n_rules; i++)
	{
		if (SBase_getTypeCode((SBase_t*)nw.m->getRule(i)) != SBML_ASSIGNMENT_RULE)
			continue;
			
		cur_rule = (AssignmentRule*)nw.m->getRule(i);
		
		/* Store id data in the array. */
		nw.RulesID[i] = cur_rule->getVariable();
		
		if (verbose)
		{
			printf ("%s\t", nw.RulesID[i].data());
			printf ("%s\n", cur_rule->getFormula().data());
		}
		
		nw.n_assignmentRules ++;
	}
	

	/*
	 * Get parameters ID list.
	 * 
	 */
	if(verbose)
	{
		printf ("Global Parameters:\n");
	}
	nw.n_parameters = nw.m->getNumParameters();
	nw.ParametersID = new string[nw.n_parameters];
	for (i = 0; i < nw.n_parameters; i++)
	{
		cur_parameter = nw.m->getParameter(i);
		nw.ParametersID[i] = cur_parameter->getId();
		if (verbose)
		{
			printf ("%s\t%f\n", nw.ParametersID[i].data(), cur_parameter->getValue());
		}
	}
	
	// Initialize LocalParamIDs
	nw.LocalParamIDs = new string[20];

	/*
	 * 
	 * The main part:
	 * compute Transition Matrix
	 * step 1: build the stoichiometric matrix, and the reaction rate c_mju array;
	 * step 2: build the state space, and initialize it;
	 * step 3: enter the loop which compute transition
	 *         matrix item value one by one.
	 * 
	 */




	/*
	 * step 1.
	 * build stoichiometic matrix according to reactions in the model.
	 * and, acquire c_mju array from the model.
	 */
	nw.stoichiometry = new Stoichiometry();
	int f = nw.stoichiometry->genStoichiometricMatrix(nw.m);
	if (f == -1)
	{
		printf ("Exit caused by error.\n");
		return -1;
	}
	
	if (verbose)
		nw.stoichiometry->printStoichiometricMatrix();
	
	
	/*
	 * step 2.
	 * The construction of state space,
	 * there is a critical problem here that
	 * that how to set the upper bound of every species.
	 * Note that the scale of the state space is
	 * [bound]^[2*n_species] that is very large.
	 * e.g. if bound is 6, n_species is 10, then
	 * we will have a 60466176*60466176 transition matrix.
	 * Oooh, terrible!!
	 * 
	 */
	
	/*
	 * OK! Now, this problem is solved by reading state space from a file.
	 * The state space is calculated with an independent program: buildStateSpace
	 */
	StatesC = new StateSpace();
	StatesC->readStateSpace(statespacefile);
	nw.n_states = StatesC->getNumStates();
	nw.States = StatesC->getStates();


	if (s_reaction >= (int)nw.n_reactions)
	{
		cout << "There are totally " << nw.n_reactions << " reactions in this system," << endl;
		cout << "please limit your -x option in range [0," << nw.n_reactions-1 << "]." << endl;
		return 0;
	}

	/*
	 * Statistics of hash table.
	 */
	if (verbose)
	{
		unsigned long n_buckets;
		unsigned long bucketsize;
		HashTable*    pHT = 0;
		
		pHT = StatesC->getHashTable();
		n_buckets = pHT->getNumBuckets();
		
		// VARIABLES NEEDED FOR EVALUATING HASH TABLE
		unsigned long BucketsDistribution[BUCKETSIZEDIST];
		unsigned long maxBucketSize  = 0;
		for (i = 0; i < BUCKETSIZEDIST; i++)
		{
			BucketsDistribution[i] = 0;
		}
			
		for (i = 0; i < n_buckets; i++)
		{
			bucketsize = pHT->getSizeofBucket(i);
			if (bucketsize == 0)
			{
				BucketsDistribution[0] ++;
				continue;
			}
			else
			{
				if (bucketsize < BUCKETSIZEDIST) // for hash evaluation
				{
					BucketsDistribution[bucketsize] ++;
					if (bucketsize > maxBucketSize) { maxBucketSize = bucketsize; }
				}
			}
		}
			
		if (verbose)
		{
			cout << "EVALUATION OF HASH TABLE:" << endl;
			cout << "MAXIMUM BUCKET SIZE: " << maxBucketSize << endl;
			cout << "BUCSIZE\tNUM" << endl;
			for (i = 0; i < BUCKETSIZEDIST; i++)
			{
				cout << i << "\t" << BucketsDistribution[i] << endl;
			}
		}
	}
	//
	
	/*
	 * ATTENTION: 
	 * IF n_states IS GREATER THAN MAX_SCALE, THEN EXIT!!!
	 * THE SCALE IS TOO LARGE TO BE HANDLED!
	 */
	
	/* WE SHOULD REDEFINE THIS SCALE, HOW LARGE IS THE UPPER LIMIT THAT WE ARE ABLE TO DO?
	if (n_states > MAX_SCALE)
	{
		printf ("The scale of this problem is too large to be handled.\n");
		return -1;
	}*/
	
	


	/*
	 * 
	 * step 3.
	 * Now, we have the state space.
	 * Next step is to build the transition matrix
	 * from the state space.
	 * 
	 * look, the loop is below...
	 * 
	 */
	
	/*
	 * allocating memory space first for the transition matrix.
	 */
	
	/* How many states I should consider in this problem?
	 * With this variable, I can choose the real scale feasibly.
	 */
	nw.n_realStates = nw.n_states;
	nw.TransMatrix = new SparseMatrix(nw.n_reactions);
	nw.TransMatrix->SetNumStates (nw.n_realStates);

	//double percentage = 0;
	bool canReact = true;
	bool canReach = true;
	int  reactFlag = 0;
	
	Reaction*   re = 0;
	KineticLaw* klaw = 0;
	State* sourceState = new State(nw.n_species);
	State* destState = new State(nw.n_species);
	//State* realDestState = new State(n_species);
	State* new_state = 0;
	State** states_array;
	states_array  = new State*[nw.n_reactions];
	unsigned int num_newstates = 0;
	for (unsigned int i = 0; i < nw.n_reactions; i++)
	{
		states_array[i] = new State(nw.n_species);
	}

	
	/*
	 * and, the loop.
	 */
	
	/*
	 * This is important, if the scale of state space need to be changed,
	 * I can just change this variable.
	 */
	
	/*if (progress)
	{
		printf ("Progress:\n");
	}*/

	// Improve the performance of net2matrix.
	vector<unsigned int> r;
	int    ind = 0;
	int    idx = 0;
	double value = 0;
	double diagonal_value = 0;
	int    isGenerated = 0;
	for (j = 0; j < nw.n_realStates; j++)
	{
		// progress...
		//if (progress)
		//{
			//cout << j+1 << endl;
		//	percentage = j*100/nw.n_realStates;
		//	printf ("%d - %.2f%%\r", j+1, percentage);
			//cout << j+1 << " - " << percentage << endl;
		//}

		// Get source state first
		for (l = 0; l < nw.n_species; l++)
		{
			sourceState->setStateValue(l, nw.States[j * nw.n_species + l]);
		}

		// Generate all possible states from state_i.
		// And put them into an array.
		num_newstates = 0;
		if ( s_reaction >= 0 && s_reaction < (int)nw.n_reactions )
		{
			ind = nw.stoichiometry->reactWithState (s_reaction, sourceState, states_array[num_newstates]);
			if (ind == 1) // A NEW STATE IS GENERATED.
			{
				num_newstates ++;
			}
		}
		else
		{
			for (k = 0; k < nw.n_reactions; k++)
			{
				ind = nw.stoichiometry->reactWithState (k, sourceState, states_array[num_newstates]);
				if (ind == 1) // A NEW STATE IS GENERATED.
				{
					// Make sure new_state is not ever generated from sourceState by other reactions.
					isGenerated = 0;
					for (l=0; l<num_newstates; l++)
					{
						if (*(states_array[l]) == *(states_array[num_newstates]))
						{
							isGenerated = 1;
							break;
						}
					}
					if (isGenerated == 0) 
					{
						num_newstates ++;
					}
				}
			}
		}

		// If stack is not empty, do...
		diagonal_value = 0;
		for (unsigned int s = 0; s < num_newstates; s++)
		{
			new_state = states_array[s];
			idx = StatesC->findState(new_state);
			if (idx < 0)
			{
				continue;
			}
			else
			{
				i = idx;
			}
			value = 0;
			if (i >= 0 && i != j)
			{
				r.clear();
				if ( s_reaction >= 0 && s_reaction < (int)nw.n_reactions )
				{
					reactFlag = nw.stoichiometry->reactWithState(s_reaction, sourceState, destState);
					if (reactFlag != 1) canReact = false;
					else
					{
						canReact = true;
						if (*new_state == *destState)
						{
							canReach = true;
						}
						else
						{
							canReach = false;
						}
					}
					if (canReact && canReach)
					{
						re = nw.m->getReaction(abs(nw.stoichiometry->getReactionAttribute(s_reaction)) - 1);
						klaw = re->getKineticLaw();
						value += nw.getReactionRate(klaw, s_reaction, sourceState );
						r.push_back(s_reaction);
					}
				}
				else
				{
					for (k = 0; k < nw.n_reactions; k++)
					{
						reactFlag = nw.stoichiometry->reactWithState(k, sourceState, destState);
						if (reactFlag != 1) canReact = false;
						else
						{
							canReact = true;
							if (*new_state == *destState)
							{
								canReach = true;
							}
							else
							{
								canReach = false;
							}
						}
						if (canReact && canReach)
						{
							re = nw.m->getReaction(abs(nw.stoichiometry->getReactionAttribute(k)) - 1);
							klaw = re->getKineticLaw();
							value += nw.getReactionRate(klaw, k, sourceState );
							r.push_back(k);
						}
					}
				}

				// If off-diagonal element is negative , then something must be wrong!
				if (value < 0)
				{
					printf ("ERROR: Negative off-diagonal element: %d, %d\n", i+1, j+1);
				}

				if (value != 0)
				{
					// ADDED ON NOV. 12, 2007 AT CHICAGO.
					p_cur = new MatrixElement (i, j, value, r);
					r.clear();
					nw.TransMatrix->Add(p_cur);

					// trace the counter of TransMatrix
					if (verbose)
					{
						printf ("%ld\n", nw.TransMatrix->getSize());
					}
				}
			}

			diagonal_value -= value;
		}

		// Compute diagonal element
		/*
		 * if p_cur->value is greater than or equal to zero, then something wrong.
		 */
		if (diagonal_value >= 0 && s_reaction == -1)
		{
			printf ("ERROR: Non-negative diagonal element: state %d\n", j+1); // ONE-based index number, so, j+1
		}
				
		/*
		 * Insert p_cur element into the end of the linked list.
		 */
		// ADDED ON NOV. 12, 2007 AT CHICAGO.
		p_cur = new MatrixElement(j, j, diagonal_value);
		nw.TransMatrix->Add(p_cur);

		nw.TransMatrix->SaveCurCol(rlabel);
	}
	
	/*
	 * OUTPUT OF THE TRANSITION MATRIX 
	 */
	nw.TransMatrix->WriteMatrix(tmfile,rlabel);
	
	if(verbose)
	{
		printf ("Output of Transition matrix: Done\n");
	}
	/*
	 * END OF OUTPUT OF THE TRANSITION MATRIX
	 */
	
	
	
	if (verbose)
	{
		/*
		 * VALIDATION OF TRANSITION MATRIX: TransMatrix
		 * RULES:
		 * 1. ALL DIAGONAL ELEMENTS ARE NEGATIVE;
		 * 2. ALL OFF-DIAGONAL ELEMENTS ARE NON-NEGATIVE;
		 * 3. ALL ITS COLUMNS SUM TO EXACTLY ZERO. 
		 */
		nw.TransMatrix->validateTransMatrix();
	}
	
	
	/*
	 * recollect memory space
	 */

	return 0;
}


