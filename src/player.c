#include "player.h"

extern SDL_Surface *screen;
extern int keyboard[];
extern KEYCONFIG keyconfig;
extern double fps_factor;
extern SPRITE *sprite;
extern int difficulty;

SPRITE *player;

void player_init()
{

	PLAYER_DATA *data;

	player=sprite_add_file("ship-med.png",11,PR_PLAYER);
	player->type=SP_PLAYER;
	player->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	player->mover=player_move;
	player->x=screen->w/2;
	player->y=screen->h-player->h;
	player->aktframe=5;
	data=mmalloc(sizeof(PLAYER_DATA));
	player->data=data;
	data->level=1;
	data->bossmode=0;
	data->state=PL_SAVE;
	data->explode=0;
	data->save_delay=120;
	data->anim_delay=0;
	data->weapon=0;
	data->weapon_wait=0;
	data->extra=PLX_NONE;
	data->extra_wait=0;
	data->extra_time=0;
	data->player_speed=3;

	data->lives=5;
	data->score=0;
}

void player_move(SPRITE *s)
{

	PLAYER_DATA *d=(PLAYER_DATA *)s->data;

	switch(d->state) {

		case PL_NORMAL:

			player_keycontrol(s);
			player_colcheck(s,SP_SHOW_ENEMYS|SP_SHOW_ENEMY_WEAPONS|SP_SHOW_BONUS);

			break;

		case PL_EXPLODE:

			if(!d->explode) {
				playChunk(4);
				explosion_add(s->x+5,s->y+5,0,rand()%3+1);
				explosion_add(s->x+5,s->y+20,0,rand()%3+1);
				explosion_add(s->x+20,s->y+5,0,rand()%3+1);
				explosion_add(s->x+20,s->y+20,0,rand()%3+1);
				s->flags|=SP_FLAG_VISIBLE;
				d->save_delay=120;
				d->state=PL_SAVE;
				d->weapon-=difficulty+2;
				if ( d->weapon<0)
					d->weapon=0; //denis - reset weapons

				if(d->player_speed>3)
					d->player_speed--;
			}
			break;

		case PL_SAVE:

			player_keycontrol(s);
			player_colcheck(s,SP_SHOW_BONUS);


			if(d->save_delay>0) {
				d->save_delay-=fps_factor;
				s->alpha=80;
			} else {
				d->state=PL_NORMAL;
				s->alpha=255;
			}
			break;
	}

}

void player_keycontrol(SPRITE *s)
{
	int direction=0;
	PLAYER_DATA *d=(PLAYER_DATA *)s->data;

	/* if player is invisible (state gameover) allow no keycontrol */
	if(!(s->flags&SP_FLAG_VISIBLE))
		return;

	if(keyboard[keyconfig.l]) {
		direction=-1;
		if(s->x>0) {
			s->x-=d->player_speed*fps_factor;
		}
	}

	if(keyboard[keyconfig.r]) {
		direction=1;
		if(s->x<screen->w-s->w) {
			s->x+=d->player_speed*fps_factor;
		}
	}

	if((keyboard[keyconfig.u]) && (s->y>0)) {
		s->y-=d->player_speed*fps_factor;
		//parsys_add(NULL,50,0,s->x+s->w/2,s->y+s->h,10,90,10,100,PIXELATE,NULL);

	}

	if((keyboard[keyconfig.d]) && (s->y<screen->h-s->h)) {
		s->y+=d->player_speed*fps_factor;
	}

    if(keyboard[keyconfig.ul]) {
		direction=-1;
		if((s->x>0)&& (s->y>0)) {
			s->x-=d->player_speed*fps_factor;//left
			s->y-=d->player_speed*fps_factor;//up
		}
	}
	if(keyboard[keyconfig.ur]) {
		direction=1;
		if((s->x<screen->w-s->w)&& (s->y>0)) {
			s->x+=d->player_speed*fps_factor;//right
			s->y-=d->player_speed*fps_factor;//up
		}
	}
	//
	if(keyboard[keyconfig.dl]) {
		direction=-1;
		if((s->x>0)&& (s->y<screen->h-s->h)) {
			s->x-=d->player_speed*fps_factor;//left
			s->y+=d->player_speed*fps_factor;//down
		}
	}
	if(keyboard[keyconfig.dr]) {
		direction=1;
		if((s->x<screen->w-s->w)&& (s->y<screen->h-s->h)) {
			s->x+=d->player_speed*fps_factor;//right
			s->y+=d->player_speed*fps_factor;//down
		}
	}

	if(d->weapon_wait>0)
		d->weapon_wait-=fps_factor;

	if(d->extra_wait>0)
		d->extra_wait-=fps_factor;

	if(d->extra_time>0)
		d->extra_time-=fps_factor;
	else
		d->extra=PLX_NONE;

	if(keyboard[keyconfig.f]) {
		if(d->weapon_wait<=0) {
			playChunk(0);
			switch(d->weapon) {
				case WP_PLASMA:
					player_add_plasma(s);
					d->weapon_wait=5;
					break;
				case WP_DOUBLEPLASMA:
					player_add_doubleplasma(s);
					d->weapon_wait=5;
					break;
				case WP_QUADPLASMA:
					player_add_quadplasma(s);
					d->weapon_wait=5;
					break;
				case WP_FIREBALL:
					player_add_fireball(s);
					d->weapon_wait=4;
					break;
				case WP_DOUBLEFIREBALL:
					player_add_doublefireball(s);
					d->weapon_wait=4;
					break;
				case WP_QUADFIREBALL:
					player_add_quadfireball(s);
					d->weapon_wait=4;
					break;
				case WP_KILLRAY:
					player_add_killray(s);
					d->weapon_wait=4;
					break;
			}
		}
		if(d->extra_wait<=0) {
			switch(d->extra) {
				case PLX_HOMING:
					player_add_homing(s);
					d->extra_wait=60;
					break;
				case PLX_HLASER:
					player_add_hlaser(s);
					d->extra_wait=60;
					break;
			}
		}
	}

	if(d->anim_delay>0) {
		d->anim_delay-=fps_factor;
	} else {
		d->anim_delay=2;
		switch(direction) {
			case -1:
				if(s->aktframe>0) s->aktframe--;
				break;
			case 1:
				if(s->aktframe<10) s->aktframe++;
				break;
			default:
				if(s->aktframe>5) s->aktframe--;
				if(s->aktframe<5) s->aktframe++;
				break;
		}
	}
}

