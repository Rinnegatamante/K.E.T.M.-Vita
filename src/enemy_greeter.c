#include "enemy.h"

extern double fps_factor;

typedef struct {
	ENEMY_BASE b;
	int state;
	int level;
	double angle;
	double speed;
	double max_y;
} GREETER_DATA;

void enemy_greeter_add(int lv)
{
	int i;
	SPRITE *s;
	GREETER_DATA *data;
	CONTROLLER *c;
	int *id_array;
	static int last=0;

	c=controller_add();
	c->max=5;
	id_array=mmalloc(sizeof(int)*(c->max+2));
	c->e=id_array;
	c->con=enemy_greeter_controller;

	for(i=0;i<c->max;i++) {
		s=sprite_add_file("ming.png",36,PR_ENEMY);
		id_array[i]=s->id;
		s->type=SP_EN_GREETER;
		s->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
		s->mover=enemy_greeter_move;
		s->anim_speed=0;
		data=mmalloc(sizeof(GREETER_DATA));
		s->data=data;
		if(last)
			s->x=WIDTH-s->w;
		else
			s->x=0;
		s->y=-i*s->h;
		data->angle=atan2((HEIGHT-s->h-60)-s->y,WIDTH/2-s->x);
		data->max_y=(HEIGHT-s->h-60);
		data->speed=5;
		data->state=0;
		data->b.score=5;
		data->b.health=1;
		data->level=lv;
	}
	last=~last;

}

void enemy_greeter_controller(CONTROLLER *c)
{
	int i;
	int *id_array=c->e;
	SPRITE *s;
	int invisible=0;
	static int bonus=SP_BONUS_ADDSPEED;

	for(i=0;i<c->max;i++) {
		s=sprite_get_by_id(id_array[i]);
		if(s!=NULL)
			if(!s->flags&SP_FLAG_VISIBLE)
				invisible++;
	}
	if(invisible==c->max) {
		for(i=0;i<c->max;i++) {
			s=sprite_get_by_id(id_array[i]);
			s->type=-1;
		}
		controller_remove(c);
		return;
	}

	for(i=0;i<c->max;i++) {
		s=sprite_get_by_id(id_array[i]);
		if(s!=NULL) {
			id_array[c->max]=s->x;
			id_array[c->max+1]=s->y;
			return;
		}
	}
	bonus_add(id_array[c->max],id_array[c->max+1],bonus);
	bonus++;
	if(bonus==SP_BONUS_LAST)
		bonus=SP_BONUS_FIREPOWER;
	controller_remove(c);
}

void enemy_greeter_move(SPRITE *s)
{
	GREETER_DATA *d=(GREETER_DATA *)s->data;

	switch(d->state) {
	case 0:	/* nach unten */
		if(s->y>=d->max_y) {
			d->angle=atan2(0-s->y,WIDTH/2-s->x);
			d->state=1;
			d->speed=7;
		}
		if(d->level)
			if(rand()%(100-d->level*10)==0)
				enemy_bullet_create(s,3+d->level);

		break;
	case 1:
		if(s->y<-s->h) {
			s->flags&=~SP_FLAG_VISIBLE;
		}
		break;
	}
	s->x+=cos(d->angle)*d->speed*fps_factor;
	s->y+=sin(d->angle)*d->speed*fps_factor;
	s->aktframe=radtodeg(d->angle)/10;
	s->aktframe%=36;
}
