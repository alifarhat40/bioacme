#ifndef SPARSEMATRIX_H_
#define SPARSEMATRIX_H_

/*
 * SparseMatrix class specifically designed for handling sparse matrix.
 * Its elements are stored as MatrixElement.
 */

//#include "MatrixElement.h"
class MatrixElement;

#define MAXELENUMINMEM 1000000

class SparseMatrix
{
private:
	unsigned long 	num_states;
	unsigned long 	num_nonzeroelements;
	unsigned long   num_elementsinmemory;
	//unsigned long   NextEmptyBufSite;
	//MatrixElement*  MatrixBuf;
	//MatrixElement*  CurCol;
	//unsigned long   NextEmptyInCurCol;
	MatrixElement* 	head;
	unsigned int    num_ElementsInCurCol;
	//MatrixElement*  tail;
	MatrixElement*  p_curcol;
	//MatrixElement*  p0;
	//MatrixElement*  p_cur;
	//MatrixElement*  p_end;

	char* tmpMatrixFile;
	bool  DataAreSavedToTMPFile;

public:
	SparseMatrix (unsigned int n_reactions);
	virtual ~SparseMatrix ();

	MatrixElement* getHead();
	unsigned long SetNumStates(unsigned long num);
	//unsigned long SetNumNonZeroElements(unsigned long num);
	unsigned long Increment1NumNonZeroElements();

	//unsigned int  SaveCurCol();
	MatrixElement* SaveCurCol(bool w_rlabels);
	
	//unsigned int  Add (MatrixElement* node);
	MatrixElement*  Add (MatrixElement* node);

	unsigned long getSize() const;
	MatrixElement* getHead() const;
	
	int WriteMatrix (char* filepath, bool w_rlabels);

	int validateTransMatrix();
};

#endif /*SPARSEMATRIX_H_*/

