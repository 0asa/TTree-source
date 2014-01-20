/*
 *  Stack.cpp
 *  TTree
 *
 *  Created by Vincent Botta on 18/12/08.
 *  Copyright 2008 University of Liège. All rights reserved.
 *
 */

#include "Stack.h"

Stack::Stack() {}

Stack::~Stack() {}

unsigned int Stack::nbpush = 0;

void Stack::push(Node *set) {
	set->setId(nbpush++);
	s.push_back(set);
}

Node* Stack::pop() {
	Node *ret = s[s.size()-1];
	s.pop_back();
	return ret;
}

bool Stack::empty() {
	return s.empty();
}