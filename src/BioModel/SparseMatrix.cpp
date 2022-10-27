
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "MatrixElement.h"
#include "SparseMatrix.h"

using namespace std;

SparseMatrix::SparseMatrix(unsigned int n_reactions)
{
	num_states = 0;
	num_nonzeroelements = 0;
	num_elementsinmemory = 0;
	//NextEmptyBufSite = 0;
	//MatrixBuf = new MatrixElement[MAXELENUMINMEM];
	//CurCol = new MatrixElement[n_reactions];
	//CurCol = 0;
	head = 0;
	/*for (unsigned int i = 0; i < n_reactions; i++)
	{
		MatrixElement* p = new MatrixElement();
		if (CurCol == 0)
		{
			head = CurCol = p;
		}
		else
		{
			p->next = head;
			head = CurCol = p;
		}
	}*/
	num_ElementsInCurCol = 0;
	//NextEmptyInCurCol = 0;
	//tail = 0;
	tmpMatrixFile = 0;
	p_curcol = 0;
        //p0 = 0;
        //p_cur = 0;
        //p_end = 0;
	DataAreSavedToTMPFile = false;

	srand((unsigned long)time(0));
	unsigned long rn = rand();
        tmpMatrixFile = new char[64];
        sprintf (tmpMatrixFile, "/tmp/_mxt%lx", rn);
}

SparseMatrix::~SparseMatrix()
{
}

MatrixElement* SparseMatrix::getHead()
{
	return head;
}

unsigned long SparseMatrix::SetNumStates(unsigned long num)
{
	num_states = num;
	return num_states;
}

/*
unsigned long SparseMatrix::SetNumNonZeroElements(unsigned long num)
{
	num_nonzeroelements = num;
	return num_nonzeroelements;
}
*/

unsigned long SparseMatrix::Increment1NumNonZeroElements()
{
	num_nonzeroelements ++;
	num_elementsinmemory ++;
	return num_nonzeroelements;
}


