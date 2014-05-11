/*
 * =============================================================================
 *
 *       Filename:  eval.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/10/14 13:21:47
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Matthew Gruda (mg), matthewgruda@gmail.com
 *   Organization:  
 *
 * =============================================================================
 */

#include "cgen.h"

#define OUT(S)	fprintf(f_out, "%s\n", S);

int do_at_panic(lua_State *l){
	fclose(f_out);
	fclose(f_in);
	printf("ERROR: %s\n", lua_tostring(l, -1));
	return 0;
}

int new_print(lua_State *l){
	int nargs = lua_gettop(l);
	int i = 1;
	for(; i <= nargs; i++){
		fprintf(f_out, "%s\n", lua_tostring(l, i));
	}
	return 0;
}

lua_State *init_lua(){
	lua_State *l;
	l = lua_open();
	luaL_openlibs(l);
	lua_atpanic(l, do_at_panic);
	lua_getglobal(l, "print");
	lua_setglobal(l, "debug");
	lua_pushcfunction(l, &new_print);
	lua_setglobal(l, "print");	
	return l;
}

void run_buf(lua_State *l){
	lua_getglobal(l, "loadstring");
	lua_pushvalue(l, -2);
	lua_remove(l, -3);
	int err = lua_pcall(l, 1, 1, 0);
	if(err == LUA_ERRRUN){
		fprintf(stderr, "ERR1: %s\n", lua_tostring(l, -1));
		return;
	}
	err = lua_pcall(l, 0, 0, 0);
	if(err == LUA_ERRRUN){
		fprintf(stderr, "ERR2: %s\n", lua_tostring(l, -1));
		return;
	}
}

luaL_Buffer b;

const char *to_prts(lua_State *l, char *line){
	luaL_buffinit(l, &b);
	luaL_addstring(&b, "print(\"");
	size_t len = strlen(line);
	size_t i = 1, idx = 0;
	for(; i < len; i++){
		lua_checkstack(l, 5);
		if(*(line + i) == '%'){
			if(*(line + i + 1) == '%'){
				luaL_addchar(&b, '%');
				continue;
			}
			luaL_addstring(&b, "\" .. ");
			idx = strcspn(line + i + 1, "%");
			luaL_addlstring(&b, line + i + 1, idx);
			luaL_addstring(&b, " .. \"");
			i += idx + 1;
		}else if(*(line + i) == '\"'){
			luaL_addstring(&b, "\\\"");
		}else{
			luaL_addchar(&b, *(line + i));
		}
	}
	luaL_addstring(&b, "\")");
	luaL_pushresult(&b);
	const char *str = lua_tolstring(l, -1, &len);
	char *s = malloc(sizeof(char) * len);
	memcpy(s, str, len);
	lua_pop(l, 1);
	return s;
}

void eval(lang_f cb, int print_breaks){
	char *line = rline(f_in);
	int val = 0, mode = BEGIN_EVAL;
	lua_State *l = init_lua();
	luaL_Buffer buf;

	val = (cb)(line);
	if(val == END_EVAL){
		mode = END_EVAL;
		if(print_breaks){
			OUT(line);
		}
		free(line);
		line = rline(f_in);
	}else{
		luaL_buffinit(l, &buf);
	}
	for(; line != NULL; line = rline(f_in)){
		val = (cb)(line);
		switch(val){
			case BEGIN_EVAL:
				mode = BEGIN_EVAL;
				if(print_breaks){
					OUT(line);
				}
				free(line);
				line = NULL;
				luaL_buffinit(l, &buf);
				break;
			case END_EVAL:
				mode = END_EVAL;
				luaL_pushresult(&buf);
				if(!print_breaks){
					line = NULL;
				}
				run_buf(l);
				break;
			case NO_ACTION:
				break;
		}
		if(line == NULL){
			continue;
		}
		if(mode == END_EVAL){
			OUT(line);
		}else if(mode == BEGIN_EVAL){
			if(*line == ','){
				luaL_addstring(&buf, to_prts(l, line));
				luaL_addchar(&buf, '\n');
			}else{
				luaL_addstring(&buf, line);
				luaL_addchar(&buf, '\n');
			}
		}
		free(line);
	}
	if(mode == BEGIN_EVAL){
		luaL_pushresult(&buf);
		run_buf(l);
	}
	lua_close(l);
}

