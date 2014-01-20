/*
 *  Probas.h
 *  TTree
 *
 *  Created by Vincent Botta on 14/01/09.
 *  Copyright 2009 University of Liège. All rights reserved.
 *
 */

#ifndef __PROBAS__
#define __PROBAS__

#include <vector>

class Probas {
private:

	std::vector<unsigned int> object;
	std::vector<unsigned short int> realclass;
	unsigned int size;
public:
	Probas();
	~Probas();
	void push(Probas &p);
	void push(unsigned int o, double p, double rc);
	unsigned int getSize() const;
	void print(FILE* out = stdout) const;
	unsigned int getObj(unsigned int i) const;
	double getProb(unsigned int i) const;
	double getRealClass(unsigned int i) const;
	std::vector<double> probas;
	void shrink() {
		std::vector<unsigned int>(object.begin(),object.end()).swap(object);
		std::vector<double>(probas.begin(),probas.end()).swap(probas);
		std::vector<unsigned short int>(realclass.begin(),realclass.end()).swap(realclass);
	};
	
};

#endif

