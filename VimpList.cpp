/*
 *  VimpList.cpp
 *  TTree
 *
 *  Created by Vincent Botta on 14/01/09.
 *  Copyright 2009 University of Liège. All rights reserved.
 *
 */

#include "VimpList.h"

VimpList::VimpList() {}

VimpList::~VimpList() {}

void VimpList::push(unsigned int n, double v) {
	NUMIMP x; x.num = n; x.imp = v;
	imp.push_back(x);
}
