/*
 *  GStump.cpp
 *  TTree
 *
 *  Created by Vincent Botta on 08/04/09.
 *  Copyright 2009 University of Liège. All rights reserved.
 *
 */

#include "GStump.h"
#include "Sampler.h"

GStump::GStump(DB *database) : Learner(database) {
	nbtree = 0;
	//for (int i = 500 ; i < 1500 ; ++i)
	//	emptyls.push_back(i);
	//Sampler::print(emptyls);
}

GStump::GStump(DB *database, std::vector<unsigned int> *att) : Learner(database) {
	setAttCand(att);
}

GStump::~GStump() {
	for (int i = 0 ; i < nbtree ; ++i) 
		delete term[i];
}

void GStump::setAttCand(std::vector<unsigned int> *_cand) {
	cand = _cand;
	nbtree = (int) _cand->size();
	term = new ClassicTree*[nbtree];
}

double GStump::learn() {
	clock_t clock_start = clock();
	if (ls->size() > 0) 
		nmin = (int) ls->size();
	else 
		fprintf(stderr,"######### WARNING : NO LS SPECIFIED !!! #########\n");
	for (int t = 0 ; t < nbtree ; ++t) {
		term[t] = new ClassicTree(db,nmin);
		term[t]->setLearnFonction(); 
		term[t]->setLS(ls); // CONSUMMING...
		std::vector<unsigned int> a; a.push_back((*cand)[t]);
		term[t]->setAttCand(&a);
		term[t]->learn();
		//term[t]->setLS(emptyls);
		//fprintf(stderr, "Tree %03d - Att %03d - Complexity : %03d\n",t+1,a[0],term[t]->getComplexity());
	}
	return (clock() - clock_start);	
}

Result GStump::test() {
	//fprintf(stderr,"GStump::test()\n");
	return test(ts);
}

Result GStump::test(unsigned int obj) {
	std::vector<unsigned int> v;
	v.push_back(obj);
	return test(&v);
}


void GStump::test(Result &result, unsigned int obj) {
	
}


Result GStump::test(std::vector<unsigned int> *set) {
	//fprintf(stderr,"GStump::test(&set)\n");
	Result r(db);
	std::vector<Result> vr;
	for (int t = 0 ; t < nbtree ; ++t) 
		vr.push_back(term[t]->test(set));
	r.aggregate(vr);
	return r;
}


void GStump::computeVimp() { // should be done !
	for (int t = 0 ; t < nbtree ; ++t) {
		term[t]->computeVimp();
		VimpList interm = term[t]->getVimpCompact(false);
		for (int i = 0 ; i < interm.size() ; ++i)
			vimp[interm.getNum(i)] += interm.getImp(i);		
	}
}

//void GStump::computeVimp(std::vector<float> &vim) {}
void GStump::computeVimp(std::vector<float> &vim, std::vector<unsigned int> &set) {}


