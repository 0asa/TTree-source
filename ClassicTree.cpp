/*
 *  ClassicTree.cpp
 *  TTree
 *
 *  Created by Vincent Botta on 21/01/09.
 *  Copyright 2009 University of Liège. All rights reserved.
 *
 */

#include "ClassicTree.h"
#include "BestFirstStack.h"
#include "TrueBestFirstStack.h"
#include "RandomStack.h"
#include "Scorer.h"
#include "ScoreInfo.h"
#include "Sampler.h"
#include <algorithm>

//#define NOLSFORTT

ClassicTree::ClassicTree(DB* database, int n, int _k) : Learner(database,n), k(_k) { setLearnFonction(); maxdepth = 0 ; }

ClassicTree::~ClassicTree() {}

double ClassicTree::learn() {
	return (*this.*learnFonction)();
}

double ClassicTree::learnnbf() { 
	clock_t clock_start = clock();
	Scorer s(db);
	std::vector<unsigned int> lsleft;
	std::vector<unsigned int> lsright;
	
	
	Stack stack;
	root = new Node();	root->setLS((*ls));	stack.push(root);
	while (!stack.empty() && complexity < maxcomplexity) {
		Node *n = stack.pop();
		std::vector<unsigned int> set = n->getLS();
		if (set.size() >= nmin) {
			std::vector<unsigned int> kcand;
			if (k > 0)
				kcand = Sampler::random((*cand),k);
			else
				kcand = (*cand); 
			ScoreInfo si = s.getBestScore(set,kcand);
			if (si.score > 0) {
				db->getLRSet(si,set,lsleft,lsright);
				n->setTest(si);
				Node *left  = new Node();	left->setParent(n); 
				left->setDepth(n->getDepth()+1); // 09-07-2012
				left->setLS(lsleft);	n->setLeft(left);	stack.push(left);
				Node *right = new Node();	right->setParent(n); 
				right->setDepth(n->getDepth()+1); // 09-07-2012
				right->setLS(lsright);	n->setRight(right);	stack.push(right);
				if (maxdepth < n->getDepth()+1) { maxdepth = n->getDepth()+1; }
				complexity++;
				// BEGIN-02-08-2012 : all nmin at once
				// need to attach vote at each node
				std::vector<double> vote(2,0.0);
				for (int i = 0 ; i < set.size() ; ++i)
					vote[db->getOutput(set[i])] += db->getWeight(set[i]);
				for (int i = 0 ; i < vote.size() ; ++i) 
					vote[i] /= set.size();
				n->setVote(vote);
				// END-02-08-2012
			}
		}
	}	
	createProbsInLeaf();
	return (clock() - clock_start);
}

double ClassicTree::learnfbf() {
	clock_t clock_start = clock();
	Scorer s(db);
	std::vector<unsigned int> lsleft;
	std::vector<unsigned int> lsright;
	BestFirstStack stack(db);
	root = new Node();	root->setLS((*ls));	stack.push(root);
	while (!stack.empty() && complexity < maxcomplexity) {
		Node *n = stack.pop();
		std::vector<unsigned int> set = n->getLS();
		if (set.size() >= nmin) {
			std::vector<unsigned int> kcand;
			if (k > 0)
				kcand = Sampler::random((*cand),k);
			else
				kcand = (*cand); 
			ScoreInfo si = s.getBestScore(set,kcand);
			if (si.score > 0) {
				db->getLRSet(si,set,lsleft,lsright);
				n->setTest(si);
				Node *left  = new Node();	left->setParent(n);
				left->setDepth(n->getDepth()+1); // 09-07-2012
				left->setLS(lsleft);	n->setLeft(left);	stack.push(left);
				Node *right = new Node();	right->setParent(n);
				right->setDepth(n->getDepth()+1); // 09-07-2012
				right->setLS(lsright);	n->setRight(right);	stack.push(right);
				if (maxdepth < n->getDepth()+1) { maxdepth = n->getDepth()+1; }
				complexity++;
			}
		}
	}	
	createProbsInLeaf();
	return (clock() - clock_start);
}

