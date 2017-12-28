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
	int dir;
	#ifdef GP2X
	float speed;
	#else
	double speed;
	#endif
	int level;
} PROBALL_DATA;

void enemy_proball_add(int lv)
{
	int i;
	SPRITE *s;
	PROBALL_DATA *data;
	CONTROLLER *c;
	int *id_array;

	c=controller_add();
	c->max=24;
	id_array=mmalloc(sizeof(int)*(c->max+2));
	c->e=id_array;
	c->con=enemy_proball_controller;

	for(i=0;i<c->max;i++) {
		s=sprite_add_file("protectball.png",11,PR_ENEMY);
		id_array[i]=s->id;
		s->type=SP_EN_PROBALL;
		s->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
		s->mover=enemy_proball_move;
		s->anim_speed=3;
		s->aktframe=i%11;
		s->x=(WIDTH/2)-s->w/2;
		s->y=-s->h-i*s->h;
		data=mmalloc(sizeof(PROBALL_DATA));
		s->data=data;
		data->b.score=10;
		data->b.health=2;
		data->state=0;
		data->speed=5+lv;
		data->dir=i%2==0?0:1;
		data->level=lv;
	}
}

void enemy_proball_controller(CONTROLLER *c)
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

void enemy_proball_move(SPRITE *s)
{
	PROBALL_DATA *d=(PROBALL_DATA *)s->data;

	switch(d->state) {
		case 0:
			s->y+=d->speed*fps_factor;
			if(s->y>=250) {
				d->speed=2;
				d->state=1;
			}
			break;
		case 1:
			d->speed+=0.1;
			if(d->dir==0) {
				s->x-=d->speed*fps_factor;
				if(s->x<=10) {
					s->x=10;
					d->speed=3;
					d->state=2;
				}
			} else {
				s->x+=d->speed*fps_factor;
				if(s->x>=(WIDTH-10)-s->w) {
					s->x=(WIDTH-10)-s->w;
					d->speed=3;
					d->state=2;
				}
			}
			break;
		case 2:
			s->y-=d->speed*fps_factor;
			if(s->y<=0) {
				s->y=0;
				d->speed=2;
				d->state=3;
			}
			break;
		case 3:
			d->speed+=0.2;
			s->y+=d->speed*fps_factor;
			if(d->dir==0) {
				s->x+=d->speed*fps_factor;
			} else {
				s->x-=d->speed*fps_factor;
			}
			#ifdef GP2X
			if(s->y>=240) //Farox
			#else
			if(s->y>=272) //denis 480
			#endif
				// s->type=-1;
				s->flags&=~SP_FLAG_VISIBLE;
			break;
	}
}
