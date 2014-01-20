/*
 *  Result.cpp
 *  TTree
 *
 *  Created by Vincent Botta on 14/01/09.
 *  Copyright 2009 University of Liège. All rights reserved.
 *
 */

#include "Result.h"
#include "Sampler.h"
#include <algorithm>

//Result::Result() {}
Result::Result(DB *d) { db = d; /*vimp = VimpList(db->getNbAtt());*/ }
Result::~Result() { }

void Result::printProb(FILE* out) const {
	prob.print(out);
}

/*void Result::printVimp(FILE* out) const {
	vimp.print(out);
}*/

void Result::setDB(DB *d) { db = d; }

void Result::pushProb(Probas &p) {
		prob.push(p);
}

double Result::getErrorRate(double th) const {
	double err = 0.0;
	double val;
	for (int i = 0 ; i < prob.getSize() ; ++i) {
		if (prob.getProb(i)>th)
			val = 1.0;
		else
			val = 0.0;
		if (val != prob.getRealClass(i))
			err++;
	}
	return err / (double) prob.getSize();
}

std::vector<double> Result::getPossibleProbTh() const {
	std::vector<double> ret;
	std::vector<double> pcopy(prob.probas);
	sort(pcopy.begin(),pcopy.end());
	double v = pcopy[0];
	for (int i = 1 ; i < pcopy.size() ; ++i) {
		if (pcopy[i] != v) {
			ret.push_back((v+pcopy[i])/2.0);
			v = pcopy[i];
		}
	}
	return ret;
}

ContingencyTable Result::getContingencyTable(double th) const {
	ContingencyTable ctable(2,2);
	unsigned int val;
	for (int i = 0 ; i < prob.getSize() ; ++i) {
		if (prob.getProb(i)>th)
			val = 1;
		else
			val = 0;
		ctable.addToTable(prob.getRealClass(i), val, db->getWeight(i));
	}
	return ctable;
}

/*void Result::setVimp(std::vector<double> &v, bool add) {
	for (int i = 0 ; i < v.size() ; ++i)
		vimp.set(i,v[i],add);
}*/

void Result::aggregate(std::vector<Result> &vr) {
	for (int i = 1 ; i < vr.size() ; ++i)
		for (int p = 0 ; p < vr[0].prob.getSize() ; ++p)
			vr[0].prob.probas[p] += vr[i].prob.probas[p];
	
	for (int p = 0 ; p < vr[0].prob.getSize() ; ++p)
		vr[0].prob.probas[p] /= vr.size();
	
	prob = vr[0].prob;
}