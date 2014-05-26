/*
 *  RandomTTForest.cpp
 *  TTree
 *
 *  Created by Vincent Botta on 05/03/09.
 *  Copyright 2009 University of Liège. All rights reserved.
 *
 */

#include "RandomTTForest.h"
#include "ClassicTree.h"
#include "Sampler.h"

RandomTTForest::RandomTTForest(DB *db, int t, int _k, int n) : GroupLearner(db,n), nbtree(t), k(_k) {
	treeType = TRUEBESTFIRST;
	internalComp = 1;
	setKint(0);
	term = new ClassicNodeTree*[nbtree];
}

RandomTTForest::~RandomTTForest() {
	for (int i = 0 ; i < nbtree ; ++i)
		delete term[i];
}

double RandomTTForest::learn() {
	clock_t clock_start = clock();
	for (int t = 0 ; t < nbtree ; ++t) {
		fprintf(stderr, "# TTree %3d ",t+1);
		term[t] = new ClassicNodeTree((BlockDB*)db,k, nmin);
		term[t]->setKint(kint);
		//term[t]->setMaxComplexity(2);
		term[t]->setGroupCand(groupcand);
		term[t]->setInternalComp(internalComp);
		term[t]->setTreeType(treeType);
		std::vector<unsigned int> bsls = Sampler::bootstrap((*ls));
		term[t]->setLS(&bsls);
		term[t]->setAttCand(cand);
		term[t]->learn();
		fprintf(_treeinfo, "Tree %03d - Complexity : %03d - ",t+1,term[t]->getComplexity());
		// 27-07-2012
		fprintf(_treeinfo, "MaxDepth : %03d - ",term[t]->getMaxDepth());
		Result result(db);
		result = test(t+1,ts);
		fprintf(_treeinfo,"Error rate : %f %%\n", result.getErrorRate());
		result.printProb(_treeinfo);
	}
	
	return (clock() - clock_start);
}

Result RandomTTForest::test() {
	return test(ts);
}

Result RandomTTForest::test(unsigned int obj) {
	std::vector<unsigned int> v;
	v.push_back(obj);
	return test(&v);
}

void RandomTTForest::test(Result &result, unsigned int obj) {}

Result RandomTTForest::test(std::vector<unsigned int> *set) {
	Result r(db);
	std::vector<Result> vr;
	for (int t = 0 ; t < nbtree ; ++t) 
		vr.push_back(term[t]->test(set));
	r.aggregate(vr);
	return r;
}

// 27-07-2012
// added to test the ensemble of trees while growing a forest
Result RandomTTForest::test(int t, std::vector<unsigned int> *set) {
	Result r(db);
	std::vector<Result> vr;
	for (int _t = 0 ; _t < t ; ++_t) 
		vr.push_back(term[_t]->test(set));
	r.aggregate(vr);
	return r;
}

// BEGIN-02-08-2012 : all nmin at once
Result RandomTTForest::testnmin(std::vector<unsigned int> *set, int nmin, int t) {
	Result r(db);
	std::vector<Result> vr;
	for (int _t = 0 ; _t < t ; ++_t) 
		vr.push_back(term[_t]->testnmin(set,nmin));
	r.aggregate(vr);
	return r;
}
// END-02-08-2012

void RandomTTForest::computeVimp() {
	for (int t = 0 ; t < nbtree ; ++t) {
		term[t]->computeVimp();
		VimpList interm = term[t]->getVimpCompact(false);
		for (int i = 0 ; i < interm.size() ; ++i)
				vimp[interm.getNum(i)] += interm.getImp(i);		
	}
} 

//void RandomTTForest::computeVimp(std::vector<float> &vim) {
//	computeVimp(vim,(*ls));
//}
void RandomTTForest::computeVimp(std::vector<float> &vim, std::vector<unsigned int> &set) {
	for (int t = 0 ; t < nbtree ; ++t) 
		term[t]->computeVimp(vim,set);
}

void RandomTTForest::computeGroupVimp() {
	for (int t = 0 ; t < nbtree ; ++t) {
		term[t]->computeGroupVimp();
		VimpList interm = term[t]->getGroupVimpCompact(false);
		for (int i = 0 ; i < interm.size() ; ++i)
				groupVimp[interm.getNum(i)] += interm.getImp(i);
	}
}

//void RandomTTForest::computeGroupVimp(std::vector<float> &gim) {
//	computeVimp(gim,(*ls));
//}
void RandomTTForest::computeGroupVimp(std::vector<float> &gim, std::vector<unsigned int> &set) {
	for (int t = 0 ; t < nbtree ; ++t) 
		term[t]->computeGroupVimp(gim,set);
}

