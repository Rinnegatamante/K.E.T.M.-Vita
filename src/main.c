#include "main.h"
#include <vitasdk.h>
#include <vita2d.h>

extern SDL_Surface *screen;
extern int keyboard[];

GAMESTATE state;
GAMESTATE laststate;

int score;
int lives;
int volume=2; /* 0-3 */
int difficulty=DIFF_EASY;

char mods[5][20];
int modcount=0;
int selected=0;
char tempstr[256];

void wait(){
        int i,j;
        for (i=0; i<10000; i++) {
                for(j=0; j<2000; j++)
			j++;
	}
}


void readModDirs( ) {
/*
	fprintf(stdout,"readDataDirs\n");
  DIR *dp;
  struct dirent *dir;
  if ( (dp = opendir(".")) == NULL ) {
    fprintf(stderr, "Can't open directory: ");
    exit(1);
  }
  while ((dir = readdir(dp)) != NULL) {
	if (dir->d_name[0]!='_') continue;
    fprintf(stdout,"%s\n", dir->d_name);
    strcpy(mods[modcount],dir->d_name);
    modcount++;
  }
  closedir(dp);
*/
    strcpy(mods[0],"KETM-episode1");
    strcpy(mods[1],"1941");
	strcpy(mods[2],"Star Wars");
    strcpy(mods[3],"Iron Man");
    strcpy(mods[4],"Mario Kart");
    modcount=5;
}

int y = 20;
uint32_t white;
vita2d_pgf* debug_font;
void consolePrintf(const char *format, ...){
	char str[512] = { 0 };
	va_list va;

	va_start(va, format);
	vsnprintf(str, 512, format, va);
	va_end(va);

	vita2d_pgf_draw_text(debug_font, 5, y, white, 1.0, str);
	y+=20;
}

void chooseModDir() {
	int i;
	int done = 0;
    uint32_t keys;
	memset(moddir,0,64);

	readModDirs();
	
	vita2d_init();
	vita2d_set_clear_color(RGBA8(0x00, 0x00, 0x00, 0xFF));
	debug_font = vita2d_load_default_pgf();
	white = RGBA8(0xFF, 0xFF, 0xFF, 0xFF);
	
	while (!done) {
		vita2d_start_drawing();
		vita2d_clear_screen();
		consolePrintf("K.E.T.M. VITA v.1.0");
		consolePrintf(" ");
		consolePrintf("Please choose the mod:");
		consolePrintf(" ");
		consolePrintf(" ");
		for (i=0; i<modcount; i++) {
			if (i==selected) consolePrintf(">>> %s",mods[i]);
			else consolePrintf("    %s",mods[i]);	
		}
		SceCtrlData pad;
		sceCtrlPeekBufferPositive(0, &pad, 1);
		if ((pad.buttons & SCE_CTRL_UP) && (!(keys & SCE_CTRL_UP))){
			if (selected>0) selected--;
		}
		if ((pad.buttons & SCE_CTRL_DOWN) && (!(keys & SCE_CTRL_DOWN))){
			if (selected<modcount-1) selected++;
		}
		if (pad.buttons & SCE_CTRL_CROSS){ 
			sprintf(moddir,"ux0:data/KETM/%s",mods[selected]);
			done=1;
		}
		
		consolePrintf(" ");
		consolePrintf(" ");
		consolePrintf(" ");
		consolePrintf("Thanks to: ");
		consolePrintf("- Billy McLaughlin II");
		consolePrintf("- Styde Pregny");
		consolePrintf("- XandridFire");
		consolePrintf("for their awesome support on Patreon!");
		
		vita2d_end_drawing();
		vita2d_wait_rendering_done();
		vita2d_swap_buffers();
		keys = pad.buttons;
		y = 20;
	}
	vita2d_fini();
	wait();
}


int main(int argc, char *argv[])
{
// In case game dir doesn't exist
    sceIoMkdir("ux0:data/KETM", 0777);

	printf("Starting K.E.T.M.\n");

	chooseModDir(); 
	
	for(int i=0;i<modcount;i++){
		sprintf(tempstr,"ux0:data/KETM/%s",mods[i]);
		sceIoMkdir(tempstr, 0777);
	}

	game_init(argc, argv);
	playMusic("intro");
	while (state.mainstate!=ST_GAME_QUIT) {
		keyboard_poll();

		switch(state.mainstate) {
			case ST_START_INTRO:
				if(state.newstate) {
					startintro_init();
					state.newstate=0;
				}
				startintro_work();
				break;
			case ST_INTRO:
				if(state.newstate) {
					intro_init();
					state.newstate=0;
				}
				intro_work();
				break;
			case ST_GAME_PLAY:
				if(state.newstate) {
					playChunk(1);
					playMusic("stage1");
					thegame_init();
					state.newstate=0;
				}
				thegame_work();
				break;
			case ST_GAME_OVER:
				if(state.newstate) {
					playChunk(4);
					stopMusic();
					gameover_init();
					playMusic("intro");
					state.newstate=0;
				}
				gameover_work();
                                //newstate(ST_INTRO,0,1);
				break;
			case ST_MENU:
				if(state.newstate) {
					menu_init();
					state.newstate=0;
				}
				menu_work();
				break;
			case ST_SHOW_HCLIST:
				if(state.newstate) {
					hsc_show_init();
					state.newstate=0;
				}
				hsc_show_work();
				break;
			case ST_ENTRY_HCLIST:
				if(state.newstate) {
					hsc_entry_init();
					state.newstate=0;
				}
				hsc_entry_work();
				break;
		}
		
		fps_show();
		SDL_Flip(screen);
		fps_newframe();
	}

	/* TODO: Free everything (memory, SDL_Surfaces, Joysticks...) */

	hsc_save();
	fprintf(stdout,"Thank you for playing\n");
	exit(0);
}
