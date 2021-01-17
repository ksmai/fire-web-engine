#include "SDL/SDL.h"
#include "App/abort.h"
#include "App/Logger.h"
#include "Audio/Music.h"

FW::Music::Music(Data&& data) {
  SDL_RWops* rw{SDL_RWFromConstMem(&data[0], data.size())};
  if (!rw) {
    Logger::error("SDL_RWFromConstMem() failed: %s",  SDL_GetError());
    abort();
  }
  music = Mix_LoadMUSType_RW(rw, MUS_NONE, 1);
  if (!music) {
    Logger::error("Mix_loadMUS_RW() failed: %s", Mix_GetError());
    abort();
  }
}

FW::Music::Music(Music&& moved) {
  music = moved.music;
  moved.music = nullptr;
}

FW::Music::~Music() {
  if (music) {
    Mix_FreeMusic(music);
  }
}
