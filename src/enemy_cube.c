#include "enemy.h"

extern double fps_factor;

typedef struct {
	ENEMY_BASE b;
	#ifdef GP2X
	float radius;
	float angle;
	#else
	double radius;
	double angle;
	#endif
	int flag0;
	int flag1;
	int flag2;
	int level;
} CUBE_DATA;

void enemy_cube_add(int lv)
{
	int i;
	SPRITE *s;
	CUBE_DATA *data;

	for(i=0;i<16;i++) {
		s=sprite_add_file("cube.png",16,PR_ENEMY);
		s->type=SP_EN_CUBE;
		s->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
		s->mover=enemy_cube_move;
		s->anim_speed=1;
		s->aktframe=i%s->frames;
		data=mmalloc(sizeof(CUBE_DATA));
		s->data=data;
		data->b.score=15*(1+lv);
		data->b.health=1+lv;
		data->radius=350;
		data->angle=360/16*i;
		data->flag0=1;
		data->flag1=0;
		data->flag2=0;
		data->level=lv;
	}
}

void enemy_cube_move(SPRITE *s)
{
	CUBE_DATA *d=(CUBE_DATA *)s->data;

	d->flag2+=fps_factor;
	if(d->flag2<500) {
		if(!d->flag1) {
			d->radius+=fps_factor;
			if(d->radius>150) d->flag1=1;
		} else {
			d->radius-=fps_factor;
			if(d->radius<=10) d->flag1=0;
		}
		d->angle-=3*fps_factor;
		if(d->angle<0)
			d->angle+=360;


		if(!(rand()%500)) {
			enemy_bullet_create(s,1);
		}

		if(!(rand()%600)) {
			bonus_add(s->x,s->y,SP_BONUS_FIREPOWER);
		}
	} else {
		d->radius+=2*fps_factor;
		if(d->radius>350)
			s->type=-1;
	}

	s->x=(cos(degtorad(d->angle))*d->radius)+WIDTH/2;
	s->y=(sin(degtorad(d->angle))*d->radius)+HEIGHT/2;

}
