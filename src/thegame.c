#include "thegame.h"

extern SDL_Surface *screen;
extern int keyboard[];
extern SPRITE *player;
extern GAMESTATE state;
extern KEYCONFIG keyconfig;
extern HSC_LIST hsc_table[];
extern int lastscore;
extern LEVELENTRY *leveltab;

int world_y;
int world_dir=1;
int gframe;
int gameover_delay;
Uint32 level_start_time;
Uint32 game_start_time;
char filename[20];


void thegame_init()
{
	int level;

	gframe=0;
	gameover_delay=100;
	controller_remove_all();
	player_init();
	game_start_time=SDL_GetTicks();
	level_start_time=SDL_GetTicks();

	level=((PLAYER_DATA *)player->data)->level;

	if(loadlv(level)==0)
		error(ERR_WARN,"no entrys for level %d",level);
}


void thegame_work()
{
	PLAYER_DATA *d=(PLAYER_DATA *)player->data;
	Uint32 gt;
	LEVELENTRY *l;
	int *level=&d->level;

	if(state.mainstate!=ST_GAME_PLAY || state.newstate==1) return;

	/* gt=Zeit seit Spielbeginn in 1/10 sec. */
	gt=(SDL_GetTicks()-level_start_time)/100;

	if(d->lives>0) {
		if(keyboard[keyconfig.e]) {
			newstate(ST_MENU,MEN_PAUSE,1);
		}

		l=leveltab;
		while(l!=NULL) {
			if((gt>=l->time)&&(l->done==0)) {
				thegame_level(l, *level);
				l->done=1;
			}
			l=l->next;
		}


		if(((PLAYER_DATA *)player->data)->bossmode==2) {

			((PLAYER_DATA *)player->data)->bossmode=0;
			level_start_time=SDL_GetTicks();
			(*level)++;
			gt=(SDL_GetTicks()-level_start_time)/100; //denis
			playChunk(3);
			// change music soundtrack
			sprintf(filename,"stage%d",*level);
			playMusic(filename);
			/* Load next level */
			if(loadlv(*level)==0)
				error(ERR_WARN,"no entrys for level %d",*level);
		}

		if(*level==5 && gt>605) {
                       error(ERR_DEBUG,"sorry, no more levels in this alpha-version");
			d->lives=0;
                        return;
		}


		bg_work();
		sprite_work(SP_SHOW_ALL);
		sprite_display(SP_SHOW_ALL);
		controller_work();
		//parsys_display();
		score_display();
		gframe++;

	} else {
		player->flags&=~SP_FLAG_VISIBLE;
		if(gameover_delay) {
			gameover_delay--;
			bg_work();
			controller_work();
			sprite_work(SP_SHOW_ALL);
			sprite_display(SP_SHOW_ALL);
			//parsys_display();
			score_display();
		} else {
			bg_destroy();
			lastscore=((PLAYER_DATA *)player->data)->score;
			controller_remove_all();
			sprite_remove_all(SP_SHOW_ALL);
			parsys_remove_all();
			score_cleanup();
			newstate(ST_GAME_OVER,0,1);
		}
	}
}


void thegame_level(LEVELENTRY *l, int lev)
{
	switch(l->command) {
	case 'E':
		/* add enemy */
		if(!strcmp(l->para1,"XEV")) {
			enemy_xev_add(l->para2);
		} else if(!strcmp(l->para1,"CRUSHER")) {
			enemy_crusher_add(l->para2);
		} else if(!strcmp(l->para1,"EYEFO")) {
			enemy_eyefo_add(l->para2);
		} else if(!strcmp(l->para1,"CUBE")) {
			enemy_cube_add(l->para2);
		} else if(!strcmp(l->para1,"DRAGER")) {
			enemy_drager_add(l->para2);
		} else if(!strcmp(l->para1,"MINE")) {
			enemy_mine_add(l->para2);
		} else if(!strcmp(l->para1,"RWINGX")) {
			enemy_rwingx_add(l->para2);
		} else if(!strcmp(l->para1,"CIR")) {
			enemy_cir_add(l->para2);
		} else if(!strcmp(l->para1,"ZATAK")) {
			enemy_zatak_add(l->para2);
		} else if(!strcmp(l->para1,"BADGUY")) {
			enemy_badguy_add(l->para2);
		} else if(!strcmp(l->para1,"PROBALL")) {
			enemy_proball_add(l->para2);
		} else if(!strcmp(l->para1,"PLASMABALL")) {
			enemy_plasmaball_add(l->para2);
		} else if(!strcmp(l->para1,"MING")) {
			enemy_ming_add(l->para2);
		} else if(!strcmp(l->para1,"GREETER")) {
			enemy_greeter_add(l->para2);
		} else if(!strcmp(l->para1,"GROUNDER")) {
			enemy_grounder_add(l->para2,'1');
		} else if(!strcmp(l->para1,"CURVER")) {
			enemy_curver_add(l->para2);
		} else if(!strcmp(l->para1,"BOSS01")) {
			enemy_boss01_add(l->para2);
		} else if(!strcmp(l->para1,"BOSS02")) {
			enemy_boss02_add(l->para2);
		} else if(!strcmp(l->para1,"BOSS03")) {
			enemy_boss03_add(l->para2);
		} else {
			error(ERR_WARN,"unknown enemy '%s'",l->para1);
		}

		break;
	case 'T':
		/* text */
		enemy_gametext_add(l->para1,l->para2);
		break;
	case 'B':
		/* Background */
		bg_destroy();
		switch(l->para2) {
		case 0: /* Clouds */
			bg_init(BG_CLOUDS,lev);
			break;
		case 1: /* Stars */
			bg_init(BG_STARS,lev);
			break;
		case 2: /* Tile */
			bg_init(BG_TILE,lev);
			break;
		default:
			error(ERR_WARN,"unknown background %d, using clouds",l->para2);
			bg_init(BG_CLOUDS,lev);
			break;
		}
		break;

	default:
		// add background tiles....
		if(!strcmp(l->para1,"BGPANEL2")) {
			enemy_bgpanel2_add(l->para2,l->command);
		} else if(!strcmp(l->para1,"BGPANEL")) {
			enemy_bgpanel_add(l->para2,l->command);
		} else if(!strcmp(l->para1,"GROUNDER")) {
			enemy_grounder_add(l->para2,l->command);
		}else
			error(ERR_WARN,"unknown command '%c' in levelfile",l->command);
	}
}

void adjust_start_time(Uint32 pause_time) {
level_start_time+=SDL_GetTicks()-pause_time;
}
