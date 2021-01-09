#include "Resource/RawLoader.h"
#include "Resource/SimpleResource.h"

std::regex FW::RawLoader::pattern() const {
  return std::regex(".*");
}

FW::Resource* FW::RawLoader::load(FW::Resource::Data&& data) const {
  return new FW::SimpleResource{std::move(data)};
}
