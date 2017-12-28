#include "enemy.h"

#ifdef GP2X
extern float fps_factor;
#else
extern double fps_factor;
#endif
extern SPRITE *player;

typedef struct {
	ENEMY_BASE b;
	#ifdef GP2X
	float angle;
	float speed;
	float turnspeed;
	float left;
	float right;
	#else
	double angle;
	double speed;
	double turnspeed;
	double left;
	double right;
	#endif
	int state;
	int level;
} CURVER_DATA;

void enemy_curver_add(int lv)
{
	int i;
	SPRITE *s;
	CURVER_DATA *data;
	CONTROLLER *c;
	int *id_array;

	c=controller_add();
	c->max=10;
	id_array=mmalloc(sizeof(int)*(c->max+2));
	c->e=id_array;
	c->con=enemy_curver_controller;

	for(i=0;i<c->max;i++) {
		s=sprite_add_file("rwingx.png",37,PR_ENEMY);
		id_array[i]=s->id;
		s->type=SP_EN_CURVER;
		s->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
		s->mover=enemy_curver_move;
		s->anim_speed=0;
		data=mmalloc(sizeof(CURVER_DATA));
		s->data=data;
		data->b.score=25;
		data->b.health=3;
		data->angle=0;
		data->speed=7;
		data->turnspeed=8;
		data->left=50;
		data->right=WIDTH-50-s->w;
		data->state=0;
		data->level=lv;
		s->x=-(s->w*(i+1))*0.5;
		s->y=50;
	}
}

void enemy_curver_controller(CONTROLLER *c)
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

void enemy_curver_move(SPRITE *s)
{
	CURVER_DATA *d=(CURVER_DATA *)s->data;

	switch(d->state) {
		case 0:
			if(s->x>=d->right)
				d->state=1;
			break;
		case 1:
			d->angle+=d->turnspeed*fps_factor;
			if(d->angle>=180) {
				d->angle=180;
				d->state=2;
				if(d->level>0)
					enemy_bullet_create(s,3+d->level);

			}
			break;
		case 2:
			if(s->x<=d->left)
				d->state=3;
			break;
		case 3:
			d->angle-=d->turnspeed*fps_factor;
			if(d->angle<=0) {
				d->angle=360;
				d->state=4;
				if(d->level>0)
					enemy_bullet_create(s,3+d->level);
			}
			break;
		case 4:
			if(s->x>=d->right)
				d->state=5;
			break;
		case 5:
			d->angle-=d->turnspeed*fps_factor;
			if(d->angle<=180) {
				d->angle=180;
				d->state=6;
				if(d->level>0)
					enemy_bullet_create(s,3+d->level);
			}
			break;
		case 6:
			if(s->x<=d->left)
				d->state=7;
			break;
		case 7:
			d->angle+=d->turnspeed*fps_factor;
			if(d->angle>=360) {
				d->angle=0;
				d->state=8;
				if(d->level>0)
					enemy_bullet_create(s,3+d->level);
			}
			break;
		case 8:
			if(s->x>WIDTH)
				s->flags&=~SP_FLAG_VISIBLE;
			break;
	}

	s->x+=cos(degtorad(d->angle))*d->speed*fps_factor;
	s->y+=sin(degtorad(d->angle))*d->speed*fps_factor;
	s->aktframe=(((int)d->angle+270)%360)/10;
}