void player_colcheck(SPRITE *s, int mask)
{
	SPRITE *c;
	//SDL_Surface *spimg;
	PLAYER_DATA *d=(PLAYER_DATA *)s->data;

	/* Kollision Player <> Feind, Feindwaffe oder Bonusitem */
	if((c=sprite_colcheck(s,mask))!=NULL) {

		switch(c->type) {
			case SP_BONUS_FIREPOWER:
				playChunk(5);
				if(d->weapon<WP_LAST-1)
					d->weapon++;
				else
					d->weapon=WP_LAST-2;
				//spimg=sprite_getcurrimg(c);
				//parsys_add(spimg, c->w,1, c->x,c->y, 5, 0, 0, 80, LINESPLIT, NULL);
				//SDL_FreeSurface(spimg);
				bonus_info_add(c->x,c->y,"weapon.png");
				c->type=-1;
				break;

			case SP_BONUS_BOMB:
				playChunk(5);
				player_add_bomb(c);
				//spimg=sprite_getcurrimg(c);
				//parsys_add(spimg, c->w,1, c->x,c->y, 5, 0, 0, 80, LINESPLIT, NULL);
				//SDL_FreeSurface(spimg);
				bonus_info_add(c->x,c->y,"firebomb.png");
				c->type=-1;
				break;

			case SP_BONUS_COIN:
				playChunk(5);
				d->score+=1000;
				//spimg=sprite_getcurrimg(c);
				//parsys_add(spimg, c->w,1, c->x,c->y, 5, 0, 0, 80, LINESPLIT, NULL);
				//SDL_FreeSurface(spimg);
				bonus_info_add(c->x,c->y,"plus1000.png");
				c->type=-1;
				break;

			case SP_BONUS_ADDSPEED:
				playChunk(5);
				//spimg=sprite_getcurrimg(c);
				//parsys_add(spimg, c->w,1, c->x,c->y, 5, 0, 0, 80, LINESPLIT, NULL);
				//SDL_FreeSurface(spimg);
				if(d->player_speed<6) {
					d->player_speed++;
					bonus_info_add(c->x,c->y,"speed.png");
				} else {
					d->score+=1000;
					bonus_info_add(c->x,c->y,"plus1000.png");
				}
				c->type=-1;
				break;

			case SP_BONUS_EXTRA:
				playChunk(5);
				sprite_remove_all_type(SP_PL_SHIELD);
				d->extra=(rand()%(PLX_LAST-1))+1;
				switch(d->extra) {
					case PLX_HOMING:
						d->extra_time=800;
						bonus_info_text(c->x,c->y,"homing missiles",FONT04);
						break;
					case PLX_SHIELD:
						d->extra_time=800;
						player_add_shield(player);
						bonus_info_text(c->x,c->y,"shield",FONT04);
						break;
					case PLX_HLASER:
						d->extra_time=1000;
						bonus_info_text(c->x,c->y,"homing laser",FONT04);
						break;
				}
				//spimg=sprite_getcurrimg(c);
				//parsys_add(spimg, c->w,1, c->x,c->y, 5, 0, 0, 80, LINESPLIT, NULL);
				//SDL_FreeSurface(spimg);
				bonus_info_add(c->x,c->y,"extra.png");
				c->type=-1;
				break;

			case SP_BONUS_HEALTH:
				playChunk(6);
				//spimg=sprite_getcurrimg(c);
				//parsys_add(spimg, c->w,1, c->x,c->y, 5, 0, 0, 80, LINESPLIT, NULL);
				//SDL_FreeSurface(spimg);
				if(d->lives<9) {
					d->lives++;
					bonus_info_add(c->x,c->y,"energy.png");
				} else {
					d->score+=1000;
					bonus_info_add(c->x,c->y,"plus1000.png");
				}
				c->type=-1;
				break;

			case SP_EN_BOSS01:
				enemy_boss01_hitbyplayer(c);
				break;

			case SP_EN_BOSS02:
				enemy_boss02_hitbyplayer(c);
				break;
			case SP_EN_BOSS03:
				enemy_boss03_hitbyplayer(c);
				break;

			default:
			//	spimg=sprite_getcurrimg(c);
			//	parsys_add(spimg, 2,2, c->x,c->y, 10, 270, 10, 30, EXPLODE|DIFFSIZE, NULL);
				//SDL_FreeSurface(spimg);
			//	spimg=NULL;
				d->state=PL_EXPLODE;
				explosion_add(c->x+5,c->y+5,0,rand()%3+1);//denis
				d->explode=0;

				//spimg=sprite_getcurrimg(s);
				//parsys_add(spimg, 2,2, c->x,c->y, 10, 90, 10, 30, EXPLODE|DIFFSIZE, &d->explode);
				//SDL_FreeSurface(spimg);
				//spimg=NULL;
				c->type=-1;
				s->flags&=~SP_FLAG_VISIBLE;
				d->lives--;
		}
	}
}

