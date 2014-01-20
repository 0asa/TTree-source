/*
 *  ExtraTTrees.cpp
 *  TTree
 *
 *  Created by Vincent Botta on 11/03/09.
 *  Copyright 2009 University of Liège. All rights reserved.
 *
 */

#include "ExtraTTrees.h"

ExtraTTrees::ExtraTTrees(DB *db, int _t, int _k, int n) : GroupLearner(db, n), nbtree(_t), k(_k) {
	treeType = EXTRATREE;
	internalComp = 1;
	setKint(0);
	term = new ClassicNodeTree*[nbtree];
}

ExtraTTrees::~ExtraTTrees() {
	for (int i = 0 ; i < nbtree ; ++i)
		delete term[i];
}

double ExtraTTrees::learn() {
	clock_t clock_start = clock();
	for (int t = 0 ; t < nbtree ; ++t) {
		fprintf(stderr, "# TTree %3d ",t+1);
		term[t] = new ClassicNodeTree((BlockDB*)db,k);
		term[t]->setKint(kint);
		term[t]->setTreeType(treeType);
		term[t]->setGroupCand(groupcand);
		term[t]->setInternalComp(internalComp);
		term[t]->setLS(ls);
		term[t]->setAttCand(cand);
		term[t]->learn();
		fprintf(stderr, "\t(complexity = %d)\n",term[t]->getComplexity());
		//fprintf(stderr, ".");
	}
	
	return (clock() - clock_start);
}

Result ExtraTTrees::test() {
	return test(ts);
}

Result ExtraTTrees::test(unsigned int obj) {
	std::vector<unsigned int> v;
	v.push_back(obj);
	return test(&v);
}

void ExtraTTrees::test(Result &result, unsigned int obj) {}

Result ExtraTTrees::test(std::vector<unsigned int> *set) {
	Result r(db);
	std::vector<Result> vr;
	for (int t = 0 ; t < nbtree ; ++t) 
		vr.push_back(term[t]->test(set));
	r.aggregate(vr);
	return r;
}
void ExtraTTrees::computeVimp() {
	for (int t = 0 ; t < nbtree ; ++t) {
		term[t]->computeVimp();
		VimpList interm = term[t]->getVimpCompact(false);
		for (int i = 0 ; i < interm.size() ; ++i)
			vimp[interm.getNum(i)] += interm.getImp(i);		
	}
}

//void ExtraTTrees::computeVimp(std::vector<float> &vim) {
//	computeVimp(vim,(*ls));
//}
void ExtraTTrees::computeVimp(std::vector<float> &vim, std::vector<unsigned int> &set) {
	for (int t = 0 ; t < nbtree ; ++t)
		term[t]->computeVimp(vim,set);
}

void ExtraTTrees::computeGroupVimp() {
	for (int t = 0 ; t < nbtree ; ++t) {
		term[t]->computeGroupVimp();
		VimpList interm = term[t]->getGroupVimpCompact(false);
		for (int i = 0 ; i < interm.size() ; ++i)
			groupVimp[interm.getNum(i)] += interm.getImp(i);
	}
}

//void ExtraTTrees::computeGroupVimp(std::vector<float> &gim) {
//	computeVimp(gim,(*ls));
//};
void ExtraTTrees::computeGroupVimp(std::vector<float> &gim, std::vector<unsigned int> &set) {
	for (int t = 0 ; t < nbtree ; ++t) 
		term[t]->computeGroupVimp(gim,set);
}

