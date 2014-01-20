/*
 *  GroupLearner.h
 *  TTree
 *
 *  Created by Vincent Botta on 09/03/09.
 *  Copyright 2009 University of Liège. All rights reserved.
 *
 */

#ifndef __GROUPLEARNER__
#define __GROUPLEARNER__

#include "Learner.h"

class GroupLearner : public Learner {
private:
protected:
	unsigned int internalComp;
	unsigned int kint;
	unsigned int treeType;
public:
	GroupLearner(DB *db, int n = 1);
	~GroupLearner();
	
	void setInternalComp(unsigned int comp) { internalComp = comp; }
	
	std::vector<double> groupVimp;
	void normalizeGroupVimp();
	void resetGroupVimp();	
	std::vector<double> getGroupVimp(bool norm = true) const;
	VimpList getGroupVimpCompact(bool norm) const;
	void printGroupVimp(FILE* out = stdout, bool nozero = true) const;
	void printGroupVimp(std::vector<float> &gim, FILE* out = stdout, int norma = 2) const;
	std::vector<unsigned int> groupcand;
	void setGroupCand(std::vector<unsigned int> &gc);
	void setKint(unsigned int kin) { kint = kin; }
	void setTreeType(unsigned int tt) { treeType = tt; }
	
	virtual void computeGroupVimp() = 0;
	
	virtual void computeGroupVimp(std::vector<float> &gim, std::vector<unsigned int> &set) = 0;
	
	void print(FILE* out = stdout) const {
		Learner::print(out);
		fprintf(out,"Internal Complexity = %d\n",internalComp);
		fprintf(out,"Internal K = %d\n",kint);
		fprintf(out,"########## Group Candidate Attributes :\n");
		for (int i = 0 ; i < groupcand.size() ; ++i)
			fprintf(out,"%d ",groupcand[i]);
		fprintf(out,"\n");
	}

};

#endif