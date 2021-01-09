#ifndef __SIMPLE_RESOURCE_H__
#define __SIMPLE_RESOURCE_H__

#include <string>
#include "Resource/Resource.h"

namespace FW {
  class SimpleResource: public Resource {
  public:
    SimpleResource(Resource::Data&& text): text{std::move(text)} {
    };

    SimpleResource(SimpleResource&& other): text{std::move(other.text)} {
    };

    virtual Resource::Buffer buffer() const override {
      return &text[0];
    }

    virtual std::size_t size() const override {
      return text.size();
    }

  private:
    Resource::Data text;
  };
}

#endif