MatrixElement* 
//unsigned int
SparseMatrix::SaveCurCol(bool w_rlabels)
{
	// THIS IS THE NEW WAY TO SAVE CURRENT COLUMN TO BUFFER.
	// FIRST, CHECK IF THE MatrixBuf IS FULL,
	// IF IT IS FULL, FLUSH IT TO HARD DISK FIRST, AND SAVE NEW ELEMENTS TO BUFFER,
	// ELSE, GO ON SAVING ELEMENTS TO BUFFER DIRECTLY.

	// NO NO, WE DON'T NEED TO USE BUFFER, JUST PUT THEM DIRECTLY TO HARD DISK.
	// THE OS WILL DO THE BUFFER.
	//\\ REALLY? YES, BUT THE SYSTEM BUFFER IS VERY SMALL AND THE SYSTEM SPEND
	//\\ TOO MUCH TIME ON DISK IO. Nov. 21 2007, in Shanghai.
	//if ( (MAXELENUMINMEM - NextEmptyBufSite) < NextEmptyInCurCol)
	//{
	/*
        FILE* ftmp = 0;
        ftmp = fopen (tmpMatrixFile, "a+");

	MatrixElement* p_out = head;
	MatrixElement* p_d = head;
	
	//for debug
	//cout << "??problem is here??" << endl;

	//unsigned int i = 0;
	//while (NextEmptyInCurCol > 0)
	while (p_out != 0)
	{
		// FIND THE ELEMENT WITH MINIMUM i VALUE IN CurCol.
		//unsigned int min = 0;

		//for (i = 0; i < NextEmptyInCurCol; i++)
		//{
		//	if (CurCol[i].i < CurCol[min].i)
		//	{ min = i; }
		//}
               	//fprintf (ftmp, "%d %d %0.10g\n", CurCol[min].i+1, CurCol[min].j+1, (double)CurCol[min].value);
               	fprintf (ftmp, "%d %d %0.10g\n", p_out->i+1, p_out->j+1, (double)p_out->value);
		//for (i = min; i < NextEmptyInCurCol-1; i++) // REMOVE CURRENT ELEMENT FROM ARRAY CurCol.
		//{
		//	CurCol[i].i = CurCol[i+1].i;
		//	CurCol[i].j = CurCol[i+1].j;
		//	CurCol[i].value = CurCol[i+1].value;
		//}
		//NextEmptyInCurCol --;

		p_d = p_out;
		p_out = p_out->next;
		delete p_d;


		//p_out = head;
		//head = head->next;
		//p_out->next = 0;
	}
	head = 0;
	num_ElementsInCurCol = 0;

	//for debug
	//cout << "??or, problem is here??" << endl;

        DataAreSavedToTMPFile = true;

        fflush (ftmp);
        fclose (ftmp);
	*/

	//}

	/*
	// SAVE MATRIX ELEMENTS IN CurCol TO MatrixBuf.
	int num_remainingItem = NextEmptyInCurCol;
	while (num_remainingItem > 0)
	{
		// FIND THE ELEMENT WITH MINIMUM i VALUE IN CurCol.
		unsigned int min = 0;

		for (unsigned int i = 0; i < NextEmptyInCurCol; i++)
		{
			if (CurCol[i].i < CurCol[min].i)
			{ min = i; }
		}

		// ADD THE NODE TO MatrixBuf
		MatrixBuf[NextEmptyBufSite].SetValues(CurCol[min].i, CurCol[min].j, CurCol[min].value);
		for (unsigned int i = min; i < NextEmptyInCurCol-1; i++) // REMOVE CURRENT ELEMENT FROM ARRAY CurCol.
		{
			CurCol[i].i = CurCol[i+1].i;
			CurCol[i].j = CurCol[i+1].j;
			CurCol[i].value = CurCol[i+1].value;
		}
		NextEmptyBufSite ++;
		NextEmptyInCurCol --;

		num_remainingItem --;
	}*/

	

	/*
	*/
	// BELOW IS THE PREVIOUS WAY TO RENEW CURRENT COLUMN POINTER.
	if (p_curcol == 0)
	{
		p_curcol = head;
	}

	// MOVE CURRENT COLUMN POINTER TO THE END OF THE LIST.
	while (p_curcol->next != 0)
	{
		p_curcol = p_curcol->next;
	}

	// IF THE NUMBER OF ELEMENTS IN MEMORY REACH THE PREDEFINED MAX NUMBER
	// THEN, DUMP THEM TO A TEMPORARY FILE IN HARD DISK.
	if (num_elementsinmemory > MAXELENUMINMEM)
	{
		MatrixElement* p_element = 0;
		p_element = head;

		FILE* ftmp = 0;
		ftmp = fopen (tmpMatrixFile, "a+");
		while (p_element != p_curcol)
		{
			head = head->next;
			p_element->next = NULL;

			if (w_rlabels)
			{
				fprintf (ftmp, "%d %d %.12g", p_element->i+1, p_element->j+1, (double)p_element->value);
				for( unsigned int i = 0; i < p_element->r->size(); i++ ) {
					fprintf (ftmp, ", %d", p_element->r->at(i));
				}
				fprintf (ftmp, "\n");
			}
			else
			{
				fprintf (ftmp, "%d %d %.12g\n", p_element->i+1, p_element->j+1, (double)p_element->value);
			}

			delete p_element;
			p_element = head;
			num_elementsinmemory --;
		}

		DataAreSavedToTMPFile = true;

		fflush (ftmp);
		fclose (ftmp);
	}

	return p_curcol;

	//return 0;
}

