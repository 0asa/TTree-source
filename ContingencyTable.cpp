#include <iostream>
#include "ContingencyTable.h"

ContingencyTable::ContingencyTable(int nbc, int nbs) : nbclass(nbc), nbsymbol(nbs) {
	for (int i = 0 ; i < nbc ; ++i) 
		table.push_back(std::vector<double>(nbs,0.0));
	less.assign(nbc,0.0);
	geq.assign(nbc,0.0);
	sum.assign(nbc,0.0);
	t_less = 0.0;
	t_geq = 0.0;
}

ContingencyTable::~ContingencyTable() {}

void ContingencyTable::resetLessGeq() {
	less.assign(nbclass,0.0);
	geq.assign(nbclass,0.0);
	sum.assign(nbclass,0.0);
	t_less = 0.0;
	t_geq = 0.0;
}

void ContingencyTable::reset() {
	for (int i = 0 ; i < nbclass ; ++i)
		table[i].assign(nbsymbol,0.0);
	less.assign(nbclass,0.0);
	geq.assign(nbclass,0.0);
	sum.assign(nbclass,0.0);
	t_less = 0.0;
	t_geq = 0.0;
}

void ContingencyTable::addToTable(int c, int s, double v) {
	//fprintf(stderr,"addToTable(%d,%d,%1.0f\n",c,s,v);
	table[c][s] += v;
}

void ContingencyTable::compact(double th) {
	resetLessGeq();
	for (int c = 0 ; c < nbclass ; ++c)
		for (int i = 0 ; i < nbsymbol ; ++i) 
			if ((double) i > th)
				addGeq(c,table[c][i]);
			else 
				addLess(c,table[c][i]);
}

void ContingencyTable::addLess(int class_number) {
  less[class_number] += 1.0;
  t_less += 1.0;
}

void ContingencyTable::addLess(int class_number,double v) {
  less[class_number] += v;
  t_less += v;
}

void ContingencyTable::addGeq(int class_number) {
  geq[class_number] += 1.0;
  t_geq += 1.0;
}

void ContingencyTable::addGeq(int class_number,double v) {
  geq[class_number] += v;
  t_geq += v;
}

void ContingencyTable::print() {  
  for (int i = 0 ; i < nbclass ; i++) 
    std::cout << less[i] << "\t";
  std::cout << std::endl;
  for (int i = 0 ; i < nbclass ; i++) 
    std::cout << geq[i] << "\t";
  std::cout << std::endl;
	fprintf(stderr,"- - - - - - - - -\n");
	for (int s = 0 ; s < nbsymbol ; ++s) {
		for (int c = 0 ; c < nbclass ; ++c) {
			fprintf(stderr,"%1.0f\t",table[c][s]);
		}
		fprintf(stderr,"\n");
	}
}

bool ContingencyTable::isConst() {
	return getPossibleTh().empty();
}

std::vector<double> ContingencyTable::getPossibleTh() {
	std::vector<double> ret;
	int lasti = 0;
	while (isSymbolEmpty(lasti)) 
		lasti++;
	for (int i = lasti+1 ; i < nbsymbol ; ++i) {
		if (!isSymbolEmpty(i)) {
			ret.push_back((lasti+i)/2.0);
			lasti = i;
		}
	}
	return ret;
}

bool ContingencyTable::isSymbolEmpty(unsigned int s) const {
	bool ret = true;
	for (int i = 0 ; i < nbclass ; ++i) 
		ret = ret && (table[i][s] == 0.0);
	return ret;	
}

bool ContingencyTable::isClassEmpty(unsigned int c) const {
	bool ret = true;
	for (int i = 0 ; i < nbsymbol ; ++i) 
		ret = ret && (table[c][i] == 0.0);
	return ret;
}

double ContingencyTable::totalLess() const { return t_less; }
double ContingencyTable::totalGeq()  const { return t_geq; }
std::vector<double> ContingencyTable::getLess() const { return less; }
std::vector<double> ContingencyTable::getGeq()  const { return geq; }
std::vector<double> ContingencyTable::getSum()  const { return sum; }
double ContingencyTable::total() const { return t_less + t_geq; }

