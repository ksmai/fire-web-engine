#ifndef __LOADER_H__
#define __LOADER_H__

#include <regex>
#include "Resource/Resource.h"

namespace FW {
  class Loader {
  public:
    virtual std::regex pattern() const;
    virtual Resource* load(Resource::Data&& data) const;

    virtual ~Loader() {}
  };
}
#endif
