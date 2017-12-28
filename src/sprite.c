#include "sprite.h"

SPRITE *sprite=NULL;
CONTROLLER *cont=NULL;
COLMAP_CACHE *colmap_cache=NULL;
extern SDL_Surface *screen;
extern double fps_factor;
SDL_Rect rc;

COLMAP_CACHE *sprite_add_colmap(SDL_Surface *img, int frames)
{
	COLMAP_CACHE *c;
	int i;
	int colbsize;

	if((c=sprite_get_colmap(img,frames))!=NULL) {
		c->refcount++;
		return(c);
	}

	c=mmalloc(sizeof(COLMAP_CACHE));
	c->src=img;
	c->frames=frames;
	c->refcount=1;
	c->col=mmalloc(sizeof(Uint8 *)*frames);

	colbsize=((img->w/frames)*(img->h+2))/8;

	for(i=0;i<frames;i++) {
		c->col[i]=mmalloc(colbsize);
		memset(c->col[i],0x00,(colbsize));
		sprite_create_colmap(img,c->col[i],img->w/frames,i);
	}

	if(colmap_cache==NULL) {
		c->next=NULL;
	} else {
		c->next=colmap_cache;
	}
	colmap_cache=c;
	return(c);
}

COLMAP_CACHE *sprite_get_colmap(SDL_Surface *img, int frames)
{
	COLMAP_CACHE *c=colmap_cache;

	while(c!=NULL) {
		if((c->src==img)&&(c->frames==frames))
			return(c);
		c=c->next;
	}

	return(NULL);
}

void sprite_remove_colmap(COLMAP_CACHE *c)
{
	if(c->refcount>0) {
		c->refcount--;
	} else {
		CHECKPOINT;
		error(ERR_WARN,"COLMAP_CACHE refcount already zero");
	}
}


SPRITE *sprite_add_file(char *filename, int frames, Uint8 priority)
{
	SDL_Surface *s;
	SPRITE *sp;

	s=loadbmp(filename);
	SDL_SetColorKey(s,SDL_SRCCOLORKEY|SDL_RLEACCEL,0x00000000);
	//SDL_SetColorKey(s,SDL_SRCCOLORKEY|SDL_RLEACCEL,0x000000);//prova farox
	sp=sprite_add(s,frames,priority,0);
	return sp;
}


SPRITE *sprite_add(SDL_Surface *surface, int frames, Uint8 priority, int nocache)
{
	SPRITE *s, *t, *p=NULL;
	static int current_id=0;

	s=mmalloc(sizeof(SPRITE));

	t=sprite;
	p=NULL;
	while((t!=NULL)&&(t->priority<priority)) {
		p=t;
		t=t->next;
	}
	s->next=t;
	if(p!=NULL)
		p->next=s;
	else
		sprite=s;

	s->id=current_id;
	current_id++;
	s->priority=priority;
	s->frames=frames;
	s->aktframe=0;
	s->anim_speed=0;
	s->anim_count=0;
	s->mover=NULL;
	s->alpha=255;
	s->img=surface;
	s->w=surface->w/frames;
	s->h=surface->h;
	s->cmap=sprite_add_colmap(surface, frames);
	s->flags=0;
	s->ticks=0;
	if(nocache) s->flags|=SP_FLAG_NOCACHE;
	s->data=NULL;

	return(s);
}

