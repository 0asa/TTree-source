/*
 *  Scorer.cpp
 *  TTree
 *
 *  Created by Vincent Botta on 10/12/08.
 *  Copyright 2008 University of Liège. All rights reserved.
 *
 */

#include "Sampler.h"
#include "Scorer.h"

Scorer::Scorer(DB *database) { 
	changedb(database); 
	setBestScoreFonction(db->getType()); 
	setRandScoreFonction(db->getType());
}

Scorer::~Scorer() {}

ScoreInfo Scorer::getBestGroupScore(std::vector<unsigned int> &obj, std::vector<unsigned int> &group, unsigned int internalComplexity, int treeType, int kint) {
	Score score;
	ScoreInfo si;
	Learner *weak[group.size()];

	for (int g = 0 ; g < group.size() ; ++g) {
		if (treeType == RANDOMTTSTUMP || treeType == EXTRATTSTUMP)
			weak[g] = new GStump(db);
		else {
			weak[g] = new ClassicTree(db,1,kint);
			weak[g]->setMaxComplexity(internalComplexity);
			((ClassicTree*)weak[g])->setLearnFonction(EXTRATREE);
		}
	}
	

	for (int g = 0 ; g < group.size() ; ++g) {
		std::vector<unsigned int> attcand = ((BlockDB*)db)->getGroup(group[g]);
		getGroupScore(weak[g],obj,attcand,si,treeType);
	}
	
	for (int g = 0; g < group.size() ; ++g) {
		
		if (weak[g] != si.weak) {
			if (treeType == RANDOMTTSTUMP || treeType == EXTRATTSTUMP) {
				delete ((GStump*)weak[g]);
			} else {
				delete weak[g];
			}
		} else
			si.group = group[g];
	}
	//if (si.group != -1)
	//	fprintf(stderr,"si.group = %0.4d\tsi.Score = %0.6f\tsi.th = %0.6f\n",si.group,si.score,si.threshold);
	return si;
}
/*
ScoreInfo Scorer::getBestGroupStumpScore(std::vector<unsigned int> &ind, std::vector<unsigned int> &group, unsigned int internalComplexity, int treeType) {
	Score score;
	ScoreInfo si;
	GStump *stump[group.size()];
	for (int g = 0 ; g < group.size() ; ++g) {
		std::vector<unsigned int> attcand = ((BlockDB*)db)->getGroup(group[g]);
		stump[g] = new GStump(db,attcand);
	}
	for (int g = 0 ; g < group.size() ; ++g) {
		getGroupStumpScore(stump[g],ind,si,treeType);
	}
	
	for (int g ; g < group.size() ; ++g) {
		if (stump[g] != si.stump)
			delete stump[g];
		else
			si.group = group[g];
	}
	
	return si;
}*/
/*
ScoreInfo Scorer::getBestGroupSNPScore(std::vector<unsigned int> &ind, std::vector<unsigned int> &group, unsigned int internalComplexity, int treeType, int kint) {
	Score score;
	ScoreInfo si;	
	ClassicTree *tree[group.size()];
	for (int g = 0 ; g < group.size() ; ++g) {
		tree[g] = new ClassicTree(db,1,kint);
		tree[g]->setMaxComplexity(internalComplexity);
		//tree[g]->setLearnFonction(treeType);
	}
	for (int g = 0 ; g < group.size() ; ++g) {
		std::vector<unsigned int> attcand = ((BlockDB*)db)->getGroup(group[g]);
		//fprintf(stderr,"------[cand:%d] ",attcand.size());
		getGroupSNPScore(tree[g],ind,attcand,si, treeType);
		//if (g % 100 == 0) 
		//	fprintf(stderr,".",g);
	}
	//fprintf(stderr,"\n");
	for (int g = 0 ; g < group.size() ; ++g)
		if (tree[g] != si.tree) 
			delete tree[g];
		else
			si.group = group[g];
	return si;
}*/
/*
void Scorer::getGroupStumpScore(GStump *stump, std::vector<unsigned int> &ind, ScoreInfo &si, int treeType) {
	Score score;
	stump->setLS(ind);
	stump->learn();
	Result result = stump->test(ind);
	ContingencyTable ctable = result.getContingencyTable();
	std::vector<double> th = result.getPossibleProbTh();
	if (treeType == EXTRATREE)
		th = Sampler::random(th,1);
	for(int t = 0 ; t < th.size() ; ++t) {
		ctable = result.getContingencyTable(th[t]);
		ctable.compact(th[t]);
		si.update(stump,th[t],score.getScore(ctable));
	}
}*/

