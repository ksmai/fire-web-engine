#include <iostream>
#include "Resource/ResourceCache.h"

FW::ResourceCache::ResourceCache(): fetcher{Fetcher{this}} {
}

void FW::ResourceCache::addLoader(Loader* loader) {
  loaders.push_back(std::unique_ptr<Loader>{loader});
}

bool FW::ResourceCache::isLoading() const {
  return numLoading > 0;
}

void FW::ResourceCache::loadZipFile(const std::string& url) {
  ++numLoading;
  fetcher.fetch(url, fetchSuccessCallback, fetchErrorCallback);
}

void FW::ResourceCache::fetchSuccessCallback(emscripten_fetch_t* fetch) {
  if (!fetch->userData) {
    std::cout << "Can't find ResourceCache instance in fetchSuccessCallback\n";
    throw "Can't find ResourceCache instance in fetchSuccessCallback\n";
  }
  std::cout << "Downloaded " << fetch->numBytes << " bytes from " << fetch->url << "\n";
  static_cast<ResourceCache*>(fetch->userData)->processZipFile(
    reinterpret_cast<ZipFile::ZipFileData>(fetch->data),
    fetch->numBytes
  );
  emscripten_fetch_close(fetch);
}

void FW::ResourceCache::fetchErrorCallback(emscripten_fetch_t* fetch) {
  std::cout << "Fail to download " << fetch->url << " [" << fetch->status << "]\n";
  emscripten_fetch_close(fetch);
  throw "fetch error";
}

void FW::ResourceCache::processZipFile(ZipFile::ZipFileData data, ZipFile::Size size) {
  ZipFile file{data, size};
  for (std::size_t i = 0; i < file.getNumFiles(); ++i) {
    if (file.getFileSize(i) == 0) {
      continue;
    }
    std::string name = file.getFileName(i);
    if (resources.find(name) != resources.end()) {
      std::cout << "Duplicated file name: " << name << "\n";
      throw "Duplicated file name";
    }
    Loader* loader = getLoader(name);
    if (!loader) {
      continue;
    }
    resources[name] = std::unique_ptr<Resource>{loader->load(file.getFileContent(i))};
    std::cout << "Loaded: " << name << "\n";
  }
  --numLoading;
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
