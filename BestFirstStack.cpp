/*
 *  BestFirstStack.cpp
 *  TTree
 *
 *  Created by Vincent Botta on 27/01/09.
 *  Copyright 2009 University of Liège. All rights reserved.
 *
 */

#include "BestFirstStack.h"
#include "Score.h"

BestFirstStack::BestFirstStack(DB* d) { db = d; }

BestFirstStack::~BestFirstStack() {}

Node* BestFirstStack::pop() {
	Node *ret = s[s.size()-1];
	s.pop_back();
	values.pop_back();
	return ret;
}

/*
 this push override the parent one to order the node by H(c)
 */
void BestFirstStack::push(Node* n) {
	n->setId(nbpush++);
	std::vector<unsigned int> set = n->getLS();
	//printf("Node Size : %d\n",set.size());
	double total = 0.0;
	std::vector<double> sum(2,0.0);
	for (int i = 0 ; i < set.size() ; ++i) {
		total += db->getWeight(set[i]);
		sum[db->getOutput(set[i])] += db->getWeight(set[i]);
	}
	Score score;
	double hc = score.getShannonEntropy(sum,total) * total;
	if (hc > 0.0) {
	std::vector<double>::iterator it;
	std::vector< Node* >::iterator itn;
	for( it = values.begin(), itn = s.begin() ; it != values.end(); it++, itn++ )    {
		if (hc <= *it) 
			break;
	}
	values.insert(it,hc);
	
	s.insert(itn,n);
	}
}