void player_add_fireball(SPRITE *player)
{
	SPRITE *shot;
	PL_FIREBALL_DATA *data;

	shot=sprite_add_file("fireball1.png",11,PR_PLAYER);
	shot->anim_speed=1;
	shot->type=SP_PL_FIREBALL;
	shot->x=player->x+player->w/2-shot->w/2;
	shot->y=player->y-shot->h+20;
	shot->mover=player_move_fireball;
	shot->flags|=SP_FLAG_VISIBLE;
	data=mmalloc(sizeof(PL_FIREBALL_DATA));
	shot->data=data;
	data->angle=270;
	data->speed=12;
	data->strength=2;
}

void player_add_doublefireball(SPRITE *player)
{
	SPRITE *shot;
	PL_FIREBALL_DATA *data;

	shot=sprite_add_file("fireball1.png",11,PR_PLAYER);
	shot->anim_speed=1;
	shot->type=SP_PL_FIREBALL;
	shot->x=player->x+player->w/2-shot->w/2-5;
	shot->y=player->y-shot->h+20;
	shot->mover=player_move_fireball;
	shot->flags|=SP_FLAG_VISIBLE;
	data=mmalloc(sizeof(PL_FIREBALL_DATA));
	shot->data=data;
	data->angle=270;
	data->speed=12;
	data->strength=2;

	shot=sprite_add_file("fireball1.png",11,PR_PLAYER);
	shot->anim_speed=1;
	shot->type=SP_PL_FIREBALL;
	shot->x=player->x+player->w/2-shot->w/2+5;
	shot->y=player->y-shot->h+20;
	shot->mover=player_move_fireball;
	shot->flags|=SP_FLAG_VISIBLE;
	data=mmalloc(sizeof(PL_FIREBALL_DATA));
	shot->data=data;
	data->angle=270;
	data->speed=12;
	data->strength=2;
}

