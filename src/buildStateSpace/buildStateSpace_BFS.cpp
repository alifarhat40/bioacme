
#if HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <iostream>
#include <cstring>
#include "../BioModel/Stoichiometry.h"
#include "../BioModel/State.h"
#include "../BioModel/Queue.h"
#include "../BioModel/HashTable.h"
#include "../BioModel/InitialConds.h"
#include "../BioModel/SLinkedList.h"
#include <sbml/SBMLTypes.h>
#include <cmath>
#include <ctime>
#include "anyoption.h"

#if defined(_WIN32)
#include <direct.h>
#endif

#define excludeNullState 2

using namespace std;

int main (int argc, char* argv[])
{
	char*         sbmlfilename = 0; /* input file of sbml model */
	char*         initfilename = 0;
	char*         statesfilename = 0; /* for the output of state space */
	bool          verbose;
	/////////////////////////////////
	HashTable*    ssHashTable = 0; // store and search states quickly. //
	SLinkedList*  StatesList = 0;  // store in sequence. //
	/////////////////////////////////
	Queue         QueueList;
	unsigned int  QueueDepth = 0;
	State*        astate = 0;
	State*        cur_state = 0;
	//State*        p_state = 0;
	unsigned int  n_states = 0;
	unsigned int  n_species = 0;
	unsigned int  n_reactions = 0;
	string*       speciesID = 0;
	
	SBMLDocument*        d = 0;
	Model*               m = 0;

	Stoichiometry*       stoichiometry = 0;
	InitialConds         initialcond;

	unsigned int i = 0, j = 0;

	// Recode total running time.
	long tic=0, tac=0;
	time_t start_t, end_t;

	tic = clock();
	start_t = time(NULL);
	
        /* CREATE AN OBJECT TO HANDLE COMMAND LINE */
        AnyOption *opt = new AnyOption();

        /* SET THE USAGE/HELP   */
        opt->addUsage( "" );
        opt->addUsage( "Usage: " );
        opt->addUsage( "" );
        opt->addUsage( " -h  --help  			Prints this help " );
        opt->addUsage( " -m  --sbml <sbmlfile>		Input SBML file" );
        opt->addUsage( " -i  --init <initfile> 		Initial condition file" );
        opt->addUsage( " -s  --statespace <ssfile>	Output state space file" );
        opt->addUsage( " -v  --verbose    		Switch for more output " );
        opt->addUsage( "" );

        /* SET THE OPTION STRINGS/CHARACTERS */
        opt->setFlag(  "help", 'h' );
        opt->setOption(  "sbml", 'm' );
        opt->setOption(  "init", 'i' );
        opt->setOption(  "statespace", 's' );
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
        if( opt->getFlag( "verbose" ) || opt->getFlag( 'v' ) ) { verbose = 1; }
	else { verbose = 0; }

	if( opt->getValue( 'm' ) != NULL  || opt->getValue( "sbml" ) != NULL  )
		sbmlfilename = opt->getValue( 'm' );
	else {
		printf ("Please specify the SBML filename.\n\n");
		opt->printUsage();
		return 1;
	}

	if( opt->getValue( 's' ) != NULL  || opt->getValue( "statespace" ) != NULL  )
		statesfilename = opt->getValue( 's' );
	else {
		printf ("Please specify the state space filename.\n\n");
		opt->printUsage();
		return 1;
	}

	if( opt->getValue( 'i' ) != NULL  || opt->getValue( "init" ) != NULL  )
		initfilename = opt->getValue( 'i' );
	else
	{
		printf ("Please specify the initial condition filename.\n\n");
		opt->printUsage();
		return 1;
	}
	//cout << endl;

	
	// initialize HashTable.
	ssHashTable = new HashTable();
	
	// initialize StatesList
	StatesList  = new SLinkedList();
	
	/* read SBML model */
	d = readSBML(sbmlfilename);
	m = d->getModel();
	
	/* get the number of species */
	n_species = m->getNumSpecies();
	QueueList.setNumSpecies(n_species);
	
	/* initialize speciesID */
	speciesID = new string[n_species];
	/* get all species ID in the model */
	for (i = 0; i < n_species; i++)
	{
		speciesID[i] = m->getSpecies(i)->getId();
	}
	
	/*
	 * get stoichiometric matrix from the model.
	 */
	stoichiometry = new Stoichiometry();
	
	int f = stoichiometry->genStoichiometricMatrix(m);
	if (f == -1)
	{
		printf ("Exit caused by an error.\n");
		return -1;
	}
	if(verbose)
	{
		stoichiometry->printStoichiometricMatrix();
	}

	/***************************************\
	\***************************************/
	/*
	 * Read initial conditions from a file
	 */
	if (initialcond.readInitialConds(initfilename) == -1)
	{
		printf ("ERROR:  Read initial condition file error.\n");
		return -1;
	}
	
	// INITIALIZE THE VARIABLE: cur_state.
	cur_state = new State(n_species); // CONTAINER OF NEW STATES;
	State* dst_state = new State(n_species); // CONTAINER OF SOME TEMP STATES;

	/*
	 * build a stack of states and initialize it.
	 */
	//astate = new State(n_species);
	//QueueList = new Queue();
	
	/*
	 * push the initial state into the stack.
	 * ATTENTION: only consider the first initial state now.
	 */
	QueueList.push(initialcond.getInitialState(0));
	QueueDepth ++;
	
	// DEFINE A INTEGER TO COUNT THE NUMBER OF STATES
	// THAT HAVE BEEN ADDED TO THE StatesList
	// OR ALREADY WRITTEN TO THE HD.
	unsigned long numberOfStates = 0;

	// OPEN STATE SPACE FILE TO WRITE.
	srand((unsigned long)time(0));
	unsigned long rn = rand();
	char* tmpFileName = new char[64];
	#if defined(_WIN32)
		sprintf (tmpFileName, "_sst%lx", rn);
	#else
		sprintf (tmpFileName, "/tmp/_sst%lx", rn);
	#endif

	FILE* fpss = 0;
	fpss = fopen (tmpFileName, "w");
	if (!fpss)
	{
		printf ("ERROR: Can not create temporary file: %s\n", tmpFileName);
		return -1;
	}


	int flag = 0;
	i = 0;
	unsigned int childReactFlag = 0;
	n_reactions = 0;
	int success = 0;
	while ( !QueueList.isEmpty())
	{
		/*
		 * popup a state from the top of the stack.
		 */
		astate = QueueList.pop();
		//cout << " TEST " << endl;
		QueueDepth --;
		
		// ADD NEW STATE TO HASH TABLE: ssHashTable.
		// HASH TABLE CLASS WILL CHECK REPEATNESS AUTOMATICALLY.
		flag = ssHashTable->Find(astate);
		if (flag >= 0) // If astate has been in current ssHashTable, skip to next loop.
		{
			delete astate;
			continue;
		}
		else
		{
			// Nov. 9, 2007, at Chicago.
			// TO SAVE MEMORY SPACE, JUST STORE ONLY ONE COPY OF EACH STATE IN MEMORY: StatesList.
			// HashTable DOES NOT KEEP HARD COPY OF STATES ANY MORE,
			// IT JUST KEEP A POINTER AND AN INTEGER TO INDICATE THE POSITION OF EACH STATE IN StatesList.
			ssHashTable->Insert(astate);
			// AUTOINCRUMENT OF COUNTER, AND WRITE NEW STATE INTO HD.
			numberOfStates ++;
			for (i = 0; i < astate->n_species; i++)
			{
				fprintf (fpss, "%d", astate->getStateValue(i));
				if (i < astate->n_species - 1)
				{
					fprintf (fpss, " ");
				}
				else if(i == astate->n_species - 1)
				{
					fprintf (fpss, "\n");
				}
			}

			//State* cpstate = new State(astate);
			//StatesList->AddtoTail(cpstate);
			/*if (StatesList->AddtoTail(astate) == 0)
			{
				printf ("ERROR: empty pointer.\n");
				return 0;
			}*/
		}
		
		/*
		 * calculate all possible reachable states from this state
		 * according to stoichiometric matrix, and push them into the
		 * stack if new states never appeared before.
		 */
		childReactFlag = 0;
		n_reactions = stoichiometry->getNumReactions();
		for (i = 0; i < n_reactions; i++)
		{
			success = stoichiometry->reactWithState(i, astate, cur_state);
			if (success > 0) // THIS MEANS I HAVE GOT A NEW STATE.
			{
				childReactFlag = 0;
				for (j = 0; j < n_reactions; j++)
				{
					if (stoichiometry->reactWithState(j, cur_state, dst_state) > 0)
					{
						childReactFlag = 1;
						break;
					}
				}
				
				/*
				 * push the cur_state into the stack if it is a new state.
				 */
				// STORAGE OPTIMIZATION
				// only unseen states are pushed into stack!
				if (childReactFlag == 1 && ssHashTable->Find(cur_state) < 0)
				{
					State* new_state = new State (cur_state);
					//cout << "OK, Push now: " << new_state << endl;
					QueueList.push(new_state);
					new_state = NULL;
					//cout << "OK, Pushed in" << endl;
					QueueDepth ++;
					cur_state->initialize();
				}
				else
				{
					cur_state->initialize();
				}
			}
		}
	}
	
	// CLOSE STATE SPACE FILE
	fclose(fpss);

	// remove temporary dir.
//	rmdir (QueueList.DirName);
	
	#if defined(_WIN32)
		_rmdir (QueueList.DirName);
	#else	
		rmdir (QueueList.DirName);	// ATTENTION: THIS FUNCTION IS OS-DEPENDENT! //
	#endif	


	// OUTPUT OF STATE SPACE FROM HASHTABLE
	unsigned int n_buckets = 0;
	n_states = ssHashTable->getNumTotalItems();
	// OPEN FILE HANDLE
	FILE* fp = 0;
	
	fp = fopen(statesfilename, "w");
	if (!fp)
	{
		printf("ERROR: Can not creat file: %s\n", statesfilename);
		return -1;
	}
	
	// VARIABLES NEEDED FOR EVALUATING HASH TABLE
	unsigned long BucketsDistribution[BUCKETSIZEDIST];
	unsigned long maxBucketSize  = 0;
	for (i = 0; i < BUCKETSIZEDIST; i++)
	{
		BucketsDistribution[i] = 0;
	}
	
	// OUTPUT FILE HEADER HERE
	/* the first line */
	fprintf (fp, "%d %d\n", n_species, n_states);
	
	/* the second line */
	for (i = 0; i < n_species; i++)
	{
		fprintf (fp, "%s", speciesID[i].data());
		if (i < n_species - 1)
		{
			fprintf (fp, " ");
		}
	}
	fprintf (fp, "\n");
	fclose(fp);

	// TRANSFER STATES FROM TEMP FILE TO THIS FILE.
	FILE* from = 0;
	FILE* to   = 0;

	// READ/WRITE BUFFER
	//unsigned int BUFSIZE = 1000000;
	char buffer[BUFSIZ];
	unsigned int amount;
	unsigned int written;
	bool  result = true;
	
	from = fopen (tmpFileName, "r");
	to   = fopen (statesfilename, "a+");
	do
	{
		amount  = fread (buffer, sizeof(char), BUFSIZ, from);
		if (amount)
		{
			written = fwrite (buffer, sizeof(char), amount, to);
			if (written != amount)
			{
				result = false;
			}
		}
	} while ((result == true) && (amount == BUFSIZ));

	fclose (from);
	fclose (to);

	// REMOVE TEMP STATE SPACE FILE.
	remove (tmpFileName);

	// OUTPUT STATES HERE
	if (verbose)
		n_buckets = ssHashTable->getNumBuckets();
	else
		n_buckets = 0; // disable output from HashTable
	//StatesList->WriteListA(statesfilename);
	/*p_state = StatesList->getHead();
	while(p_state)
	{
		for (j = 0; j < n_species; j++)
		{
			fprintf (fp, "%d", p_state->getStateValue(j));
			if (j < n_species - 1)
			{
				fprintf (fp, " ");
			}
		}
		fprintf (fp, "\n");
		
		p_state = p_state->getNext();
	}*/
	
	for (i = 0; i < n_buckets; i++)
	{
		unsigned long bucketsize = ssHashTable->getSizeofBucket(i);
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
			/*p_state = ssHashTable->getHeadofBucket(i);
			while(p_state)
			{
				for (j = 0; j < n_species; j++)
				{
					fprintf (fp, "%d", p_state->getStateValue(j));
					if (j < n_species - 1)
					{
						fprintf (fp, " ");
					}
				}
				fprintf (fp, "\n");
				
				p_state = p_state->getNext();
			}*/
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
		


	/*
	 * Compute and print out total running time.
	 */
	tac = clock();
	end_t = time(NULL);

	if (verbose)
	{
		printf ("Total CPU time: %ld seconds.\n", (tac - tic) / CLOCKS_PER_SEC);
		printf ("Total running time: %ld seconds.\n", end_t - start_t);
	}
	
	return 0;
}

/* END OF CODE */

