#ifndef __RESOURCE_CACHE_H__
#define __RESOURCE_CACHE_H__

#include <string>
#include <vector>
#include <map>
#include <memory>
#include "Resource/Fetcher.h"
#include "Resource/ZipFile.h"
#include "Resource/Loader.h"

namespace FW {
  class ResourceCache {
  public:
    using ResourcePtr = std::unique_ptr<Resource>;
    using LoaderPtr = std::unique_ptr<Loader>;

    ResourceCache();
    ResourceCache(const ResourceCache&) =delete;
    ResourceCache& operator=(const ResourceCache&) =delete;

    void addLoader(Loader*);

    bool isLoading() const;

    void loadZipFile(const std::string&);

    Resource* getResource(const std::string&) const;

    static void fetchSuccessCallback(emscripten_fetch_t*);
    static void fetchErrorCallback(emscripten_fetch_t*);

  private:
    void processZipFile(ZipFile::ZipFileData, ZipFile::Size);

    Loader* getLoader(const std::string&) const;

    Fetcher fetcher;
    std::vector<LoaderPtr> loaders;
    std::map<std::string, ResourcePtr> resources;
    int numLoading = 0;
  };
}

#endif