void player_add_quadfireball(SPRITE *player)
{
	SPRITE *shot;
	PL_FIREBALL_DATA *data;

	shot=sprite_add_file("fireball1.png",11,PR_PLAYER);
	shot->anim_speed=1;
	shot->type=SP_PL_FIREBALL;
	shot->x=player->x+player->w/2-shot->w/2-15;
	shot->y=player->y-shot->h+25;
	shot->mover=player_move_fireball;
	shot->flags|=SP_FLAG_VISIBLE;
	data=mmalloc(sizeof(PL_FIREBALL_DATA));
	shot->data=data;
	data->angle=270-5;
	data->speed=12;
	data->strength=2;

	shot=sprite_add_file("fireball1.png",11,PR_PLAYER);
	shot->anim_speed=1;
	shot->type=SP_PL_FIREBALL;
	shot->x=player->x+player->w/2-shot->w/2-5;
	shot->y=player->y-shot->h+20;
	shot->mover=player_move_fireball;
	shot->flags|=SP_FLAG_VISIBLE;
	data=mmalloc(sizeof(PL_FIREBALL_DATA));
	shot->data=data;
	data->angle=270;
	data->speed=12;
	data->strength=2;

	shot=sprite_add_file("fireball1.png",11,PR_PLAYER);
	shot->anim_speed=1;
	shot->type=SP_PL_FIREBALL;
	shot->x=player->x+player->w/2-shot->w/2+5;
	shot->y=player->y-shot->h+20;
	shot->mover=player_move_fireball;
	shot->flags|=SP_FLAG_VISIBLE;
	data=mmalloc(sizeof(PL_FIREBALL_DATA));
	shot->data=data;
	data->angle=270;
	data->speed=12;
	data->strength=2;

	shot=sprite_add_file("fireball1.png",11,PR_PLAYER);
	shot->anim_speed=1;
	shot->type=SP_PL_FIREBALL;
	shot->x=player->x+player->w/2-shot->w/2+15;
	shot->y=player->y-shot->h+25;
	shot->mover=player_move_fireball;
	shot->flags|=SP_FLAG_VISIBLE;
	data=mmalloc(sizeof(PL_FIREBALL_DATA));
	shot->data=data;
	data->angle=270+5;
	data->speed=12;
	data->strength=2;
}

void player_move_fireball(SPRITE *s)
{
	PL_FIREBALL_DATA *p=(PL_FIREBALL_DATA *)s->data;


	s->x+=cos(degtorad(p->angle))*p->speed*fps_factor;
	s->y+=sin(degtorad(p->angle))*p->speed*fps_factor;

	if(s->y<-10) {
		s->type=-1;
	} else {
		weapon_colcheck(s,p->angle,1,0);
	}
}

void player_add_plasma(SPRITE *player)
{
	SPRITE *shot;
	PL_PLASMA_DATA *data;

	shot=sprite_add_file("plasma.png",1,PR_PLAYER);
	shot->type=SP_PL_PLASMA;
	shot->x=player->x+player->w/2-shot->w/2;
	shot->y=player->y-shot->h+20;
	shot->mover=player_move_plasma;
	shot->flags|=SP_FLAG_VISIBLE;
	data=mmalloc(sizeof(PL_PLASMA_DATA));
	shot->data=data;
	data->angle=270;
	data->speed=12;
	data->strength=1;
}

void player_add_doubleplasma(SPRITE *player)
{
	SPRITE *shot;
	PL_PLASMA_DATA *data;

	shot=sprite_add_file("plasma.png",1,PR_PLAYER);
	shot->type=SP_PL_PLASMA;
	shot->x=player->x+player->w/2-shot->w/2-5;
	shot->y=player->y-shot->h+20;
	shot->mover=player_move_plasma;
	shot->flags|=SP_FLAG_VISIBLE;
	data=mmalloc(sizeof(PL_PLASMA_DATA));
	shot->data=data;
	data->angle=270;
	data->speed=12;
	data->strength=1;

	shot=sprite_add_file("plasma.png",1,PR_PLAYER);
	shot->type=SP_PL_PLASMA;
	shot->x=player->x+player->w/2-shot->w/2+5;
	shot->y=player->y-shot->h+20;
	shot->mover=player_move_plasma;
	shot->flags|=SP_FLAG_VISIBLE;
	data=mmalloc(sizeof(PL_PLASMA_DATA));
	shot->data=data;
	data->angle=270;
	data->speed=12;
	data->strength=1;
}

void player_add_quadplasma(SPRITE *player)
{
	SPRITE *shot;
	PL_PLASMA_DATA *data;

	shot=sprite_add_file("plasma.png",1,PR_PLAYER);
	shot->type=SP_PL_PLASMA;
	shot->x=player->x+player->w/2-shot->w/2-15;
	shot->y=player->y-shot->h+25;
	shot->mover=player_move_plasma;
	shot->flags|=SP_FLAG_VISIBLE;
	data=mmalloc(sizeof(PL_PLASMA_DATA));
	shot->data=data;
	data->angle=270-5;
	data->speed=12;
	data->strength=1;

	shot=sprite_add_file("plasma.png",1,PR_PLAYER);
	shot->type=SP_PL_PLASMA;
	shot->x=player->x+player->w/2-shot->w/2-5;
	shot->y=player->y-shot->h+20;
	shot->mover=player_move_plasma;
	shot->flags|=SP_FLAG_VISIBLE;
	data=mmalloc(sizeof(PL_PLASMA_DATA));
	shot->data=data;
	data->angle=270;
	data->speed=12;
	data->strength=1;

	shot=sprite_add_file("plasma.png",1,PR_PLAYER);
	shot->type=SP_PL_PLASMA;
	shot->x=player->x+player->w/2-shot->w/2+5;
	shot->y=player->y-shot->h+20;
	shot->mover=player_move_plasma;
	shot->flags|=SP_FLAG_VISIBLE;
	data=mmalloc(sizeof(PL_PLASMA_DATA));
	shot->data=data;
	data->angle=270;
	data->speed=12;
	data->strength=1;

	shot=sprite_add_file("plasma.png",1,PR_PLAYER);
	shot->type=SP_PL_PLASMA;
	shot->x=player->x+player->w/2-shot->w/2+15;
	shot->y=player->y-shot->h+25;
	shot->mover=player_move_plasma;
	shot->flags|=SP_FLAG_VISIBLE;
	data=mmalloc(sizeof(PL_PLASMA_DATA));
	shot->data=data;
	data->angle=270+5;
	data->speed=12;
	data->strength=1;
}

