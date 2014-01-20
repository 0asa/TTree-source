/*
 *  ExtraTrees.h
 *  TTree
 *
 *  Created by Vincent Botta on 03/03/09.
 *  Copyright 2009 University of Liège. All rights reserved.
 *
 */

#ifndef __EXTRATREES__
#define __EXTRATREES__

#include "Learner.h"
#include "ClassicTree.h"

class ExtraTrees : public Learner {
private:
	unsigned int nbtree;
	unsigned int k;
	ClassicTree **term;
	FILE* _treeinfo; // 27-07-2012
public:
	ExtraTrees(DB *db, int t = 1, int k = 1, int n = 1);
	~ExtraTrees();
	double learn();
	Result test();
	Result test(unsigned int obj);
	void test(Result &result, unsigned int obj);
	Result test(std::vector<unsigned int> *set);
	Result test(int t, std::vector<unsigned int> *set); // 09-07-2012
	
	// BEGIN-02-08-2012 : all nmin at once
	Result testnmin(std::vector<unsigned int> *set,int nmin = 1, int t = 0);
	// END-02-08-2012 
	
	void computeVimp();	
	//void computeVimp(std::vector<float> &vim);
	void computeVimp(std::vector<float> &vim, std::vector<unsigned int> &set);
	
	void print(FILE* out = stdout) { 
		Learner::print(out); 
		fprintf(out,"EXTRATREE\n");
		fprintf(out,"K parameter : %d\n",k); 
		fprintf(out,"NbTrees : %d\n",nbtree); 
	//	for (int i = 0 ; i < nbtree ; ++i) {
	//		fprintf(out,"___________________TREE %d INFOS\n",i+1);
	//		term[i]->print(out);
	//	}
	}
	
	void setTreeInfo(FILE* ti) { _treeinfo = ti; }
	void dotit(FILE* out = stdout, int maxt = 0) {
		if (maxt == 0 || maxt > nbtree) { maxt = nbtree; }
		fprintf(out,"digraph G {\n");
		for (int i = 0 ; i < maxt ; ++i) {
			term[i]->dotit(out);
		}
		fprintf(out,"}\n");
	}
};

#endif