#include "enemy.h"

SPRITE *sb01[6];
extern SPRITE *player;
#ifdef GP2X
extern float fps_factor;
#else
extern double fps_factor;
#endif
extern SDL_Surface *screen;

typedef struct _boss01_data {
	ENEMY_BASE b;
	int state;
	int health_flag;
	int level;
} BOSS01_DATA;

void enemy_boss01_add(int lv)
{
	int i;
	BOSS01_DATA *b;

	sb01[0]=sprite_add_file("boss01-lo.png",2,PR_ENEMY);
	sb01[1]=sprite_add_file("boss01-mo.png",2,PR_ENEMY);
	sb01[2]=sprite_add_file("boss01-ro.png",2,PR_ENEMY);
	sb01[3]=sprite_add_file("boss01-lu.png",2,PR_ENEMY);
	sb01[4]=sprite_add_file("boss01-mu.png",2,PR_ENEMY);
	sb01[5]=sprite_add_file("boss01-ru.png",2,PR_ENEMY);

	for(i=0;i<6;i++) {
		sb01[i]->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
		sb01[i]->anim_speed=0;
		sb01[i]->aktframe=0;
		sb01[i]->type=SP_EN_BOSS01;
		b=mmalloc(sizeof(BOSS01_DATA));
		sb01[i]->data=b;
		b->b.health=40;  //denis was 40
		b->b.score=500*(lv+1);
		b->health_flag=0;
		b->level=lv;
		if(i==1) {
			b->state=0;
			sb01[i]->mover=enemy_boss01_move;
		}

	}

	((PLAYER_DATA *)player->data)->bossmode=1;
}

void enemy_boss01_setpos(int x, int y)
{
	sb01[0]->x=x;
	sb01[0]->y=y;
	sb01[1]->x=sb01[0]->w+sb01[0]->x;
	sb01[1]->y=y;
	sb01[2]->x=sb01[1]->w+sb01[1]->x;
	sb01[2]->y=y;
	sb01[3]->x=x;
	sb01[3]->y=sb01[0]->h+y;
	sb01[4]->x=sb01[3]->w+sb01[3]->x;
	sb01[4]->y=sb01[1]->h+y;
	sb01[5]->x=sb01[4]->w+sb01[4]->x;
	sb01[5]->y=sb01[2]->h+y;
}

/* boss wurde von player berührt */
void enemy_boss01_hitbyplayer(SPRITE *c)
{
}

/* boss wurde von player-weapon berührt */
void enemy_boss01_hitbyweapon(SPRITE *c, SPRITE *s, int angle)
{
	int i,j;
	BOSS01_DATA *b;
	WEAPON_BASE *w=(WEAPON_BASE *)s->data;

	for(i=0;i<6;i++)
		if(c==sb01[i])
			break;

	switch(i) {
		case 0:
			if(sb01[3]->flags&SP_FLAG_VISIBLE)
				i=3;
			break;

		case 1:
			if(sb01[4]->flags&SP_FLAG_VISIBLE)
				i=4;
			else if(sb01[3]->flags&SP_FLAG_VISIBLE)
				i=3;
			else if(sb01[5]->flags&SP_FLAG_VISIBLE)
				i=5;
			else if(sb01[0]->flags&SP_FLAG_VISIBLE)
				i=0;
			else if(sb01[2]->flags&SP_FLAG_VISIBLE)
				i=2;
			break;

		case 2:
			if(sb01[5]->flags&SP_FLAG_VISIBLE)
				i=5;
			break;
	}

	b=(BOSS01_DATA *)sb01[i]->data;
	b->b.health-=w->strength;

	explosion_add(s->x,s->y,0,rand()%3+1);
	//parsys_add(NULL,100,0,s->x,s->y,30,0,0,50,PIXELATE,NULL);


	if((b->b.health<=15)&&(b->health_flag==0)) {
		b->health_flag=1;
		explosion_add(sb01[i]->x+sb01[i]->w/2,sb01[i]->y+sb01[i]->h/2,0,0);
	}
	if(b->b.health<=0) {
		explosion_add(sb01[i]->x+sb01[i]->w/2,sb01[i]->y+sb01[i]->h/2,0,0);
		sb01[i]->flags&=~SP_FLAG_VISIBLE;
		((PLAYER_DATA *)player->data)->score+=b->b.score;
	}

	j=0;
	for(i=0;i<6;i++) {
		if (b->health_flag==1)
			sb01[i]->aktframe=1;
		if(sb01[i]->flags&SP_FLAG_VISIBLE)
			j++;
	}
	if(j==0) {
		/* boss komplett zerstört */
		for(i=0;i<6;i++) {
			sb01[i]->type=-1;
			explosion_add(sb01[i]->x+sb01[i]->w/2,sb01[i]->y+sb01[i]->h/2,rand()%20,0);
		}
		((PLAYER_DATA *)player->data)->bossmode=2;
	}
}


