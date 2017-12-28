#include "score.h"

extern HSC_LIST hsc_table[];
extern SDL_Surface *screen;
extern SPRITE *player;

void score_display()
{
	PLAYER_DATA *p=(PLAYER_DATA *)player->data;
	char buffer[100];
	char buf[50];


	//sprintf(buffer,"SHIPS : %d",p->lives);
	//font_print(buffer,FONT07,0,10);

	//sprintf(buffer,"SPEED : %d",p->player_speed);
	//font_print(buffer,FONT07,0,20);

	//sprintf(buffer,"STAGE : %d",p->level);
	//font_print(buffer,FONT07,0,30);

	//sprintf(buf,"WEAPON: ");
	sprintf(buf,"W: ");
	switch(p->weapon) {
	case WP_PLASMA:
		strcat(buf,"PLASMA");
		break;
	case WP_DOUBLEPLASMA:
		//strcat(buf,"DOUBLE-PLASMA");
		strcat(buf,"2X-PLASMA");
		break;
	case WP_QUADPLASMA:
		//strcat(buf,"QUAD-PLASMA");
		strcat(buf,"4X-PLASMA");
		break;
	case WP_FIREBALL:
		strcat(buf,"FIRERAY");
		break;
	case WP_DOUBLEFIREBALL:
		//strcat(buf,"DOUBLE-FIRERAY");
		strcat(buf,"2X-FIRERAY");
		break;
	case WP_QUADFIREBALL:
		//strcat(buf,"QUAD-FIRERAY");
		strcat(buf,"4X-FIRERAY");
		break;
	case WP_KILLRAY:
		strcat(buf,"KILLRAY");
		break;
	default:
		strcat(buf,"UNKNOWN ???");
		break;
	}

	sprintf(buffer,"SCORE: %06d  LIVES: %d  %s",p->score, p->lives, buf);
	font_print(buffer,FONT07,0,0);

	if(p->extra!=PLX_NONE) {
		sprintf(buffer,"EXTRA: ");
		switch(p->extra) {
		case PLX_HOMING:
			strcat(buffer,"HOMING MISSILE");
			break;
		case PLX_SHIELD:
			strcat(buffer,"SHIELD");
			break;
		case PLX_HLASER:
			strcat(buffer,"HOMING LASER");
			break;
		default:
			strcat(buffer,"UNKNOWN ???");
			break;
		}
		sprintf(buf,"  X-TIME: %d",(int)p->extra_time/10);
		strcat(buffer,buf);

		font_print(buffer,FONT07,0,10);
	}

}

void score_cleanup()
{
}
