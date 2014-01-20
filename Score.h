#ifndef __SCORE__
#define __SCORE__

#include "ContingencyTable.h"

#define CHISQ 0
#define SHANNON 1
#define GINI 2

class Score {
 private:
  int type;
  inline double getShannonTerm(double p);
  double getShannonEntropy(double t1, double t2);
  double getMutualInfo(std::vector<double> &less,std::vector<double> &geq,
                       double t1,double t2);
  double getCondShannonEntropy(std::vector<double> &less,
                               std::vector<double> &geq,
                               double total);
  
 public:
	Score() { type = SHANNON; }
	~Score() {}
	double getScore(ContingencyTable& t);
	double getMutualInfo(ContingencyTable &t);
	double getShannonEntropy(std::vector<double> &v, double total);
};

#endif
