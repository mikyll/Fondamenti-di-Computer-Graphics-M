#pragma once

#include "commons.h"

void initSoundModule(int channelNumbers);
void loadMusic(const char* id, const char* musicName);
void loadSoundEffect(const char* id, const char* soundEffectName);
int getMusicVolume();
void setMusicVolume(int volume);
void playMusic(const char* id);
void stopMusic();
int getSoundEffectsVolume(int channel);
void setSoundEffectsVolume(int channel, int volume);
void playSoundEffect(int channel, const char* soundName);