double ClassicTree::learntbf() { 
	clock_t clock_start = clock();
	Scorer s(db);
	std::vector<unsigned int> lsleft;
	std::vector<unsigned int> lsright;
	TrueBestFirstStack stack(db);
	stack.setK(k);
	root = new Node();	root->setLS((*ls));	stack.push(root, (*cand));

	while (!stack.empty() && complexity < maxcomplexity) {
		Node *n = stack.pop();
		std::vector<unsigned int> set = n->getLS();
	
		if (set.size() >= nmin && !n->isLeaf()) { // !isLeaf() because of the TBF !
			stack.push(n->getLeft(), (*cand));
			stack.push(n->getRight(), (*cand));
			complexity++;
		} 
	}
	stack.clearStack();
	createProbsInLeaf();
	return (clock() - clock_start);
}

double ClassicTree::learnET() {
	clock_t clock_start = clock();
	Scorer s(db);
	std::vector<unsigned int> lsleft;
	std::vector<unsigned int> lsright;
	RandomStack stack; // IMPORTANT FOR TTREES WITH INTERNAL ET
	root = new Node();	root->setLS((*ls));	stack.push(root);
	while (!stack.empty() && complexity < maxcomplexity) {
		Node *n = stack.pop();
		std::vector<unsigned int> set = n->getLS();
		if (set.size() >= nmin) {
			std::vector<unsigned int> kcand;
			if (k > 0)
				kcand = Sampler::random((*cand),k);
			else
				kcand = (*cand); 
			ScoreInfo si = s.getRandScore(set,kcand);
			if (si.score > 0) {
				db->getLRSet(si,set,lsleft,lsright);
				n->setTest(si);
				Node *left  = new Node();	left->setParent(n);
				left->setDepth(n->getDepth()+1); // 09-07-2012
				left->setLS(lsleft);	n->setLeft(left);	stack.push(left);
				Node *right = new Node();	right->setParent(n);
				right->setDepth(n->getDepth()+1); // 09-07-2012
				right->setLS(lsright);	n->setRight(right);	stack.push(right);
				if (maxdepth < n->getDepth()+1) { maxdepth = n->getDepth()+1; }
				complexity++;
				// BEGIN-02-08-2012 : all nmin at once
				// need to attach vote at each node
				std::vector<double> vote(2,0.0);
				for (int i = 0 ; i < set.size() ; ++i)
					vote[db->getOutput(set[i])] += db->getWeight(set[i]);
				for (int i = 0 ; i < vote.size() ; ++i) 
					vote[i] /= set.size();
				n->setVote(vote);
				// END-02-08-2012
				//#ifdef NOLSFORTT // LS STORAGE ANNULATION
				std::vector<unsigned int> ls_null; n->setLS(ls_null);
				//#endif
			}
		}
	}
	// Clearing sets
	std::vector<unsigned int>().swap(lsleft);
	std::vector<unsigned int>().swap(lsright);
	createProbsInLeaf();
	return (clock() - clock_start);
}

void ClassicTree::createProbsInLeaf() {
	Node* n = root;
	Stack stack;
	stack.push(n);
	while (!stack.empty()) {
		n = stack.pop();
		if (!n->isLeaf()) {
			stack.push(n->getLeft());
			stack.push(n->getRight());
		} else {
			// create vote and add it to corresponding to the leaf
			std::vector<double> vote(2,0.0);
			std::vector<unsigned int> list = n->getLS();
			for (int i = 0 ; i < list.size() ; ++i)
				vote[db->getOutput(list[i])] += db->getWeight(list[i]);
			for (int i = 0 ; i < vote.size() ; ++i) 
				vote[i] /= list.size();

			n->setVote(vote);
		}
	}
}

