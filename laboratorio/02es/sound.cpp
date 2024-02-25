#include "sound.h"

// SDL2
#include <SDL.h>
#include <SDL_mixer.h>

// sound list
std::map<std::string, Mix_Music*> musicTracks;
std::map<std::string, Mix_Chunk*> soundEffects;
bool soundModuleInitialized = false;

void initSoundModule(int channelNumbers)
{
	// Init SDL2 (we use it only to play sounds)
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
    {
        std::cout << "SDL could not be initialized!" << std::endl
            << "  SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    // Init SDL2_mixer
    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
    {
        std::cout << "SDL2_mixer could not be initialized!" << std::endl
            << "  SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    Mix_AllocateChannels(channelNumbers);

    soundModuleInitialized = true;
}

void loadMusic(const char* id, const char* musicName)
{
    Mix_Music* music;

    if (!soundModuleInitialized)
    {
        std::cout << "Music '" << musicName << "' could not be loaded!" << std::endl
            << "  Sound module not initialized" << std::endl;
        return;
    }

    if (!(music = Mix_LoadMUS(musicName)))
    {
        std::cout << "Music '" << musicName << "' could not be loaded!" << std::endl
            << "  SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    musicTracks.insert({ id, music });
}

void loadSoundEffect(const char* id, const char* soundEffectName)
{
    Mix_Chunk* sound;

    if (!soundModuleInitialized)
    {
        std::cout << "Sound effect '" << soundEffectName << "' could not be loaded!" << std::endl
            << "  Sound module not initialized" << std::endl;
        return;
    }

    if (!(sound = Mix_LoadWAV(soundEffectName)))
    {
        std::cout << "Sound effect '" << soundEffectName << "' could not be loaded!" << std::endl
            << "  SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    soundEffects.insert({ id, sound });
}

int getMusicVolume()
{
    return Mix_VolumeMusic(-1);
}

void setMusicVolume(int volume)
{
    if (volume < 0 || volume > MIX_MAX_VOLUME)
    {
        std::cout << "Invalid music volume: " << volume << ".  Value must be between 0 and " << MIX_MAX_VOLUME << std::endl;
    }

    Mix_VolumeMusic(volume);
}

void playMusic(const char* id)
{
    if (musicTracks.count(id))
    {
        Mix_PlayMusic(musicTracks.at(id), -1);
    }
}

// Stops any music playing
void stopMusic()
{
    Mix_HaltMusic();
}

int getSoundEffectsVolume(int channel)
{
    return Mix_Volume(channel, -1);
}

void setSoundEffectsVolume(int channel, int volume)
{
    if (volume < 0 || volume > MIX_MAX_VOLUME)
    {
        std::cout << "Invalid sound effects volume: " << volume << ".  Value must be between 0 and " << MIX_MAX_VOLUME << std::endl;
    }

    Mix_Volume(channel, -1);
}

void playSoundEffect(int channel, const char* id)
{
    if (soundEffects.count(id))
    {
        Mix_PlayChannel(channel, soundEffects.at(id), 0);
    }
}