#include "Resource/StringLoader.h"

std::regex FW::StringLoader::pattern() const {
  return std::regex("(.*)\\.glsl");
}

FW::Resource* FW::StringLoader::load(FW::Resource::Data&& data) const {
  data.push_back('\0');
  return new Resource{std::move(data)};
}
