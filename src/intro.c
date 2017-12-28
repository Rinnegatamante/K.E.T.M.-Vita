#include "intro.h"

extern int keyboard[];
extern SDL_Surface *screen;
extern GAMESTATE state;
extern KEYCONFIG keyconfig;
extern double fps_factor;

SDL_Surface *intropic=NULL;
SDL_Surface *ketm_logo=NULL;
int alpha;

double delay;
int keydelay;

void intro_init()
{
	if(intropic==NULL) {
		intropic=loadbmp("moon.jpg");
	}
	if(ketm_logo==NULL) {
		ketm_logo=loadbmp("ketm.png");
		SDL_SetColorKey(ketm_logo,SDL_SRCCOLORKEY,0x00000000);
	}
	SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format,0,0,0));
	alpha=0;
	delay=0;
	newstate(ST_INTRO,INTRO_FADEIN_MOON,0);
	keyboard_clear();
	keydelay=20;
}
void intro_work()
{
	SDL_Rect r;

	if(state.mainstate!=ST_INTRO || state.newstate==1) return;

	if(keydelay) {
		keydelay--;
	} else {

		if(keyboard_keypressed()) {
			newstate(ST_INTRO,INTRO_QUIT,0);
		}

		if(keyboard[keyconfig.e]) newstate(ST_GAME_QUIT,0,1);
	}

	switch(state.substate) {
		case INTRO_FADEIN_MOON:
			intro_fadein_moon();
			break;
		case INTRO_FADEIN_KETM:
			intro_fadein_ketm();
			break;
		case INTRO_SLEEP:
			intro_sleep();
			break;
		case INTRO_QUIT:
			SDL_SetAlpha(intropic, SDL_SRCALPHA, 255);
			SDL_BlitSurface(intropic,NULL,screen,NULL);
			SDL_SetAlpha(ketm_logo, SDL_SRCALPHA, 255);
			r.x=(WIDTH/2)-(ketm_logo->w/2);
			#ifdef GP2X
            r.y=15;
            #else
            r.y=30;
            #endif
			r.w=ketm_logo->w;
			r.h=ketm_logo->h;
			SDL_BlitSurface(ketm_logo,NULL,screen,&r);
			unloadbmp_by_name("moon.jpg");
			intropic=NULL;
			newstate(ST_MENU,MEN_START,1);
	}
}

void intro_fadein_moon()
{
	// alpha+=2*fps_factor;
	alpha+=10;
	if(alpha>=255) {
		alpha=255;
		delay=100;
		newstate(ST_INTRO,INTRO_FADEIN_KETM,0);
	}
	SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format,0,0,0));
	SDL_SetAlpha(intropic, SDL_SRCALPHA, alpha);
	SDL_BlitSurface(intropic, NULL, screen, NULL);
}

void intro_fadein_ketm()
{
	SDL_Rect r;

	SDL_SetAlpha(intropic,SDL_SRCALPHA,255);
	SDL_BlitSurface(intropic, NULL, screen, NULL);
	// alpha-=2*fps_factor;
	alpha-=10;
	if(alpha<=0) {
		alpha=0;
		delay=100;
		newstate(ST_INTRO,INTRO_SLEEP,0);
	}

	SDL_SetAlpha(ketm_logo,SDL_SRCALPHA,255-alpha);
	r.x=(WIDTH/2)-(ketm_logo->w/2);
	#ifdef GP2X
	r.y=15;
	#else
	r.y=30;
	#endif
	r.w=ketm_logo->w;
	r.h=ketm_logo->h;
	SDL_BlitSurface(ketm_logo,NULL,screen,&r);
}

void intro_sleep()
{
	delay-=fps_factor;
	// delay--;
	if(delay>=0) {
		newstate(ST_INTRO,INTRO_QUIT,0);
	}
}
