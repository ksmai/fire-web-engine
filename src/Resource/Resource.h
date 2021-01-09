#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include <vector>

namespace FW {
  class Resource {
  public:
    using Data = std::vector<unsigned char>;
    using Buffer = const unsigned char*;
    using Size = std::size_t;

    Resource() =default;
    Resource(const Resource&) =delete;
    Resource& operator=(const Resource&) =delete;

    virtual ~Resource() {
    }

    virtual Buffer buffer() const =0;
    virtual Size size() const =0;
  };
}

#endif
