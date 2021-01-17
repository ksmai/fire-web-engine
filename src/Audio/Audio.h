#ifndef __AUDIO_H__
#define __AUDIO_H__

#include "SDL/SDL_mixer.h"
#include "File/Data.h"
#include "Audio/Music.h"
#include "Audio/Sound.h"

namespace FW {
  class Audio {
  public:
    Audio();
    Audio(const Audio&) =delete;
    Audio& operator=(const Audio&) =delete;
    ~Audio();

    Music loadMusic(Data&&) const;
    void playMusic(const Music&) const;
    void stopMusic() const;
    bool isPlayingMusic() const;

    Sound loadSound(Data&&) const;
    void playSound(const Sound&) const;

  private:
    static bool created;
  };
}
#endif
