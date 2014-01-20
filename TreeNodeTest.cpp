/*
 *  TreeNodeTest.cpp
 *  TTree
 *
 *  Created by Vincent Botta on 18/12/08.
 *  Copyright 2008 University of Liège. All rights reserved.
 *
 */

// this class is made for a "tree node"
#include "TreeNodeTest.h"

TreeNodeTest::TreeNodeTest() : group(-1) , th(0.5) {}

TreeNodeTest::~TreeNodeTest() {
	//printf("~TreeNodeTest()\n"); fflush(stdout);
	delete weak;
}

bool TreeNodeTest::test(double v) {	
	Result r = weak->test((unsigned int) v);
	if (r.prob.getProb(0)>=th)
		return true; 
	else
		return false;
}

void TreeNodeTest::setWeak(Learner* w) { weak = w; }

void TreeNodeTest::setGroup(unsigned int g) { group = g; }

void TreeNodeTest::setTreshold(double t) { th = t; }

unsigned int TreeNodeTest::getGroup() const { return group; }

/*
void TreeNodeTest::getLRSet(std::vector<unsigned int> &s, std::vector<unsigned int> &left, std::vector<unsigned int> &right) {
	left.clear();
	right.clear();
	for (int i = 0 ; i < s.size() ; ++i) {
		Result r = weak->test(s[i]);
		if (r.prob.getProb(0)>=th)
			left.push_back(s[i]);
		else 
			right.push_back(s[i]);
	}
	// SHRINK TO FIT TRICK :
	std::vector<unsigned int>(left.begin(),left.end()).swap(left);
	std::vector<unsigned int>(right.begin(),right.end()).swap(right);
}
*/