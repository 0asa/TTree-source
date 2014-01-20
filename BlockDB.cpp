/*
 *  BlockDB.cpp
 *  TTree
 *
 *  Created by Vincent Botta on 12/01/09.
 *  Copyright 2009 University of Liège. All rights reserved.
 *
 */

#include "BlockDB.h"

BlockDB::BlockDB() : DB(), nbGroup(0), blocked(false) {}
BlockDB::BlockDB(unsigned int nbo, unsigned int nba, unsigned nbg) : DB(nbo,nba) , nbGroup(nbg), blocked(false) {}
BlockDB::~BlockDB() {}

unsigned int BlockDB::getNbGroup() const { return nbGroup; }
void BlockDB::setNbGroup(unsigned int nbg) { nbGroup = nbg ; }
bool BlockDB::isBlocked() const { return blocked; }

std::vector<unsigned int> BlockDB::getGroup(unsigned int i) const {
	return groups[i];
}

std::vector<unsigned int> BlockDB::selectAllGroup() const {
	std::vector<unsigned int> ret;
	for (int i = 0 ; i < nbGroup ; ++i)
		ret.push_back(i);
	return ret;
}

void BlockDB::print(FILE* out, bool all) const {
	DB::print(out,all);
	fprintf(out,"########## Groups Att. :\n");
	for (int i = 0 ; i < nbGroup ; ++i) {
		for (int j = 0 ; j < groups[i].size() ; ++j)
			fprintf(out,"%d ",groups[i][j]);
		fprintf(out,"\n");
	}	
}




