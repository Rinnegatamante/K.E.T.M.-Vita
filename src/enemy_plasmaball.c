#include "enemy.h"

#ifdef GP2X
extern float fps_factor;
#else
extern double fps_factor;
#endif

typedef struct {
	ENEMY_BASE b;
	int state;
	int destx, desty;
	#ifdef GP2X
	float speed;
	float ang;
	#else
	double speed;
	double ang;
	#endif
	int r;
	int c;
	int level;
} PLASMABALL_DATA;

void enemy_plasmaball_add(int lv)
{
	int i,j;
	SPRITE *s;
	PLASMABALL_DATA *data;

	for(j=0;j<4;j++) {
		for(i=0;i<6;i++) {
			s=sprite_add_file("plasmaball.png",11,PR_ENEMY);
			s->type=SP_EN_PLASMABALL;
			s->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
			s->mover=enemy_plasmaball_move;
			s->anim_speed=1;
			s->aktframe=rand()%s->frames;
			data=mmalloc(sizeof(PLASMABALL_DATA));
			s->data=data;
			data->b.score=25;
			data->b.health=2;
			data->destx=i*35+40;
			data->desty=200-(j*40);
			data->state=0;
			#ifdef GP2X
			data->speed=3*((float)rand()/RAND_MAX+1);
			#else
			data->speed=3*((double)rand()/RAND_MAX+1);
			#endif
			s->x=data->destx;
			s->y=-s->w-i*20-(j*50);
			data->level=lv;
		}
	}
}

void enemy_plasmaball_move(SPRITE *s)
{
	PLASMABALL_DATA *d=(PLASMABALL_DATA *)s->data;

	switch(d->state) {
		case 0:
			s->y+=d->speed*fps_factor;
			if(s->y>=d->desty) {
				s->y=d->desty;
				d->state=1;
				d->ang=0;
				d->r=0;
				d->c=0;
			}
			break;
		case 1:
			if(d->r==0) {
				d->ang+=0.3*fps_factor;
				if(d->ang>2*M_PI) {
					d->r=1;
					d->c++;
				}
			} else {
				d->ang-=0.3*fps_factor;
				if(d->ang<0)
					d->r=0;
			}
			#ifdef GP2X
			s->y=(float)d->desty+cos(d->ang)*(1+6-d->c);
			#else
			s->y=(double)d->desty+cos(d->ang)*(1+6-d->c);
			#endif
			if((d->ang>-0.7)&&(d->ang<0.4)&&(d->c==5)) {
				d->state=2;
				if(rand()%20==0)
					bonus_add(s->x,s->y,SP_BONUS_EXTRA);
				if(rand()%20==0)
					bonus_add(s->x,s->y,SP_BONUS_HEALTH);
				if(rand()%20==0)
					enemy_laser_create(s,5);
			}
			break;
		case 2:
			d->desty+=50;
			#ifdef GP2X
			if(d->desty>240) //Farox
			#else
			if(d->desty>272) //denis 480
			#endif
				s->type=-1;
			else
				d->state=0;
			break;
	}
}
