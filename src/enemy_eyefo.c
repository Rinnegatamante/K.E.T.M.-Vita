#include "enemy.h"

#ifdef GP2X
extern float fps_factor;
#else
extern double fps_factor;
#endif

typedef struct {
	ENEMY_BASE b;
	#ifdef GP2X
	double radius;
	int flag1;
	double angle;
	double counter;
	double xcenter;
	double ycenter;
	int level;
	#else
	double radius;
	int flag1;
	double angle;
	double counter;
	double xcenter;
	double ycenter;
	int level;
	#endif
} EYEFO_DATA;

void enemy_eyefo_add(int lv)
{
	int i;
	SPRITE *s;
	EYEFO_DATA *data;
	CONTROLLER *c;
	int *id_array;

	c=controller_add();
	c->max=16;
	id_array=mmalloc(sizeof(int)*(c->max+2));
	c->e=id_array;
	c->con=enemy_eyefo_controller;

	for(i=0;i<16;i++) {
		s=sprite_add_file("eyefo.png",15,PR_ENEMY);
		id_array[i]=s->id;
		s->type=SP_EN_EYEFO;
		s->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
		s->mover=enemy_eyefo_move;
		s->anim_speed=2;
		s->aktframe=i%s->frames;
		data=mmalloc(sizeof(EYEFO_DATA));
		s->data=data;
		data->b.score=10;
		data->b.health=1;
		data->radius=10;
		data->flag1=0;
		data->angle=360/16*i;
		data->counter=0;
		data->xcenter=(WIDTH/2);
		data->ycenter=-100;
		data->level=lv;

	}
}

void enemy_eyefo_controller(CONTROLLER *c)
{
	int i;
	int *id_array=c->e;
	SPRITE *s;
	int invisible=0;

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
	bonus_add(id_array[c->max],id_array[c->max+1],SP_BONUS_EXTRA);
	controller_remove(c);
}

void enemy_eyefo_move(SPRITE *s)
{
	EYEFO_DATA *d=(EYEFO_DATA *)s->data;

	if(!d->flag1) {
		d->radius+=fps_factor;
		if(d->radius>150)
			d->flag1=1;
	} else {
		d->radius-=fps_factor;
		if(d->radius<=10)
			d->flag1=0;
	}
	d->angle+=5*fps_factor;
	if(d->angle>=360)
		d->angle-=360;
	s->x=(cos(degtorad(d->angle))*d->radius)+d->xcenter;
	s->y=(sin(degtorad(d->angle))*d->radius)+d->ycenter;

	if(d->level)
		if(rand()%(1000-d->level*100)==0)
			enemy_homing_create(s);

	d->counter+=fps_factor;
	if(d->counter<300) {
	    #ifdef GP2X
	    if(d->ycenter<240)  //Farox
	    #else
		if(d->ycenter<272)  //denis
		#endif
			d->ycenter+=fps_factor;
	} else if(d->counter>600) {
			d->ycenter-=fps_factor;
			if(d->ycenter+s->y<-s->w)
				// s->type=-1;
				s->flags&=~SP_FLAG_VISIBLE;
	}
}
