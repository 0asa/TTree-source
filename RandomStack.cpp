/*
 *  RandomStack.cpp
 *  TTree
 *
 *  Created by Vincent Botta on 18/02/10.
 *  Copyright 2010 University of Liège. All rights reserved.
 *
 */

#include "RandomStack.h"
#include <algorithm>

RandomStack::RandomStack() {

}

RandomStack::~RandomStack() {}

void RandomStack::push(Node *set) {
	set->setId(nbpush++);
	s.push_back(set);
	std::random_shuffle(s.begin(),s.end());
}