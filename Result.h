/*
 *  Result.h
 *  TTree
 *
 *  Created by Vincent Botta on 14/01/09.
 *  Copyright 2009 University of Liège. All rights reserved.
 *
 */

#ifndef __RESULT__
#define __RESULT__

#include <vector>
#include "Probas.h"
#include "VimpList.h"
#include "ContingencyTable.h"
#include "DB.h"

class Result {
private:
	DB *db;

public:
	Probas prob;
	//VimpList vimp;
	
	Result(DB *db);
	~Result();
	
	void aggregate(std::vector<Result> &vr);
	
	void pushProb(Probas &r);
	void printProb(FILE* out = stdout) const;
	//void printVimp(FILE* out = stdout) const;
	//void setVimp(std::vector<double> &v, bool add = false);
	void setDB(DB *d);
	double getErrorRate(double th = 0.5) const;
	ContingencyTable getContingencyTable(double th = 0.5) const;
	std::vector<double> getPossibleProbTh() const;
	void shrink() { prob.shrink(); }; 
};

#endif
