#include "enemy.h"

extern SDL_Surface *screen;
extern SPRITE *player;
#ifdef GP2X
extern float fps_factor;
#else
extern double fps_factor;
#endif

typedef struct {
	ENEMY_BASE b;
	int anim_dir;
	int anim_speed;
	int anim_count;
	int level;
} DRAGER_DATA;

void enemy_drager_add(int lv)
{
	int i;
	SPRITE *s;
	DRAGER_DATA *data;

	for(i=0;i<2;i++) {
		s=sprite_add_file("iris.png",11,PR_ENEMY);
		s->type=SP_EN_DRAGER;
		s->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
		s->mover=enemy_drager_move;
		s->anim_speed=0;
		s->aktframe=0;
		s->x=90+300*i;
		s->y=-30;
		data=mmalloc(sizeof(DRAGER_DATA));
		s->data=data;
		data->b.score=15;
		data->b.health=3;
		data->anim_dir=0;
		data->anim_speed=5;
		data->anim_count=0;
		data->level=lv;
	}
}

void enemy_drager_move(SPRITE *s)
{
	DRAGER_DATA *d=(DRAGER_DATA *)s->data;

	d->anim_count++;
	if(d->anim_count==d->anim_speed) {
		d->anim_count=0;
		if(d->anim_dir==0) {
			s->aktframe++;
			if(s->aktframe==10)
				d->anim_dir=1;
		} else {
			s->aktframe--;
			if(s->aktframe==0)
				d->anim_dir=0;
		}
	}

	s->y+=1.5*fps_factor;
	if (s->x>240)
		s->x+=-0.6*fps_factor;
	else
		s->x+=0.6*fps_factor;
    #ifdef GP2X
    if(s->y>240) //Farox
    #else
	if(s->y>272) //denis 480
	#endif
		s->type=-1;
}