// BEGIN-02-08-2012 : all nmin at once
void ClassicTree::testnmin(Result &r, unsigned int obj, int nmin) { 
	Node* n = root;
	while (!n->isLeaf() && n->getSize() >= nmin ) {
		n = n->propagate((double)db->getAttVal(obj, n->getAtt()));
	}
	r.prob.push(obj,n->getVote(1),(double)db->getOutput(obj));
}

Result ClassicTree::testnmin(unsigned int obj, int nmin) {
	Result r(db);
	testnmin(r,obj,nmin);
	return r;
}

Result ClassicTree::testnmin(std::vector<unsigned int> *set, int nmin) {
	Result r(db);
	for (int i = 0 ; i < set->size() ; ++i) {
		testnmin(r,set->at(i),nmin);
	}
	return r;
}
// END-02-08-2012

Result ClassicTree::test(unsigned int obj) {
	Result r(db);
	test(r,obj);
	return r;
}

void ClassicTree::test(Result &r, unsigned int obj) { 
	Node* n = root;
	while (!n->isLeaf()) {
		n = n->propagate((double)db->getAttVal(obj, n->getAtt()));
	}
	r.prob.push(obj,n->getVote(1),(double)db->getOutput(obj));
}

Result ClassicTree::test(std::vector<unsigned int> *set) {
	Result r(db);
	for (int i = 0 ; i < set->size() ; ++i) {
		test(r,set->at(i));
	}
	return r;
}

void ClassicTree::computeVimp() {
	Node* n = root;
	Score sc;
	Scorer s(db);
	ScoreInfo si;
	std::vector<unsigned int> left;
	std::vector<unsigned int> right;
	Stack stack;
	stack.push(n);
	while (!stack.empty()) {
		n = stack.pop();
		if (!n->isLeaf()) {
			si.attribute = n->getAtt();
			si.threshold = n->getTh();
			std::vector<unsigned int> set = n->getLS();
			ContingencyTable ct = s.getContingencyTable(set,si.attribute,si.threshold);
			vimp[n->getAtt()] += sc.getMutualInfo(ct) * set.size();
			stack.push(n->getLeft());
			stack.push(n->getRight());
		}
	}
}

//void ClassicTree::computeVimp(std::vector<float> &vim) {
//	computeVimp(vim,(*ls));
//}

void ClassicTree::computeVimp(std::vector<float> &vim, std::vector<unsigned int> &set) {
	Node* n = root;
	Score sc;
	Scorer s(db);
	ScoreInfo si;
	std::vector<unsigned int> left;
	std::vector<unsigned int> right;
	Stack stack;
	n->setLS(set); // pushing the set on which we want to compute the variable importances
	stack.push(n);
	while (!stack.empty()) {
		n = stack.pop();
		if (!n->isLeaf()) {
			si.attribute = n->getAtt();
			si.threshold = n->getTh();
			std::vector<unsigned int> ss = n->getLS();
			ContingencyTable ct = s.getContingencyTable(ss,si.attribute,si.threshold);
			vim[si.attribute] += sc.getMutualInfo(ct) * ss.size();
			db->getLRSet(si,ss,left,right); // compute left and right partition
			//fprintf(stderr,"ATT: %d %f %f %d %d %d\n",si.attribute,si.threshold,sc.getMutualInfo(ct),ss.size(),left.size(),right.size());
			// push left and right
			n->getLeft()->setLS(left); stack.push(n->getLeft());
			n->getRight()->setLS(right); stack.push(n->getRight());			
		}
		// empty set inside parent node
		std::vector<unsigned int> empty;
		n->setLS(empty);
	}
}

void ClassicTree::print(FILE* out) const {
	Learner::print(out);
	fprintf(out,"CLASSICTREE\n");
	fprintf(out,"K=%d\n",k);
	fprintf(out,"TreeType=%s\n",me);
	
}


