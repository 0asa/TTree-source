/*
 *  Node.cpp
 *  TTree
 *
 *  Created by Vincent Botta on 18/12/08.
 *  Copyright 2008 University of Liège. All rights reserved.
 *
 */

#include "Node.h"

Node::Node() { _size = 0 ; _depth = 0; _id = 0; parent = NULL; right = NULL ; left = NULL; treeType = -1; }

Node::~Node() { 
	if (!isLeaf()) {
		delete test;
		delete left;
		delete right;
	} 
}

Node* Node::getParent() const { return parent ; }
Node* Node::getLeft() const { return left; }
Node* Node::getRight() const { return right ; }

void Node::setTest(ScoreInfo &si) {
	test = new SimpleNodeTest(si.attribute,si.threshold);
}

void Node::setTest(Learner *t, double th, unsigned int g, int _treeType) {
	treeType = _treeType;
	test = new TreeNodeTest();
	((TreeNodeTest*)test)->setTreshold(th);
	((TreeNodeTest*)test)->setWeak(t);
	((TreeNodeTest*)test)->setGroup(g);
}

void Node::setParent(Node *p) { parent = p ; }
void Node::setLeft(Node *n) { left = n ; }
void Node::setRight(Node *n) { right = n ; }

bool Node::isRoot() { return parent == NULL; }
bool Node::isLeaf() { return left==NULL && right==NULL; }

//void Node::setLS(std::vector<unsigned int> s) { ls = s; } //*p
//std::vector<unsigned int>& Node::getLS() { return ls; } //*p

int Node::getAtt() const { return ((SimpleNodeTest*)test)->getAtt() ; }
double Node::getTh() const { return ((SimpleNodeTest*)test)->getTh() ; }

Node* Node::propagate(double v) const {
	if (test->test(v))
		return getLeft();
	else
		return getRight();
}

Node* Node::propagate(unsigned int obj) const {
	if (test->test((double)obj))
		return getLeft();
	else
		return getRight();
}

void Node::setVote(std::vector<double> &v) { vote = v ; }


void Node::print(FILE* out) {
	if (treeType == -1) {  // NORMAL TEST NODES
		if (!isLeaf()) { 
			fprintf(out,"N%d [label=\"att=%d\\nth=%0.2f\\nsize=%d\\nc1=%0.3f\\nc2=%0.3f\"]\n",(int)getId(),getAtt(),getTh(),(int)getSize(),vote[0],vote[1]); //*p
			//fprintf(out,"N%d [peripheries=2,label=\"size=%d\\nc1=%0.3f\\nc2=%0.3f\"]\n",(int)getId(),(int)getSize()); //*p	
			fprintf(out,"N%d -> N%d;\n",getId(),getLeft()->getId());
			fprintf(out,"N%d -> N%d;\n",getId(),getRight()->getId());
			getLeft()->print(out);
			getRight()->print(out);
		} else {
			fprintf(out,"N%d [peripheries=2,label=\"size=%d\\nc1=%0.3f\\nc2=%0.3f\"]\n",(int)getId(),(int)getSize(),vote[0],vote[1]); //*p	
		}
	} else { // TTREE NODES
		if (!isLeaf()) { 
			fprintf(out,"N%d [label=\"grp=%d\\nth=%0.2f\\nsize=%d\"]\n",(int)getId(),((TreeNodeTest*)test)->getGroup(),((TreeNodeTest*)test)->getTh(),(int)getSize()); 
			
			fprintf(out,"N%d -> N%d;\n",getId(),getLeft()->getId()); 
			fprintf(out,"N%d -> N%d;\n",getId(),getRight()->getId()); 
			
			getLeft()->print(out);
			getRight()->print(out);
		} else { 
			//fprintf(out,"N%d [peripheries=2,label=\"size=%d\\nc1=%0.3f\\nc2=%0.3f\"]\n",(int)getId(),(int)getSize(),vote[0],vote[1]); 
			fprintf(out,"N%d [peripheries=2,label=\"size=%d\"]\n",(int)getId(),(int)getSize()/*,vote[0],vote[1]*/); 
		}
	}
	
}