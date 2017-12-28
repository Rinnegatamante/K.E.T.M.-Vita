#ifndef _INTRO_H_
#define _INTRO_H_
#include <SDL/SDL.h>

#include "sprite.h"
#include "menu.h"

enum _intro_states {
	INTRO_INIT,
	INTRO_FADEIN_MOON,
	INTRO_FADEIN_KETM,
	INTRO_SLEEP,
	INTRO_FADEOUT,
	INTRO_QUIT
};

void intro_init();
void intro_work();
void intro_fadein_moon();
void intro_fadein_ketm();
void intro_sleep();
#endif
