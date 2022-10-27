
#define VALUETYPE double

#include <iostream>
#include <vector>
#include "MatrixElement.h"

using namespace std;

MatrixElement::MatrixElement()
{
	this->i     = 0;
	this->j     = 0;
	this->value = 0.0;
	this->r = new vector<unsigned int>();
	this->next  = 0;
}


MatrixElement::MatrixElement(unsigned int i, unsigned int j, VALUETYPE v)
{
	this->i = i;
	this->j = j;
	this->value = v;
	this->r = new vector<unsigned int>();
	this->next = 0;
}

MatrixElement::MatrixElement(unsigned int i, unsigned int j, VALUETYPE v, vector<unsigned int>& rv)
{
	this->i = i;
	this->j = j;
	this->value = v;
	this->r = new vector<unsigned int>();
	while( !rv.empty() )
	{
		r->push_back(rv.back());
		rv.pop_back();
	}
	this->next = 0;
}

MatrixElement::~MatrixElement()
{
	//next = 0;

	// for debug
	//cout << ">>>Free memory, please!<<<" << endl;
}


unsigned int MatrixElement::SetValues (unsigned int i, unsigned int j, VALUETYPE v, unsigned int k)
{
	this->i = i;
	this->j = j;
	this->value = v;
	this->r->push_back(k);
	this->next = 0;

	return 0;
}


