#ifndef __TEXT_LOADER_H__
#define __TEXT_LOADER_H__

#include "Resource/Loader.h"

namespace FW {
  class TextLoader: public Loader {
  public:
    virtual std::regex pattern() const override;

    virtual Resource* load(Resource::Data&& data) const override;
  };
}

#endif
