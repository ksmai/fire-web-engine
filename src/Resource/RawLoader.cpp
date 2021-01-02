#include "Resource/RawLoader.h"

std::regex FW::RawLoader::pattern() const {
  return std::regex(".*");
}

FW::Resource* FW::RawLoader::load(FW::Resource::Data&& data) const {
  return new FW::Resource{std::move(data)};
}
