/*
 *  Node.h
 *  TTree
 *
 *  Created by Vincent Botta on 18/12/08.
 *  Copyright 2008 University of Liège. All rights reserved.
 *
 */

#ifndef __NODE__
#define __NODE__

#include <vector>
#include "NodeTest.h"
#include "TreeNodeTest.h"
#include "SimpleNodeTest.h"
#include "ScoreInfo.h"
#include "Learner.h"

class Learner;
class ScoreInfo;

class Node {
private:
	std::vector<unsigned int> ls; //*p
	Node *parent;
	Node *left;
	Node *right;
	NodeTest* test;
	std::vector<double> vote;
	unsigned int _id;
	unsigned int _depth; // 09-07-2012
	unsigned int _size; // 10-07-2012
	int treeType;
public:
	Node();
	~Node();
	Node* getParent() const;
	Node* getLeft() const;
	Node* getRight() const;
	std::vector<unsigned int>& getLS() { return ls; }; //*p
	void setParent(Node *p);
	void setLeft(Node *p);
	void setRight(Node *p);
	void setTest(ScoreInfo &si);
	void setTest(Learner *t, double th, unsigned int g,int _treeType = -1);
	NodeTest* getTest() const { return test; };
	void setLS(std::vector<unsigned int> s) { 
		ls = s ; 
		if (ls.size() > _size)  // 10-07-2012
			setSize((int) ls.size()); // 10-07-2012
	} ; //*p
	Node* propagate(double v) const;
	Node* propagate(unsigned int obj) const;
	int getAtt() const;
	double getTh() const;
	bool isRoot();
	bool isLeaf();
	void setVote(std::vector<double> &v);
	double getVote(unsigned int i) const { return vote[i]; }
	void print(FILE* out = stdout);
	void setId(unsigned int i) { _id = i; }
	unsigned int getId() { return _id; }
	void setDepth(unsigned int d) { _depth = d; } // 09-07-2012
	unsigned int getDepth() { return _depth; } // 09-07-2012
	void setSize(unsigned int s) { _size = s; }
	unsigned int getSize() { return _size; }
	void setTreeType(int _type) { treeType = _type;}
	int getTreeType() const { return treeType; }
};

#endif
