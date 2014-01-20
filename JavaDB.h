/*
 *  JavaDB.h
 *  TTree
 *
 *  Created by Vincent Botta on 16/01/09.
 *  Copyright 2009 University of Liège. All rights reserved.
 *
 */

#ifndef __JAVADB__
#define __JAVADB__

#include <string>
#include "BlockDB.h"

class JavaDB : public BlockDB {
private:
	std::string filename;
	bool load(std::string jdbfile);
	void readline(std::string l);
	long int countLineAtt(std::string jdbfile);
	long int countAtt(std::string l);
public:
	JavaDB();
	JavaDB(std::string jdbfile);
	~JavaDB();
	void readBlock(std::string blockfile);
	void readBlockStartEnd(std::string blockfile);
};

#endif

