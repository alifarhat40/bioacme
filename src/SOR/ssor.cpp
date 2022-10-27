// Solving Ax = 0 with Successive Over-Relaxation


#include "anyoption.h"
#include <iostream>
#include <string>
#include <cstring>
#include <cmath>
#include <vector>
#include <time.h>

using namespace std;

double difftime(clock_t clock1, clock_t clock2);

int main(int argc, char* argv[])
{
	//long maxn = 5000000;
	//long nzmax = 40000000;
	//int maxnev = 12;
	//int maxncv = 30;
	//long ldv = maxn;
	//int iparam[11];
	//int ipntr[14];
	//double* ax = 0;
	//double* d = 0;
	//double* resid = 0;
	//double* w = 0;
	//double vsum;
	//double sigma = -10;
	//double zero = 0.0E0;
	long n = 0;
	long nnz = 0;
	double* v = 0;
	double tol = 1E-16;
	double eqtol = 1E-17;
	double omega = 1.0;
	long   maxniter = 3000000;
	double err = 0, err0 = 0;

	int verbose = 0;
	char*	matrixfile = 0;
	char*	steadyfile = 0;


	long* ia = 0;
	long* ja = 0;
	double* a = 0;
	long* ib = 0;
	long* jb = 0;
	double* b = 0;
	//double* lu = 0;
	double* dd = 0;
	long* nnrow = 0;
	long* ridx = 0;
	long* idx = 0;
	double* pd = 0;
	double sumssv = 0;
	//double dt = 0;
	double maxdiag = 0;
	long counter = 0;
	long eqerrcnter = 0;
	double delta = 0;
	long i, j = 0;

        /* CREATE AN OBJECT TO HANDLE COMMAND LINE */
        AnyOption *opt = new AnyOption();

        /* SET THE USAGE/HELP   */
        opt->addUsage( "" );
        opt->addUsage( "Usage: " );
        opt->addUsage( "" );
        opt->addUsage( " -h  --help  				Prints this help " );
        opt->addUsage( " -m  --mtrx <matrix file>		Input matrix file" );
        opt->addUsage( " -s  --steady <steady state file>	Output steady state distribution file" );
        opt->addUsage( " -o  --omega <omega value> 		Omega value for Succesive Over Relaxation" );
        opt->addUsage( " -v  --verbose 0  			Switch for more output " );
        opt->addUsage( "" );

        /* SET THE OPTION STRINGS/CHARACTERS */
        opt->setFlag(  "help", 'h' );
        opt->setOption(  "mtrx", 'm' );
        opt->setOption(  "steady", 's' );
        opt->setOption(  "omega", 'o' );
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
	if( opt->getValue( 'm' ) != NULL  || opt->getValue( "mtrx" ) != NULL  )
		matrixfile = opt->getValue( 'm' );
	else
	{
		printf ("Please specify the matrix filename.\n\n");
		opt->printUsage();
		return 1;
	}
	if( opt->getValue( 's' ) != NULL  || opt->getValue( "steady" ) != NULL  )
		steadyfile = opt->getValue( 's' );
	else
	{
		printf ("Please specify the output steady state distribution filename.\n\n");
		opt->printUsage();
		return 1;
	}
	if( opt->getValue( 'o' ) != NULL  || opt->getValue( "omega" ) != NULL  )
		omega = atof(opt->getValue( 'o' ));
	else
	{
		omega = 1.0;
	}

	// Read transition matrix
	ifstream ftm(matrixfile, ios::in);
	if (!ftm.is_open())
	{
		cout << "Unable to open file: " << matrixfile << endl;
		return -1;
	}
	ftm >> n >> nnz;
	v = new double[n+1];
	pd = new double[n+1];
	dd = new double[n+1];
	idx = new long[n+1];
	ridx = new long[n+1];
	ia = new long [nnz+1];
	ja = new long [nnz+1];
	a  = new double [nnz+1];
	ib = new long [nnz+1];
	jb = new long [nnz+1];
	b  = new double [nnz+1];
	nnrow = new long[n+1];

	// Initialization
	for (i = 0; i <= n; i++)
	{
		v[i] = 0.0;
		pd[i] = 0.0;
		dd[i] = 0.0;
		idx[i] = 0;
		ridx[i] = 0;
		nnrow[i] = 0;
	}
	for (i = 0; i <= nnz; i++)
	{
		ia[i] = 0;
		ja[i] = 0;
		a[i] =  0.0;
		ib[i] = 0;
		jb[i] = 0;
		b[i]  = 0.0;
	}

	i = 0;
	while (!ftm.eof())
	{
		ftm >> ia[i] >> ja[i] >> a[i];
		i ++;
	}
	ftm.close();
	cout << i << endl;

	// Initial condition
	v[1] = v[2] = v[3] = v[4] = v[5] = 0.2;

	// Change the matrix to row storage
	for (i = 0; i < nnz; i ++)
	{
		nnrow[ia[i]] ++;
	}

	ridx[1] = 1;
	for (i = 2; i <= n; i ++)
	{
		ridx[i] = ridx[i-1] + nnrow[i-1];
	}

	maxdiag = 0.0;
	for (i = 0; i < nnz; i ++)
	{
		if (abs(a[i] ) > maxdiag)
		{
			maxdiag = abs(a[i]);
		}
	}
	
	for (i = 0; i < nnz; i ++)
	{
		a[i] = a[i] / maxdiag;
	}

	for (i = 0; i < nnz; i++)
	{
		ib[ridx[ia[i]] + idx[ia[i]]] = ia[i];
		jb[ridx[ia[i]] + idx[ia[i]]] = ja[i];
		b[ridx[ia[i]] + idx[ia[i]]] = a[i];
		idx[ia[i]] = idx[ia[i]] + 1;
	}

	// Take out the diagonal elements
	for (i = 1; i <= nnz; i ++)
	{
		if (ib[i] == jb[i])
		{
			dd[ib[i]] = b[i];
			//cout << "dd[" << ib[i] << "]=" << dd[ib[i]] << endl;
		}
	}


	counter = 0;
	eqerrcnter = 0;
	err = 100;
	err0 = 100;
	clock_t begint = clock();
	while (counter < maxniter && err > tol && eqerrcnter < 10)
	{
		err = 0.0;
		for (i = 1; i <= n; i ++)
		{
			delta = 0.0;
			for (j = 0; j < nnrow[i]; j ++)
			{
				delta = delta - b[ridx[i]+j] * v[jb[ridx[i]+j]];
				//cout << "bv: " << b[ridx[i]+j-1] << " " << v[jb[ridx[i]+j-1]] << endl;
			}
			//cout << "nnrow[" << i << "]=" << nnrow[i] << ", delta=" << delta << ", omega=" << omega << ", dd[" << i << "]=" << dd[i] << endl;
			delta = omega * delta / dd[i];
			v[i] += delta;
			//cout << "delta=" << delta << endl;

			if (abs(delta) > err)
			{
				err = abs(delta);
			}
		}

		if (abs(err-err0) < eqtol)
		{
			eqerrcnter = eqerrcnter + 1;
		}
		else
		{
			eqerrcnter = 0;
		}
		err0 = err;

		counter ++;
		cout.precision(8);
		cout << "Iteration: " << counter << ", dist: " << scientific << err << endl;

	}
	clock_t endt = clock();

	cout << "CPU time used: " << double(difftime(endt,begint)) << " s." << endl;


	ofstream oss(steadyfile, ios::out);
	oss << 1 << endl;
	oss.precision(10);
	sumssv = 0;
	for (i = 1; i <= n; i ++)
	{
		sumssv += v[i];
		pd[i] = v[i];
	}

	for (i = 1; i <= n; i ++)
	{
		pd[i] = pd[i] / sumssv;
		oss << scientific << pd[i] << endl;
	}
	

	return 0;
}


double difftime(clock_t clock1, clock_t clock2)
{
	long diffticks = clock1 - clock2;
	double diffms = double(diffticks) / double(CLOCKS_PER_SEC);
	return diffms;
}


