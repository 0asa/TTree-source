/*
 *  RandomTTForest.h
 *  TTree
 *
 *  Created by Vincent Botta on 05/03/09.
 *  Copyright 2009 University of Liège. All rights reserved.
 *
 */

#ifndef __RANDOMTTFOREST__
#define __RANDOMTTFOREST__

#include "GroupLearner.h"
#include "ClassicTree.h"
#include "ClassicNodeTree.h"

class RandomTTForest : public GroupLearner {
private:
	unsigned int nbtree;
	ClassicNodeTree **term;
	unsigned int k;
	FILE* _treeinfo; // 27-07-2012
	
public:
	RandomTTForest(DB *db, int t = 1, int k = 0, int n = 1);
	~RandomTTForest();
	double learn();
	Result test();
	Result test(unsigned int obj);
	void test(Result &result, unsigned int obj);
	Result test(std::vector<unsigned int> *set);
	Result test(int t, std::vector<unsigned int> *set); // 27-07-2012
	
	Result testnmin(std::vector<unsigned int> *set, int nmin = 1, int t = 0); // 02-08-2012
	
	void computeVimp();
	//void computeVimp(std::vector<float> &vim);
	void computeVimp(std::vector<float> &vim, std::vector<unsigned int> &set);
	
	void computeGroupVimp();
	//void computeGroupVimp(std::vector<float> &gim);
	void computeGroupVimp(std::vector<float> &gim, std::vector<unsigned int> &set);
	void setTreeInfo(FILE* ti) { _treeinfo = ti; } // 27-07-2012
	void print(FILE* out = stdout) const { 
		GroupLearner::print(out);
		fprintf(out,"RANDOMTTFOREST\n");
		fprintf(out,"NbTree = %d\n",nbtree);
		//for (int i = 0 ; i < nbtree ; ++i) {
		//	fprintf(out,"___________________TREE %d INFOS\n",i+1);
		//	term[i]->print(out);
		//}
	}
	
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