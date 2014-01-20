/*
 *  Logs.h
 *  TTree
 *
 *  Created by Vincent Botta on 26/02/09.
 *  Copyright 2009 University of Liège. All rights reserved.
 *
 */

#ifndef __LOGS__
#define __LOGS__
#include <string>
#include <vector>

class Logs {
private:
	FILE* _main;
	//FILE* _err;
	FILE* _roc;
	FILE* _varimp;
	FILE* _groupimp;
	FILE* _dot;
	FILE* _tree;
	std::string prefix;
	unsigned int counter;
	
	void init() {
		// write date into log
		time_t theTime;
		time( &theTime );   
		tm *t = localtime( &theTime );  
		fprintf(_main,"Time : %s",asctime(t));
		fprintf(_main,"Prefix output : %s\n",prefix.c_str());
	}
	
	void prefixit() {
		char tmp[256];
		sprintf(tmp, "%s_%04d.log",prefix.c_str(),counter);		_main = fopen(tmp,"wb");			
		sprintf(tmp, "%s_%04d.roc",prefix.c_str(),counter);		_roc = fopen(tmp,"wb");
		sprintf(tmp, "%s_%04d.vim",prefix.c_str(),counter);		_varimp = fopen(tmp,"wb");
		sprintf(tmp, "%s_%04d.gim",prefix.c_str(),counter);		_groupimp = fopen(tmp,"wb");
		sprintf(tmp, "%s_%04d.dot",prefix.c_str(),counter);		_dot = fopen(tmp,"wb");
		sprintf(tmp, "%s_%04d.tree",prefix.c_str(),counter);		_tree = fopen(tmp,"wb");
		//sprintf(tmp, "%s_%04d.err",prefix.c_str(),counter);		_err = fopen(tmp,"wb");
		init();
	}
public:
	FILE* main()	 { return _main; }
	//FILE* err()		 { return _err; }
	FILE* roc()		 { return _roc; }
	FILE* varimp()	 { return _varimp; }
	FILE* groupimp() { return _groupimp; }
	FILE* dot()		 { return _dot; }
	FILE* tree()	 { return _tree; }
	
	//void err(char* s) {
	//	fprintf(_err,"%s",s);
	//}
	
	void main(const char* s) {
		fprintf(_main,"%s",s);
	}
	
	Logs(char* p = NULL, unsigned int c = 0) {
		counter = c;
		if (p == NULL) {
			_main = stdout;
		//	_err = stderr;
			_roc = stdout;
			_varimp = stdout;
			_groupimp = stdout;
			_dot = stdout;
			_tree = stdout;
			init();
		} else {
			prefix = p;
			prefixit();
		}
	}
	
	void flush() {
		fflush(_main);
		//fflush(_err);
		fflush(_roc);
		fflush(_varimp);
		fflush(_groupimp);
		fflush(_dot);
		fflush(_tree);
	}
	
	void inc() {
		counter++;
		fclose(_main);
		//fclose(_err);
		fclose(_roc);
		fclose(_varimp);
		fclose(_groupimp);
		fclose(_dot);
		fclose(_tree);
		prefixit();
	}
	
	~Logs() { 
		fclose(_main);
		//fclose(_err);
		fclose(_roc);
		fclose(_varimp);
		fclose(_groupimp);
		fclose(_dot);
		fclose(_tree);
	};
} ;


#endif
