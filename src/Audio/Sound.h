#ifndef __SOUND_H__
#define __SOUND_H__

#include "SDL/SDL_mixer.h"
#include "File/Data.h"

namespace FW {
  class Sound {
  public:
    friend class Audio;
    
    Sound(Data&&);
    Sound(const Sound&) =delete;
    Sound& operator=(const Sound&) =delete;
    Sound(Sound&&);
    ~Sound();

  private:
    Mix_Chunk* chunk;
  };
}
#endif
