/*
 *  Scorer.h
 *  TTree
 *
 *  Created by Vincent Botta on 10/12/08.
 *  Copyright 2008 University of Liège. All rights reserved.
 *
 */

#ifndef __SCORER__
#define __SCORER__

#include "Score.h"
#include "ScoreInfo.h"
#include "ContingencyTable.h"
#include "TToydb.h"
#include "DB.h"
#include "ClassicTree.h"
#include "GStump.h"
#include <vector>

class Scorer {
private:
	DB *db;
public:
	Scorer(DB *database);
	~Scorer();
	
	void changedb(DB *database);
	
	ScoreInfo getBestScore(std::vector<unsigned int> &obj, std::vector<unsigned int> &att) {
		return (*this.*bestScoreFonction)(obj,att);
	}
	
	ScoreInfo getRandScore(std::vector<unsigned int> &obj, std::vector<unsigned int> &att) {
		return (*this.*randScoreFonction)(obj,att);
	}
	
	ScoreInfo (Scorer::*bestScoreFonction)(std::vector<unsigned int> &obj, std::vector<unsigned int> &att);
	ScoreInfo (Scorer::*randScoreFonction)(std::vector<unsigned int> &obj, std::vector<unsigned int> &att);
	
	void setBestScoreFonction(unsigned int type = NUMERICAL) {
		if (type == NUMERICAL) 
			bestScoreFonction = &Scorer::getBestNumericalScore;
		else if (type == SNP) 
			bestScoreFonction = &Scorer::getBestSNPScore;
	}
	
	void setRandScoreFonction(unsigned int type = NUMERICAL) {
		if (type == NUMERICAL) 
			randScoreFonction = &Scorer::getRandNumericalScore;
		else if (type == SNP) 
			randScoreFonction = &Scorer::getRandSNPScore;
	}

	ScoreInfo getRandNumericalScore(std::vector<unsigned int> &obj, std::vector<unsigned int> &att);
	void getRandNumericalScore(std::vector<unsigned int> &obj, unsigned int att, ScoreInfo &si);
	
	ScoreInfo getRandSNPScore(std::vector<unsigned int> &ind, std::vector<unsigned int> &snp);
	void getRandSNPScore(std::vector<unsigned int> &ind, unsigned int snp, ScoreInfo &si);
	
	ScoreInfo getBestNumericalScore(std::vector<unsigned int> &obj, std::vector<unsigned int> &att);
	void getBestNumericalScore(std::vector<unsigned int> &obj, unsigned int att, ScoreInfo &si);
//	double getNumericalScore(std::vector<unsigned int> &obj, unsigned int att, double th);
	
	ScoreInfo getBestSNPScore(std::vector<unsigned int> &ind, std::vector<unsigned int> &snp);
	void getSNPScore(std::vector<unsigned int> &ind, unsigned snp, ScoreInfo &si) ;
	ScoreInfo getSNPScore(std::vector<unsigned int> &ind, unsigned snp) ;
	
	ContingencyTable getContingencyTable(std::vector<unsigned int> &ind, unsigned int snp, double th);
	
	ScoreInfo getBestGroupSNPScore(std::vector<unsigned int> &obj, std::vector<unsigned int> &group, unsigned int internalComplexity, int treeType = TRUEBESTFIRST, int kint = 0);
	void getGroupSNPScore(ClassicTree* t,std::vector<unsigned int> &ind, std::vector<unsigned int> &cand, ScoreInfo &si, int treeType = TRUEBESTFIRST);
	
	ScoreInfo getBestGroupStumpScore(std::vector<unsigned int> &ind, std::vector<unsigned int> &group, unsigned int internalComplexity,int treeType = TRUEBESTFIRST);
	void getGroupStumpScore(GStump* stump, std::vector<unsigned int> &ind, ScoreInfo &si, int treeType);
	
	ScoreInfo getBestGroupScore(std::vector<unsigned int> &obj, std::vector<unsigned int> &group, unsigned int internalComplexity, int treeType = TRUEBESTFIRST, int kint = 0);
	void getGroupScore(Learner* weak,std::vector<unsigned int> &ind, std::vector<unsigned int> &cand, ScoreInfo &si, int treeType = TRUEBESTFIRST);
	
};

#endif
