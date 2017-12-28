#ifndef _STARTINTRO_H_
#define _STARTINTRO_H_

#include <SDL/SDL.h>

#include "support.h"
#include "particle.h"
#include "font.h"

enum _startintro_states {
	STIN_INIT,
	#ifdef PACC
	STIN_FADEIN_PACC, ////added for PACC
	#endif
	STIN_FADEIN_BB,
	STIN_FADEIN_PRESENTS,
	STIN_FADEIN_KETM,
	STIN_SLEEP,
	STIN_QUIT
};

void startintro_init();
void startintro_work();
//void startintro_centerimg();
void startintro_centerimg(SDL_Surface *src, double scale);
#endif
