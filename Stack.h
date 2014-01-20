/*
 *  Stack.h
 *  TTree
 *
 *  Created by Vincent Botta on 18/12/08.
 *  Copyright 2008 University of Liège. All rights reserved.
 *
 */

#ifndef __STACK__
#define __STACK__
#include <vector>
#include "Node.h"

class Stack {
private:

public:
	Stack();
	~Stack();
	void push(Node *n );
	Node* pop();
	bool empty();
protected:
	std::vector< Node* > s;
	static unsigned int nbpush;
};

#endif

