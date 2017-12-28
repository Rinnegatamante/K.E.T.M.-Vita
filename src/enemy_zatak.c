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
	int state;
	float wait;
	int level;
	#else
	double angle;
	double speed;
	int state;
	double wait;
	int level;
	#endif
} ZATAK_DATA;

void enemy_zatak_add(int lv)
{
	int i;
	SPRITE *s;
	ZATAK_DATA *data;

	for(i=0;i<12;i++) {
		s=sprite_add_file("rwingx.png",37,PR_ENEMY);
		s->type=SP_EN_ZATAK;
		s->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
		s->mover=enemy_zatak_move;
		s->anim_speed=0;
		s->aktframe=0;
		data=mmalloc(sizeof(ZATAK_DATA));
		s->data=data;
		data->b.score=25;
		data->b.health=1;
		data->state=0;
		data->wait=0;
		#ifdef GP2X
		data->speed=2.0*(((float)rand()/RAND_MAX)*2);
		#else
		data->speed=2.0*(((double)rand()/RAND_MAX)*2);
		#endif
		if(i<6) {
			data->angle=10;
			s->x=-s->w;
			s->y=50;
		} else {
			data->angle=170;
			s->x=WIDTH;
			s->y=50;
		}
		data->level=lv;
	}
}


void enemy_zatak_move(SPRITE *s)
{
	ZATAK_DATA *d=(ZATAK_DATA *)s->data;

	d->wait+=fps_factor;
	switch(d->state) {
		case 0:
			if(d->wait>=40) {
				d->state=1;
			}
			break;
		case 1:
			d->speed-=(0.02*fps_factor);
			if(d->speed<=0) {
				d->speed=0;
				d->state=2;
			}
			break;
		case 2:
			if(d->angle<90) {
				d->angle+=3*fps_factor;
				if(d->angle>=89) {
					d->angle=90;
					d->state=3;
					if(d->level)
						enemy_laser_create(s,5+d->level);
				}
			} else {
				d->angle-=3*fps_factor;
				if(d->angle<=91) {
					d->angle=90;
					d->state=3;
					if(d->level)
						enemy_laser_create(s,5+d->level);
				}
			}
			break;
		case 3:
			d->speed+=0.3;
			if(d->speed>=(6+d->level)) {
				d->speed=6+d->level;
				d->state=4;
			}
			break;
		case 4:
			if(s->y<0)
				s->type=-1;
			break;
	}
	s->x+=cos(degtorad(d->angle))*d->speed*fps_factor;
	s->y+=sin(degtorad(d->angle))*d->speed*fps_factor;
	s->aktframe=(((int)d->angle+270)%360)/10;
}
