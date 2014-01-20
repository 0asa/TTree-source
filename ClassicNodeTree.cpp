/*
 *  ClassicNodeTree.cpp
 *  TTree
 *
 *  Created by Vincent Botta on 22/01/09.
 *  Copyright 2009 University of Liège. All rights reserved.
 *
 */

#include <algorithm>
#include "ClassicNodeTree.h"
#include "Stack.h"
#include "BestFirstStack.h"
#include "Scorer.h"
#include "ScoreInfo.h"
#include "TreeNodeTest.h"
#include "Sampler.h"


ClassicNodeTree::ClassicNodeTree(BlockDB* database, int _k, int n) : GroupLearner(database,n), k(_k) {
	setInternalComp(1);
	maxdepth = 0 ; 
	treeType = EXTRATREE;//TRUEBESTFIRST;
}

ClassicNodeTree::~ClassicNodeTree() { }

double ClassicNodeTree::learn() {
	clock_t clock_start = clock();
	Scorer s(db);

	std::vector<unsigned int> lsleft;
	std::vector<unsigned int> lsright;
	Stack stack;
	root = new Node();	root->setLS((*ls));	stack.push(root);
	while (!stack.empty() && complexity < maxcomplexity) {
		Node *n = stack.pop();
		std::vector<unsigned int> set = n->getLS();
		lsleft.clear();		lsright.clear();
		if (set.size() >= nmin) {
			// for each group construct a tree and keep the best
			std::vector<unsigned int> kgroupcand;
			if (k == 0) {
				kgroupcand = groupcand;
			} else {
				kgroupcand = Sampler::random(groupcand,k);
			}
			//Sampler::print(kgroupcand);
			//ScoreInfo si = s.getBestGroupSNPScore(set,kgroupcand,internalComp,treeType,kint);
			//ScoreInfo si = s.getBestGroupStumpScore(set,kgroupcand,internalComp,treeType);

			ScoreInfo si = s.getBestGroupScore(set,kgroupcand,internalComp,treeType,kint);
			if (si.score > 0) {
				fprintf(stderr,".");
				n->setTest(si.weak,si.threshold, si.group,treeType);
				
				//fprintf(stderr,"Group:%d\tScore:%1.4f\t(*%d)\n", si.group,si.score, set.size()); 
				// mise a jour ls-left-right 
				Result r = si.weak->test(&set);
				for (int i = 0 ; i < set.size(); ++i) {
					if (r.prob.getProb(i)>=si.threshold)
						lsleft.push_back(set[i]);
					else
						lsright.push_back(set[i]);
				}
				// SHRINK TO FIT TRICK :
				std::vector<unsigned int>(lsleft.begin(),lsleft.end()).swap(lsleft);
				std::vector<unsigned int>(lsright.begin(),lsright.end()).swap(lsright);
				Node *left = new Node();	left->setParent(n); left->setTreeType(treeType);
				left->setDepth(n->getDepth()+1); // 27-07-2012
				left->setLS(lsleft);	n->setLeft(left);	stack.push(left);
				Node *right = new Node();	right->setParent(n); right->setTreeType(treeType);
				right->setLS(lsright);	n->setRight(right);	stack.push(right);
				right->setDepth(n->getDepth()+1); // 27-07-2012
				if (maxdepth < n->getDepth()+1) { maxdepth = n->getDepth()+1; } 
				complexity++;

			}
		}
	}	
	fprintf(stderr,"\n");
	return (clock() - clock_start);
}

Result ClassicNodeTree::test(unsigned int obj) {
	Result r(db);
	test(r,obj);
	return r;
}


void ClassicNodeTree::test(Result &r, unsigned int obj) { 
	Node* n = root;
	std::vector<double> vote(2,0.0);
	while (!n->isLeaf()) {
		n = n->propagate(obj);
	}
	std::vector<unsigned int> list = n->getLS();
	for (int i = 0 ; i < list.size() ; ++i)
		vote[db->getOutput(list[i])]++;
	vote[0] /= list.size();
	vote[1] /= list.size();
	r.prob.push(obj,vote[1],(double)db->getOutput(obj));
}

Result ClassicNodeTree::test(std::vector<unsigned int> *set) {
	Result r(db);
	for (int i = 0 ; i < set->size() ; ++i) {
		test(r,(*set)[i]);
	}
	return r;
}

// BEGIN-02-08-2012 : all nmin at once
void ClassicNodeTree::testnmin(Result &r, unsigned int obj,int nmin) { 
	Node* n = root;
	std::vector<double> vote(2,0.0);
	while (!n->isLeaf() && n->getSize() >= nmin) {
		n = n->propagate(obj);
	}
	std::vector<unsigned int> list = n->getLS();
	for (int i = 0 ; i < list.size() ; ++i)
		vote[db->getOutput(list[i])]++;
	vote[0] /= list.size();
	vote[1] /= list.size();
	r.prob.push(obj,vote[1],(double)db->getOutput(obj));
}

Result ClassicNodeTree::testnmin(std::vector<unsigned int> *set, int nmin) {
	Result r(db);
	for (int i = 0 ; i < set->size() ; ++i) {
		testnmin(r,(*set)[i],nmin);
	}
	return r;
}
// END-02-08-2012

