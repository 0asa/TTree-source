/*
 *  SimpleNodeTest.cpp
 *  TTree
 *
 *  Created by Vincent Botta on 18/12/08.
 *  Copyright 2008 University of Liège. All rights reserved.
 *
 */

#include "SimpleNodeTest.h"

SimpleNodeTest::SimpleNodeTest() : att(0) , th(-1.0) {}
SimpleNodeTest::SimpleNodeTest(int a,double t) : att(a) , th(t) {}

SimpleNodeTest::~SimpleNodeTest() {
	//printf("~SimpleNodeTest()\n"); fflush(stdout);
}

void SimpleNodeTest::setAtt(int a) { att = a; }
void SimpleNodeTest::setTh(double t) { th = t ; }

int SimpleNodeTest::getAtt() const { return att; }
double SimpleNodeTest::getTh() const { return th;  }

bool SimpleNodeTest::test(double v) { 
	if (v>=th) 
		return true; 
	else
		return false;
}