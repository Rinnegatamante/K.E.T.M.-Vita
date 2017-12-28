#include "enemy.h"

extern SPRITE *player;
extern double fps_factor;

typedef struct {
	ENEMY_BASE b;
	int state;
	int tx;
	int ty;
	#ifdef GP2X
	float speed;
	#else
	double speed;
	#endif
	int level;
} BGPANEL_DATA2;

void enemy_bgpanel2_add(int lv, char spd) //actually lv is the x coord
{
	SPRITE *s;
	BGPANEL_DATA2 *data;

	s=sprite_add_file("bgpanel2.png",1,PR_BACK0);
	s->type=SP_EN_BGPANEL;
	//s->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	s->flags|=(SP_FLAG_VISIBLE);
	s->mover=enemy_bgpanel2_move;
	s->anim_speed=6;
	s->aktframe=0;
	s->x=lv/1000;
	s->y=-s->h-lv%1000;
	data=mmalloc(sizeof(BGPANEL_DATA2));
	s->data=data;
	data->b.score=10;
	data->b.health=2;
	data->state=0;
	data->tx=player->x;
	data->ty=player->y;



	switch(spd) {
        	case '1':  data->speed=0.2; break;
        	case '2':  data->speed=0.4; break;
        	case '3':  data->speed=0.6; break;
        	case '4':  data->speed=0.8; break;
        	case '5':  data->speed=1; break;
        	case '6':  data->speed=1.2; break;
        	case '7':  data->speed=1.4; break;
        	case '8':  data->speed=1.6; break;
        	case '9':  data->speed=1.8; break;
		default:   data->speed=1;
	}


	data->level=0;

}

void enemy_bgpanel2_move(SPRITE *s)
{
	//double angle;
	BGPANEL_DATA2 *d=(BGPANEL_DATA2 *)s->data;

	if(s->y > 274)
		s->type=-1;

	s->y+=fps_factor*d->speed;
}
