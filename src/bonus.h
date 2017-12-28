#ifndef _BONUS_H_
#define _BONUS_H_

//#include <SDL2/SDL.h>
#include <SDL/SDL.h>
#include <stdlib.h>
#include <math.h>

#include "sprite.h"
#include "support.h"

typedef struct {
	double speed;
} BONUS_DATA;

typedef struct {
	double distance;
} BIT_DATA;

void bonus_add(int x, int y, int type);
void bonus_move(SPRITE *s);

void bonus_info_add(int x, int y, char *filename);
void bonus_info_move(SPRITE *c);

void bonus_info_text(int x, int y, char *text, int font);
void bonus_infotext_move(SPRITE *c);

#endif