void Scorer::getGroupScore(Learner* weak,std::vector<unsigned int> &ind, std::vector<unsigned int> &cand, ScoreInfo &si, int treeType) {
	Score score;
	weak->setLS(&ind);
	if (treeType == RANDOMTTSTUMP || treeType == EXTRATTSTUMP)
		((GStump*)weak)->setAttCand(&cand);
	else 
		weak->setAttCand(&cand);
	weak->learn();

	Result result = weak->test(&ind);
	result.shrink();
	
	ContingencyTable ctable = result.getContingencyTable();	
	std::vector<double> th = result.getPossibleProbTh();

	if (treeType == EXTRATREE || treeType == EXTRATTSTUMP)
		th = Sampler::random(th, 1);
	
	for (int t = 0 ; t < th.size() ; ++t) {
		ctable = result.getContingencyTable(th[t]);
		ctable.compact(th[t]);
		si.update(weak,th[t],score.getScore(ctable));
	}
	
}
/*
void Scorer::getGroupSNPScore(ClassicTree *ctree,std::vector<unsigned int> &ind, std::vector<unsigned int> &cand, ScoreInfo &si, int treeType) {
	Score score;
	ctree->setLS(ind);
	ctree->setAttCand(cand);
	ctree->learn();
	//fprintf(stderr,"lt:%f sec\n",ctree->learn()/CLOCKS_PER_SEC);
	Result result = ctree->test(ind);
	ContingencyTable ctable = result.getContingencyTable();

	std::vector<double> th = result.getPossibleProbTh();
	
	if (treeType == EXTRATREE)
		th = Sampler::random(th, 1);
	
	for (int t = 0 ; t < th.size() ; ++t) {
		ctable = result.getContingencyTable(th[t]);
		ctable.compact(th[t]);
		si.update(ctree,th[t],score.getScore(ctable));
	}
	//ctable.compact(0.5);
	//si.update(ctree,score.getScore(ctable));
}*/

ScoreInfo Scorer::getRandSNPScore(std::vector<unsigned int> &ind, std::vector<unsigned int> &snp) {
	ScoreInfo si;
	for (int s = 0 ; s < snp.size() ; ++s) 
		getRandSNPScore(ind,snp[s],si);
	return si;
}

void Scorer::getRandSNPScore(std::vector<unsigned int> &ind, unsigned int snp, ScoreInfo &si) {
	Score score;
	ContingencyTable ctable(2,3); 
	for (int i = 0 ; i < ind.size() ; ++i)
		ctable.addToTable(db->getOutput(ind[i]), db->getAttVal(ind[i], snp),db->getWeight(ind[i]));
	std::vector<double> th = ctable.getPossibleTh();
	if (th.size() > 0) {
		int t = rand() % th.size();
		ctable.compact(th[t]);
		si.update(snp,th[t],score.getScore(ctable));
	}
}

ScoreInfo Scorer::getBestSNPScore(std::vector<unsigned int> &ind, std::vector<unsigned int> &snp) {
	//Score score;
	//printf("getBestSNPScore\n");
	ScoreInfo si;
	for (int s = 0 ; s < snp.size() ; ++s) 
		getSNPScore(ind,snp[s],si);
	return si;
}

 void Scorer::getSNPScore(std::vector<unsigned int> &ind, unsigned int snp, ScoreInfo &si) {
	Score score;
	ContingencyTable ctable(2,3); 
	for (int i = 0 ; i < ind.size() ; ++i)
		ctable.addToTable(db->getOutput(ind[i]), db->getAttVal(ind[i], snp),db->getWeight(ind[i]));
	std::vector<double> th = ctable.getPossibleTh();
	for (int t = 0 ; t < th.size() ; ++t) {
		ctable.compact(th[t]);
		//ctable.print();
		si.update(snp,th[t],score.getScore(ctable));
	}
}

ScoreInfo Scorer::getSNPScore(std::vector<unsigned int> &ind, unsigned int snp) {
	Score score;
	ScoreInfo si;	
	ContingencyTable ctable(2,3);
	for (int i = 0 ; i < ind.size() ; ++i)
		ctable.addToTable(db->getOutput(ind[i]), db->getAttVal(ind[i], snp), db->getWeight(ind[i]));
	std::vector<double> th = ctable.getPossibleTh();
	for (int t = 0 ; t < th.size() ; ++t) {
		ctable.compact(th[t]);
		si.update(snp,th[t],score.getScore(ctable));
	}
	return si;
}
/*
ContingencyTable Scorer::getContingencyTable(std::vector<unsigned int> &ind, unsigned int snp, double th) {
	ContingencyTable ctable(2,3);
	for (int i = 0 ; i < ind.size(); ++i)
		ctable.addToTable(db->getOutput(ind[i]), db->getAttVal(ind[i], snp),db->getWeight(ind[i]));
	ctable.compact(th);
	return ctable;
}*/

