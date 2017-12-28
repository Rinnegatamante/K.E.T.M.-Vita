#include "enemy.h"

#ifdef GP2X
extern float fps_factor;
#else
extern double fps_factor;
#endif
extern SPRITE *player;

typedef struct {
	ENEMY_BASE b;
	int state;
	#ifdef GP2X
	float angle;
	float speed;
	#else
	double angle;
	double speed;
	#endif
	int type;
	int level;
} MING_DATA;

void enemy_ming_add(int lv)
{
	int i;
	SPRITE *s;
	MING_DATA *data;
	CONTROLLER *c;
	int *id_array;

	c=controller_add();
	c->max=10;
	id_array=mmalloc(sizeof(int)*(c->max+2));
	c->e=id_array;
	c->con=enemy_ming_controller;


	for(i=0;i<c->max;i++) {
		s=sprite_add_file("ming.png",36,PR_ENEMY);
		id_array[i]=s->id;
		s->type=SP_EN_MING;
		s->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
		s->mover=enemy_ming_move;
		s->anim_speed=0;
		s->aktframe=0;
		data=mmalloc(sizeof(MING_DATA));
		s->data=data;
		data->b.score=25;
		data->b.health=1;
		data->angle=90;
		data->speed=4+lv;
		data->state=0;
		data->level=lv;
		switch(i) {
		case 0:
			s->x=(WIDTH/2);
			s->y=-30;
			data->type=0;
			break;
		case 1:
			s->x=(WIDTH/2)-(s->w/2);
			s->y=-30-s->h;
			data->type=1;
			break;
		case 2:
			s->x=(WIDTH/2)+(s->w/2);
			s->y=-30-s->h;
			data->type=2;
			break;
		case 3:
			s->x=(WIDTH/2)-s->w;
			s->y=-30-2*s->h;
			data->type=1;
			break;
		case 4:
			s->x=(WIDTH/2);
			s->y=-30-2*s->h;
			data->type=0;
			break;
		case 5:
			s->x=(WIDTH/2)+s->w;
			s->y=-30-2*s->h;
			data->type=2;
			break;
		case 6:
			s->x=(WIDTH/2)-s->w-(s->w/2);
			s->y=-30-3*s->h;
			data->type=1;
			break;
		case 7:
			s->x=(WIDTH/2)-s->w/2;
			s->y=-30-3*s->h;
			data->type=1;
			break;
		case 8:
			s->x=(WIDTH/2)+s->w/2;
			s->y=-30-3*s->h;
			data->type=2;
			break;
		case 9:
			s->x=(WIDTH/2)+s->w+s->w/2;
			s->y=-30-3*s->h;
			data->type=2;
			break;
		}
	}
}

void enemy_ming_controller(CONTROLLER *c)
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
	bonus_add(id_array[c->max],id_array[c->max+1],SP_BONUS_FIREPOWER);
	controller_remove(c);
}



void enemy_ming_move(SPRITE *s)
{
	MING_DATA *d=(MING_DATA *)s->data;

	switch(d->state) {
	case 0:
		if(s->y>=200) {
			d->state=1;
			d->speed=5;
		}
		break;
	case 1:
		if(d->type==1) {
			d->angle+=4*fps_factor;
			if(d->angle>=180) {
				d->angle=180;
				d->state=2;
				if(d->level>0)
					enemy_bullet_create(s,d->level+2);
			}
		} else if(d->type==2) {
			d->angle-=4*fps_factor;
			if(d->angle<=0) {
				d->angle=0;
				d->state=2;
				if(d->level>0)
					enemy_bullet_create(s,d->level+2);
			}
		} else {
			d->state=2;
			if(d->level>0)
				enemy_bullet_create(s,d->level+2);
		}
		break;
	case 2:
		if((s->x<-s->w)||(s->x>WIDTH)||(s->y<-s->h)||(s->y>HEIGHT))
			s->flags&=~SP_FLAG_VISIBLE;
		break;


	}

	s->x+=cos(degtorad(d->angle))*d->speed*fps_factor;
	s->y+=sin(degtorad(d->angle))*d->speed*fps_factor;
	s->aktframe=((int)d->angle%360)/10;
	s->aktframe%=36;
}
