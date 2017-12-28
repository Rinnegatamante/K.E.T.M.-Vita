#include "enemy.h"

extern SPRITE *player;
#ifdef GP2X
extern float fps_factor;
#else
extern double fps_factor;
#endif

typedef struct {
	ENEMY_BASE b;
	int state;
	int tx;
	int ty;
	int level;
} XEV_DATA;

void enemy_xev_add(int lv)
{
	int i;
	SPRITE *s;
	XEV_DATA *data;

	for(i=0;i<3;i++) {
		s=sprite_add_file("crusher.png",15,PR_ENEMY);
		s->type=SP_EN_XEV;
		s->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
		s->mover=enemy_xev_move;
		s->anim_speed=1;
		s->aktframe=0;
		switch(rand()%3) {
			case 0: s->x=rand()%20; break;
			case 1: s->x=WIDTH-rand()%20; break;
			case 2: s->x=(WIDTH/2)+10+rand()%20; break;
		}
		s->y=rand()%20-50;
		data=mmalloc(sizeof(XEV_DATA));
		s->data=data;
		data->b.score=5;
		data->b.health=1;
		data->state=0;
		data->tx=player->x;
		data->ty=player->y;
		data->level=lv;
	}
}

void enemy_xev_move(SPRITE *s)
{
	#ifdef GP2X
	float angle;
	float speed=0;
	#else
	double angle;
	double speed=0;
	#endif
	XEV_DATA *d=(XEV_DATA *)s->data;

	switch(d->state) {
		case 0:
			speed=3;
			if((s->x >= player->x)||(s->y>400)) {
				d->state=1;
				s->anim_speed=-1;
				d->tx=-100;
				d->ty=player->y;
				if(d->level)
					enemy_bullet_create(s,1+d->level);
				bonus_add(s->x,s->y,rand()%(SP_BONUS_LAST-SP_BONUS_FIREPOWER)+SP_BONUS_FIREPOWER);
			}

		case 1:
			speed=6;
			if(s->x <= -s->w) {
				s->type=-1;
			}
	}

	angle=atan2(d->ty-s->y,d->tx-s->x);
	s->x+=cos(angle)*speed*fps_factor;
	s->y+=sin(angle)*speed*fps_factor;
}
