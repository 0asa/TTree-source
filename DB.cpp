/*
 *  DB.cpp
 *  TTree
 *
 *  Created by Vincent Botta on 17/12/08.
 *  Copyright 2008 University of Liège. All rights reserved.
 *
 */

#include "DB.h"

DB::DB() : nbobject(0) , nbatt(0) { setType(); }

DB::DB(int nbo, int nba) : nbobject(nbo) , nbatt(nba) {
	weight.resize(nbo);// = std::vector<double>(nbo);
	setWeight(1.0);
}

DB::~DB() {}

void DB::setNbClass(int i) {
	if (i > 0) 
		nbclass = i;
	else {
		// Scan output vector and count the different occurencies
		int b = 0;
		for (int j = 0 ; j < output.size(); ++j) {
			b |= (1 << (int)output[j]);
		}
	}
	nbclass = 2;
}
 

void DB::setWeight(double w) {
	for (int i = 0 ; i < nbobject ; ++i) 
		weight[i] = w;
}

void DB::initWeight(unsigned int size, double w) {
	weight.resize(size);
	setWeight(w);
}

void DB::setWeight(std::vector<double> w) {
	for (int i = 0 ; i < nbobject ; ++i) 
		weight[i] = w[i];
}

std::vector<SETREP> DB::selectAll(bool a) const {
	int lim = nbobject;
	if (a) lim = nbatt;
	return select(0,lim,a);
}

std::vector<SETREP> DB::select(int start, int end, bool a) const {
	if (start < 0) start = 0;
	if (a) {
		if (end < 0 || end > nbatt) end = nbatt;
	} else {
		if (end < 0 || end > nbobject) end = nbobject;
	}
	std::vector<SETREP> ret(end - start);
	for (int i = 0 ; i < end - start ; ++i) 
		ret[i] = start+i;
	return ret;
}

std::vector<unsigned int> DB::selectAllAtt() const { 
	int lim = nbatt;
	return selectAtt(0,lim); 
}


std::vector<unsigned int> DB::selectAtt(int start,int end) const { 
	if (start < 0) start = 0;
	if (end < 0 || end > nbatt) end = nbatt;
	std::vector<unsigned int> ret(end-start);
	for (int i = 0 ; i < end - start ; ++i)
		ret[i] = start + i;
	return ret;

}

void DB::setNbObject(unsigned int nbo) { nbobject = nbo; }
void DB::setNbAtt(unsigned int nba) { nbatt = nba ; }

unsigned int DB::getNbAtt() const { return nbatt ; }

void DB::getLRSet(ScoreInfo &si, std::vector<unsigned int> &set,
					  std::vector<unsigned int> &left, std::vector<unsigned int> &right) {
	left.clear();
	right.clear();
	
	for (int i = 0 ; i < set.size() ; ++i) {
		if (getAttVal(set[i], si.attribute) >= si.threshold)
			left.push_back(set[i]);
		else 
			right.push_back(set[i]);
	}
	// SHRINK TO FIT TRICK :
	std::vector<unsigned int>(left.begin(),left.end()).swap(left);
	std::vector<unsigned int>(right.begin(),right.end()).swap(right);
}


void DB::print(FILE* out, bool all) const {
	fprintf(out, "NbObject:\t%d\n", nbobject);
	fprintf(out, "NbAtt   :\t%d\n", nbatt);
	if (all) {
		for (int o = 0 ; o < nbobject ; ++o) {
			fprintf(out,"obj%d:%d,\t",o,(int) output[o]);
			for (int a = 0 ; a < nbatt; ++a) {
				fprintf(out,"%f ",(float) getAttVal(o, a));//(unsigned int) att[o*nbatt+a]);
			}
			fprintf(out,"\n");
		}
	}
	
}
