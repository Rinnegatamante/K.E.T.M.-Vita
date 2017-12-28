#include "enemy.h"


extern SDL_Surface *screen;
extern SPRITE *player;
extern double fps_factor;

void enemy_bullet_create(SPRITE *s, double speed)

{
	SPRITE *h;
	BULLET_DATA *data;

	h=sprite_add_file("kugel.png",1,PR_ENEMY);
	h->type=SP_EN_BULLET;
	h->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	h->mover=enemy_bullet_move;
	h->aktframe=0;
	h->x=s->x+(s->w/2-h->w/2);
	h->y=s->y+(s->h/2-h->h/2);

	data=mmalloc(sizeof(BULLET_DATA));
	h->data=data;

	data->angle=atan2(player->y-s->y,player->x-s->x);
	data->speed=speed;

}

void enemy_bullet_move(SPRITE *s)
{
	BULLET_DATA *d=(BULLET_DATA *)s->data;

	s->x+=cos(d->angle)*d->speed*fps_factor;
	s->y+=sin(d->angle)*d->speed*fps_factor;
	if((s->x<0)||(s->x>WIDTH)||(s->y<0)||(s->y>HEIGHT)) {
		s->type=-1;
	}
}

void enemy_laser_create(SPRITE *s, double speed)
{
	SPRITE *h;
	LASER_DATA *data;

	h=sprite_add_file("bshoot.png",18,PR_ENEMY);
	h->type=SP_EN_LASER;
	h->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	h->mover=enemy_laser_move;
	h->aktframe=0;
	h->x=s->x+(s->w/2-h->w/2);
	h->y=s->y+(s->h/2-h->h/2);

	data=mmalloc(sizeof(LASER_DATA));
	h->data=data;

	data->angle=atan2(player->y-s->y,player->x-s->x);
	data->speed=speed;

}

void enemy_laser_move(SPRITE *s)
{
	LASER_DATA *d=(LASER_DATA *)s->data;

	s->aktframe=(radtodeg(d->angle)/10)+9;
	s->aktframe%=18;
	s->x+=cos(d->angle)*d->speed*fps_factor;
	s->y+=sin(d->angle)*d->speed*fps_factor;
	if((s->x<0)||(s->x>WIDTH)||(s->y<0)||(s->y>HEIGHT)) {
		s->type=-1;
	}
}

void enemy_homing_create(SPRITE *s)
{
	SPRITE *r;
	HOMING_DATA *data;

	r=sprite_add_file("rotating_rocket.png",20,PR_ENEMY);
	r->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	r->type=SP_EN_HOMING;
	r->x=s->x;
	r->y=s->y;
	r->mover=enemy_homing_move;
	data=mmalloc(sizeof(HOMING_DATA));
	r->data=data;
	data->b.score=5;
	data->b.health=1;
	data->angle=0;
	data->speed=2;
	data->range=200;
	data->delay=10;

	enemy_homing_update(r);
}

void enemy_homing_update(SPRITE *s)
{

	HOMING_DATA *d=(HOMING_DATA *)s->data;

	/* Zielwinkel erfassen */
	d->angle=atan2(player->y-s->y,player->x-s->x);
	s->aktframe=(radtodeg(d->angle)*20)/360;
	s->aktframe%=20;
}

void enemy_homing_move(SPRITE *s)
{
	//SDL_Surface *tmp;
	HOMING_DATA *d=(HOMING_DATA *)s->data;

	d->delay-=fps_factor;
	if(d->delay >0) {
		s->x+=cos(d->angle)*d->speed*fps_factor;
		s->y+=sin(d->angle)*d->speed*fps_factor;
	} else {
		/* nur jeden 10ten Frame Winkel aktualisieren */
		d->delay=10;
		enemy_homing_update(s);
	}


	/* max. reichweite erreicht? self-destruct! */
	d->range-=fps_factor;
	if(d->range<=0) {
		s->type=-1;
		//tmp=sprite_getcurrimg(s);
		//parsys_add(tmp,1,1,s->x,s->y,10,0,0,30,EXPLODE|DIFFSIZE,NULL);
		//SDL_FreeSurface(tmp);
	}
}

#ifdef GP2X
void explosion_add(int x, int y, float wait, int type)
#else
void explosion_add(int x, int y, double wait, int type)
#endif
{
	SPRITE *ex=NULL;
	EXPLOSION_DATA *d;

	d=mmalloc(sizeof(EXPLOSION_DATA));
	switch(type) {
	case 0:
		ex=sprite_add_file("ex.png",29,PR_BONUS);
		d->framewait=2;
		break;
	case 1:
		ex=sprite_add_file("tr_blue.png",6,PR_BONUS);
		d->framewait=1;
		break;
	case 2:
		ex=sprite_add_file("tr_red.png",6,PR_BONUS);
		d->framewait=1;
		break;
	case 3:
		ex=sprite_add_file("tr_green.png",6,PR_BONUS);
		d->framewait=1;
		break;
	default:
		error(ERR_WARN,"explosion_add: unknown type: %d",type);
	}


	ex->flags&=~SP_FLAG_VISIBLE;
	ex->anim_speed=0;
	ex->aktframe=-1;
	ex->type=SP_ETC;
	ex->x=x-(ex->w/2);
	ex->y=y-(ex->h/2);
	ex->mover=explosion_move;
	ex->data=d;
	d->wait=wait;
	d->framestat=0;
}

void explosion_move(SPRITE *c)
{
	EXPLOSION_DATA *d=(EXPLOSION_DATA *)c->data;

	if(d->wait>0)
		d->wait-=fps_factor;
	else {
		d->framestat+=fps_factor;
		if(d->framestat>d->framewait) {
			d->framestat=0;
			c->flags|=SP_FLAG_VISIBLE;
			if(c->aktframe<c->frames-1)
				c->aktframe++;
			else
				c->type=-1;
		}
	}
}
