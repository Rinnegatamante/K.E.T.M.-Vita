#ifndef _PARTICLE_H_
#define _PARTICLE_H_
//#include <SDL2/SDL.h>
#include <SDL/SDL.h>
#include <stdlib.h>
#include <math.h>

#include "support.h"

enum FX { EXPLODE=0, LINESPLIT, PIXELATE, PIXELIZE, DIFFSIZE=16 };

typedef struct _parsys {
	int x_size;	/* Partikelgroesse */
	int y_size;
	int x_pos;	/* Start-Koordinaten */
	int y_pos;
	int maxspeed;
	int gangle;	/* Gravitation */
	#ifdef GP2X
	float gspeed;
	#else
	double gspeed;
	#endif
	int ttl;	/* Lebenszeit */
	int active;	/* Anzahl aktiver Partikel */
	int fx;		/* Effekt */
	int *msg;	/* wird auf 0 gesetzt, wenn PS tot ist */
	struct _par *particle;	/* Erster Partikel im System */
	struct _parsys *next;	/* Naechstes Partikelsystem oder NULL */
} PARSYS;

typedef struct _par {
	SDL_Surface *img;	/* Image oder NULL */
	double x,y;		/* akt. position */
	double xv,yv;		/* bewegungsvektor */
	// Uint16 ttl;		/* time to live */
	double ttl;
	Uint32 color;		/* Pixelfarbe (nur bei PIXELATE) */
	struct _par *next;	/* next one (NULL=last) */
} PAR;

void parsys_add(SDL_Surface *src, int xs, int ys, int xp, int yp, int maxspeed, int gangle, double gspeed, int ttl, int fx, int *msg);
void parsys_display();
void parsys_remove_all();
PAR *particle_init(SDL_Surface *src,int xs,int ys, int xp, int yp, int maxspeed, int gangle, double gspeed, int ttl, int fx);
int particle_calc(PAR *);
void particle_free(PAR *);
void particle_display(PAR *);

#endif