void player_move_plasma(SPRITE *s)
{
	PL_PLASMA_DATA *p=(PL_PLASMA_DATA *)s->data;


	s->x+=cos(degtorad(p->angle))*p->speed*fps_factor;
	s->y+=sin(degtorad(p->angle))*p->speed*fps_factor;

	if(s->y<-10) {
		s->type=-1;
	} else {
		weapon_colcheck(s,p->angle,1,0);
	}
}

void player_add_killray(SPRITE *player)
{
	SPRITE *shot;
	PL_KILLRAY_DATA *data;
	static int last=0;

	if(last) {
		shot=sprite_add_file("killray-r.png",12,PR_PLAYER);
		last=0;
	} else {
		shot=sprite_add_file("killray-b.png",12,PR_PLAYER);
		last=1;
	}
	shot->type=SP_PL_KILLRAY;
	shot->x=player->x+player->w/2-shot->w/2;
	shot->y=player->y-shot->h+20;
	shot->mover=player_move_killray;
	shot->flags|=SP_FLAG_VISIBLE;
	data=mmalloc(sizeof(PL_KILLRAY_DATA));
	shot->data=data;
	data->angle=270;
	data->speed=5;
	data->dist=0;
	data->strength=6;
}

void player_move_killray(SPRITE *s)
{
	PL_KILLRAY_DATA *p=(PL_KILLRAY_DATA *)s->data;


	s->x+=cos(degtorad(p->angle))*p->speed*fps_factor;
	s->y+=sin(degtorad(p->angle))*p->speed*fps_factor;
	p->dist+=p->speed*fps_factor;

	if((p->dist>20)&&(s->aktframe<11)) {
		p->dist=0;
		s->aktframe++;
	}

	if(s->y<-10) {
		s->type=-1;
	} else {
		weapon_colcheck(s,p->angle,1,0);
	}
}


void player_add_bomb(SPRITE *s)
{
	SPRITE *c;
	PL_BOMB_DATA *b;

	int a;

	for(a=0;a<355;a+=5) {
		c=sprite_add_file("fireball.png",1,PR_PLAYER);
		c->type=SP_PL_BOMB;
		c->x=player->x+player->w/2;
		c->y=player->y+player->h/2;
		c->mover=player_move_bomb;
		c->flags|=SP_FLAG_VISIBLE;
		b=mmalloc(sizeof(PL_BOMB_DATA));
		c->data=b;
		b->angle=a;
		b->speed=3;
		b->strength=2;
	}
}

void player_move_bomb(SPRITE *s)
{
	PL_BOMB_DATA *b=(PL_BOMB_DATA *)s->data;

	s->x+=cos(degtorad(b->angle))*b->speed*fps_factor;
	s->y+=sin(degtorad(b->angle))*b->speed*fps_factor;

	if((s->x<0)||(s->x>WIDTH)||(s->y<0)||(s->y>HEIGHT)) {
		s->type=-1;
	} else {
		weapon_colcheck(s,b->angle,1,0);
	}
}

void player_add_homing(SPRITE *s)
{
	SPRITE *c;
	SPRITE *t;
	PL_HOMING_DATA *b;

	c=sprite_add_file("homing.png",20,PR_PLAYER);
	c->type=SP_PL_HOMING;
	c->x=(s->x+s->w/2)-40;
	c->y=s->y+s->h/2;
	c->mover=player_move_homing;
	c->flags|=SP_FLAG_VISIBLE;
	b=mmalloc(sizeof(PL_HOMING_DATA));
	c->data=b;
	b->nr=0;
	b->angle=100;
	b->speed=7;
	b->strength=5;
	b->range=350;
	b->state=0;
	b->target_id=-1;
	t=sprite_add_file("target.png",1,PR_TARGET);
	b->tgsprite=t;
	t->type=SP_ETC;
	t->flags&=~(SP_FLAG_COLCHECK|SP_FLAG_VISIBLE);
	t->mover=NULL;
	t->data=NULL;
	t->anim_speed=0;

	c=sprite_add_file("homing.png",20,PR_PLAYER);
	c->type=SP_PL_HOMING;
	c->x=(s->x+s->w/2)-10;
	c->y=s->y+s->h/2;
	c->mover=player_move_homing;
	c->flags|=SP_FLAG_VISIBLE;
	b=mmalloc(sizeof(PL_HOMING_DATA));
	c->data=b;
	b->nr=1;
	b->angle=80;
	b->speed=7;
	b->strength=5;
	b->range=350;
	b->state=0;
	b->target_id=-1;
	t=sprite_add_file("target.png",1,PR_TARGET);
	b->tgsprite=t;
	t->type=SP_ETC;
	t->flags&=~(SP_FLAG_COLCHECK|SP_FLAG_VISIBLE);
	t->mover=NULL;
	t->data=NULL;
	t->anim_speed=0;
}

