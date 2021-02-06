#ifndef __SPRITESHEET_REPOSITORY_H__
#define __SPRITESHEET_REPOSITORY_H__

#include "Game/Repository.h"
#include "Graphics/Graphics.h"

namespace FW {
  class SpriteSheetRepository: public Repository<SpriteSheet> {
  public:
    SpriteSheetRepository(Graphics* graphics);
    virtual void load(ZipFile& resources) override;

  private:
    Graphics* graphics;
  };
}

#endif
