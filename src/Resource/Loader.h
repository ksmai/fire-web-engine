#ifndef __LOADER_H__
#define __LOADER_H__

#include <regex>
#include "Resource/Resource.h"

namespace FW {
  class Loader {
  public:
    virtual std::regex pattern() const =0;
    virtual Resource* load(Resource::Data&& data) const =0;

    virtual ~Loader() {}
  };
}
#endif
