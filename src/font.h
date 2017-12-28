#ifndef _FONT_H_
#define _FONT_H_
#include <SDL/SDL.h>
#include <string.h>

#include "support.h"

enum FONTS { FONT01, FONT02, FONT04, FONT05, FONT06, FONT07, LAST_FONT };

#ifdef GP2X

#define FONT01NAME "font01.png"
#define FONT01CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890+-.:'\"x/=><*()!?@_ "
#define FONT01W 17
#define FONT01H 15 //17

#define FONT02NAME "font02.png"
#define FONT02CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZ.,:!?[]/-1234567890"
#define FONT02W 16
#define FONT02H 16


#define FONT04NAME "font04.png"
#define FONT04CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789@(){},.-;:_#*+?=!\"§$%&/öäüÖÄÜµ^°'`´ß\\<>| "
#define FONT04W 18
#define FONT04H 33 //37

#define FONT05NAME "font05.png"
#define FONT05CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890+-.:'\"x/=><*()!?@_ "
#define FONT05W 17
#define FONT05H 15 //17

#define FONT06NAME "font06.png"
#define FONT06CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZ,-./0123456789:<>?'()@!# "
#define FONT06W 16
#define FONT06H 14 //16

#define FONT07NAME "font07.bmp"
#define FONT07CHARS " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^"
#define FONT07W 8
#define FONT07H 8

#else

#define FONT01NAME "font01.png"
#define FONT01CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890+-.:'\"x/=><*()!?@_ "
#define FONT01W 17
#define FONT01H 17

#define FONT02NAME "font02.png"
#define FONT02CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZ.,:!?[]/-1234567890"
#define FONT02W 16
#define FONT02H 16


#define FONT04NAME "font04.png"
#define FONT04CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789@(){},.-;:_#*+?=!\"§$%&/öäüÖÄÜµ^°'`´ß\\<>| "
#define FONT04W 18
#define FONT04H 37

#define FONT05NAME "font05.png"
#define FONT05CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890+-.:'\"x/=><*()!?@_ "
#define FONT05W 17
#define FONT05H 17

#define FONT06NAME "font06.png"
#define FONT06CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZ,-./0123456789:<>?'()@!# "
#define FONT06W 16
#define FONT06H 16

#define FONT07NAME "font07.bmp"
#define FONT07CHARS " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^"
#define FONT07W 8
#define FONT07H 8

#endif

typedef struct {
	char filename[256];
	SDL_Surface *fontimg;
	int w;
	int h;
	char charorder[256];
} FONT;

void font_init();
SDL_Surface *font_render(char *text,int fontnr);
void font_print(char *text, int fontnr, int x, int y);
#endif
