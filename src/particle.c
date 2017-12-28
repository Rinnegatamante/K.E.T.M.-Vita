#include "particle.h"

PARSYS *parsys=NULL;
extern SDL_Surface *screen;
extern double fps_factor;

/* Neues Partikelsystem
   src:		Quell-Surface (bei PIXELATE NULL)
   xs,ys:	Partikelgroesse (bei PIXELATE: xs=Anzahl der Partikel, ys=N/A)
   xp,yp:	Startposition
   maxspeed:	Max. Speed
   gangle:	Grav. Winkel
   gspeed:	Grav. Geschwindigkeit
   ttl:		TimeToLive
   tx:		Effekt (EXPLODE, LINESPLIT, PIXELATE, evtl. or mit DIFFSIZE)
   msg:		NULL oder pointer auf int, wird auf 0 gesetzt wenn explosion beendet
*/

void parsys_add(SDL_Surface *src, int xs, int ys, int xp, int yp, int maxspeed, int gangle, double gspeed, int ttl, int fx, int *msg)

{ /*
	PARSYS *ps,*pl=parsys;
	SDL_Surface *t;
	SDL_Rect d;
	int i;
	int txp=0,typ=0;

	if((fx==(PIXELATE|DIFFSIZE))||(fx==(PIXELIZE|DIFFSIZE))) {
		CHECKPOINT;
		error(ERR_FATAL,"PIXELEFFECT|DIFFSIZE!");
	}
	if(fx>>4) {
		for(i=0;i<4;i++) {
			if((t=SDL_CreateRGBSurface(SDL_SRCCOLORKEY|SDL_HWSURFACE,src->w/2,src->h/2,
				screen->format->BitsPerPixel,
				screen->format->Rmask,
				screen->format->Gmask,
				screen->format->Bmask,
				screen->format->Amask))==NULL) {
				CHECKPOINT;
				error(ERR_FATAL,"no RGBSurface for particle system: %s",SDL_GetError());
			}
			d.w=src->w/2;
			d.h=src->h/2;
			switch(i) {
				case 0:
					d.x=0;
					d.y=0;
					txp=xp;
					typ=yp;
					break;
				case 1:
					d.x=d.w;
					d.y=0;
					txp=xp+d.w;
					typ=yp;
					break;
				case 2:
					d.x=0;
					d.y=d.h;
					txp=xp;
					typ=yp+d.h;
					break;
				case 4:
					d.x=d.w;
					d.y=d.h;
					txp=xp+d.w;
					typ=yp+d.h;
					break;
			}
			SDL_BlitSurface(src,&d,t,NULL);
			parsys_add(t, xs+i, ys+i, txp, typ, maxspeed, gangle, gspeed, ttl, fx&0x0f, msg);
			//SDL_FreeSurface(t);
			t=NULL;
		}
		return;
	}



	ps=mmalloc(sizeof(PARSYS));
	ps->x_size=xs;
	ps->y_size=ys;
	ps->x_pos=xp;
	ps->y_pos=yp;
	ps->maxspeed=maxspeed;
	ps->gangle=gangle;
	ps->gspeed=gspeed;
	ps->ttl=ttl;
	ps->fx=fx&0x0f;
	ps->active=0;
	ps->msg=msg;
	ps->particle=particle_init(src,xs,ys,xp,yp,maxspeed,gangle,gspeed,ttl,fx);
	ps->next=NULL;

	if(ps->msg!=NULL) *(ps->msg)=1;

	if(parsys==NULL) {
		parsys=ps;
	} else {
		while(pl->next!=NULL) pl=pl->next;
		pl->next=ps;
	}
*/
}

void parsys_display()
{
/*
	PARSYS *p=parsys,*prev=NULL,*next=NULL;

	while(p!=NULL) {
		next=p->next;
		if((p->active=particle_calc(p->particle))==0) {
			particle_free(p->particle);
			if(prev==NULL) {
				parsys=p->next;
			} else {
				prev->next=next;
			}
			if(p->msg!=NULL) *(p->msg)=0;
			free(p);
		} else {
			if(p->fx==PIXELATE) {
				if(SDL_MUSTLOCK(screen))
					SDL_LockSurface(screen);
			}
			particle_display(p->particle);
			if(p->fx==PIXELATE) {
				if(SDL_MUSTLOCK(screen))
					SDL_UnlockSurface(screen);
			}
			prev=p;
		}

		p=next;
	}
*/
}

void parsys_remove_all()
{
/*
	PARSYS *p=parsys,*prev=NULL,*next=NULL;

	while(p!=NULL) {
		next=p->next;
		particle_free(p->particle);
		if(prev==NULL) {
			parsys=p->next;
		} else {
			prev->next=next;
		}
		if(p->msg!=NULL) *(p->msg)=0;
		free(p);
		p=next;
	}
*/
}

