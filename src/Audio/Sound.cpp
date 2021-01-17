#include "SDL/SDL.h"
#include "App/abort.h"
#include "App/Logger.h"
#include "Audio/Sound.h"

FW::Sound::Sound(Data&& data) {
  SDL_RWops* rw{SDL_RWFromConstMem(&data[0], data.size())};
  if (!rw) {
    Logger::error("SDL_RWFromConstMem() failed: %s",  SDL_GetError());
    abort();
  }
  chunk = Mix_LoadWAV_RW(rw, 1);
  if (!chunk) {
    Logger::error("Mix_loadWAV_RW() failed: %s", Mix_GetError());
    abort();
  }
}

FW::Sound::Sound(Sound&& moved) {
  chunk = moved.chunk;
  moved.chunk = nullptr;
}

FW::Sound::~Sound() {
  if (chunk) {
    Mix_FreeChunk(chunk);
  }
}
