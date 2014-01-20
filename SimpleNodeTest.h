/*
 *  SimpleNodeTest.h
 *  TTree
 *
 *  Created by Vincent Botta on 18/12/08.
 *  Copyright 2008 University of Liège. All rights reserved.
 *
 */

#ifndef __SIMPLENODETEST__
#define __SIMPLENODETEST__

#include "NodeTest.h"

class SimpleNodeTest : public NodeTest {
private:
	int att;
	double th;
public:
	SimpleNodeTest();
	SimpleNodeTest(int a,double t);
	~SimpleNodeTest();
	void setAtt(int a);
	void setTh(double t);
	int getAtt() const;
	double getTh() const;
	bool test(double v);
};

#endif
