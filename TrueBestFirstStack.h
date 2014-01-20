/*
 *  TrueBestFirstStack.h
 *  TTree
 *
 *  Created by Vincent Botta on 27/01/09.
 *  Copyright 2009 University of Liège. All rights reserved.
 *
 */

#ifndef __TRUEBESTFIRSTSTACK__
#define __TRUEBESTFIRSTSTACK__

#include "Stack.h"

class TrueBestFirstStack : public Stack {
private:
	DB* db;
	std::vector<double> values;
	unsigned int k;
public:
	TrueBestFirstStack(DB* d);
	~TrueBestFirstStack();
	void setK(unsigned int k);
	void push(Node* n);
	void push(Node* n,std::vector<unsigned int> &cand);
	void clearStack();
	Node* pop();
};

#endif
