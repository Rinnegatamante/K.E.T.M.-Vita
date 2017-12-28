#include "enemy.h"

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
	int delay;
	int level;
} MINE_DATA;

void enemy_mine_add(int lv)
{
	SPRITE *s;
	MINE_DATA *data;

	s=sprite_add_file("12side.png",19,PR_ENEMY);
	s->type=SP_EN_MINE;
	s->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	s->mover=enemy_mine_move;
	s->anim_speed=1;
	s->aktframe=0;
	s->x=rand()%(WIDTH+10)-10;
	s->y=-40;
	data=mmalloc(sizeof(MINE_DATA));
	s->data=data;
	data->b.score=30;
	data->b.health=5;
	data->tx=rand()%270+50;
	data->ty=rand()%300+50;
	data->state=0;
	data->level=lv;
}

void enemy_mine_move(SPRITE *s)
{
	MINE_DATA *d=(MINE_DATA *)s->data;
	#ifdef GP2X
	float angle;
	#else
	double angle;
	#endif

	switch(d->state) {
		case 0: /* move */
			angle=atan2(d->ty-s->y,d->tx-s->x);
			s->x+=cos(angle)*1.5*fps_factor;
			s->y+=sin(angle)*1.5*fps_factor;
			if((s->x >= d->tx-1) && (s->x <= d->tx+1) &&
			   (s->y >= d->ty-1) && (s->y <= d->ty+1)) {
				d->state=1;
				d->delay=50;
			}
			break;
		case 1:
			d->delay-=fps_factor;
			if(d->delay<=0) {
				d->state=2;
				d->delay=30;
				s->anim_speed=0;
			}
			break;
		case 2:
			d->delay-=fps_factor;
			if(d->delay<=0) {
				enemy_supermine_add(s,d->level);
				s->type=-1;
			}
			break;
	}
}

void enemy_supermine_add(SPRITE *c, int lv)
{
	SPRITE *s;
	MINE_DATA *data;
	int i;

	for(i=0;i<8;i++) {
		s=sprite_add_file("12side.png",19,PR_ENEMY);
		s->type=SP_EN_MINE;
		s->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
		s->mover=enemy_supermine_move;
		s->anim_speed=2;
		s->aktframe=0;
		s->x=c->x;
		s->y=c->y;
		data=mmalloc(sizeof(MINE_DATA));
		s->data=data;
		data->b.score=10*(1+lv);
		data->b.health=1+lv;
		data->state=(360/8)*i;
		data->level=lv;
	}
}

void enemy_supermine_move(SPRITE *s)
{
	MINE_DATA *d=(MINE_DATA *)s->data;

	if(d->level)
		if(rand()%(100-d->level*10)==0)
			enemy_bullet_create(s,3+d->level);

	s->x+=cos(degtorad(d->state))*3*fps_factor;
	s->y+=sin(degtorad(d->state))*3*fps_factor;

	if((s->x<0-s->w)||(s->x>WIDTH)||(s->y<0-s->h)||(s->y>HEIGHT))
		s->type=-1;
}
