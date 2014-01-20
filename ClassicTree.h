/*
 *  ClassicTree.h
 *  TTree
 *
 *  Created by Vincent Botta on 21/01/09.
 *  Copyright 2009 University of Liège. All rights reserved.
 *
 */

#ifndef __CLASSICTREE__
#define __CLASSICTREE__
#include "Learner.h"

#define NOTBESTFIRST	0
#define FALSEBESTFIRST	1
#define TRUEBESTFIRST	2
#define EXTRATREE		3
#define STUMP			4 // not implemented
#define RANDOMTTSTUMP	5
#define EXTRATTSTUMP	6

class ClassicTree : public Learner {
private:
	void createProbsInLeaf() ;
	unsigned int k;
	char me[256];
	unsigned int maxdepth;
public:
	ClassicTree(DB* database, int n = 1, int k = 0);
	~ClassicTree();
	
	double (ClassicTree::*learnFonction)();
	void setLearnFonction(int meth = NOTBESTFIRST) {
		if (meth == NOTBESTFIRST) {
			learnFonction = &ClassicTree::learnnbf;
			sprintf(me,"NOTBESTFIRST");
		} else if (meth == FALSEBESTFIRST) {
			learnFonction = &ClassicTree::learnfbf;
			sprintf(me ,"FALSEBESTFIRST");
		} else if (meth == TRUEBESTFIRST) {
			learnFonction = &ClassicTree::learntbf;
			sprintf(me,"TRUEBESTFIRST");
		} else if (meth == EXTRATREE) {
			learnFonction = &ClassicTree::learnET;
			sprintf(me,"EXTRATREE");
		}
	}
	
	double learn();
	double learnfbf();
	double learnnbf();
	double learntbf();
	double learnET();
	Result test() { return test(ts); }
	Result test(unsigned int obj);
	void test(Result &result, unsigned int obj); 
	Result test(std::vector<unsigned int> *set);
	unsigned int getMaxDepth() { return maxdepth; }
	void computeVimp();
	//void computeVimp(std::vector<float> &vim);
	void computeVimp(std::vector<float> &vim, std::vector<unsigned int> &set);
	void print(FILE* out = stdout) const;
	void dotit(FILE* out = stdout) { root->print(out) ; fprintf(out,"\n\n"); }
	
	// BEGIN-02-08-2012 : all nmin at once
	Result testnmin(int nmin = 1) { return testnmin(ts,nmin); }
	Result testnmin(unsigned int obj,int nmin = 1);
	void testnmin(Result &result, unsigned int obj, int nmin = 1); 
	Result testnmin(std::vector<unsigned int> *set, int nmin = 1);
	// END-02-08-2012 
	
};

#endif

