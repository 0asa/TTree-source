/*
 *  Probas.cpp
 *  TTree
 *
 *  Created by Vincent Botta on 14/01/09.
 *  Copyright 2009 University of Liège. All rights reserved.
 *
 */

#include "Probas.h"

Probas::Probas() : size(0) {}

Probas::~Probas() {  }

void Probas::print(FILE* out) const {
	for (int i = 0 ; i < size ; ++i) 
		fprintf(out,"%d\t%f\t%d\n",object[i],probas[i],realclass[i]);
	
}

unsigned int Probas::getSize() const { return size; }

void Probas::push(Probas &p) {
	for (int i = 0 ; i < p.getSize() ; ++i) {
		object.push_back(p.getObj(i));
		probas.push_back(p.getProb(i));
		realclass.push_back(p.getRealClass(i));
		++size;
	}
}

unsigned int Probas::getObj(unsigned int i) const { return object[i]; }
double Probas::getProb(unsigned int i) const { return probas[i]; }
double Probas::getRealClass(unsigned int i) const { return realclass[i]; }

void Probas::push(unsigned int o, double p, double rc) {
	object.push_back(o);
	probas.push_back(p);
	realclass.push_back(rc);
	++size;
}

