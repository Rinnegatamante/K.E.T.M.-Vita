#include "bonus.h"

#ifdef GP2X
extern float fps_factor;
#else
extern double fps_factor;
#endif
extern SDL_Surface *screen;

void bonus_add(int x, int y, int type)
{
	SPRITE *s;
	BONUS_DATA *data;

	switch(type) {
	case SP_BONUS_FIREPOWER:
		s=sprite_add_file("bonus_p.png",16,PR_BONUS);
		s->anim_speed=3;
		break;
	case SP_BONUS_ADDSPEED:
		s=sprite_add_file("bonus_s.png",16,PR_BONUS);
		s->anim_speed=2;
		break;
	case SP_BONUS_BOMB:
		s=sprite_add_file("bonus_f.png",16,PR_BONUS);
		s->anim_speed=3;
		break;
	case SP_BONUS_COIN:
		s=sprite_add_file("coin.png",20,PR_BONUS);
		s->anim_speed=3;
		break;
	case SP_BONUS_EXTRA:
		s=sprite_add_file("bonus_x.png",16,PR_BONUS);
		s->anim_speed=3;
		break;
	case SP_BONUS_HEALTH:
		s=sprite_add_file("bonus_h.png",9,PR_BONUS);
		s->anim_speed=2;
		break;
	default:
		CHECKPOINT;
		error(ERR_WARN,"cant add unknown bonus! debug me! please! now!");
		return;
	}
	s->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	#ifdef GP2X
	if(x<300) // farox .....to check
		s->x=x+20;
	else
		s->x=x-10;
	#else
	if(x<600)
		s->x=x+20;
	else
		s->x=x-10;
    #endif
	s->y=y;
	s->type=type;
	s->mover=bonus_move;
	data=mmalloc(sizeof(BONUS_DATA));
	s->data=data;
	data->speed=rand()%400/100+1;
}

void bonus_move(SPRITE *s)
{
	BONUS_DATA *d=(BONUS_DATA *)s->data;

	s->y+=d->speed*fps_factor;
	if(s->y>240) s->type=-1; //Farox
}

void bonus_info_add(int x, int y, char *filename)
{
	SPRITE *c;
	c=sprite_add_file(filename,1,PR_BONUS);
	c->flags=SP_FLAG_VISIBLE;
	c->type=SP_ETC;
	c->x=x;
	c->y=y;
	c->alpha=255;
	c->mover=bonus_info_move;
}

void bonus_info_move(SPRITE *c)
{
	if(c->alpha>=3*fps_factor) {
		c->alpha-=3*fps_factor;
		c->y-=fps_factor;
	} else {
		c->type=-1;
	}
}

void bonus_info_text(int x, int y, char *text, int font)
{
	SDL_Surface *t;
	SPRITE *s;

	t=font_render(text,font);
	s=sprite_add(t,1,PR_BONUS,1);
	s->flags|=SP_FLAG_FREESURFACE|SP_FLAG_VISIBLE;
	s->type=SP_ETC;
	s->x=x;
	s->y=y;
	s->alpha=255;
	s->data=mmalloc(sizeof(BIT_DATA));
	((BIT_DATA *)s->data)->distance=255;
	s->mover=bonus_infotext_move;
}

void bonus_infotext_move(SPRITE *c)
{
	BIT_DATA *b=(BIT_DATA *)c->data;
	 SDL_Surface *s;

	c->y+=fps_factor*2;
	b->distance-=fps_factor*3;
	c->alpha=b->distance;
	if(b->distance<=0) {
		 //s=sprite_getcurrimg(c);
		 parsys_add(s,1,1,c->x,c->y,30,0,0,100,PIXELIZE,NULL);
		c->type=-1;
	}

}
