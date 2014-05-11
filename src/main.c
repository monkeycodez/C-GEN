/*
 * =============================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  Main file for c-gen
 *
 *        Version:  1.0
 *        Created:  05/10/14 10:04:27
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Matthew Gruda (mg), matthewgruda@gmail.com
 *   Organization:  
 *
 * =============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "cgen.h"

FILE* f_in = NULL;
FILE* f_out = NULL;

int c_is_switch(char *line){
	if(strcmp(line, "//@BEGIN") == 0){
		return BEGIN_EVAL;
	}else if(strcmp(line, "//@END") == 0){
		return END_EVAL;
	}
	return NO_ACTION;
}

int none_lang_eval(char *line){
	return NO_ACTION;
}


int main(int argc, char *argv[]){
	int inp = 0;
	int lang = LANG_C;
	int c = 0;
	char *in = NULL;
	char *out = NULL;

	opterr = 0;

	while((c = getopt(argc, argv, "ic")) != -1){
		switch(c){
			case 'i':
				inp = 1;
				break;
			case 'c':
				lang = 1;
				break;
			case '?':
				fprintf(stderr, "Unknown option '-%c'\n", optopt);
				return -1;
		}
	}
	
	//Get the filenames
	if(inp && argc - optind == 1){
		in = argv[optind];
		out = argv[optind];
		f_out = tmpfile();
		if(!f_out){
			fprintf(stderr, "ERROR: cannot open temp file");
		}
	}else if(argc - optind == 2){
		in = argv[optind];
		out = argv[optind + 1];
		f_out = fopen(out, "w");
		if(!f_out){
			fprintf(stderr, "Cannot open outfile");
		}
	}else{
		fprintf(stderr, "Unknown extra arguments");
		return -1;
	}

	//Open tempfile and in file

	f_in = fopen(in, "r");
	if(!f_in){
		fprintf(stderr, "ERROR: cannot open file %s", in);
	}
	
	lang_f langfunc = NULL;
	//Select lang function
	switch(lang){
		case LANG_C:
			langfunc = &c_is_switch;
			break;
		case LANG_NONE:
		default:
			langfunc = &none_lang_eval;
	}
	eval(langfunc);
	fclose(f_in);
	fclose(f_out);



	return 0;
}
