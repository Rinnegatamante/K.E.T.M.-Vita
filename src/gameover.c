#include "gameover.h"

extern SDL_Surface *screen;
extern GAMESTATE state;
extern HSC_LIST hsc_table[];
#ifdef GP2X
extern float fps_factor;
#else
extern double fps_factor;
#endif
extern GAMESTATE state;
extern int lastscore;

SDL_Surface *bg=NULL;

SDL_Surface *go_surface1, *go_surface2;
int go_ex1,go_ex2;

void gameover_init()
{
	char scoretext[50];

	sprite_remove_all(SP_SHOW_ALL);
	//parsys_remove_all(); //not used ..by farox

	go_surface1=font_render("GAME OVER",FONT05);
	sprintf(scoretext,"SCORE: %d",lastscore);
	go_surface2=font_render(scoretext,FONT05);


	bg=SDL_CreateRGBSurface(SDL_HWSURFACE,screen->w,screen->h,

		screen->format->BitsPerPixel,
		screen->format->Rmask,
		screen->format->Gmask,
		screen->format->Bmask,
		screen->format->Amask);
	if(bg==NULL) {
		CHECKPOINT;
		error(ERR_FATAL,"cant create SDL_Surface: %s",SDL_GetError());
	}

	SDL_SetAlpha(screen,SDL_SRCALPHA,128);
	SDL_BlitSurface(screen,NULL,bg,NULL);
	SDL_SetAlpha(screen,SDL_SRCALPHA,255);
	newstate(ST_GAME_OVER,GO_FADEINIT,0);
}

void gameover_work()
{
	SDL_Surface *tmpsurface;
	SDL_Rect s,d;
	static int wait=0;
	#ifdef GP2X
	static float go_size1, go_size2;
	#else
	static double go_size1, go_size2;
	#endif

	if(state.mainstate!=ST_GAME_OVER || state.newstate==1) return;
	SDL_BlitSurface(bg,NULL,screen,NULL);

	switch(state.substate) {

		case GO_FADEINIT:
			go_size1=0;
			go_size2=0;
			newstate(ST_GAME_OVER,GO_FADEIN1,0);
			break;

		case GO_FADEIN1:
			gameover_display(go_size1,go_size2);
			go_size1+=0.05*fps_factor;
			if(go_size1>=1.5) {
				newstate(ST_GAME_OVER,GO_FADEIN2,0);
			}
			break;

		case GO_FADEIN2:
			gameover_display(go_size1,go_size2);
			go_size2+=0.05*fps_factor;
			if(go_size2>=1.0) {
				wait=200;
				newstate(ST_GAME_OVER,GO_WAIT,0);
			}
			break;

		case GO_WAIT:
			gameover_display(go_size1,go_size2);
			wait-=fps_factor;
			if(wait<=0) {
				newstate(ST_GAME_OVER,GO_START_EXPLODE,0);
			}
			break;

		case GO_START_EXPLODE:
			go_ex1=0;
			go_ex2=0;
			s.x=0;
			s.y=0;
			d.x=0;
			d.y=0;
			s.w=go_surface1->w;
			s.h=go_surface1->h;
			d.w=go_surface1->w*go_size1;
			d.h=go_surface1->h*go_size1;
			//tmpsurface=SDL_CreateRGBSurface(SDL_HWSURFACE,d.w,d.h, //changed by Farox to avoid trashing background on GP2X
			#ifdef GP2X
			tmpsurface=SDL_CreateRGBSurface(SDL_SWSURFACE,d.w,d.h,
			#else
			tmpsurface=SDL_CreateRGBSurface(SDL_HWSURFACE,d.w,d.h,
			#endif
				screen->format->BitsPerPixel,
				screen->format->Rmask,
				screen->format->Gmask,
				screen->format->Bmask,
				screen->format->Amask);
			if(tmpsurface==NULL) {
				CHECKPOINT;
				error(ERR_FATAL,"cant create SDL_Surface: %s",SDL_GetError());
			}
			blit_scaled(go_surface1,&s,tmpsurface,&d);
			parsys_add(tmpsurface,d.w,1,screen->w/2-d.w/2,screen->h/2-d.h/2-30,10,0,0,100,LINESPLIT,&go_ex1);
			//SDL_FreeSurface(tmpsurface);

			s.w=go_surface2->w;
			s.h=go_surface2->h;
			d.w=go_surface2->w*go_size2;
			d.h=go_surface2->h*go_size2;
			//tmpsurface=SDL_CreateRGBSurface(SDL_HWSURFACE,d.w,d.h,
			#ifdef GP2X
			tmpsurface=SDL_CreateRGBSurface(SDL_SWSURFACE,d.w,d.h,
			#else
			tmpsurface=SDL_CreateRGBSurface(SDL_HWSURFACE,d.w,d.h,
			#endif
				screen->format->BitsPerPixel,
				screen->format->Rmask,
				screen->format->Gmask,
				screen->format->Bmask,
				screen->format->Amask);
			if(tmpsurface==NULL) {
				CHECKPOINT;
				error(ERR_FATAL,"cant create SDL_Surface: %s",SDL_GetError());
			}
			blit_scaled(go_surface2,&s,tmpsurface,&d);
			parsys_add(tmpsurface,d.w,1,screen->w/2-d.w/2,screen->h/2-d.h/2-30,10,0,0,100,LINESPLIT,&go_ex2);
			//SDL_FreeSurface(tmpsurface);

			newstate(ST_GAME_OVER,GO_WAIT_EXPLODE,0);
			break;

		case GO_WAIT_EXPLODE:
			parsys_display();
			if((go_ex1==0)&&(go_ex2==0)) {
				newstate(ST_GAME_OVER,GO_QUIT,0);
			}
			break;

		case GO_QUIT:
			//SDL_FreeSurface(go_surface1);
			//SDL_FreeSurface(go_surface2);
			//SDL_FreeSurface(bg);
			if(lastscore > hsc_table[4].score) {
				/* you made it! enter your name in the hiscore-list */
				newstate(ST_ENTRY_HCLIST,0,1);
			} else {
				/* you'd better play barbie */
				newstate(ST_INTRO,0,1);
			}
			break;
	}
}

#ifdef GP2X
void gameover_display(float s1, float s2)
#else
void gameover_display(double s1, double s2)
#endif
{
	SDL_Rect s,d;

	s.x=0;
	s.y=0;

	s.w=go_surface1->w;
	s.h=go_surface1->h;
	d.w=go_surface1->w*s1;
	d.h=go_surface1->h*s1;
	d.x=screen->w/2-d.w/2;
	d.y=screen->h/2-d.h/2-30;
	blit_scaled(go_surface1,&s,screen,&d);

	s.w=go_surface2->w;
	s.h=go_surface2->h;
	d.w=go_surface2->w*s2;
	d.h=go_surface2->h*s2;
	d.x=screen->w/2-d.w/2;
	d.y=screen->h/2-d.h/2+30;
	blit_scaled(go_surface2,&s,screen,&d);
}