SDL_Surface *sprite_getcurrimg(SPRITE *s)
{

	SDL_Surface *i;
	SDL_Rect r;

	//if((i=SDL_CreateRGBSurface(SDL_SRCCOLORKEY|SDL_HWSURFACE,s->w,s->h,
	#ifdef GP2X
	if((i=SDL_CreateRGBSurface(SDL_SRCCOLORKEY|SDL_SWSURFACE,s->w,s->h,
	#else
	if((i=SDL_CreateRGBSurface(SDL_SRCCOLORKEY|SDL_HWSURFACE,s->w,s->h,
	#endif
		screen->format->BitsPerPixel,
		screen->format->Rmask,
		screen->format->Gmask,
		screen->format->Bmask,
		screen->format->Amask))==NULL) {
		CHECKPOINT;
		error(ERR_FATAL,"cant create surface from sprite");
	}
	SDL_SetColorKey(i,SDL_SRCCOLORKEY| SDL_RLEACCEL,0x00000000); //denis


	r.x=s->w*s->aktframe;
	r.y=0;
	r.w=s->w;
	r.h=s->h;
	SDL_BlitSurface(s->img,&r,i,NULL);

	return i;
}


void sprite_display(int type)
{
	SPRITE *s=sprite;
	SDL_Rect r;
	SDL_Rect t;

	t.y=0;
	while(s!=NULL) {
		if((s->flags&SP_FLAG_VISIBLE) && (s->type&type)) {
			r.x=(int)s->x;
			r.y=(int)s->y;
			r.w=s->w;
			r.h=s->h;

			t.w=s->w;
			t.h=s->h;
			t.x=s->w*s->aktframe;

			if(s->flags&SP_FLAG_CHEAPALPHA) {
				blit_calpha(s->img,&t,screen,&r);
			} else {
				SDL_SetAlpha(s->img,SDL_SRCALPHA,s->alpha);
				SDL_BlitSurface(s->img,&t,screen,&r);
			}
		}
		s=s->next;
	}
}

int sprite_count(int type)
{
	SPRITE *s=sprite;
	int c=0;

	while(s!=NULL) {
		if(s->type&type)
			c++;
		s=s->next;
	}
	return c;
}

void sprite_remove(SPRITE *sr)
{
	SPRITE *s=sprite, *p=NULL, *n=NULL;

	while(s!=NULL) {
		n=s->next;
		if(s==sr) {
			if(p==NULL) {
				sprite=n;
			} else {
				p->next=n;
			}
			if(!s->flags&SP_FLAG_NOCACHE) {
				unloadbmp_by_surface(s->img);
			}
			if(s->flags&SP_FLAG_FREESURFACE) {
				//SDL_FreeSurface(s->img);
			}
			sprite_remove_colmap(s->cmap);
			if(s->data!=NULL) free(s->data);
			free(s);
			return;
		}
		p=s;
		s=n;
	}

	CHECKPOINT;
	error(ERR_WARN,"sprite not found in list");
}

void sprite_remove_all(int type)
{
	SPRITE *s=sprite, *n=NULL;

	while(s!=NULL) {
		n=s->next;
		if(s->type&type)
			sprite_remove(s);
		s=n;
	}
}

void sprite_remove_all_type(int type)
{
	SPRITE *s=sprite, *n=NULL;

	while(s!=NULL) {
		n=s->next;
		if(s->type==type)
			sprite_remove(s);
		s=n;
	}
}

void sprite_work(int type)
{
	SPRITE *s=sprite, *n;

	while(s!=NULL) {
		n=s->next;
		/* animate */
		s->ticks++;
		if(s->type&type) {
			if(s->anim_speed!=0) {
				// s->anim_count++;
				s->anim_count+=fps_factor;
				// if(s->anim_count==abs(s->anim_speed)) {
				if(s->anim_count>=abs(s->anim_speed)) {
					s->anim_count=0;
					if(s->anim_speed>0) {
						s->aktframe++;
						s->aktframe%=s->frames;
					} else {
						s->aktframe--;
						if(s->aktframe<0) s->aktframe=s->frames-1;
					}

				}
			}
			/* move */
			if(s->mover!=NULL) {
				(s->mover)(s);
			}
		}
		s=n;
	}
	s=sprite;
	while(s!=NULL) {
		n=s->next;
		if (s->ticks>1000) {
			s->ticks=0;
		 if ((s->priority==PR_ENEMY || s->priority== PR_ENEMY_WEAPON) && s->type!=SP_EN_BOSS01 && s->type!=SP_EN_BOSS02 && s->type!=SP_EN_BOSS03 && s->type!=SP_MENUTEXT)
			s->type=-1;
		}
		if(s->type==-1) {
			sprite_remove(s);
		}
		s=n;
	}
}

SPRITE *sprite_colcheck(SPRITE *tocheck, int type)
{
	SPRITE *s=sprite;

	while(s!=NULL) {
		if( (s!=tocheck) &&	/* zu testendes Sprite ueberspringen */
		    ((s->flags&(SP_FLAG_COLCHECK|SP_FLAG_VISIBLE))==(SP_FLAG_COLCHECK|SP_FLAG_VISIBLE)) &&
		    (s->type!=-1) &&
		    (s->type&type)) {	/* Passender Sprite-Type? */
			if(sprite_col_bounding(s,tocheck)) { /* Bounding Box Collosion? */
				if(sprite_col_pixel(s,tocheck)) { /* Pixel-Level Collison */
					return s;
				}
			}
		}
		s=s->next;
	}
	return NULL;
}


int sprite_col_bounding(SPRITE *a, SPRITE *b)
{
	if(a->x < b->x) {
		if(a->x+a->w > b->x) {
			if(a->y < b->y) {
				if(a->y+a->h > b->y) {
					rc.x=b->x;
					rc.y=b->y;
					return 1;
				}
			} else {
				if(b->y+b->h > a->y) {
					rc.x=b->x;
					rc.y=a->y;
					return 1;
				}
			}
		}
	} else {
		if(b->x+b->w > a->x) {
			if(b->y < a->y) {
				if(b->y+b->h > a->y) {
					rc.x=a->x;
					rc.y=a->y;
					return 1;
				}
			} else {
				if(a->y+a->h > b->y) {
					rc.x=a->x;
					rc.y=b->y;
					return 1;
				}
			}
		}
	}
	return 0;
}

int sprite_col_pixel(SPRITE *a, SPRITE *b)
{
	Sint16 w1=a->w;
	Sint16 h1=a->h;
	Sint16 x1=a->x;
	Sint16 y1=a->y;
	Sint16 w2=b->w;
	Sint16 h2=b->h;
	Sint16 x2=b->x;
	Sint16 y2=b->y;

	Sint32 x1o=0,x2o=0,y1o=0,y2o=0,offs;
	int i1=0, i2=0;

	Sint16 y,l;
	Uint8 *map1=a->cmap->col[a->aktframe];
	Uint8 *map2=b->cmap->col[b->aktframe];

	if(rc.x==x2 && rc.y==y2) {
		x1o=x2-x1;
		y1o=y2-y1;
		offs=w1*y1o+x1o;
		map1+=offs/8;
		i1=offs%8;
	} else if(rc.x==x2 && rc.y==y1) {
		x1o=x2-x1;
		y2o=y1-y2;
		map1+=x1o/8;
		i1=x1o%8;
		offs=w2*y2o;
		map2+=offs/8;
		i2=offs%8;
	} else if(rc.x==x1 && rc.y==y1) {
		x2o=x1-x2;
		y2o=y1-y2;
		offs=w2*y2o+x2o;
		map2+=offs/8;
		i2=offs%8;
	} else if(rc.x==x1 && rc.y==y2) {
		x2o=x1-x2;
		y1o=y2-y1;
		offs=w1*y1o;
		map1+=offs/8;
		i1=offs%8;
		map2+=x2o/8;
		i2=x2o%8;
	} else return 0;

	if(x1+w1 < x2+w2) {
		l=w1-x1o;
	} else {
		l=w2-x2o;
	}


	for(y=rc.y; y<=y1+h1 && y<=y2+h2; y++) {
		offs=sprite_memand(map1,map2,i1,i2,l);
		if(offs) {
			// col-koord: x=rc.x+offs-1, y=y
			return 1;
		}
		offs=(y-y1)*w1+x1o;
		map1=a->cmap->col[a->aktframe];
		map1+=offs/8;
		i1=offs%8;

		offs=(y-y2)*w2+x2o;
		map2=b->cmap->col[b->aktframe];
		map2+=offs/8;
		i2=offs%8;
	}

	return 0;

}

int sprite_memand(Uint8 *s1, Uint8 *s2, int shift1, int shift2, int n)
{
	int b;
	Uint16 i1=1<<shift1, i2=1<<shift2;

	for(b=0; b<n; b++) {
		if(i1==0x100) { i1=0x01; s1++; }
		if(i2==0x100) { i2=0x01; s2++; }
		if((*s1&i1) && (*s2&i2)) return b+1;
		i1<<=1; i2<<=1;
	}
	return 0;
}

void sprite_create_colmap(SDL_Surface *src, Uint8 *dst,int w,int f)
{
	Sint16 xstart,xstop,x,y, bit=1;
	Uint32 color_key;

	color_key=src->format->colorkey;
	xstart=w*f;
	xstop=w*(f+1);

	if(SDL_MUSTLOCK(src)) SDL_LockSurface(src);
	for(y=0; y<src->h; y++) {
		for(x=xstart; x<xstop; x++) {
			if(bit==0x100) { bit=1; dst++; }
			if(getpixel(src,x,y) != color_key) {
				*dst=*dst|bit;
			}
			bit<<=1;
		}
	}
	if(SDL_MUSTLOCK(src)) SDL_UnlockSurface(src);
}

SPRITE *sprite_get_by_id(int id)
{
	SPRITE *s=sprite;

	if(id==-1)
		return NULL;

	while(s!=NULL) {
		if(s->id==id)
			return(s);
		s=s->next;
	}
	return NULL;
}

/* sprite controller */

CONTROLLER *controller_add()
{
	CONTROLLER *new;

	new=mmalloc(sizeof(CONTROLLER));
	new->next=cont;
	cont=new;
	new->e=NULL;
	new->con=NULL;
	new->max=0;
	return new;
}

void controller_remove(CONTROLLER *id)
{
	CONTROLLER *c=cont, *p=NULL;

	while(c!=NULL) {
		if(c==id) {
			if(p!=NULL)
				p->next=c->next;
			else
				cont=c->next;
			if(c->e!=NULL)
				free(c->e);
			free(c);
			return;
		}
		p=c;
		c=c->next;
	}
	error(ERR_WARN,"controller %x not found",id);
}

void controller_remove_all()
{
	CONTROLLER *c=cont, *n;

	while(c!=NULL) {
		n=c->next;
		if(c->e)
			free(c->e);
		free(c);
		c=n;
	}
	cont=NULL;
}

void controller_work()
{
	CONTROLLER *c=cont, *n=NULL;

	while(c!=NULL) {
		n=c->next;
		if(c->con!=NULL)
			(c->con)(c);
		c=n;
	}
}
