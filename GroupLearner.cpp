/*
 *  GroupLearner.cpp
 *  TTree
 *
 *  Created by Vincent Botta on 09/03/09.
 *  Copyright 2009 University of Liège. All rights reserved.
 *
 */

#include "GroupLearner.h"

GroupLearner::GroupLearner(DB *_db, int n) : Learner(_db,n) {}

GroupLearner::~GroupLearner() {}

void GroupLearner::resetGroupVimp() {
	groupVimp.assign(groupVimp.size(),0.0);
}

void GroupLearner::normalizeGroupVimp() {
	double max = *max_element(groupVimp.begin(), groupVimp.end());
	for (int i = 0 ; i < groupVimp.size() ; ++i) 
		groupVimp[i] /= max;
}

VimpList GroupLearner::getGroupVimpCompact(bool norm) const {
	std::vector<double> ret = groupVimp;
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

std::vector<double> GroupLearner::getGroupVimp(bool norm) const {
	std::vector<double> ret = groupVimp;
	if (!norm) {
		return ret;
	} else {
		double max = *max_element(ret.begin(), ret.end());
		for (int i = 0 ; i < ret.size() ; ++i) 
			ret[i] /= max;
		return ret;
	}
}

void GroupLearner::printGroupVimp(FILE* out, bool nozero) const {
	std::vector<double> gv = getGroupVimp();
	for (int i = 0 ; i < gv.size() ; ++i)
		if (nozero && gv[i] != 0.0) {
			fprintf(out,"%d\t%f\n",i,gv[i]);
		} else if (!nozero) {
			fprintf(out,"%d\t%f\n",i,gv[i]);
		}
}

// norma = 0 : no normalisation
// norma = 1 : by max
// norma = 2 : by sum - sum gimp = 1 - DEFAULT
void GroupLearner::printGroupVimp(std::vector<float> &gim, FILE* out, int norma) const {
	if (norma == 0) {
		for (int i = 0 ; i < gim.size() ; ++i) {
			fprintf(out,"%d\t%f\n",i,gim[i]);
		}
	} else if (norma == 1) {
		double max = *max_element(gim.begin(), gim.end());
		for (int i = 0 ; i < gim.size() ; ++i) {
			fprintf(out,"%d\t%f\n",i,gim[i]/max);
		}
	} else if (norma == 2) {
		double sum = 0.0;
		for (int i = 0 ; i < gim.size() ; ++i) sum += gim[i];
		for (int i = 0 ; i < gim.size() ; ++i) {
			fprintf(out,"%d\t%f\n",i,gim[i]/sum);
		}
	}
}


void GroupLearner::setGroupCand(std::vector<unsigned int> &gc) {
	groupcand = gc;
}
