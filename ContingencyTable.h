#ifndef __CONTINGENCYTABLE__
#define __CONTINGENCYTABLE__

#include <vector>

class ContingencyTable {
private:
	int nbclass;
	int nbsymbol;
	std::vector< std::vector<double> > table;
	std::vector<double> less;
	std::vector<double> geq;
	std::vector<double> sum;
	double t_less;
	double t_geq;
public:
	ContingencyTable(int nbc, int nbs);
	~ContingencyTable();
	void addToTable(int c,int s, double v = 1.0);
	void compact(double th = 0.5);
	void addLess(int class_number);
	void addLess(int class_number, double v);
	void addGeq(int class_number);
	void addGeq(int class_number, double v);
	void reset();
	void resetLessGeq();
	void print();
	
	bool isConst() ;
	std::vector<double> getPossibleTh();
	bool isClassEmpty(unsigned int c) const ;
	bool isSymbolEmpty(unsigned int s) const;
	
	double totalLess() const;
	double totalGeq()  const;
	std::vector<double> getLess() const;
	std::vector<double> getGeq()  const;
	std::vector<double> getSum()  const;
	double total() const ;
};

#endif
