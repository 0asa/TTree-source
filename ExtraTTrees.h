/*
 *  ExtraTTrees.h
 *  TTree
 *
 *  Created by Vincent Botta on 11/03/09.
 *  Copyright 2009 University of Liège. All rights reserved.
 *
 */

#include "GroupLearner.h"
#include "ClassicNodeTree.h"

class ExtraTTrees : public GroupLearner {
private:
	unsigned int nbtree;
	ClassicNodeTree **term;
	unsigned int k;

public:
	ExtraTTrees(DB *db, int t = 1, int k = 0, int n = 1);
	~ExtraTTrees();
	
	double learn();
	Result test();
	Result test(unsigned int obj);
	void test(Result &result, unsigned int obj);
	Result test(std::vector<unsigned int> *set);
	
	void computeVimp();
	//void computeVimp(std::vector<float> &vim);
	void computeVimp(std::vector<float> &vim, std::vector<unsigned int> &set);
	
	void computeGroupVimp();
	//void computeGroupVimp(std::vector<float> &gim);
	void computeGroupVimp(std::vector<float> &gim, std::vector<unsigned int> &set);
	
	void print(FILE* out = stdout) const { 
		GroupLearner::print(out);
		fprintf(out,"EXTRATTREE\n");
		fprintf(out,"NbTree = %d\n",nbtree);
		//for (int i = 0 ; i < nbtree ; ++i) {
		//	fprintf(out,"___________________TREE %d INFOS\n",i+1);
		//	term[i]->print(out);
		//}
	}
	
};