void enemy_boss01_move(SPRITE *c)
{
	BOSS01_DATA *b=(BOSS01_DATA *)c->data;
	#ifdef GP2X
	static float x,y;
	static float w;
	static float firewait1;
	#else
	static double x,y;
	static double w;
	static double firewait1;
	#endif
	static int firewait2;

	switch(b->state) {
		case 0:
			x=screen->w/2-(sb01[0]->w+sb01[1]->w+sb01[2]->w)/2;
			y=-100;
			enemy_boss01_setpos(x,y);
			b->state=1;
			firewait1=45;
			firewait2=4;
			break;
		case 1:
			y+=fps_factor*(b->level+1);
			enemy_boss01_setpos(x,y);
			if(y>=30) {
				b->state=2;
				w=0;
			}
			break;
		case 2:
			w+=fps_factor;
			if(w>=40) {
				b->state=3;
			}
			break;
		case 3:
			y+=fps_factor*(b->level+1);
			enemy_boss01_setpos(x,y);
			if(y>=80) {
				b->state=4;
				w=0;
			}
			break;
		case 4:
			w+=fps_factor;
			if(w>=30) {
				b->state=5;
				w=0;
			}
			break;
		case 5:
			y+=fps_factor*(b->level+1);
			x-=2*fps_factor*(b->level+1);
			w+=fps_factor;
			enemy_boss01_setpos(x,y);
			if(w>=60)
				b->state=6;
			break;
		case 6:
			y-=fps_factor*(b->level+1);
			x+=2*fps_factor*(b->level+1);
			w-=fps_factor;
			enemy_boss01_setpos(x,y);
			if(w<=0)
				b->state=7;
			break;
		case 7:
			y+=fps_factor*(b->level+1);
			x+=2*fps_factor*(b->level+1);
			w+=fps_factor;
			enemy_boss01_setpos(x,y);
			if(w>=60)
				b->state=8;
			break;
		case 8:
			y-=fps_factor*(b->level+1);
			x-=2*fps_factor*(b->level+1);
			w-=fps_factor;
			enemy_boss01_setpos(x,y);
			if(w<=0)
				b->state=4;
			break;
	}
	firewait1-=fps_factor*(b->level+1);
	if(firewait1<=0) {
		firewait1=45;
		firewait2--;
		if(firewait2==0) {
			enemy_boss01_fire(2);
			firewait2=4;
		} else {
			enemy_boss01_fire(0);
			enemy_boss01_fire(1);
		}
	}
}

void enemy_boss01_fire(int where) /* 0: left, 1: right, 2: bombenhagel */
{
	SPRITE *b;
	LASER_DATA *ldata;
	int angle;

	switch(where) {
		case 0:
			if(sb01[3]->flags&SP_FLAG_VISIBLE)
				enemy_bullet_create(sb01[3],5);
			break;
		case 1:
			if(sb01[5]->flags&SP_FLAG_VISIBLE)
				enemy_bullet_create(sb01[5],5);
			break;
		case 2:
			for(angle=0;angle<=180;angle+=20) {
				b=sprite_add_file("bshoot.png",18,PR_ENEMY);
				b->type=SP_EN_LASER;
				b->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
				b->mover=enemy_laser_move;
				//b->aktframe=0;
				b->x=sb01[4]->x;
				b->y=sb01[4]->y;
				ldata=mmalloc(sizeof(LASER_DATA));
				b->data=ldata;
				ldata->angle=degtorad(angle);
				ldata->speed=6;
			}
			break;
	}
}
