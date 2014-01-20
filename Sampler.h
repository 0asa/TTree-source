/*
 *  Sampler.h
 *  TTree
 *
 *  Created by Vincent Botta on 17/12/08.
 *  Copyright 2008 University of Liège. All rights reserved.
 *
 */

#ifndef __SAMPLER__
#define __SAMPLER__

#include <vector>
#include <iostream>
#include <fstream>
#include "StrTools.h"

//#define SETREP unsigned int

class Sampler {
public:
	template<class T> static std::vector<T> bootstrap(std::vector<T> &v);
	template<class T> static void shake(std::vector<T> &v, int s = -1);
	template<class T> static std::vector<T> random(std::vector<T> &v, int nb);
	template<class T> static void applyMap(std::vector<T> &v,std::vector<int> &map);
	template<class T> static void print(std::vector<T> &v, int start = -1, int end = -1);
	static std::vector<int> permutationMap(int nb);
	
	static std::vector<unsigned int> readSet(std::string file) {
		std::vector<unsigned int> ret;
		std::ifstream fs(file.c_str());
		if (!fs) {
			fprintf(stderr, "ReadSet() Error : Cannot find file %s\n",file.c_str());
			return ret;
		}
		std::string line;
		int ln = 0;
		while (getline(fs, line)) {
			std::vector<std::string> ex = StrTools::explode(line,const_cast<char *>("\t"));
			// treat ex and pushback...
			if (ex.size() > 0) {
				for (int i = 0 ; i < ex.size(); ++i)
					ret.push_back(atoi(ex[i].c_str()));
			} else {
				fprintf(stderr, "ReadSet() Error : Incorrect file format ?\n");
			}
			++ln;
		}
		fs.close();	
		return ret;
	}
	
	static std::vector<unsigned int> readStartEndSet(std::string file) {
		std::vector<unsigned int> ret;
		std::ifstream fs(file.c_str());
		if (!fs) {
			fprintf(stderr, "ReadStartEndSet() Error : Cannot find file %s\n",file.c_str());
			return ret;
		}
		std::string line;
		int ln = 0;
		while (getline(fs, line)) {
			std::vector<std::string> ex = StrTools::explode(line,const_cast<char *>("\t"));
			// treat ex and pushback start to end...
			if (ex.size() == 2) {
				int start = atoi(ex[0].c_str());
				int end = atoi(ex[1].c_str());
				for (int i = start ; i <= end ; ++i)
					ret.push_back(i);
			} else {
				fprintf(stderr, "ReadStartEndSet() Error : Incorrect file format ?\n");
			}
			++ln;
		}
		fs.close();
		return ret;
	}
};

template<class T> std::vector<T> Sampler::random(std::vector<T> &v, int nb) {
	std::vector<T> ret(nb);
	std::vector<T> tmp(v);
	Sampler::shake(tmp,nb);
	if (nb<v.size()) 
		for (int i = 0 ; i < nb ; ++i) 
			ret[i] = tmp[i];
	else
		return tmp;
	return ret;
}

template<class T> void Sampler::shake(std::vector<T> &v, int s) {
	int pos = 0;
	if (s<0 || s> (int) v.size()) s = (int) v.size();
	for (int i = 0 ; i < s; ++i) {
		int p = pos + (rand() % (v.size()-pos));
		T tmp = v[pos];
		v[pos++] = v[p];
		v[p] = tmp;
	}
}

template<class T> std::vector<T> Sampler::bootstrap(std::vector<T> &v) {
	std::vector<T> ret(v.size());
	for (int i = 0 ; i < v.size() ; ++i)
		ret[i] = v[rand()%v.size()]; 
	return ret;
}

template<class T> void Sampler::print(std::vector<T> &v, int start,int end) {
	if (v.size() == 0) std::cout << "Empty vector" << std::endl;
	if (start < 0) start = 0;
	if (end < 0 || end < start || end > v.size()) end = v.size();
	for (int i = start ; i < end ; ++i) 
		std::cout << v[i] << ",";
	std::cout << std::endl;
}


#endif

