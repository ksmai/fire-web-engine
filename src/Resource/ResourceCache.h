#ifndef __RESOURCE_CACHE_H__
#define __RESOURCE_CACHE_H__

#include <string>
#include <vector>
#include <map>
#include <memory>
#include "File/ZipFile.h"
#include "File/RemoteFile.h"
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
    void update();

    Resource* getResource(const std::string&) const;

  private:
    void processZipFile();
    Loader* getLoader(const std::string&) const;

    RemoteFile file{"/resources.zip"};
    bool processed{false};
    std::vector<LoaderPtr> loaders;
    std::map<std::string, ResourcePtr> resources;
  };
}

#endif
