/*
 *  Learner.h
 *  TTree
 *
 *  Created by Vincent Botta on 21/01/09.
 *  Copyright 2009 University of Liège. All rights reserved.
 *
 */

#ifndef __LEARNER__
#define __LEARNER__

#include <vector>
#include <algorithm>
#include "DB.h"
#include "Node.h"
#include "Result.h"

class Node;

class Learner {
private:

public:
	Learner(DB *database,int n = 1);
	~Learner();
	void setLS(std::vector<unsigned int> *_ls);
	void setTS(std::vector<unsigned int> *_ts);
	void setAttCand(std::vector<unsigned int> *_cand);
	void setMaxComplexity(int c);
	int getComplexity() const;
	void print(FILE* out = stdout) const;
	void printVimp(FILE* out = stdout, bool nozero = true) const;
	std::vector<float> getVimp(bool norm = true) const;
	VimpList getVimpCompact(bool norm) const;

	virtual double learn() = 0;
	virtual Result test() = 0;
	virtual Result test(unsigned int obj) = 0;
	virtual void test(Result &result, unsigned int obj) = 0;
	virtual Result test(std::vector<unsigned int> *set) = 0;
	virtual void computeVimp() = 0;
	virtual void computeVimp(std::vector<float> &vim, std::vector<unsigned int> &set) = 0;
	void printVimp(std::vector<float> &vim, FILE *out = stdout, int norma = 2);

protected:
	DB *db;
	int nmin;
	int complexity;
	int maxcomplexity;

	std::vector<unsigned int> *ls;
	std::vector<unsigned int> *ts;
	std::vector<unsigned int> *cand;
	std::vector<float> vimp;
	Node *root;

	void normalizeVimp();
	void resetVimp();
};

#endif

