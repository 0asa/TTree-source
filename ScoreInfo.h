/*
 *  ScoreInfo.h
 *  TTree
 *
 *  Created by Vincent Botta on 16/12/08.
 *  Copyright 2008 University of Liège. All rights reserved.
 *
 */

#ifndef __SCOREINFO__
#define __SCOREINFO__

#include <iostream>
//#include "ClassicTree.h"

class ClassicTree;
class GStump;
class Learner;

class ScoreInfo {
public:
	Learner *weak;
	ClassicTree *tree;
	GStump *stump;
	int group;
	int attribute;
	double threshold;
	double score;
	ScoreInfo();
	~ScoreInfo() {}
	void update(int att, double th, double score);
	void update(ClassicTree *tree ,double th, double score);
	void update(GStump *stump ,double th, double score);
	void update(Learner *w, double th, double score);
	void print() const;
} ;

#endif
