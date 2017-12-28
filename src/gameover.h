#ifndef _GAMEOVER_H_
#define _GAMEOVER_H_

//#include <SDL2/SDL.h>
#include <SDL/SDL.h>
#include "support.h"
#include "sprite.h"
#include "particle.h"
#include "player.h"

enum _gameover_states {
	GO_INIT,
	GO_FADEINIT,
	GO_FADEIN1,
	GO_FADEIN2,
	GO_WAIT,
	GO_START_EXPLODE,
	GO_WAIT_EXPLODE,
	GO_QUIT
};

void gameover_init();
void gameover_work();

void gameover_display(double s1, double s2);

#endif
