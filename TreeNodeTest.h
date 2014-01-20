/*
 *  TreeNodeTest.h
 *  TTree
 *
 *  Created by Vincent Botta on 18/12/08.
 *  Copyright 2008 University of Liège. All rights reserved.
 *
 */

#ifndef __TREENODETEST__
#define __TREENODETEST__

#include "NodeTest.h"
#include "Learner.h"

class Learner;

class TreeNodeTest : public NodeTest {
private:
	double th;
	unsigned int group;
public:
	Learner *weak;
	TreeNodeTest();
	~TreeNodeTest();
	bool test(double v);
	void setWeak(Learner *w);
	void setGroup(unsigned int g) ;
	void setTreshold(double t); 
	double getTh() const { return th; }
	unsigned int getGroup() const ;
	//void getLRSet(std::vector<unsigned int> &s, std::vector<unsigned int> &left, std::vector<unsigned int> &right);
};

#endif

