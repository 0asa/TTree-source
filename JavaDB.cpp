/*
 *  JavaDB.cpp
 *  TTree
 *
 *  Created by Vincent Botta on 16/01/09.
 *  Copyright 2009 University of Liège. All rights reserved.
 *
 */

#include "JavaDB.h"
#include <iostream>
#include <fstream>
#include "StrTools.h"

JavaDB::JavaDB() {}

JavaDB::JavaDB(std::string f) : filename(f) {
	load(filename);
	//setNbClass();
}

void JavaDB::readBlockStartEnd(std::string file) {
	std::ifstream fs(file.c_str());
	if (!fs)
		fprintf(stderr, "Error : Cannot find block file %s\n",file.c_str());
	std::string line;
	int ln = 0;
	while (getline(fs, line)) {
		std::vector<std::string> ex = StrTools::explode(line,const_cast<char *>("\t"));
		std::vector<unsigned int> g;
		// treat ex and pushback to block start end...
		if (ex.size() == 2) {
			int start = atoi(ex[0].c_str());
			int end = atoi(ex[1].c_str());
			for (int i = start ; i <= end ; ++i)
				g.push_back(i);
			groups.push_back(g);
		} else {
			fprintf(stderr, "Error : Incorrect block file format ?\n");
		}
		++ln;
	}
	setNbGroup((int) groups.size());
	blocked = true;
	fs.close();
}

void JavaDB::readBlock(std::string file) {
	std::ifstream fs(file.c_str());
	if (!fs)
		fprintf(stderr, "Error : Cannot find block file %s\n",file.c_str());
	std::string line;
	int ln = 0;
	while (getline(fs, line)) {
		std::vector<std::string> ex = StrTools::explode(line,const_cast<char *>("\t"));
		std::vector<unsigned int> g;
		// treat ex and pushback attribute into g...
		if (ex.size() > 0) {
			for (int i = 0 ; i < ex.size(); ++i)
				g.push_back(atoi(ex[i].c_str()));
			groups.push_back(g);
		} else {
			fprintf(stderr, "Error : Incorrect block file format ?\n");
		}
		++ln;
	}
	setNbGroup((int) groups.size());
	blocked = true;
	fs.close();
}

bool JavaDB::load(std::string f) {
  long int memo = countLineAtt(f);
  printf ("memo = %ld - att.max_size() = %ld\n",memo,att.max_size());
  att.reserve(memo+10);
	std::ifstream fs(f.c_str());
	if (!fs)
		return false;
	std::string line;
	int ln = 0;
	while (getline(fs, line)) {
		//if (ln==5) printf("%s",line.c_str());
		if (ln>4)
			readline(line);
		ln++;
		if ((ln-5) % 200 == 0) 
			fprintf(stderr,"%d...",ln-5);
	}
	fprintf(stderr,"\n");
	fprintf(stderr,"%d individuals found\n",ln-5);
	fprintf(stderr,"%d attributes found\n",nbatt);
	setNbObject(ln-5);
	initWeight(ln-5);
	std::vector<REPTYPE>(att.begin(),att.end()).swap(att);
	std::vector<REPTYPE>(output.begin(),output.end()).swap(output);
	fs.close();
	return true;
}

long int JavaDB::countLineAtt(std::string f) {
	std::ifstream fs(f.c_str());
	long int ln = 0;
	long int a = 0;
	std::string line;
	while(getline(fs,line)) {
		if (ln == 5)
			a = countAtt(line);
		ln += 1;
	}
	fs.close();
	//printf("%ld x %ld\n",(ln-5),a);
	return (ln-5)*a;
}

long int JavaDB::countAtt(std::string l) {
	char* lcopy = new char[l.size()+1];
	strcpy(lcopy,l.c_str());
	char delim[] = " ";
	char* result = strtok(lcopy,delim);
	// at this point result = name of object which is not used here
	int wc = 0;
	while (result != NULL) {
		result = strtok(NULL, delim);
		wc++;
	}
	delete[] lcopy;
	return wc-2;
}

void JavaDB::readline(std::string l) {
	char* lcopy = new char[l.size()+1];
	strcpy(lcopy,l.c_str());
	char delim[] = " ";
	char* result = strtok(lcopy,delim);
	// at this point result = name of object which is not used here
	int wc = 0;
	while (result != NULL) {
		result = strtok(NULL, delim);
		if (wc == 0) {// status
			output.push_back(atoi(result));
		} else if (wc > 0 && result != NULL) { // genotype 
			att.push_back(atof(result));
		}
		wc++;
	}
	delete[] lcopy;
	setNbAtt(wc-2);
}

JavaDB::~JavaDB() {}