void ClassicNodeTree::computeGroupVimp() {
	resetGroupVimp();
	Node* n = root;
	Score sc;
	//Scorer s(db);
	//ScoreInfo si;
	//std::vector<unsigned int> left;
	//std::vector<unsigned int> right;
	Stack stack;
	stack.push(n);
	while (!stack.empty()) {
		n = stack.pop();
		if (!n->isLeaf()) {
			std::vector<unsigned int> set = n->getLS();
			ContingencyTable ctable = ((TreeNodeTest*)n->getTest())->weak->test(&set).getContingencyTable(((TreeNodeTest*)n->getTest())->getTh());
			//ctable.print();
			ctable.compact(((TreeNodeTest*)n->getTest())->getTh()); // inappropié !
			int group = ((TreeNodeTest*)n->getTest())->getGroup();
			groupVimp[group] += sc.getMutualInfo(ctable)*set.size();
			//printf("setsize=%03d\tGroup\t%d\t%0.6f th=%0.6f\n",set.size(),group,sc.getMutualInfo(ctable)*set.size(),((TreeNodeTest*)n->getTest())->getTh());
			//printf("Score = %0.6f\n",sc.getScore(ctable));
			stack.push(n->getLeft());
			stack.push(n->getRight());
		}
	}
}

void ClassicNodeTree::computeGroupVimp(std::vector<float> &gim, std::vector<unsigned int> &set) {
	Node* n = root;
	n->setLS(set);
	Score sc;
	std::vector<unsigned int> left;
	std::vector<unsigned int> right;
	Stack stack;
	stack.push(n);
	while (!stack.empty()) {
		n = stack.pop();
		if (!n->isLeaf()) {
			// test local set & compute score
			std::vector<unsigned int> ss = n->getLS();
			ContingencyTable ctable = ((TreeNodeTest*)n->getTest())->weak->test(&ss).getContingencyTable(((TreeNodeTest*)n->getTest())->getTh());
			ctable.compact(((TreeNodeTest*)n->getTest())->getTh()); // inappropié !
			// get group and add to gim
			int group = ((TreeNodeTest*)n->getTest())->getGroup();
			gim[group] += sc.getMutualInfo(ctable)*ss.size();
			// compute partition left and right and push to stack getLRSet
			left.clear();
			right.clear();
			for (int i = 0 ; i < ss.size() ; ++i) {
				Result r = ((TreeNodeTest*)n->getTest())->weak->test((double) ss[i]);	
				if (r.prob.getProb(0)>=((TreeNodeTest*)n->getTest())->getTh())
					left.push_back(ss[i]);
				else 
					right.push_back(ss[i]);
			}
			// SHRINK TO FIT TRICK :
			std::vector<unsigned int>(left.begin(),left.end()).swap(left);
			std::vector<unsigned int>(right.begin(),right.end()).swap(right);
			//printf("((TreeNodeTest*)n)->getLRSet(ss,left,right);\n"); fflush(stdout);
			// push left and right
			n->getLeft()->setLS(left); stack.push(n->getLeft());
			n->getRight()->setLS(right); stack.push(n->getRight());	
		}
		std::vector<unsigned int> empty;
		n->setLS(empty);
	}
}

void ClassicNodeTree::computeVimp() {
	Node* n = root;
	//Score sc;
	//Scorer s(db);
	//ScoreInfo si;
	//std::vector<unsigned int> left;
	//std::vector<unsigned int> right;
	Stack stack;
	stack.push(n);
	while (!stack.empty()) {
		n = stack.pop();
		if (!n->isLeaf()) {
			((TreeNodeTest*)n->getTest())->weak->computeVimp();
			std::vector<float> interm = ((TreeNodeTest*)n->getTest())->weak->getVimp(false);
			for (int i = 0 ; i < interm.size() ; ++i)
				vimp[i] += interm[i];
			stack.push(n->getLeft());
			stack.push(n->getRight());
		}
	}
}


//void  ClassicNodeTree::computeVimp(std::vector<float> &vim) {
//	computeVimp(vim,(*ls));
//}

void  ClassicNodeTree::computeVimp(std::vector<float> &vim, std::vector<unsigned int> &set) {
	Node* n = root;
	Stack stack;
	std::vector<unsigned int> left;
	std::vector<unsigned int> right;
	n->setLS(set);
	stack.push(n);
	while (!stack.empty()) {
		n = stack.pop();
		if (!n->isLeaf()) {
			std::vector<unsigned int> ss = n->getLS();
			((TreeNodeTest*)n->getTest())->weak->computeVimp(vim,ss);	
			// getLRset ?
			left.clear();
			right.clear();
			for (int i = 0 ; i < ss.size() ; ++i) {
				Result r = ((TreeNodeTest*)n->getTest())->weak->test((double) ss[i]);	
				if (r.prob.getProb(0)>=((TreeNodeTest*)n->getTest())->getTh())
					left.push_back(ss[i]);
				else 
					right.push_back(ss[i]);
			}
			// SHRINK TO FIT TRICK :
			std::vector<unsigned int>(left.begin(),left.end()).swap(left);
			std::vector<unsigned int>(right.begin(),right.end()).swap(right);
			//printf("((TreeNodeTest*)n)->getLRSet(ss,left,right);\n"); fflush(stdout);
			// push left and right
			n->getLeft()->setLS(left); stack.push(n->getLeft());
			n->getRight()->setLS(right); stack.push(n->getRight());			
		}
		std::vector<unsigned int> empty;
		n->setLS(empty);
	}

}

void ClassicNodeTree::print(FILE* out) const {
	GroupLearner::print(out);
	fprintf(out,"ClassicNodeTree.TreeType = %d\n",treeType);
	
}

