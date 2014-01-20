/*
 *  BlockDB.h
 *  TTree
 *
 *  Created by Vincent Botta on 12/01/09.
 *  Copyright 2009 University of Liège. All rights reserved.
 *
 */

#ifndef __BLOCKDB__
#define __BLOCKDB__

#include "DB.h"

class BlockDB : public DB {
private:
	unsigned int nbGroup;
//	std::vector<unsigned int> start;
//	std::vector<unsigned int> end;
	
public:
	BlockDB(unsigned int nbo, unsigned int nba, unsigned int nbg = 0);
	BlockDB(DB &db);
	BlockDB();
	~BlockDB();
	//unsigned int getStart(unsigned int b) const;
	//unsigned int getEnd(unsigned int b) const;
	unsigned int getNbGroup() const;
	std::vector<unsigned int> selectAllGroup() const;
	void setNbGroup(unsigned int nbg) ;
	std::vector<unsigned int> getGroup(unsigned int i) const;
	//void pushStart(unsigned int s);
	//void pushEnd(unsigned int s);
	bool isBlocked() const;
	void print(FILE* out = stdout, bool all=false) const;
protected:
	bool blocked;
	std::vector< std::vector<unsigned int> > groups;
};

#endif