void player_move_homing(SPRITE *s)
{
	PL_HOMING_DATA *b=(PL_HOMING_DATA *)s->data;
	int ta;
	SPRITE *tg=(SPRITE *)b->tgsprite; /* Target-Fadenkreuz */
	SPRITE *target=NULL;	/* Target */
	#ifdef GP2X
	float dangle;
	#else
	double dangle;
	#endif

	switch(b->state) {
		case 0:	/* Raketen in Ausgangspos. bringen (Sprengkopf nach Norden) */
			if(b->nr==0)
				b->angle+=8*fps_factor;	/* linke Rakete: rot. im UZ */
			else
				b->angle-=8*fps_factor;	/* rechte Rekete: rot. gegen UZ */

			if(b->angle<0)
				b->angle+=360;
			if(b->angle>360)
				b->angle-=360;

			if((b->angle>=266)&&(b->angle<=274)) {
				/* Sprengkopf in Ausgangspos? */
				b->state=2;
				b->angle=270;
			}
			break;

		case 2: /* Ziel suchen */
			b->target_id=search_enemy();
			if(b->target_id!=-1) {
				target=sprite_get_by_id(b->target_id);
				if(target!=NULL) {
					b->state=3;
					target->flags|=SP_FLAG_PARANOIA;
					tg->flags|=SP_FLAG_VISIBLE;
				} else {
					tg->flags&=~SP_FLAG_VISIBLE;
				}
			}
			break;
		case 3: /* Ziel verfolgen */
			if(b->target_id==-1) {
				error(ERR_WARN,"homing: target_id==-1&&state==3 ->back to state 2 (this shouldn't happen)");
				b->state=2;
			} else {
				target=sprite_get_by_id(b->target_id);
				if((target!=NULL) &&
				   (target->flags&SP_FLAG_VISIBLE) &&
				   (target->x>0) &&
				   (target->x<screen->w-target->w) &&
				   (target->y>0) &&
				   (target->y<screen->h-target->h)) {

					ta=radtodeg(atan2(target->y-s->y,target->x-s->x));

					if(b->angle>ta) {
						if(b->angle-ta<175)
							b->angle-=9*fps_factor;
						else if(b->angle-ta>185)
							b->angle+=9*fps_factor;
					} else if(b->angle<ta) {
						if(ta-b->angle<175)
							b->angle+=9*fps_factor;
						else if(ta-b->angle>185)
							b->angle-=9*fps_factor;
					}

					if(b->angle<0)
						b->angle+=360;
					if(b->angle>360)
						b->angle-=360;

					tg->x=target->x+target->w/2-tg->w/2+rand()%6-3;
					tg->y=target->y+target->h/2-tg->h/2+rand()%6-3;
				} else {
					/* Ziel wurde anderweitig vernichtet, neues Ziel suchen */
					b->target_id=-1;
					tg->flags&=~SP_FLAG_VISIBLE;
					b->state=2;
				}
			}
			break;
	}

	s->x+=cos(degtorad(b->angle))*b->speed*fps_factor;
	s->y+=sin(degtorad(b->angle))*b->speed*fps_factor;

	dangle=b->angle+180;
	if(dangle>360)
		dangle-=360;
	//parsys_add(NULL,20,0,s->x+s->w/2,s->y+s->h/2,20,dangle,30,10,PIXELATE,NULL);

	s->aktframe=(b->angle*20)/360;
	s->aktframe%=20;

	weapon_colcheck(s,b->angle,1,0);

	b->range-=fps_factor;
	if(b->range<=0) { /* max. Reichweite erreicht? */
		s->type=-1;
		tg->type=-1;
		if(target!=NULL)
			/* Target freigeben */
			target->flags&=~SP_FLAG_PARANOIA;
	}
}

