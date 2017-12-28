#ifndef _MENU_H_
#define _MENU_H_
//#include <SDL2/SDL.h>
#include <SDL/SDL.h>
#include <stdlib.h>
#include <math.h>

#include "support.h"
#include "font.h"
#include "sprite.h"
#include "particle.h"
#include "fps.h"
#include "soundmanager.h"
#include "thegame.h" //added by Farox

/* Menues */
enum _menu_states { MEN_START, MEN_PAUSE, MEN_OPTION, MEN_VOLUME, MEN_DIFF, MEN_FPS }; //MEN_FPS added by Farox

typedef struct {
	SPRITE *opts[20][5];
	SDL_Surface *opts_surface[20];
	SDL_Surface *bg;
	int active_item;
	int select_finish;
	int nopt;
	int alpha;
	int alphadir;
	int fadeout;
	int timeout;
} MENU;

typedef struct {
	int i0;
	int i1;
	int i2;
	int i3;
} MENU_DATA;

void menusystem_init();
void menu_init();
void menu_work();
void startmenu_init();
void startmenu_work();
void pausemenu_init();
void pausemenu_work();
void optionmenu_init();
void optionmenu_work();
void volumemenu_init();
void volumemenu_work();
void difficultymenu_init();
void difficultymenu_work();
void fpsmenu_init(); //added by Farox
void fpsmenu_work(); //added by Farox
void genericmenu_init(char *options[], MENU *m, int fadeout, int timeout);
void genericmenu_work(MENU *m);
#endif

