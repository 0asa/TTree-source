#include <iostream>
#include <vector>
#include <getopt.h>
#include <time.h>
#include "NodeTest.h"
#include "SimpleNodeTest.h"
#include "BlockDB.h"
#include "JavaDB.h"
#include "ClassicTree.h"
#include "ClassicNodeTree.h"
#include "RandomForest.h"
#include "RandomTTForest.h"
#include "ExtraTrees.h"
#include "ExtraTTrees.h"
#include "Bagging.h"
#include "Logs.h"
#include "Sampler.h"
#include "GStump.h"

#define SETREP unsigned int 

void printHelp() {
	fprintf(stdout,"\nTTree Pack 1.0 (vince.bot@gmail.com)\nOptions:\n");
	fprintf(stdout,"\tRequired fields\n");
	fprintf(stdout,"\t---------------\n");
	fprintf(stdout,"\t -j <jdbfile>\n");
	fprintf(stdout,"\t -m <0|1|2|3|4>\n");
	fprintf(stdout,"\t -t <nbTree>\n");
	fprintf(stdout,"\t -k <nbRand>\n");
	fprintf(stdout,"\tOptional fields\n");
	fprintf(stdout,"\t---------------\n");
	fprintf(stdout,"\t -b <blocfile>\n");
	fprintf(stdout,"\t -s for snp datasets (optimized code will be used)\n");
	fprintf(stdout,"\t -r <seed> to specify a seed (>-1) (if not specified will be time based)\n");
	fprintf(stdout,"\t -f for full logs\n");
	fprintf(stdout,"\t -i <increment> increment for output files\n");
	fprintf(stdout,"\t -v <testsetfile>\n");
	fprintf(stdout,"\t -l <learintsetfile>\n");
	fprintf(stdout,"\t -a <attcandfile>\n");
	fprintf(stdout,"\t -g <groupcandfile>\n");
	fprintf(stdout,"\t -n <nmin>\n");
	fprintf(stdout,"\t -c <internalcomplexity>\n");
	fprintf(stdout,"\t -e not start end file for blockfile\n");
	fprintf(stdout,"\t -x not start end file for ts/ls file\n");
	fprintf(stdout,"\t -? this message...\n");
	fprintf(stdout,"\t \n");
}