int search_enemy()
{
	/* Suche Enemy-Sprite, das noch nicht verfolgt wird, liefert SPRITE-ID oder -1 */
	SPRITE *s=sprite;

	while(s!=NULL) {
		if((s->type&SP_SHOW_ENEMYS)&&(!(s->flags&SP_FLAG_PARANOIA))&&(s->flags&SP_FLAG_VISIBLE)) {
			if((s->x>0)&&(s->x<screen->w-s->w)&&(s->y>0)&&(s->y<screen->h-s->h)) {
				return s->id;
			}
		}
		s=s->next;
	}

	return -1;
}


void player_add_shield(SPRITE *s)
{
	SPRITE *c;
	PL_SHIELD_DATA *d;
	int i;

	for(i=0;i<=359;i+=45) {
		c=sprite_add_file("cshoot.png",18,PR_PLAYER);
		c->flags|=SP_FLAG_VISIBLE;
		d=mmalloc(sizeof(PL_SHIELD_DATA));
		c->data=d;
		c->mover=player_move_shield;
		d->angle=i;
		d->speed=5;
		d->rad=40;
		c->type=SP_PL_SHIELD;
	}

}

void player_move_shield(SPRITE *s)
{
	PL_SHIELD_DATA *d=(PL_SHIELD_DATA *)s->data;
	PLAYER_DATA *p=(PLAYER_DATA *)player->data;

	d->angle+=(d->speed*fps_factor);
	if(d->angle>360)
		d->angle-=360;
	s->x=player->x+player->w/2+cos(degtorad(d->angle))*d->rad-s->w/2;
	s->y=player->y+player->h/2+sin(degtorad(d->angle))*d->rad-s->h/2;
	s->aktframe=(d->angle/10);
	s->aktframe%=18;

	weapon_colcheck(s,d->angle,0,1);

	if(p->extra_time<=0)
		s->type=-1;
}

void player_add_hlaser(SPRITE *s)
{
	SPRITE *c;
	PL_HLASER_DATA *b;
	CONTROLLER *e;
	int *id_array;
	int i,j;

	for(j=0;j<2;j++) {
		e=controller_add();
		e->max=24;
		id_array=mmalloc(sizeof(int)*e->max);
		e->e=id_array;
		e->con=player_controller_hlaser;

		for(i=0;i<e->max;i++) {
			c=sprite_add_file("tshoot.png",6,PR_PLAYER);
			id_array[i]=c->id;
			c->type=SP_PL_HLASER;
			c->x=(s->x+s->w/2)-5;
			c->y=s->y+s->h/2+15;
			c->flags|=SP_FLAG_VISIBLE;
			#ifdef GP2X
			c->aktframe=5-((float)6/e->max)*i;
			#else
			c->aktframe=5-((double)6/e->max)*i;
			#endif
			if(i==0) {
				c->mover=player_move_hlaser;
				b=mmalloc(sizeof(PL_HLASER_DATA));
				c->data=b;
				b->nr=j;
				b->angle=100;
				b->speed=12;
				b->strength=6;
				b->range=200;
				b->state=0;
				b->target_id=-1;
			} else {
				c->mover=NULL;
				c->data=NULL;
			}
		}
	}
}

void player_controller_hlaser(CONTROLLER *c)
{
	int *id_array=c->e;
	SPRITE *s, *l=NULL;
	int i;

	if(sprite_get_by_id(id_array[0])==NULL) {
		for(i=1;i<c->max;i++) {
			s=sprite_get_by_id(id_array[i]);
			if(s!=NULL)
				s->type=-1;
		}
		controller_remove(c);
	} else {
		for(i=c->max-1;i>=1;i--) {
			s=sprite_get_by_id(id_array[i]);
			l=sprite_get_by_id(id_array[i-1]);
			if((s!=NULL)&&(l!=NULL)) {
				// draw_line(screen,s->x,s->y,l->x,l->y,
				// SDL_MapRGB(screen->format,0xff,0xff,0xff),
				// SDL_MapRGB(screen->format,0xa0,0xff,0xa0));
				s->x=l->x;
				s->y=l->y;
			}
		}
	}
}


