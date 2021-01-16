#ifndef __SPRITE_POSITION_H__
#define __SPRITE_POSITION_H__

namespace FW {
  struct SpritePosition {
    unsigned int x;
    unsigned int y;
    unsigned int spanX{1};
    unsigned int spanY{1};
  };
}
#endif
