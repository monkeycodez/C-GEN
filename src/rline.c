/*
 * =============================================================================
 *
 *       Filename:  rline.c
 *
 *    Description:  A Line Reading function
 *
 *        Version:  1.0
 *        Created:  05/10/14 11:00:42
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

char *rline(FILE* file){
	if(file == NULL){
		return NULL;
	}
	int bufsz = 128;
	char *buf = malloc(sizeof(char) * bufsz);
	if(buf == NULL){
		fprintf(stderr, "Out of memory (rline)\n");
		return NULL;
	}
	int ptr = 0;

	int c = getc(file);
	if(c == EOF){
		free(buf);
		return NULL;
	}

	while(c != '\n' && c != EOF){
		*(buf + ptr) = (char)c;
		ptr++;
		c = getc(file);
		if(c == bufsz){
			bufsz = bufsz *  2;
			buf = realloc(buf, bufsz);
			if(buf == NULL){
				fprintf(stderr, "Cannot realloc buffer\n");
				return NULL;
			}
		}
	}
	*(buf + ptr) = '\0';
	return buf;
}


