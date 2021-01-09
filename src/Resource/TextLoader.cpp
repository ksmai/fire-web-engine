#include "Resource/TextLoader.h"
#include "Resource/SimpleResource.h"

std::regex FW::TextLoader::pattern() const {
  return std::regex{".*\\.glsl"};
}

FW::Resource* FW::TextLoader::load(FW::Resource::Data&& data) const {
  data.push_back('\0');
  return new SimpleResource{std::move(data)};
}
