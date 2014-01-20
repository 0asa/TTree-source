#include "Score.h"
#include <math.h>


/* LWH SCORE */

double Score::getScore(ContingencyTable& t) {
  std::vector<double> less = t.getLess();
  std::vector<double> geq = t.getGeq();
  std::vector<double> sum = t.getSum();
  for (int i = 0 ; i < sum.size() ; i++) {
    sum[i] = less[i] + geq[i];
  }
  double ht = getShannonEntropy(t.totalLess(),t.totalGeq());
  double hc = getShannonEntropy(sum,t.total());
  double itc = getMutualInfo(less,
                             geq,
                             t.totalLess(),
                             t.totalGeq()); 
  return itc / ((hc + ht)/2);
  }


/* QUINLAN SCORE */
/*
double Score::getScore(ContingencyTable& t) {
  std::vector<double> less = t.getLess();
  std::vector<double> geq = t.getGeq();
  std::vector<double> sum = t.getSum();
  double ht = getShannonEntropy(t.totalLess(),t.totalGeq());
  double itc = getMutualInfo(less,
                             geq,
                             t.totalLess(),
                             t.totalGeq()); 
  return itc / ht;
}
*/

/* NONORM SCORE */
/*
double Score::getScore(ContingencyTable& t) {
  std::vector<double> less = t.getLess();
  std::vector<double> geq = t.getGeq();
  std::vector<double> sum = t.getSum();
  
  double itc = getMutualInfo(less,
                             geq,
                             t.totalLess(),
                             t.totalGeq()); 
  return itc ; 
}
*/



inline double Score::getShannonTerm(double p) {
  if (p>0)
    return (-p * log(p));
  return 0;
}

double Score::getShannonEntropy(double t1,double t2) {
  double t11 = (t1 / (t1 + t2));
  double t21 = 1 - t11;
  return (getShannonTerm(t11) + getShannonTerm(t21));
}

double Score::getShannonEntropy(std::vector<double> &v, double total) {
  double entropy = 0;
  for (int i = 0; i < v.size() ; ++i) {
    double p =  v[i] / total;
    entropy += getShannonTerm(p);
  }
  return entropy;
}

double Score::getMutualInfo(std::vector<double> &less,std::vector<double> &geq,
                            double t1,double t2) {
  double ht = getShannonEntropy(t1,t2);
  double htc = getCondShannonEntropy(less,geq,t1+t2);
  return (ht - htc) ;
}

double Score::getCondShannonEntropy(std::vector<double> &less,
                                    std::vector<double> &geq,
                                    double total) {
  double  entropy = 0;
  for (int i = 0; i < less.size() ; ++i) {
    entropy += getShannonEntropy(less[i],geq[i]) * 
      ((double) (less[i] + geq[i]) / total);
  }
  return entropy;  
}

double Score::getMutualInfo(ContingencyTable &t) {
	std::vector<double> less = t.getLess();
	std::vector<double> geq = t.getGeq();
	return getMutualInfo(less,geq,t.totalLess(),t.totalGeq());
}
