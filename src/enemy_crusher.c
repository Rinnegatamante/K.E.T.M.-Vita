#include "enemy.h"

extern double fps_factor;

typedef struct {
	ENEMY_BASE b;
	int c1;
	int c2;
	int level;
} CRUSHER_DATA;

void enemy_crusher_add(int lv)
{
	int i;
	SPRITE *s;
	CRUSHER_DATA *data;
	CONTROLLER *c;
	int *id_array;

	c=controller_add();
	c->max=10;
	id_array=mmalloc(sizeof(int)*(c->max+2));
	c->e=id_array;
	c->con=enemy_crusher_controller;

	for(i=0;i<c->max;i++) {
		s=sprite_add_file("crusher.png",15,PR_ENEMY);
		id_array[i]=s->id;
		s->type=SP_EN_CRUSHER;
		s->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
		s->mover=enemy_crusher_move;
		s->anim_speed=i-5;
		s->aktframe=i%s->frames;
		s->x=(i*25)+10;
		if(i<=4)
			s->y=-(i*10);
		else
			s->y=-((9-i)*10);
		data=mmalloc(sizeof(CRUSHER_DATA));
		s->data=data;
		data->b.score=10*(1+lv);
		data->b.health=1+lv;
		data->c1=0;
		data->c2=0;
		data->level=lv;

	}
}

void enemy_crusher_controller(CONTROLLER *c)
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
	bonus_add(id_array[c->max],id_array[c->max+1],SP_BONUS_COIN);
	controller_remove(c);
}

void enemy_crusher_move(SPRITE *s)
{
	CRUSHER_DATA *d=(CRUSHER_DATA *)s->data;

	if(d->c2<2) {
		if(!d->c1) {
			s->y+=(3+d->level)*fps_factor;
			if(s->y>200) {
				d->c1=1;
				d->c2+=1;
			}
		} else {
			s->y-=(3+d->level)*fps_factor;
			if(s->y<50)
				d->c1=0;
		}
		if(d->level>0)
			if(rand()%(100-d->level*10)==0)
				enemy_bullet_create(s,3+d->level);
	} else {
		s->y-=4;
		if(s->y<-(s->w))
			s->flags&=~SP_FLAG_VISIBLE;
	}
}
