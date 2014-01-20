/*
 *  Bagging.cpp
 *  TTree
 *
 *  Created by Vincent Botta on 11/02/09.
 *  Copyright 2009 University of Liège. All rights reserved.
 *
 */

#include "Bagging.h"
#include "ClassicTree.h"
#include "Sampler.h"

Bagging::Bagging(DB *db, int t, int n) : RandomForest(db, t, db->getNbAtt() , n) {}

Bagging::~Bagging() {
	for (int i = 0 ; i < nbtree ; ++i)
		delete term[i];
}