ContingencyTable Scorer::getContingencyTable(std::vector<unsigned int> &ind, unsigned int snp, double th) {
	ContingencyTable ctable(2,2);
	for (int i = 0 ; i < ind.size(); ++i) {
		if (db->getAttVal(ind[i], snp) > th)
			ctable.addGeq(db->getOutput(ind[i]),db->getWeight(ind[i]));
			//ctable.addToTable(db->getOutput(ind[i]), db->getAttVal(ind[i], snp),db->getWeight(ind[i]));
		else
			ctable.addLess(db->getOutput(ind[i]),db->getWeight(ind[i]));
			//ctable.addToTable(db->getOutput(ind[i]), db->getAttVal(ind[i], snp),db->getWeight(ind[i]));
			
	}
	//ctable.compact(th);
	return ctable;
}


typedef struct {
	float value;
	int ind;
} TUPLET;

bool cmp (TUPLET a, TUPLET b) {
	return a.value < b.value;
}

ScoreInfo Scorer::getRandNumericalScore(std::vector<unsigned int> &obj, std::vector<unsigned int> &att) {
	//printf("getBestNumericalScore\n");
	ScoreInfo si;
	for (int a = 0 ; a < att.size() ; ++a)
		getRandNumericalScore(obj,att[a],si);
	return si;
}

ScoreInfo Scorer::getBestNumericalScore(std::vector<unsigned int> &obj, std::vector<unsigned int> &att) {
	//printf("getBestNumericalScore\n");
	ScoreInfo si;
	for (int a = 0 ; a < att.size() ; ++a)
		getBestNumericalScore(obj,att[a],si);
	return si;
}

void Scorer::getBestNumericalScore(std::vector<unsigned int> &obj, unsigned int att, ScoreInfo &si) {		
	Score score;
	ContingencyTable ctable(2,2);
	std::vector<TUPLET> values;
	for (int i = 0 ; i < obj.size() ; ++i) {
		TUPLET a; a.value = db->getAttVal(obj[i], att) ; a.ind = obj[i];
		values.push_back(a);
	}
	sort(values.begin(), values.end(),cmp);
	double th = -1.0;
	double v = values[0].value;
	// init ctable
	for (int i = 0 ; i < obj.size() ; ++i)
		ctable.addGeq(db->getOutput(values[i].ind), db->getWeight(values[i].ind));
	int lasti = 0;
	for (int i = 1 ; i < obj.size() ; ++i) {		
		// test des scores aux valeurs intermédiaires
		if (v != values[i].value) {
			// mise a jour incrémentale de la ctable !
			for (int j = lasti ; j < i ; ++j) {
				ctable.addGeq (db->getOutput(values[j].ind) , -db->getWeight(values[j].ind));
				ctable.addLess(db->getOutput(values[j].ind) ,  db->getWeight(values[j].ind));
			}

			th = (v+values[i].value)/2.0 ;
			si.update(att,th,score.getScore(ctable));
			v = values[i].value;
			lasti = i;
		}
	}	
}

void Scorer::getRandNumericalScore(std::vector<unsigned int> &obj, unsigned int att, ScoreInfo &si) {		
	Score score;
	ContingencyTable ctable(2,2);

	std::vector<TUPLET> values;
	for (int i = 0 ; i < obj.size() ; ++i) {
		TUPLET a; a.value = db->getAttVal(obj[i], att) ; a.ind = obj[i];
		//printf("%d > %f\n",db->getAttVal(obj[i], att),a.value);
		values.push_back(a);
	}
	sort(values.begin(), values.end(),cmp);

	// get all the possible tresholds on values
	std::vector<double> th;
	double v = values[0].value;
	for (int i = 1 ; i < values.size() ; ++i) {
		if (v != values[i].value) {
			th.push_back((v+values[i].value)/2.0);
			v = values[i].value;
		}
	}
	if (th.size() > 0) {
	int r = rand() % th.size(); // got a random treshold
	// now use it to fill the ctable
	for (int i = 0 ; i < obj.size() ; ++i) {
		if (db->getAttVal(obj[i], att) < th[r])
			ctable.addLess(db->getOutput(obj[i]), db->getWeight(obj[i]));
		else
			ctable.addGeq(db->getOutput(obj[i]), db->getWeight(obj[i]));
	}
	si.update(att,th[r],score.getScore(ctable));
	}
}


void Scorer::changedb(DB *database) { db = database; }

