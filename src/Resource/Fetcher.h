#ifndef __FETCHER_H__
#define __FETCHER_H__

#include <string>
#include <functional>
#include <emscripten/fetch.h>

namespace FW {
  class Fetcher {
  public:
    using Callback = void(*)(emscripten_fetch_t*);

    Fetcher();

    void fetch(const std::string& url, Callback onsuccess, Callback onerror);

  private:
    emscripten_fetch_attr_t attr;
  };
}

#endif