MatrixElement* 
//unsigned int
SparseMatrix::Add (MatrixElement* node)
{
	// New one:
	//\\ TURN BACK TO PREVIOUS ONE. Nov 21, 2007, in Shanghai
	/*
	MatrixElement* p_0 = 0;
	MatrixElement* p_1 = 0;
	if (head == 0 && num_ElementsInCurCol == 0)
	{
		// for debug
		//cout << "~~~~for the 1st element." << endl;

		head = node;
	}
	else if (head != 0 && head->next == 0 && num_ElementsInCurCol == 1)
	{
		// for debug
		//cout << "~~~~for the 2nd element." << endl;

		if (node->i > head->i)
		{
			head->next = node;
		}
		else
		{
			node->next = head;
			head = node;
		}
	}
	else if (num_ElementsInCurCol >= 2)
	{
		// for debug
		//cout << "<><>for the 3rd element." << endl;

		p_0 = head->next;
		p_1 = head;
		if (node->i < head->i)
		{
			node->next = head;
			head = node;
		}
		else
		{
			while (p_0 != 0 && node->i > p_0->i)
			{
				p_0 = p_0->next;
				p_1 = p_1->next;
			}
			node->next = p_0;
			p_1->next = node;
		}

		// for debug
		//cout << "><><adding new node!" << endl;
	}

	num_ElementsInCurCol ++;
	num_nonzeroelements ++;
	*/

	/*
	// ALMOST IMMEDIATELY GET OUTDATED.
	// THIS IS THE NEW WAY TO ADD NEW NODE: ARRAY.
	CurCol[NextEmptyInCurCol].i = node->i;
	CurCol[NextEmptyInCurCol].j = node->j;
	CurCol[NextEmptyInCurCol].value = node->value;
	NextEmptyInCurCol ++;

	num_nonzeroelements ++;
	*/


	/*
	*/
	// BELOW IS THE PREVIOUS WAY TO ADD NEW NODE WITH LINK LIST.
	MatrixElement* p0 = 0;
	MatrixElement* p1 = 0;
	MatrixElement* p_cur = 0;

	p_cur = node;
	
	if (num_nonzeroelements == 0) // THIS MEANS THE MATRIX IS EMPTY, THIS IS THE FIRST ELEMENT TO BE ADDED.
	{
		head = node;
		p_curcol = 0;
		num_nonzeroelements = 1;
		num_elementsinmemory = 1;
	}
	else if (num_nonzeroelements > 0) // THEN, ADD NEW NODE TO THE MATRIX.
	{
		if (p_curcol == 0)
		{
			p1 = head;
			p0 = p1->next;
			if (p_cur->i < head->i)
			{
				p_cur->next = head;
				head = p_cur;
			}
			else
			{
				while (p0 != 0 && p_cur->i > p0->i)
				{
					p1 = p0;
					p0 = p0->next;
				}
				p1->next = p_cur;
				p_cur->next = p0;
			}
		}
		else
		{
			p0 = p_curcol;
			while (p0->next != 0 && p_cur->i > p0->next->i)
			{
				p0 = p0->next;
			}

			// INSERT THE NEW ELEMENT TO MATRIX.
			p_cur->next = p0->next;
			p0->next = p_cur;
		}

		num_nonzeroelements ++;
		num_elementsinmemory ++;
	}
	else // THERE MUST BE SOMETHING WRONG.
	{
		printf ("Hey, there must be something wrong! -- SparseMatrix::Add()\n");
	}

	p_cur = 0;

	return node;

	//return 1;
}


unsigned long SparseMatrix::getSize() const
{
	return num_nonzeroelements;
}


