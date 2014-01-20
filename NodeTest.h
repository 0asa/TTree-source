/*
 *  NodeTest.h
 *  TTree
 *
 *  Created by Vincent Botta on 18/12/08.
 *  Copyright 2008 University of Liège. All rights reserved.
 *
 */

#ifndef __NODETEST__
#define __NODETEST__

#include "Sampler.h"

class NodeTest {
private:
public:
	NodeTest();
	virtual ~NodeTest() {  };
	virtual bool test(double v) = 0;
};

#endif