void player_move_hlaser(SPRITE *s)
{
	PL_HLASER_DATA *b=(PL_HLASER_DATA *)s->data;
	int ta;
	SPRITE *target=NULL;	/* Target */

	switch(b->state) {
		case 0:	/* Raketen in Ausgangspos. bringen (Sprengkopf nach Norden) */
			if(b->nr==0)
				b->angle+=8*fps_factor;	/* linke Rakete: rot. im UZ */
			else
				b->angle-=8*fps_factor;	/* rechte Rekete: rot. gegen UZ */

			if(b->angle<0)
				b->angle+=360;
			if(b->angle>360)
				b->angle-=360;

			if((b->angle>=266)&&(b->angle<=274)) {
				/* Sprengkopf in Ausgangspos? */
				b->state=2;
				b->angle=270;
			}
			break;

		case 2: /* Ziel suchen */
			b->target_id=search_enemy();
			if(b->target_id!=-1) {
				target=sprite_get_by_id(b->target_id);
				if(target!=NULL) {
					b->state=3;
					target->flags|=SP_FLAG_PARANOIA;
				}
			}
			break;
		case 3: /* Ziel verfolgen */
			if(b->target_id==-1) {
				error(ERR_WARN,"homing: target_id==-1&&state==3 ->back to state 2 (this shouldn't happen)");
				b->state=2;
			} else {
				target=sprite_get_by_id(b->target_id);
				if((target!=NULL) &&
				   (target->flags&SP_FLAG_VISIBLE) &&
				   (target->x>0) &&
				   (target->x<screen->w-target->w) &&
				   (target->y>0) &&
				   (target->y<screen->h-target->h)) {

					ta=radtodeg(atan2(target->y-s->y,target->x-s->x));

					if(b->angle>ta) {
						if(b->angle-ta<175)
							b->angle-=8*fps_factor;
						else if(b->angle-ta>185)
							b->angle+=8*fps_factor;
					} else if(b->angle<ta) {
						if(ta-b->angle<175)
							b->angle+=8*fps_factor;
						else if(ta-b->angle>185)
							b->angle-=8*fps_factor;
					}

					if(b->angle<0)
						b->angle+=360;
					if(b->angle>360)
						b->angle-=360;

				} else {
					/* Ziel wurde anderweitig vernichtet, neues Ziel suchen */
					b->target_id=-1;
					b->state=2;
				}
			}
			break;
	}

	s->x+=cos(degtorad(b->angle))*b->speed*fps_factor;
	s->y+=sin(degtorad(b->angle))*b->speed*fps_factor;

	weapon_colcheck(s,b->angle,1,0);

	b->range-=fps_factor;
	if(b->range<=0) { /* max. Reichweite erreicht? */
		s->type=-1;
		if(target!=NULL)
			/* Target freigeben */
			target->flags&=~SP_FLAG_PARANOIA;
	}
}

void weapon_colcheck(SPRITE *s, int angle, int destroy, int check_bullets)
{
	SPRITE *c;
	//SDL_Surface *spimg;
	ENEMY_BASE *d;
	WEAPON_BASE *b=(WEAPON_BASE *)s->data;
	PL_HOMING_DATA *h=(PL_HOMING_DATA *)s->data;
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
	char buffer[50];

	/* wenn flag check_bullets gesetzt ist, werden auch gegnerische schuesse getroffen */
	if(check_bullets) {
		if((c=sprite_colcheck(s,SP_SHOW_ENEMY_WEAPONS))!=NULL) {
			playChunk(2);
			//spimg=sprite_getcurrimg(c);
			//parsys_add(spimg, 2,2, c->x,c->y, 10, angle, 10, 30, EXPLODE|DIFFSIZE, NULL);
			//SDL_FreeSurface(spimg);
			c->type=-1;
		}
	}

	if((c=sprite_colcheck(s,SP_SHOW_ENEMYS))!=NULL) {

		if(s->type==SP_PL_HOMING) {
			/* Gegner von Homing-Missile getroffen? */
			/* Paranoia-Flag und Target-Fadenkreuz entfernen */
			h=(PL_HOMING_DATA *)s->data;
			h->tgsprite->type=-1;
		}

		/* wenn flag destroy gesetzt ist, wird der Schuss vernichtet */
		if(destroy)
			s->type=-1;

		// ??? why did i do this ???
		// c->flags&=~SP_FLAG_PARANOIA;
		// ???

		switch(c->type) {
			case SP_EN_BOSS01:
				enemy_boss01_hitbyweapon(c,s,angle);
				break;

			case SP_EN_BOSS02:
				enemy_boss02_hitbyweapon(c,s,angle);
				break;
			case SP_EN_BOSS03:
				enemy_boss03_hitbyweapon(c,s,angle);
				break;

			default:
				d=(ENEMY_BASE *)c->data;
				d->health-=b->strength;
				//spimg=sprite_getcurrimg(c);
				if(d->health>0) {
				//	playChunk(2);
				//	parsys_add(spimg, 2,2, c->x,c->y, 10, 0, 0, 30, EXPLODE|DIFFSIZE, NULL);
				} else {
					playChunk(2);
				    	explosion_add(c->x,c->y+5,0,rand()%3+1);
					//parsys_add(spimg, 2,2, c->x,c->y, 10, angle, 10, 50, EXPLODE|DIFFSIZE, NULL);
					pd->score+=d->score;
					sprintf(buffer,"%d",d->score);
					bonus_info_text(c->x,c->y,buffer,FONT07);
					c->type=-1;
				}
				//SDL_FreeSurface(spimg);
				break;
		}
	}
}
