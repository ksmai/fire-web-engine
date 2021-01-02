#ifndef __STRING_LOADER_H__
#define __STRING_LOADER_H__

#include "Resource/Loader.h"

namespace FW {
  class StringLoader: public Loader {
  public:
    virtual std::regex pattern() const override;

    virtual Resource* load(Resource::Data&& data) const override;
  };
}

#endif
