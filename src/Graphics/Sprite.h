#ifndef __SPRITE_H__
#define __SPRITE_H__

namespace FW {
  struct Sprite {
    unsigned int x;
    unsigned int y;
    unsigned int spanX{1};
    unsigned int spanY{1};
  };
}
#endif
