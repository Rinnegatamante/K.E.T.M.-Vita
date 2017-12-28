#ifndef _ENEMY_H_
#define _ENEMY_H_
#include <SDL/SDL.h>
#include <stdlib.h>
#include <math.h>

#include "sprite.h"
#include "particle.h"
#include "support.h"
#include "bonus.h"
#include "loadlv.h"


typedef struct {
	int score;
	int health;
} ENEMY_BASE;

typedef struct {
	double angle;
	double speed;
} BULLET_DATA;

typedef struct {
	double angle;
	double speed;
} LASER_DATA;

typedef struct {
	ENEMY_BASE b;
	double angle;
	double speed;
	double range;
	double delay;
} HOMING_DATA;

typedef struct {
	double wait;
	double framewait;
	double framestat;
} EXPLOSION_DATA;

void enemy_bullet_create(SPRITE *s, double speed);
void enemy_bullet_move(SPRITE *s);
void enemy_laser_create(SPRITE *s, double speed);
void enemy_laser_move(SPRITE *s);
void enemy_homing_create(SPRITE *s);
void enemy_homing_update(SPRITE *s);
void enemy_homing_move(SPRITE *s);
void explosion_add(int x, int y, double wait, int type);
void explosion_move(SPRITE *c);

void enemy_crusher_add(int lv);
void enemy_crusher_controller(CONTROLLER *c);
void enemy_crusher_move(SPRITE *s);

void enemy_cube_add(int lv);
void enemy_cube_move(SPRITE *s);

void enemy_drager_add(int lv);
void enemy_drager_move(SPRITE *s);

void enemy_eyefo_add(int lv);
void enemy_eyefo_controller(CONTROLLER *c);
void enemy_eyefo_move(SPRITE *s);

void enemy_xev_add(int lv);
void enemy_xev_move(SPRITE *s);

void enemy_mine_add(int lv);
void enemy_mine_move(SPRITE *s);
void enemy_supermine_add(SPRITE *s,int level);
void enemy_supermine_move(SPRITE *s);

void enemy_rwingx_add(int lv);
void enemy_rwingx_move(SPRITE *s);

void enemy_cir_add(int lv);
void enemy_cir_move(SPRITE *s);

void enemy_zatak_add(int lv);
void enemy_zatak_move(SPRITE *s);

void enemy_badguy_add(int lv);
void enemy_badguy_move(SPRITE *s);

void enemy_grounder_add(int lv, char speed);
void enemy_grounder_move(SPRITE *s);

void enemy_bgpanel_add(int lv, char speed);
void enemy_bgpanel_move(SPRITE *s);

void enemy_bgpanel2_add(int lv, char speed);
void enemy_bgpanel2_move(SPRITE *s);

void enemy_proball_add(int lv);
void enemy_proball_controller(CONTROLLER *c);
void enemy_proball_move(SPRITE *s);

void enemy_plasmaball_add(int lv);
void enemy_plasmaball_move(SPRITE *s);

void enemy_ming_add(int lv);
void enemy_ming_controller(CONTROLLER *c);
void enemy_ming_move(SPRITE *s);

void enemy_greeter_add(int lv);
void enemy_greeter_controller(CONTROLLER *c);
void enemy_greeter_move(SPRITE *s);

void enemy_curver_add(int lv);
void enemy_curver_controller(CONTROLLER *c);
void enemy_curver_move(SPRITE *s);

void enemy_boss01_add(int lv);
void enemy_boss01_setpos(int x, int y);
void enemy_boss01_hitbyplayer(SPRITE *c);
void enemy_boss01_hitbyweapon(SPRITE *c, SPRITE *s, int angle);
void enemy_boss01_move(SPRITE *c);
void enemy_boss01_fire(int where);

void enemy_boss02_add(int lv);
void enemy_boss02_move(SPRITE *s);
void enemy_boss02_waitstate(SPRITE *s, int nextstate);
void enemy_boss02_hitbyplayer(SPRITE *c);
void enemy_boss02_hitbyweapon(SPRITE *c, SPRITE *s, int angle);
void enemy_boss02_sr_add(SPRITE *c);
void enemy_boss02_sr_controller(CONTROLLER *c);
void enemy_boss02_ice_add(SPRITE *c);
void enemy_boss02_ice_controller(CONTROLLER *e);
void enemy_boss02_ice_move(SPRITE *s);

void enemy_boss03_add(int lv);
void enemy_boss03_move(SPRITE *s);
void enemy_boss03_hitbyplayer(SPRITE *c); //added by Farox
void enemy_boss03_hitbyweapon(SPRITE *c, SPRITE *s, int angle); //added by Farox
void enemy_boss03_fire(int where);  //added by Farox


void enemy_gametext_add(char *text, int y);
void enemy_gametext_mover(SPRITE *s);
#endif