#ifdef GP2X
PAR *particle_init(SDL_Surface *src,int xs,int ys, int xp, int yp, int maxspeed, int gangle, float gspeed, int ttl, int fx)
#else
PAR *particle_init(SDL_Surface *src,int xs,int ys, int xp, int yp, int maxspeed, int gangle, double gspeed, int ttl, int fx)
#endif
{
	PAR *first=NULL,*akt=NULL,*last=NULL;
	int i,j;
	SDL_Rect r;
	#ifdef GP2X
	float angle, speed;
	#else
	double angle, speed;
	#endif
	Uint32 key;
	Uint32 color;

	switch(fx) {
		case PIXELATE:
			for(i=0;i<xs;i++) {
				last=akt;
				akt=mmalloc(sizeof(PAR));
				if(first==NULL){ first=akt; last=akt; }
				last->next=akt;
				akt->x=xp;
				akt->y=yp;
				angle=(rand()*2*M_PI)/RAND_MAX;
				speed=((rand()%100)/maxspeed)+0.1;
				akt->yv=sin(angle)*speed;
				akt->yv+=sin(degtorad(gangle))*gspeed;
				akt->xv=cos(angle)*speed;
				akt->xv+=cos(degtorad(gangle))*gspeed;
				akt->ttl=(rand()%ttl)+1;
				akt->img=NULL;
				j=rand()%255;
				akt->color=SDL_MapRGB(screen->format,j,j,j);
			}
			break;
		case PIXELIZE:
			if(SDL_MUSTLOCK(src))
				SDL_LockSurface(src);
			key=src->format->colorkey;
			for(i=0;i<src->w;i++) {
				for(j=0;j<src->h;j++) {
					color=getpixel(src,i,j);
					if(color!=key) {
						last=akt;
						akt=mmalloc(sizeof(PAR));
						if(first==NULL){ first=akt; last=akt; }
						last->next=akt;
						akt->x=i+xp;
						akt->y=j+yp;
						angle=(rand()*2*M_PI)/RAND_MAX;
						speed=((rand()%100)/maxspeed)+0.1;
						akt->yv=sin(angle)*speed;
						akt->yv+=sin(degtorad(gangle))*gspeed;
						akt->xv=cos(angle)*speed;
						akt->xv+=cos(degtorad(gangle))*gspeed;
						akt->ttl=(rand()%ttl)+1;
						akt->img=NULL;
						akt->color=color;
					}
				}
			}
			break;
		default:
			for(i=0;i<src->w;i+=xs) {
				for(j=0;j<src->h;j+=ys) {
					last=akt;
					akt=mmalloc(sizeof(PAR));
					if(first==NULL){ first=akt; last=akt; }
					last->next=akt;
					akt->x=i+xp;
					akt->y=j+yp;

					angle=(rand()*2*M_PI)/RAND_MAX;
					speed=((rand()%100)/maxspeed)+0.1;
					akt->yv=sin(angle)*speed;
					akt->yv+=sin(degtorad(gangle))*gspeed;

					if(fx==LINESPLIT) {
						akt->xv=0;
					} else {
						akt->xv=cos(angle)*speed;
						akt->xv+=cos(degtorad(gangle))*gspeed;
					}

					akt->ttl=(rand()%ttl)+1;
					//akt->img=SDL_CreateRGBSurface(SDL_SRCCOLORKEY|SDL_HWSURFACE,xs,ys,
					#ifdef GP2X
					akt->img=SDL_CreateRGBSurface(SDL_SRCCOLORKEY|SDL_SWSURFACE,xs,ys,
					#else
					akt->img=SDL_CreateRGBSurface(SDL_SRCCOLORKEY|SDL_HWSURFACE,xs,ys,
					#endif
						src->format->BitsPerPixel,
						src->format->Rmask,
						src->format->Gmask,
						src->format->Bmask,
						src->format->Amask);
					if(akt->img==NULL) {
						CHECKPOINT;
						error(ERR_FATAL,"cant create SDL_Surface: %s",SDL_GetError());
					}

					SDL_SetColorKey(akt->img,SDL_SRCCOLORKEY|SDL_RLEACCEL,0x00000000);
					r.w=xs;
					r.h=ys;
					r.x=i;
					r.y=j;
					SDL_BlitSurface(src,&r,akt->img,NULL);
				}
			}
			break;
	}

	akt->next=NULL;

	return(first);
}

int particle_calc(PAR *p)
{
	int alive=0;

	while(p!=NULL) {
		// if(p->ttl) {
		if(p->ttl>0) {
			alive++;
			p->x+=p->xv*fps_factor;
			p->y+=p->yv*fps_factor;
			// p->ttl--;
			p->ttl-=fps_factor;
		}
		p=p->next;
	}
	return(alive);
}

void particle_free(PAR *p)
{
	PAR *l;
	do {
		if(p->img!=NULL)
			SDL_FreeSurface(p->img);
		l=p;
		p=p->next;
		free(l);
	} while(p!=NULL);
}

void particle_display(PAR *p)
{
	SDL_Rect re;
	do {
		// if(p->ttl) {
		if(p->ttl>0) {
			if(p->img!=NULL) {
				re.x=p->x;
				re.y=p->y;
				re.w=p->img->w;
				re.h=p->img->h;
				SDL_BlitSurface(p->img,NULL,screen,&re);
			} else {
				putpixel(screen,p->x,p->y,p->color);
			}
		}
		p=p->next;
	} while(p!=NULL);
}
