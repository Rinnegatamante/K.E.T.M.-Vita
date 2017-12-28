#ifndef _THEGAME_H_
#define _THEGAME_H_

#include <SDL/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "sprite.h"
#include "particle.h"
#include "player.h"
#include "enemy.h"
#include "menu.h"
#include "score.h"
#include "bg.h"
#include "loadlv.h"
#include "soundmanager.h" //added by Farox

void thegame_init();
void thegame_work();
void thegame_level();
void adjust_start_time(Uint32 pause_time);//added by Farox
#endif
