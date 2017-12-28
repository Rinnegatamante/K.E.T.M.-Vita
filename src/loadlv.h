#ifndef _LOADLV_H_
#define _LOADLV_H_

//#include <SDL2/SDL.h>
#include <SDL/SDL.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


typedef struct _levelentry {
	Uint32 time;
	char command;
	//char para1[64];
	char para1[128];//..by farox ?
	int para2;
	int done;
	struct _levelentry *next;
} LEVELENTRY;

#include "support.h"


int loadlv(int level);
char *loadlv_getint(char *c, int *nr);
char *loadlv_getstr(char *c, char *buffer);
void loadlv_serror(char *filename, int line_nr);
void loadlv_addentry(Uint32 time, char command, char *para1, int para2);
void loadlv_freeentry();

#endif
