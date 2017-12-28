#include "enemy.h"

#ifdef GP2X
extern float fps_factor;
#else
extern double fps_factor;
#endif
extern SPRITE *player;

typedef struct {
	ENEMY_BASE b;
	int angle;
	#ifdef GP2X
	float speed;
	int state;
	float wait;
	#else
	double speed;
	int state;
	double wait;
	#endif
	int level;
} RWINGX_DATA;

void enemy_rwingx_add(int lv)
{
	int i;
	SPRITE *s;
	RWINGX_DATA *data;

	for(i=0;i<8;i++) {
		s=sprite_add_file("rwingx.png",37,PR_ENEMY);
		s->type=SP_EN_RWINGX;
		s->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
		s->mover=enemy_rwingx_move;
		s->anim_speed=0;
		data=mmalloc(sizeof(RWINGX_DATA));
		s->data=data;
		data->b.score=25*(1+lv);
		data->b.health=1+lv;
		data->angle=270;
		data->speed=0.5;
		data->state=0;
		data->wait=20;
		data->level=lv;
		s->x=(WIDTH/8)*i-20;
		s->y=-50;
	}
}


void enemy_rwingx_move(SPRITE *s)
{
	RWINGX_DATA *d=(RWINGX_DATA *)s->data;

	switch(d->state) {
		case 0: /* sleep(d->wait) ticks */
			if(d->wait>0) {
				d->wait-=fps_factor;
			} else {
				/* langsam nach unten */
				d->state=1;
				d->angle=90;
				d->speed=1;
				d->wait=100;
			}
			break;
		case 1: /* move (d->wait) ticks */
			if(d->wait>0) {
				d->wait-=fps_factor;
			} else {
				d->state=2;
				d->wait=60;
				d->speed=0;
			}
			break;
		case 2: /* sleep(d->wait) ticks */
			if(d->wait>0) {
				d->wait-=fps_factor;
			} else {
				/* schnell richtung player */
				d->state=3;
				d->angle=radtodeg(atan2(player->y-s->y,player->x-s->x));
				d->speed=4+d->level;
				d->wait=50;
			}
			break;
		case 3: /* move (d->wait) ticks */
			if(s->y>=HEIGHT-150) {
				if(d->level)
					enemy_bullet_create(s,3+d->level);

				d->state=4;
				d->angle+=180;
				d->angle%=360;
				d->speed=6+d->level;
			}
			break;
		case 4:
			if((s->x<-s->w) || (s->x>WIDTH) || (s->y<-s->h) || (s->y>HEIGHT))
				s->type=-1;
			break;
	}

	s->x+=cos(degtorad(d->angle))*d->speed*fps_factor;
	s->y+=sin(degtorad(d->angle))*d->speed*fps_factor;
	s->aktframe=((d->angle+270)%360)/10;
}
