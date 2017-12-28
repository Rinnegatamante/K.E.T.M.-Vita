#include "enemy.h"

extern double fps_factor;
extern SPRITE *player;

typedef struct {
	ENEMY_BASE b;
	double angle;
	double speed;
	int state;
	int level;
} CIR_DATA;

void enemy_cir_add(int lv)
{
	int i;
	SPRITE *s;
	CIR_DATA *data;

	for(i=0;i<24;i++) {
		s=sprite_add_file("rwingx.png",37,PR_ENEMY);
		s->type=SP_EN_CIR;
		s->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
		s->mover=enemy_cir_move;
		s->anim_speed=0;
		s->aktframe=0;
		data=mmalloc(sizeof(CIR_DATA));
		s->data=data;
		data->b.score=25;
		data->b.health=1;
		data->angle=90;
		data->speed=6;
		data->state=0;
		data->level=lv;
		s->x=i%2==0?30:50;
		s->y=-(i*20);
	}
}


void enemy_cir_move(SPRITE *s)
{
	CIR_DATA *d=(CIR_DATA *)s->data;

	switch(d->state) {
		case 0:	/* down */
			if(s->y>200) {
				d->state=1;
				d->speed=3;
			}
			break;
		case 1: /* turn */
			d->angle-=2*fps_factor;
			if(d->angle<0)
				d->angle+=360;

			if((d->angle>=267)&&(d->angle<=273)) {
				d->angle=270;
				d->state=2;
				d->speed=8;
			}
			break;
		case 2: /* up */
			if(s->y<50) {
				d->state=3;
				d->speed=3;
			}
			break;
		case 3: /* turn */
			d->angle-=2*fps_factor;
			if(d->angle<0)
				d->angle+=360;
			if((d->angle>=87)&&(d->angle<=93)) {
				d->angle=90;
				d->state=4;
				d->speed=6;
			}
			break;
		case 4: /* down */
		    #ifdef GP2X
		    if(s->y>240) // Farox
		    #else
			if(s->y>272) //denis 480
			#endif
				s->type=-1;
			break;
	}
	s->x+=cos(degtorad((int)d->angle))*d->speed*fps_factor;
	s->y+=sin(degtorad((int)d->angle))*d->speed*fps_factor;
	s->aktframe=(((int)d->angle+270)%360)/10;
}
