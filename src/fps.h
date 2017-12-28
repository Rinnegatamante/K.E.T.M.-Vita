#ifndef _FPS_H_
#define _FPS_H_

//#include <SDL2/SDL.h>
#include <SDL/SDL.h>

#include "support.h"
#include "font.h"

#define TICK_INTERVAL 28
// #define PIXEL_SPEED 50
// #define PIXEL_SPEED 50

enum _fps_methods { WAITFORFRAME, MAXFPS };

void fps_init();
void toggle_fps();
void fps_show();
void fps_newframe();
Uint32 TimeLeft();
void waitfornextframe();

#endif