int SparseMatrix::WriteMatrix (char* filepath, bool w_rlabels)
{
	FILE* fpmat = 0;
	fpmat = fopen (filepath, "w");

	// OUTPUT THE HEADER OF MATRIX FILE.
	fprintf (fpmat, "%ld %ld\n", num_states, num_nonzeroelements);

	// IF THERE ARE ELEMENTS WHICH HAVE BEEN DUMPED TO HARD DISK TMP FILE,
	// THEN COPY THEM TO THE FINAL MATRIX FILE.
	if (DataAreSavedToTMPFile)
	{
		FILE* ftmp = 0;
		ftmp = fopen (tmpMatrixFile, "r");

		char buffer[BUFSIZ];
		unsigned int amount;
		unsigned int written;
		bool result = true;

		do
		{
			amount = fread (buffer, sizeof (char), BUFSIZ, ftmp);
			if (amount)
			{
				written = fwrite (buffer, sizeof (char), amount, fpmat);
				if (written != amount)
				{
					result = false;
				}
			}
		} while ((result == true) && (amount == BUFSIZ));

		fclose (ftmp);
		remove (tmpMatrixFile);
		fflush (fpmat);
	}

	/* DOES NOT NEED ANY MORE, MAYBE.
	// THE NEW WAY.
	for (unsigned int i = 0; i < NextEmptyBufSite; i++)
	{
		fprintf (fpmat, "%d %d %0.10g\n", MatrixBuf[i].i+1, MatrixBuf[i].j+1, (double)MatrixBuf[i].value);
	}
	NextEmptyBufSite = 0;
	*/

	/*
	*/
	// OUTPUT ELEMENTS IN MEMORY TO MATRIX FILE.
	MatrixElement* p_element = 0;
	p_element = head;
	while (p_element != 0 && num_elementsinmemory > 0)
	{
		if (w_rlabels)
		{
			fprintf (fpmat, "%d %d %.12g", p_element->i+1, p_element->j+1, (double)p_element->value);
			for( unsigned int i = 0; i < p_element->r->size(); i++ ) {
				fprintf (fpmat, ", %d", p_element->r->at(i));
			}
			fprintf (fpmat, "\n");
		}
		else
		{
			fprintf (fpmat, "%d %d %.12g\n", p_element->i+1, p_element->j+1, (double)p_element->value);
		}
		head = p_element->next;
		p_element->next = NULL;
		delete p_element;
		p_element = head;
		num_elementsinmemory --;
	}

	fclose (fpmat);

	return 0;
}



int SparseMatrix::validateTransMatrix()
{
	MatrixElement* TMatrix = head;

	unsigned int i, j;
	double* sum_c = new double[num_states];
	double* sum_r = new double[num_states];
	double  sum_all = 0;
	unsigned int zero_diag = 0;
	unsigned int positive_diag = 0;
	unsigned int negetive_offdiag = 0;
	
	for (i = 0; i < num_states; i++)
	{
		sum_c[i] = 0;
		sum_r[i] = 0;
	}
	
	MatrixElement* p_cur = 0;
	
	p_cur = TMatrix->next;
	while(p_cur != 0)
	{
		sum_c[p_cur->j] += p_cur->value;
		sum_r[p_cur->i] += p_cur->value;
		sum_all += p_cur->value;
		p_cur = p_cur->next;
	}
	
	
	
	for (j = 0; j < num_states; j++)
	{
		printf ("Column %d sums to: %f\n", j+1, sum_c[j]);
		//cout << "Column " << j+1 << " sums to: " << sum_c[j] << endl;
	}
	
	/*for (j = 0; j < n_states; j++)
	{
		cout << "Row " << j+1 << ": sum to " << sum_r[j] << endl;
	}*/
	
	if (sum_all != 0)
	{
		printf ("Sum all to: %f\n", sum_all);
		//cout << "Sum all to: " << sum_all << endl;
	}
	
	p_cur = TMatrix->next;
	while (p_cur != 0)
	{
		if (p_cur->i == p_cur->j && p_cur->value == 0)
                {
                        zero_diag ++;
                }
                if (p_cur->i == p_cur->j && p_cur->value > 0)
                {
                        positive_diag ++;
                }
                if (p_cur->i != p_cur->j && p_cur->value < 0)
                {
                        negetive_offdiag ++;
                }
                p_cur = p_cur->next;
        }
        printf ("Number of         zero diagonal element: %d\n", zero_diag);
        printf ("Number of     positive diagonal element: %d\n", positive_diag);
        printf ("Number of negetive off-diagonal element: %d\n", negetive_offdiag);

        //cout << "Number of         zero diagnal element: " << zero_diag << endl;
        //cout << "Number of     positive diagnal element: " << positive_diag << endl;
        //cout << "Number of negetive off-diagnal element: " << negetive_offdiag << endl;

/*      for (i = 0; i < n_states; i++)
        {
                if (TMatrix[i * n_states + i] >= 0)
                {
                        cout << "a[" << i+1 << "," << i+1 << "] = " << TMatrix[i * n_states + i] << endl;
                }
        }*/

        // free occupied memory space
        delete sum_c;
        delete sum_r;

        return 0;
}




