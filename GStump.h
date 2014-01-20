/*
 *  GStump.h
 *  TTree
 *
 *  Created by Vincent Botta on 08/04/09.
 *  Copyright 2009 University of Liège. All rights reserved.
 *
 */

#ifndef __GSTUMP__
#define __GSTUMP__

#include "Learner.h"
#include "ClassicTree.h"

class GStump : public Learner {
private:
	int nbtree;
	int nmin;
	ClassicTree **term;
	std::vector<unsigned int> emptyls;
public:
	GStump(DB *database, std::vector<unsigned int> *att);
	GStump(DB *database);
	~GStump();
	
	double learn() ;
	Result test() ;
	Result test(unsigned int obj) ;
	void setAttCand(std::vector<unsigned int> *_cand);
	void test(Result &result, unsigned int obj) ; 
	Result test(std::vector<unsigned int> *set) ;
	void computeVimp() ;
	//void computeVimp(std::vector<float> &vim);
	void computeVimp(std::vector<float> &vim, std::vector<unsigned int> &set);
};

#endif