#ifndef _HISCORE_H_
#define _HISCORE_H_

#include <SDL/SDL.h>
#include <math.h>

#include "font.h"
#include "sprite.h"
#include "player.h"

enum _hclist_show_states {
	HCLISTS_INIT,
	HCLISTS_FADEIN,
	HCLISTS_WAIT,
	HCLISTS_FADEOUT,
	HCLISTS_QUIT
};

enum _hclist_edit_states {
	HCLISTE_INIT,
	HCLISTE_ENTRY,
	HCLISTE_QUIT
};

typedef struct {
	//char name[4];
	char name[5];//..changed by Farox
	int score;
} HSC_LIST;

typedef struct {
	double xg;
	double yg;
	int dir;
	double ph;
	double phspeed;
	double amp;
	double ampspeed;
	int arrived;
} HSC_DATA;

typedef struct {
	SDL_Surface *l;
	double s;
	char ascii;
	int xpos,ypos;
} LETTER;

void hsc_init();
void hsc_show_init();
void hsc_show_work();
void hsc_show_move();

void hsc_entry_init();
void hsc_entry_work();
void hsc_entry_show();
void hsc_load(); //added by Farox
void hsc_save(); //added by Farox

#endif
