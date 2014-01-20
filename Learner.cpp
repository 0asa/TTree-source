/*
 *  Learner.cpp
 *  TTree
 *
 *  Created by Vincent Botta on 21/01/09.
 *  Copyright 2009 University of Liège. All rights reserved.
 *
 */

#include "Learner.h"

Learner::Learner(DB *database, int n) : nmin(n), complexity(0), maxcomplexity(999999999) {
	db = database;
	root = NULL;
}

Learner::~Learner() {
	if (root != NULL) 
		delete root;
}

void Learner::setLS(std::vector<unsigned int> *_ls) {
	ls = _ls;
}

void Learner::setTS(std::vector<unsigned int> *_ts) {
	ts = _ts;
}

void Learner::setAttCand(std::vector<unsigned int> *_attcand) {
	cand = _attcand;
}

int Learner::getComplexity() const { return complexity; }

void Learner::setMaxComplexity(int c) { maxcomplexity = c; }

void Learner::resetVimp() {
	vimp.assign(vimp.size(),0.0);
}

void Learner::normalizeVimp() {
	double max = *max_element(vimp.begin(), vimp.end());
	for (int i = 0 ; i < vimp.size() ; ++i) 
		vimp[i] /= max;
}

VimpList Learner::getVimpCompact(bool norm) const {
	std::vector<float> ret = vimp;
	if (norm) {
		double max = *max_element(ret.begin(), ret.end());
		for (int i = 0 ; i < ret.size() ; ++i) 
			ret[i] /= max;
	}
	VimpList v;
	for (int i=0 ;  i < ret.size() ; ++i) {
		if (ret[i] != 0.0) {
			v.push(i, ret[i]);
		}
	}
	return v;
}

std::vector<float> Learner::getVimp(bool norm) const {
	std::vector<float> ret = vimp;
	if (!norm) {
		return ret;
	} else {
		double max = *max_element(ret.begin(), ret.end());
		for (int i = 0 ; i < ret.size() ; ++i) 
			ret[i] /= max;
		return ret;
	}
}

void Learner::printVimp(FILE* out, bool nozero) const {
	std::vector<float> v = getVimp();
	for (int i = 0 ; i < v.size() ; ++i)
		if (nozero && v[i] != 0.0) {
			fprintf(out,"%d\t%f\n",i,v[i]);
		} else if (!nozero) {
			fprintf(out,"%d\t%f\n",i,v[i]);
		}
}

// norma = 0 : no normalisation
// norma = 1 : by max normalisation
// norma = 2 : by sum > sum(vim) = 1 (DEFAULT)
void Learner::printVimp(std::vector<float> &vim, FILE *out, int norma) {
	if (norma == 0) {
		for (int i = 0 ; i < vim.size() ; ++i) {
			fprintf(out,"%d\t%2.10f\n",i,vim[i]);
		}
	} else if (norma == 1) {
		double max = *max_element(vim.begin(), vim.end());
		for (int i = 0 ; i < vim.size() ; ++i) {
			fprintf(out,"%d\t%2.10f\n",i,vim[i]/max);
		}
	} else if (norma == 2) {
		double sum = 0.0;
		for (int i = 0 ; i < vim.size() ; ++i) sum += vim[i];
		for (int i = 0 ; i < vim.size() ; ++i) {
			fprintf(out,"%d\t%2.10f\n",i,vim[i]/sum);
		}
	}

};

void Learner::print(FILE* out) const {
	fprintf(out,"########## Learning Set :\n");
			for (int i = 0 ; i < ls->size() ; ++i)
			fprintf(out,"%d ",(*ls)[i]);
	fprintf(out,"\n");
	fprintf(out,"########## Test     Set :\n");
	for (int i = 0 ; i < ts->size() ; ++i)
		fprintf(out,"%d ",(*ts)[i]);
	fprintf(out,"\n");
	fprintf(out,"########## Candidate Attributes :\n");
	for (int i = 0 ; i < cand->size() ; ++i)
		fprintf(out,"%d ",(*cand)[i]);
	fprintf(out,"\n");
	fprintf(out,"NMin : %d\n",nmin);
	fprintf(out,"Max Complexity : %d\n",maxcomplexity);
}

