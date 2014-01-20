/*
 *  RandomForest.cpp
 *  TTree
 *
 *  Created by Vincent Botta on 02/03/09.
 *  Copyright 2009 University of Liège. All rights reserved.
 *
 */

#include "RandomForest.h"
#include "Sampler.h"


RandomForest::RandomForest(DB *db, int t, int _k, int n) : Learner(db,n), nbtree(t), k(_k) {
	_treeinfo = stderr;
	term = new ClassicTree*[nbtree];
}

RandomForest::~RandomForest() {
	for (int i = 0 ; i < nbtree ; ++i)
		delete term[i];
}

double RandomForest::learn() {
	clock_t clock_start = clock();
	for (int t = 0 ; t < nbtree ; ++t) {
		term[t] = new ClassicTree(db,nmin,k);
		std::vector<unsigned int> bsls = Sampler::bootstrap((*ls));
		term[t]->setLearnFonction(NOTBESTFIRST);
		term[t]->setLS(&bsls);
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

Result RandomForest::test() {
	return test(ts);
}

Result RandomForest::test(unsigned int obj) {
	std::vector<unsigned int> v;
	v.push_back(obj);
	return test(&v);
}

void RandomForest::test(Result &result, unsigned int obj) {}

Result RandomForest::test(std::vector<unsigned int> *set) {
	Result r(db);
	std::vector<Result> vr;
	for (int t = 0 ; t < nbtree ; ++t) 
		vr.push_back(term[t]->test(set));
	r.aggregate(vr);
	return r;
}

// 09-07-2012
// added to test the ensemble of trees while growing a forest
Result RandomForest::test(int t, std::vector<unsigned int> *set) {
	Result r(db);
	std::vector<Result> vr;
	for (int _t = 0 ; _t < t ; ++_t) 
		vr.push_back(term[_t]->test(set));
	r.aggregate(vr);
	return r;
}

// BEGIN-02-08-2012 : all nmin at once
Result RandomForest::testnmin(std::vector<unsigned int> *set,int nmin, int t) {
	Result r(db);
	std::vector<Result> vr;
	for (int _t = 0 ; _t < t ; ++_t) 
		vr.push_back(term[_t]->testnmin(set,nmin));
	r.aggregate(vr);
	return r;
}
// END-02-08-2012 

void RandomForest::computeVimp() {
	for (int t = 0 ; t < nbtree ; ++t) {
		term[t]->computeVimp();
		std::vector<float> interm = term[t]->getVimp(false);
		for (int i = 0 ; i < interm.size() ; ++i)
			vimp[i] += interm[i];		
	}
}

//void RandomForest::computeVimp(std::vector<float> &vim) {
//	computeVimp(vim,(*ls));
//}
void RandomForest::computeVimp(std::vector<float> &vim, std::vector<unsigned int> &set) {
	for (int t = 0 ; t < nbtree ; ++t) 
		term[t]->computeVimp(vim,set);
}
