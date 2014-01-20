/*
 *  BestFirstStack.h
 *  TTree
 *
 *  Created by Vincent Botta on 27/01/09.
 *  Copyright 2009 University of Liège. All rights reserved.
 *
 */

#ifndef __BESTFIRSTSTACK__
#define __BESTFIRSTSTACK__

#include "Stack.h"

class BestFirstStack : public Stack {
private:
	DB* db;
	std::vector<double> values;
public:
	BestFirstStack(DB* d);
	~BestFirstStack();
	void push(Node* n);
	Node* pop();
};

#endif
