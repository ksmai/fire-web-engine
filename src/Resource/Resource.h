#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include <vector>

namespace FW {
  class Resource {
  public:
    using Data = std::vector<unsigned char>;

    Resource(Data&& data): data{data} {
    };

    Resource(const Resource&) =delete;
    Resource& operator=(const Resource&) =delete;

    Resource(Resource&& other): data{std::move(other.data)} {
    };

    const unsigned char* buffer() const {
      return &data[0];
    }

  private:
    Data data;
  };
}

#endif
