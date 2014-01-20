/*
 *  VimpList.h
 *  TTree
 *
 *  Created by Vincent Botta on 14/01/09.
 *  Copyright 2009 University of Liège. All rights reserved.
 *
 */

#ifndef __VIMPLIST__
#define __VIMPLIST__

#include <vector>

typedef struct {
	double imp;
	unsigned int num;
} NUMIMP;

class VimpList {
private:
	std::vector<NUMIMP> imp;
public:
	VimpList();
	~VimpList();
	void push(unsigned int n, double v);
	int size() const { return (int) imp.size() ; }
	unsigned int getNum(int i) const { return imp[i].num ; }
	double getImp(int i) const { return imp[i].imp ; }
	// TODO ............................
	// sort by imp (asc - desc) 
	// sort by num (asc - desc)
};

#endif
