#include <iostream>
#include "Resource/ResourceCache.h"

FW::ResourceCache::ResourceCache() {
  file.open();
}

void FW::ResourceCache::addLoader(Loader* loader) {
  loaders.push_back(std::unique_ptr<Loader>{loader});
}

bool FW::ResourceCache::isLoading() const {
  return !file.isOpened() && !file.isError();
}

void FW::ResourceCache::update() {
  if (processed || isLoading()) {
    return;
  }
  processed = true;
  processZipFile();
}

void FW::ResourceCache::processZipFile() {
  ZipFile f{file.getData()};
  for (std::size_t i = 0; i < f.getNumFiles(); ++i) {
    if (f.getFileSize(i) == 0) {
      continue;
    }
    std::string name = f.getFileName(i);
    if (resources.find(name) != resources.end()) {
      std::cout << "Duplicated file name: " << name << "\n";
      throw "Duplicated file name";
    }
    Loader* loader = getLoader(name);
    if (!loader) {
      continue;
    }
    resources[name] = std::unique_ptr<Resource>{loader->load(f.getFileContent(i))};
    std::cout << "Loaded: " << name << "\n";
  }
}

FW::Loader* FW::ResourceCache::getLoader(const std::string& name) const {
  for (auto& loader: loaders) {
    if (std::regex_match(name, loader->pattern())) {
      return loader.get();
    }
  }
  return nullptr;
}

FW::Resource* FW::ResourceCache::getResource(const std::string& name) const {
  auto it = resources.find(name);
  if (it == resources.end()) {
    std::cout << "Cannot get resource: " << name << "\n";
    throw "cannot get resource";
  }
  return it->second.get();
}
