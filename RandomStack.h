/*
 *  RandomStack.h
 *  TTree
 *
 *  Created by Vincent Botta on 18/02/10.
 *  Copyright 2010 University of Liège. All rights reserved.
 *
 */

#ifndef __RANDOMSTACK__
#define __RANDOMSTACK__

#include "Stack.h"

class RandomStack : public Stack {
private:
	
public:
	// override push method, so it randomly choose the position
	// into the stack instead... break the "detphfirst"
	RandomStack();
	~RandomStack();
	void push(Node* n);
	
};

#endif