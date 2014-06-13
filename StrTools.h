/*
 *  StrTools.h
 *  TTree
 *
 *  Created by Vincent Botta on 21/01/09.
 *  Copyright 2009 University of Liège. All rights reserved.
 *
 */

#ifndef __STRTOOLS__
#define __STRTOOLS__
#include <string>
#include <cstring>
#include <vector>

class StrTools {
public:
	static std::vector<std::string> explode(std::string &str, char* delim) {
		std::vector<std::string> ret;
		char* strcopy = new char[str.size()+1];
		strcpy(strcopy,str.c_str());
		char* result = strtok(strcopy,delim);
		while (result != NULL) {
			//fprintf(stderr,"[%s]",result);
			ret.push_back((std::string)result);
			result = strtok(NULL, delim);
		}
		delete[] strcopy;
		return ret;
	}

};

//std::vector<std::string> StrTools::explode(std::string &str, char* delim)
#endif
