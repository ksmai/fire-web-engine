#ifndef __MUSIC_H__
#define __MUSIC_H__

#include "SDL/SDL_mixer.h"
#include "File/Data.h"

namespace FW {
  class Music {
  public:
    friend class Audio;
    
    Music(Data&&);
    Music(const Music&) =delete;
    Music& operator=(const Music&) =delete;
    Music(Music&&);
    ~Music();

  private:
    Mix_Music* music;
  };
}
#endif
