/*
 *  TrueBestFirstStack.cpp
 *  TTree
 *
 *  Created by Vincent Botta on 27/01/09.
 *  Copyright 2009 University of Liège. All rights reserved.
 *
 */

#include "TrueBestFirstStack.h"
#include "Score.h"
#include "ScoreInfo.h"
#include "Scorer.h"
#include "Sampler.h"

TrueBestFirstStack::TrueBestFirstStack(DB* d) { db = d; k = 0; }

TrueBestFirstStack::~TrueBestFirstStack() {}

void TrueBestFirstStack::setK(unsigned int _k) { k = _k; }

Node* TrueBestFirstStack::pop() {
	Node *ret = s[s.size()-1];
	s.pop_back();
	values.pop_back();
	return ret;
}

void TrueBestFirstStack::push(Node* n) {
	fprintf(stderr,"\n");
	fprintf(stderr,"Error - this stack does not support basic use\n");
	fprintf(stderr,"      - True Best First push need att cand list\n");
	fprintf(stderr,"\n");
}

/*
 this push override the parent one to order the node by descending H(c|t)
 */
void TrueBestFirstStack::push(Node* n, std::vector<unsigned int> &cand) {
	Scorer sc(db);
	std::vector<unsigned int> lsleft;
	std::vector<unsigned int> lsright;
	std::vector<unsigned int> set = n->getLS();
	std::vector<unsigned int> kcand;
	if (k > 0)
		kcand = Sampler::random(cand,k);
	else
		kcand = cand; 
	ScoreInfo si = sc.getBestScore(set,kcand);
	//fprintf(stderr,"TBF score: %f\n",si.score);
	//db->getLRSet(si,set,lsleft,lsright);
	double total = 0.0;
	for (int i = 0 ; i < set.size() ; ++i) {
		total += db->getWeight(set[i]);
	}
	n->setTest(si);
	if (si.score*total > 0.0) {
	db->getLRSet(si,set,lsleft,lsright);
	Node *left  = new Node();	left->setParent(n);
	left->setLS(lsleft);	n->setLeft(left);	
	Node *right = new Node();	right->setParent(n);
	right->setLS(lsright);	n->setRight(right);
	
		std::vector<double>::iterator it;
		std::vector< Node* >::iterator itn;
		for( it = values.begin(), itn = s.begin() ; it != values.end(); it++, itn++ )    {
			if (si.score*total <= *it) 
				break;
		}
		values.insert(it,si.score*total);
		//for (int i = 0 ; i < values.size() ; ++i)
		//	printf("%f|",values[i]);
		//printf("\n");
		n->setId(nbpush++);
		s.insert(itn,n);
		
		
	}
	
}

void TrueBestFirstStack::clearStack() {	
	//printf("Node left : %d\n",s.size());
	for (int i = 0 ; i < s.size() ; ++i) {
		//printf("node size : %d\n",s[i]->getLS().size());
		s[i]->setLeft(NULL);
		s[i]->setRight(NULL);
		delete s[i]->getLeft();
		delete s[i]->getRight();
	}
}