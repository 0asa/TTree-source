/*
 *  ExtraTrees.cpp
 *  TTree
 *
 *  Created by Vincent Botta on 03/03/09.
 *  Copyright 2009 University of Liège. All rights reserved.
 *
 */

#include "ExtraTrees.h"
#include "Sampler.h"


ExtraTrees::ExtraTrees(DB *db, int t, int _k, int n) : Learner(db,n), nbtree(t), k(_k) {
	_treeinfo = stderr;
	term = new ClassicTree*[nbtree];
}

ExtraTrees::~ExtraTrees() {
	for (int i = 0 ; i < nbtree ; ++i)
		delete term[i];
}

double ExtraTrees::learn() {
	clock_t clock_start = clock();
	for (int t = 0 ; t < nbtree ; ++t) {
		term[t] = new ClassicTree(db,nmin,k);
		term[t]->setLearnFonction(EXTRATREE); 
		term[t]->setLS(ls);
		term[t]->setAttCand(cand);
		term[t]->learn();
		fprintf(_treeinfo, "Tree %03d - Complexity : %03d - ",t+1,term[t]->getComplexity());
		// 09-07-2012
		fprintf(_treeinfo, "MaxDepth : %03d - ",term[t]->getMaxDepth());
		Result result(db);
		result = test(t+1,ts);
		fprintf(_treeinfo,"Error rate : %f %%\n", result.getErrorRate());
		result.printProb(_treeinfo);
	}
	return (clock() - clock_start);	
}

Result ExtraTrees::test() {
	return test(ts);
}

Result ExtraTrees::test(unsigned int obj) {
	std::vector<unsigned int> v;
	v.push_back(obj);
	return test(&v);
}

void ExtraTrees::test(Result &result, unsigned int obj) {}

Result ExtraTrees::test(std::vector<unsigned int> *set) {
	Result r(db);
	std::vector<Result> vr;
	for (int t = 0 ; t < nbtree ; ++t) 
		vr.push_back(term[t]->test(set));
	r.aggregate(vr);
	return r;
}

// 09-07-2012
// added to test the ensemble of trees while growing a forest
Result ExtraTrees::test(int t, std::vector<unsigned int> *set) {
	Result r(db);
	std::vector<Result> vr;
	for (int _t = 0 ; _t < t ; ++_t) 
		vr.push_back(term[_t]->test(set));
	r.aggregate(vr);
	return r;
}

// BEGIN-02-08-2012 : all nmin at once
Result ExtraTrees::testnmin(std::vector<unsigned int> *set,int nmin, int t) {
	Result r(db);
	std::vector<Result> vr;
	for (int _t = 0 ; _t < t ; ++_t) 
		vr.push_back(term[_t]->testnmin(set,nmin));
	r.aggregate(vr);
	return r;
}
// END-02-08-2012 

void ExtraTrees::computeVimp() {
	for (int t = 0 ; t < nbtree ; ++t) {
		term[t]->computeVimp();
		std::vector<float> interm = term[t]->getVimp(false);
		for (int i = 0 ; i < interm.size() ; ++i)
			vimp[i] += interm[i];		
	}
}

//void ExtraTrees::computeVimp(std::vector<float> &vim) {
//	computeVimp(vim,(*ls));
//}
void ExtraTrees::computeVimp(std::vector<float> &vim, std::vector<unsigned int> &set) {
	for (int t = 0 ; t < nbtree ; ++t) {
		term[t]->computeVimp(vim,set);
	}
}