int main (int argc, char * const argv[]) {
	clock_t clock_start = clock();
	
	int option_char;
	char* jdb;
	char* block;
	bool blocked = false;
	bool snp = false;
	int seed = -1;
	int method = -1;
	bool log = false;
	int nbTree = 1;
	int k = 1;
	int inc = 0;
	int nmin = 1;
	int intc = 1;
	bool startend = true;
	bool startendlsts = true;
	std::string acfile = "";
	std::string lsfile = "";
	std::string tsfile = "";
	std::string gcfile = "";
	while ((option_char = getopt(argc, argv, "x?m:j:b:r:sl:t:k:i:a:g:v:fn:c:e")) != -1) {
		switch (option_char) {
			case 'e':	startend = false; break;
			case 'x':	startendlsts = false; break;
			case 'b':	block	= optarg; blocked = true; break;
			case 'a':	acfile	= optarg; break;
			case 'v':	tsfile	= optarg; break;
			case 'l':	lsfile	= optarg; break;
			case 'g':	gcfile	= optarg; break;
			case 'j':	jdb		= optarg; break;
			case 's':	snp		= true; break;
			case 'r':	seed	= atoi(optarg); break;
			case 'm':	method	= atoi(optarg); break;
			case 't':	nbTree	= atoi(optarg); break;
			case 'k':	k		= atoi(optarg); break;
			case 'i':	inc		= atoi(optarg); break;
			case 'n':	nmin	= atoi(optarg); break;
			case 'c':	intc	= atoi(optarg); break;
			case 'f':	log		= true; break;
			case '?':	printHelp(); return -1 ; break;
			default:	printHelp(); return -1 ; break;
		}
	}
	
	//return 0;
	
	if (seed>-1)
		srand(seed);
	else
		srand((unsigned int) time(NULL));
	
	// OUTPUT FILES 
	char prefix[256];
	sprintf(prefix, "%s_k%d_m%d_t%d_ic%d_nmin%d",jdb,k,method,nbTree,intc,nmin);
	Logs l(prefix,inc);
		
	// DATABASE INPUT
	JavaDB db(jdb);
	if (blocked) {
		if (startend)
			db.readBlockStartEnd(block);
		else
			db.readBlock(block);
		l.main("BLOCK FILE:"); l.main(block); l.main("\n");
	}
	if (snp) {
		db.setType(SNP);
		l.main("SNP DB MODE : ON\n");
	}
	if (log)
		db.print(l.main());
	
	// LEARNING, TEST SETS AND CANDIDATE ATTRIBUTES SETTINGS
	std::vector<SETREP> ls;
	if (lsfile == "") 
		ls = db.selectAll();
	else {
		if (startendlsts)
			ls = Sampler::readStartEndSet(lsfile);
		else 
			ls = Sampler::readSet(lsfile);
		l.main("LEARNING SET FILE:"); l.main(lsfile.c_str()); l.main("\n");
	}
	
	std::vector<SETREP> ts;
	if (tsfile == "")
		ts = db.selectAll();
	else {
		if (startendlsts)
			ts = Sampler::readStartEndSet(tsfile);
		else
			ts = Sampler::readSet(tsfile);
		l.main("TEST SET FILE:"); l.main(tsfile.c_str()); l.main("\n");
	}
	
	std::vector<unsigned int> attcand;
	if (acfile == "")
		attcand = db.selectAllAtt();
	else {
		attcand = Sampler::readStartEndSet(acfile);
		l.main("CANDIDATE ATT. FILE:"); l.main(acfile.c_str()); l.main("\n");
	}
	
	std::vector<unsigned int> groupcand;
	if (blocked) {
		if (gcfile == "")
			groupcand = db.selectAllGroup();
		else {
			groupcand = Sampler::readStartEndSet(gcfile);
			l.main("CANDIDATE ATT. FILE:"); l.main(gcfile.c_str()); l.main("\n");
		}
	}
	
	Result result(&db);

	std::vector<float> vim(db.getNbAtt(),0.0);
	
	switch (method) {
		case 0: {
			fprintf(stderr,"CLASSIC TREE\n");
			ClassicTree ct(&db,nmin);
			ct.setLS(&ls);
			ct.setTS(&ts);
			ct.setAttCand(&attcand);
			ct.print(l.main());
			fprintf(l.main(),"Learning Time : %f sec\n",	ct.learn()/CLOCKS_PER_SEC);
			result = ct.test();
			fprintf(l.main(),"Error rate : %f %%\n", result.getErrorRate());
			fprintf(stderr,"Error rate : %f %%\n", result.getErrorRate());
			result.printProb(l.roc());
			//ct.computeVimp();
			//ct.printVimp(l.varimp());
			
			ct.computeVimp(vim,ls);
			ct.printVimp(vim,l.varimp());
			
			fprintf(l.dot(),"digraph G {\n");
			ct.dotit(l.dot());
			fprintf(l.dot(),"}\n");
			
			l.flush();
			break; 
		} case 1: {
			fprintf(stderr,"RANDOM FOREST\n");
			RandomForest rf(&db,nbTree,k,nmin);
			rf.setLS(&ls);
			rf.setTS(&ts);
			rf.setAttCand(&attcand);
			rf.print(l.main());
			rf.setTreeInfo(l.tree());
			fprintf(l.main(),"Learning Time : %f sec\n",	rf.learn()/CLOCKS_PER_SEC);
			result = rf.test();
			fprintf(l.main(),"Error rate : %f %%\n", result.getErrorRate());	
			fprintf(stderr,"Error rate : %f %%\n", result.getErrorRate());
			result.printProb(l.roc());
			
			rf.computeVimp(vim,ls);
			rf.printVimp(vim,l.varimp());
			rf.dotit(l.dot(),10);
			
			l.flush();
			break; 
		} case 2: {
			fprintf(stderr,"EXTRATREES\n");
			ExtraTrees et(&db,nbTree,k,nmin);
			et.setLS(&ls);
			et.setTS(&ts);
			et.setAttCand(&attcand);
			et.print(l.main());
			et.setTreeInfo(l.tree());
			fprintf(l.main(),"Learning Time : %f sec\n",	et.learn()/CLOCKS_PER_SEC);
			result = et.test();
			fprintf(l.main(),"Error rate : %f %%\n", result.getErrorRate());	
			fprintf(stderr,"Error rate : %f %%\n", result.getErrorRate());
			result.printProb(l.roc());
			
			et.computeVimp(vim,ls);
			et.printVimp(vim,l.varimp());
			et.dotit(l.dot(),10);
					
			l.flush();
			break;
		} case 3: {
			fprintf(stderr,"RANDOM TT FOREST\n");
			RandomTTForest rttf(&db,nbTree,k,nmin);
			rttf.setKint(0);
			rttf.setLS(&ls);
			rttf.setTS(&ts);
			rttf.setAttCand(&attcand);
			rttf.setGroupCand(groupcand);
			rttf.setInternalComp(intc);
			rttf.print(l.main());
			rttf.setTreeInfo(l.tree());
			fprintf(l.main(),"Learning Time : %f sec\n",	rttf.learn()/CLOCKS_PER_SEC);
			result = rttf.test();
			fprintf(l.main(),"Error rate : %f %%\n", result.getErrorRate());
			fprintf(stderr,"Error rate : %f %%\n", result.getErrorRate());
			result.printProb(l.roc());
			
			
			std::vector<float> gim(db.getNbGroup(),0.0);
			rttf.computeGroupVimp(gim,ls);
			rttf.printGroupVimp(gim,l.groupimp());
			
			rttf.computeVimp(vim,ls);
			rttf.printVimp(vim, l.varimp());
			rttf.dotit(l.dot(),10);
			
			
			l.flush();
			break;
		} case 4: {
			fprintf(stderr,"EXTRATTREES\n");
			ExtraTTrees ett(&db,nbTree,k,nmin);
			ett.setKint(0);
			ett.setLS(&ls);
			ett.setTS(&ts);
			ett.setAttCand(&attcand);
			ett.setGroupCand(groupcand);
			ett.setInternalComp(intc);
			ett.print(l.main());
			fprintf(l.main(),"Learning Time : %f sec\n",	ett.learn()/CLOCKS_PER_SEC);
			result = ett.test();
			fprintf(l.main(),"Error rate : %f %%\n", result.getErrorRate());
			fprintf(stderr,"Error rate : %f %%\n", result.getErrorRate());
			result.printProb(l.roc());
			
			std::vector<float> gim(db.getNbGroup(),0.0);
			ett.computeGroupVimp(gim,ls);
			ett.printGroupVimp(gim,l.groupimp());	
			
			ett.computeVimp(vim,ls);
			ett.printVimp(vim,l.varimp());
			
			l.flush();
			break;
		} case 5: { // WIP...
			fprintf(stderr,"RANDOM TT STUMP\n");
			RandomTTForest rttf(&db,nbTree,k,nmin);
			rttf.setKint(0);
			rttf.setLS(&ls);
			rttf.setTS(&ts);
			rttf.setTreeType(RANDOMTTSTUMP);
			rttf.setAttCand(&attcand);
			rttf.setGroupCand(groupcand);
			rttf.setInternalComp(intc);
			rttf.print(l.main());
			fprintf(l.main(),"Learning Time : %f sec\n",	rttf.learn()/CLOCKS_PER_SEC);
			result = rttf.test();
			fprintf(l.main(),"Error rate : %f %%\n", result.getErrorRate());
			fprintf(stderr,"Error rate : %f %%\n", result.getErrorRate());
			result.printProb(l.roc());
			rttf.computeVimp();
			rttf.printVimp(l.varimp());
			rttf.computeGroupVimp();
			rttf.printGroupVimp(l.groupimp());
			l.flush();
			break;
		} case 6: { // WIP...
			fprintf(stderr,"EXTRATTSTUMP\n");
			ExtraTTrees ett(&db,nbTree,k,nmin);
			ett.setKint(0);
			ett.setLS(&ls);
			ett.setTS(&ts);
			ett.setTreeType(EXTRATTSTUMP);
			ett.setAttCand(&attcand);
			ett.setGroupCand(groupcand);
			ett.setInternalComp(intc);
			ett.print(l.main());
			fprintf(l.main(),"Learning Time : %f sec\n",	ett.learn()/CLOCKS_PER_SEC);
			result = ett.test();
			fprintf(l.main(),"Error rate : %f %%\n", result.getErrorRate());
			fprintf(stderr,"Error rate : %f %%\n", result.getErrorRate());
			result.printProb(l.roc());
			ett.computeVimp();
			ett.printVimp(l.varimp());
			ett.computeGroupVimp();
			ett.printGroupVimp(l.groupimp());	
			l.flush();			
			break;
		} default: break;
	}

	
	fprintf(l.main(),"Overall Time : %f sec\n",(clock()-clock_start) / (double) CLOCKS_PER_SEC);
	fprintf(l.main(),"Program terminated...\n");
	fprintf(stdout,"Program terminated...\n");

	return 0;
}
