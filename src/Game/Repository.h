#ifndef __REPOSITORY_H__
#define __REPOSITORY_H__

#include <map>
#include "App/abort.h"
#include "App/Logger.h"
#include "File/ZipFile.h"

namespace FW {
  template <typename T>
  class Repository {
  public:
    using ID = std::size_t;

    Repository() =default;
    Repository(const Repository&) =delete;
    Repository& operator=(const Repository&) =delete;
    virtual ~Repository() =default;

    void add(ID id, T&& element) {
      if (elements.find(id) != elements.end()) {
        Logger::error("Duplicated element ID in repository: %d", id);
        abort();
      }
      elements.emplace(id, std::move(element));
    }

    const T& get(ID id) const {
      auto it = elements.find(id);
      if (it == elements.end()) {
        Logger::error("Failed to get element from repository: %d", id);
        abort();
      }
      return it->second;
    }

    virtual void load(ZipFile&) =0;

  private:
    std::map<ID, T> elements;
  };
}
#endif
