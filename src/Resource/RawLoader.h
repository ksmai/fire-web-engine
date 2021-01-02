#ifndef __RAW_LOADER_H__
#define __RAW_LOADER_H__

#include "Resource/Loader.h"

namespace FW {
  class RawLoader: public Loader {
  public:
    virtual std::regex pattern() const override;

    virtual Resource* load(Resource::Data&& data) const override;
  };
}

#endif
