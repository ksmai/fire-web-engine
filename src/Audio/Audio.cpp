#include "SDL/SDL.h"
#include "App/abort.h"
#include "App/Logger.h"
#include "Audio/Audio.h"

bool FW::Audio::created{false};

FW::Audio::Audio() {
  if (created) {
    Logger::error("Cannot create more than 1 Audio");
    abort();
  }
  created = true;

  if (SDL_InitSubSystem(SDL_INIT_AUDIO)) {
    Logger::error("SDL_InitSubSystem(SDL_INIT_AUDIO) failed: %s", SDL_GetError());
    abort();
  }

  // Similar to SDL_image, we can't do Mix_Init here for some reasons
  if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024)) {
    Logger::error("Mix_OpenAudio() failed: %s", Mix_GetError());
    abort();
  }
}

FW::Audio::~Audio() {
  Mix_CloseAudio();
  SDL_QuitSubSystem(SDL_INIT_AUDIO);
  created = false;
}

FW::Music FW::Audio::loadMusic(Data&& data) const {
  return Music{std::move(data)};
}

void FW::Audio::playMusic(const Music& music) const {
  if (Mix_PlayMusic(music.music, -1) == -1) {
    Logger::error("Mix_PlayMusic() failed: %s", Mix_GetError());
  }
}

void FW::Audio::stopMusic() const {
  Mix_HaltMusic();
}

bool FW::Audio::isPlayingMusic() const {
  return Mix_PlayingMusic();
}

FW::Sound FW::Audio::loadSound(Data&& data) const {
  return Sound{std::move(data)};
}

void FW::Audio::playSound(const Sound& sound) const {
  if (Mix_PlayChannel(-1, sound.chunk, 0) == -1) {
    Logger::error("Mix_PlayChannel() failed: %s", Mix_GetError());
  }
}
