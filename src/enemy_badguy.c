#include "enemy.h"

extern SPRITE *player;
extern double fps_factor;

typedef struct {
	ENEMY_BASE b;
	int state;
	int tx;
	int ty;
	#ifdef GP2X
	float speed;
	#else
	double speed;
	#endif
	int level;
} BADGUY_DATA;

void enemy_badguy_add(int lv)
{
	int i;
	SPRITE *s;
	BADGUY_DATA *data;

	for(i=0;i<10;i++) {
		s=sprite_add_file("badguy.png",9,PR_ENEMY);
		s->type=SP_EN_BADGUY;
		s->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
		s->mover=enemy_badguy_move;
		s->anim_speed=3;
		s->aktframe=0;
		switch(i%5) {
			case 0: s->x=rand()%40; break;
			case 1: s->x=WIDTH-rand()%40; break;
			case 2: s->x=220+rand()%40; break;
			case 3: s->x=100+rand()%40; break;
			case 4: s->x=340+rand()%40; break;
		}
		s->y=rand()%40-90;
		data=mmalloc(sizeof(BADGUY_DATA));
		s->data=data;
		data->b.score=5*(lv+1);
		data->b.health=1;
		data->state=0;
		data->tx=player->x;
		data->ty=player->y;
		#ifdef GP2X
		data->speed=fps_factor*((float)(rand()%200)/100)*(3+lv);
		#else
		data->speed=fps_factor*((double)(rand()%200)/100)*(3+lv);
		#endif
		data->level=lv;

	}
}

void enemy_badguy_move(SPRITE *s)
{
	double angle;
	BADGUY_DATA *d=(BADGUY_DATA *)s->data;

	switch(d->state) {
		case 0:
			if((s->x >= player->x)||(s->y>400)) {
				d->state=1;
				s->anim_speed=-3;
				d->tx=-100;
				d->ty=player->y;
				if(d->level>0)
					enemy_bullet_create(s,1+d->level);
				d->speed=6*fps_factor;
			}

		case 1:
			if(s->x <= -s->w) {
				s->type=-1;
			}
	}

	angle=atan2(d->ty-s->y,d->tx-s->x);
	s->x+=cos(angle)*d->speed;
	s->y+=sin(angle)*d->speed;
}
