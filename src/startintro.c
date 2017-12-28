#include "startintro.h"

extern int keyboard[];
extern SDL_Surface *screen;
extern GAMESTATE state;
extern double fps_factor;

SDL_Surface *badblocks=NULL;
SDL_Surface *presents=NULL;
SDL_Surface *ketm=NULL;

double scale=0;

void startintro_init()
{

	if(badblocks==NULL) {
		badblocks=loadbmp("badblocks.png");
		SDL_SetColorKey(badblocks,SDL_SRCCOLORKEY,0x00000000);
	}
	if(presents==NULL) {
		presents=font_render("PRESENT",FONT01);
	}
	if(ketm==NULL) {
		ketm=loadbmp("ketm.png");
		SDL_SetColorKey(ketm,SDL_SRCCOLORKEY,0x00000000);
	}
	SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format,0,0,0));
	scale=0;
	//newstate(ST_START_INTRO,STIN_FADEIN_BB,0);old before PACC
	#ifdef PACC
	newstate(ST_START_INTRO,STIN_FADEIN_PACC,0);
	#else
	newstate(ST_START_INTRO,STIN_FADEIN_BB,0);
	#endif
}
void startintro_work()
{
	if(state.mainstate!=ST_START_INTRO || state.newstate==1) return;

	if(keyboard_keypressed()) {

		newstate(ST_START_INTRO,STIN_QUIT,0);
	    }

	SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format,0,0,0));

	switch(state.substate) {
		case STIN_FADEIN_BB:
			//scale+=0.005*fps_factor; changed by Farox
			scale+=0.004*fps_factor;
			startintro_centerimg(badblocks,scale);
			if(scale>=1.0) {
			//	parsys_add(badblocks,2,2,screen->w/2-badblocks->w/2,screen->h/2-badblocks->h/2,30,0,0,400,PIXELIZE,NULL);
				scale=0;
				newstate(ST_START_INTRO,STIN_FADEIN_PRESENTS,0);
			}
			break;
		case STIN_FADEIN_PRESENTS:
			scale+=0.01*fps_factor;
			startintro_centerimg(presents,scale);
			if(scale>=1.0) {
			//	parsys_add(presents,presents->w,1,screen->w/2-presents->w/2,screen->h/2-presents->h/2,10,0,0,200,LINESPLIT,NULL);
				scale=0;
				newstate(ST_START_INTRO,STIN_FADEIN_KETM,0);
			}
			break;
		case STIN_FADEIN_KETM:
			scale+=0.01*fps_factor;
			startintro_centerimg(ketm,scale);
			if(scale>=1.0) {
			//	parsys_add(ketm,4,4,screen->w/2-ketm->w/2,screen->h/2-ketm->h/2,30,0,0,100,PIXELIZE,NULL);
				scale=0;
				newstate(ST_START_INTRO,STIN_SLEEP,0);
			}
			break;
		case STIN_SLEEP:
			scale+=fps_factor;
			if(scale>=100) {
				newstate(ST_START_INTRO,STIN_QUIT,0);
			}
			break;
		case STIN_QUIT:
			//parsys_remove_all();

			#ifdef PACC
			/////added for PACC
			unloadbmp_by_name("pacc.png");
			/////////////////
			#endif

			unloadbmp_by_name("badblocks.png");
			badblocks=NULL;
			//SDL_FreeSurface(presents);
			presents=NULL;
			unloadbmp_by_name("ketm.png");
			ketm=NULL;
			newstate(ST_INTRO,0,1);
	}

	parsys_display();

}

void startintro_centerimg(SDL_Surface *src, double scale)
{
	SDL_Rect sr,dr;

	sr.w=src->w;
	sr.h=src->h;
	sr.x=0;
	sr.y=0;
	dr.w=src->w*scale;
	dr.h=src->h*scale;
	dr.x=screen->w/2-dr.w/2;
	dr.y=screen->h/2-dr.h/2;
	blit_scaled(src, &sr, screen, &dr);
}
