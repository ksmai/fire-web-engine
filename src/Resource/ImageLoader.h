#ifndef __IMAGE_LOADER_H__
#define __IMAGE_LOADER_H__

#include "Resource/Loader.h"

namespace FW {
  class ImageLoader: public Loader {
  public:
    virtual std::regex pattern() const override;

    virtual Resource* load(Resource::Data&& data) const override;
  };
}

#endif
