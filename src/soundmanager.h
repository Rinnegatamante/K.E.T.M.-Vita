/*
 * $Id: soundmanager.h,v 1.1.1.1 2002/11/03 11:08:24 kenta Exp $
 *
 * Copyright 2002 Kenta Cho. All rights reserved.
 */

/**
 * BGM/SE manager header file.
 *
 * @version $Revision: 1.1.1.1 $
 */
void closeSound();
void initSound();
void playMusic(char*);
void fadeMusic();
void stopMusic();
void pauseMusic();
void resumeMusic();
void playChunk(int idx);
void setChunkVolume(int volume);
////////////////added by Farox
void GlobalVolumeUp();
void GlobalVolumeDown();
//////////////////////////////
