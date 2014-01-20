/*
 *  Bagging.h
 *  TTree
 *
 *  Created by Vincent Botta on 11/02/09.
 *  Copyright 2009 University of Liège. All rights reserved.
 *
 */

#ifndef __BAGGING__
#define __BAGGING__

#include "RandomForest.h"

class Bagging : public RandomForest {
public:
	Bagging(DB *db, int t = 1, int n = 1);
	~Bagging();
};

#endif

