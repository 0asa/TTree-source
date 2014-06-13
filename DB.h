/*
 *  DB.h
 *  TTree
 *
 *  Created by Vincent Botta on 17/12/08.
 *  Copyright 2008 University of Liège. All rights reserved.
 *
 */

#ifndef __DB__
#define __DB__

#include <iostream>
#include <vector>
#include <cstdio>
#include "ScoreInfo.h"

#define NUMERICAL 0
#define SNP 1

#define REPTYPE short int
#define SETREP unsigned int

class DB {
private:
	unsigned int nbobject;

	unsigned int nbclass;
	std::vector<double> weight;
	unsigned int type;
public:
	DB();
	DB(int nbo,int nba);
	~DB();

	void setType(unsigned int t = NUMERICAL) { type = t; }
	unsigned int getType() const { return type; }
	double getWeight(unsigned int i) const;
	void setWeight(std::vector<double> w);
	void setWeight(double w = 1.0);
	void initWeight(unsigned int size, double w = 1.0);
	std::vector<SETREP> selectAll(bool att = false) const;
	std::vector<SETREP> select(int start,int end, bool att = false) const;
	std::vector<unsigned int> selectAllAtt() const;
	std::vector<unsigned int> selectAtt(int start,int end) const;
	REPTYPE getOutput(unsigned int o) const;
	REPTYPE getAttVal(unsigned int o, unsigned int a) const;
	void setNbObject(unsigned int nbo);
	void setNbAtt(unsigned int nba);
	unsigned int getNbAtt() const;
	void getLRSet(ScoreInfo &si, std::vector<unsigned int> &set,
					  std::vector<unsigned int> &left, std::vector<unsigned int> &right);
	unsigned int getNbClass() const { return nbclass ; }


protected:
	void print(FILE* out = stdout, bool all = false) const;
	unsigned int nbatt;
	std::vector<REPTYPE> output;
	std::vector<REPTYPE> att;
	void setNbClass(int i = 0);

};

inline double DB::getWeight(unsigned int i) const {return weight[i];}
inline REPTYPE DB::getOutput(unsigned int o) const {return output[o];}
inline REPTYPE DB::getAttVal(unsigned int o, unsigned int a) const {return att[o*nbatt+a];}

#endif
