#include <conio.h>
#include <stdio.h>
#include <errno.h>

#include "flog.h"

FILE *logfile;

void flog_start(){
	 logfile = fopen("logf.txt", "w");
	 if (logfile == NULL) {
		 ///err
	 }
}

void flog(char *str){
	fprintf(logfile, "%s\n", str);
}

void flog_stop(){
	fclose(logfile);
}