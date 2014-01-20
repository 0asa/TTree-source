/*
 *  ScoreInfo.cpp
 *  TTree
 *
 *  Created by Vincent Botta on 16/12/08.
 *  Copyright 2008 University of Liège. All rights reserved.
 *
 */

#include "ScoreInfo.h"

ScoreInfo::ScoreInfo() : attribute(-1), threshold(-1.0), score(-1.0), group(-1) {}

void ScoreInfo::update(int a,double t,double s) {
	if (score < s) {
		attribute = a;
		threshold = t;
		score = s;
	}
}

void ScoreInfo::update(ClassicTree *t, double th, double s) {
	if (score < s) {
		threshold = th;
		tree = t;
		score = s;
	} 
}

void ScoreInfo::update(GStump *t, double th, double s) { 
	if (score < s) {
		threshold = th;
		stump = t;
		score = s;
	} 
}

void ScoreInfo::update(Learner* w, double th, double s) {
	if (score < s) {
		threshold = th;
		weak = w;
		score = s;
	}
}

void ScoreInfo::print() const {
	fprintf(stderr,"Att:%4d\tTh: %1.2f\tScore: %1.5f\n",attribute,threshold,score);
}