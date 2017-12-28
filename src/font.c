#include "font.h"

FONT fonts[LAST_FONT];

extern SDL_Surface *screen;

void font_init()
{
	int i;

	strcpy(fonts[FONT01].filename,FONT01NAME);
	fonts[FONT01].w=FONT01W;
	fonts[FONT01].h=FONT01H;
	strcpy(fonts[FONT01].charorder,FONT01CHARS);

	strcpy(fonts[FONT02].filename,FONT02NAME);
	fonts[FONT02].w=FONT02W;
	fonts[FONT02].h=FONT02H;
	strcpy(fonts[FONT02].charorder,FONT02CHARS);


	strcpy(fonts[FONT04].filename,FONT04NAME);
	fonts[FONT04].w=FONT04W;
	fonts[FONT04].h=FONT04H;
	strcpy(fonts[FONT04].charorder,FONT04CHARS);

	strcpy(fonts[FONT05].filename,FONT05NAME);
	fonts[FONT05].w=FONT05W;
	fonts[FONT05].h=FONT05H;
	strcpy(fonts[FONT05].charorder,FONT05CHARS);

	strcpy(fonts[FONT06].filename,FONT06NAME);
	fonts[FONT06].w=FONT06W;
	fonts[FONT06].h=FONT06H;
	strcpy(fonts[FONT06].charorder,FONT06CHARS);

	strcpy(fonts[FONT07].filename,FONT07NAME);
	fonts[FONT07].w=FONT07W;
	fonts[FONT07].h=FONT07H;
	strcpy(fonts[FONT07].charorder,FONT07CHARS);

	for(i=0;i<LAST_FONT;i++) {
		fonts[i].fontimg=loadbmp(fonts[i].filename);
	}
}

SDL_Surface *font_render(char *text,int fontnr)
{
	SDL_Surface *txtimg;
	SDL_Rect s,d;
	unsigned int i,j;


	#ifdef GP2X
	txtimg=SDL_CreateRGBSurface(SDL_SRCCOLORKEY|SDL_SWSURFACE,strlen(text)*fonts[fontnr].w,fonts[fontnr].h,
	#else
	txtimg=SDL_CreateRGBSurface(SDL_SRCCOLORKEY|SDL_HWSURFACE,strlen(text)*fonts[fontnr].w,fonts[fontnr].h,
	#endif
		screen->format->BitsPerPixel,
		screen->format->Rmask,
		screen->format->Gmask,
		screen->format->Bmask,
		screen->format->Amask);
	SDL_SetColorKey(txtimg,SDL_SRCCOLORKEY|SDL_RLEACCEL,0x00000000);


	for(i=0;i<strlen(text);i++) {

		for(j=0;j<strlen(fonts[fontnr].charorder);j++)
			if(text[i]==fonts[fontnr].charorder[j]) break;

		s.w=fonts[fontnr].w;
		s.h=fonts[fontnr].h;
		s.x=j*fonts[fontnr].w;
		s.y=0;

		d.w=fonts[fontnr].w;
		d.h=fonts[fontnr].h;
		d.x=i*fonts[fontnr].w;
		d.y=0;

		SDL_BlitSurface(fonts[fontnr].fontimg,&s,txtimg,&d);
	}
	return(txtimg);
}

void font_print(char *text, int fontnr, int x, int y)
{
	SDL_Surface *textsurface;
	SDL_Rect r;

	textsurface=font_render(text,fontnr);
	r.x=x;
	r.y=y;
	r.w=textsurface->w;
	r.h=textsurface->h;
	SDL_BlitSurface(textsurface,NULL,screen,&r);
	SDL_FreeSurface(textsurface);
}

