/*
 *  ClassicNodeTree.h
 *  TTree
 *
 *  Created by Vincent Botta on 22/01/09.
 *  Copyright 2009 University of Liège. All rights reserved.
 *
 */

#ifndef __CLASSICNODETREE__
#define __CLASSICNODETREE__
#include "GroupLearner.h"
#include "ClassicTree.h"
#include "BlockDB.h"

class ClassicNodeTree : public GroupLearner {
private:
	
	unsigned int k;
	unsigned int maxdepth;
public:
	ClassicNodeTree(BlockDB* database, int k = 0, int n = 1);
	~ClassicNodeTree();
	
	double learn();
	Result test() { return test(ts); }
	Result test(unsigned int obj);
	void test(Result &result, unsigned int obj); 
	Result test(std::vector<unsigned int> *set);
	
	// BEGIN-02-08-2012 : all nmin at once
	void testnmin(Result &result, unsigned int obj, int nmin =1); 
	Result testnmin(std::vector<unsigned int> *set, int nmin =1);
	// END-02-08-2012
	
	void computeVimp();
	//void computeVimp(std::vector<float> &vim);
	void computeVimp(std::vector<float> &vim, std::vector<unsigned int> &set);
	
	void computeGroupVimp();
	//void computeGroupVimp(std::vector<float> &gim);
	void computeGroupVimp(std::vector<float> &gim, std::vector<unsigned int> &set);
	unsigned int getMaxDepth() { return maxdepth; }
	void print(FILE* out = stdout) const;
	void dotit(FILE* out = stdout) { root->print(out) ; fprintf(out,"\n\n"); }
};

#endif


