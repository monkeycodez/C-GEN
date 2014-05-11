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

int python_is_switch(char *line){
	if(strcmp(line, "##@BEGIN") == 0){
		return BEGIN_EVAL;
	}
	if(strcmp(line, "##@END") == 0){
		return END_EVAL;
	}
	return NO_ACTION;
}

int none_lang_eval(char *line){
	return NO_ACTION;
}


int main(int argc, char *argv[]){
	int lang = LANG_C;
	int inp = 0;
	int c = 0;
	int prtbk = 0;
	char *in = NULL;
	char *out = NULL;

	opterr = 0;

	while((c = getopt(argc, argv, "pbc")) != -1){
		switch(c){
			case 'p':
				lang = LANG_PYTHON;
				break;
			case 'b':
				prtbk = 1;
				break;
			case 'c':
				lang = LANG_C;
				break;
			case '?':
				fprintf(stderr, "Unknown option '-%c'\n", optopt);
				return -1;
		}
	}
	
	//Get the filenames
	if(argc - optind == 1){
		in = argv[optind];
		out = argv[optind];
		f_out = tmpfile();
		inp = 1;
		if(!f_out){
			fprintf(stderr, "ERROR: cannot open temp file\n");
		}
	}else if(argc - optind == 2){
		in = argv[optind];
		out = argv[optind + 1];
		f_out = fopen(out, "w");
		if(!f_out){
			fprintf(stderr, "Cannot open outfile\n");
			return -1;
		}
	}else if(argc - optind == 0){
		fprintf(stderr, "Error, no filenames\n");
		return -1;
	}else{
		fprintf(stderr, "Unknown extra arguments\n");
		return -1;
	}

	f_in = fopen(in, "r");
	if(!f_in){
		fprintf(stderr, "ERROR: cannot open file %s\n", in);
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
	eval(langfunc, prtbk);
	fclose(f_in);
	if(inp){
		long int pos = ftell(f_out);
		char *buf = malloc(sizeof(char) * pos);
		rewind(f_out);
		size_t sz = fread(buf, sizeof(char), pos, f_out);
		if(sz != (unsigned long) pos){
			fprintf(stderr, "Cannot read everything\n");
			return -1;
		}
		fclose(f_out);
		f_out = fopen(out, "w");
		fwrite(buf, sizeof(char), pos, f_out);
		fclose(f_out);
	}else{
		fclose(f_out);
	}

	return 0;
}
