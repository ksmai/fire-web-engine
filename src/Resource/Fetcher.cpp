#include <cstring>
#include "Resource/Fetcher.h"

FW::Fetcher::Fetcher() {
  emscripten_fetch_attr_init(&attr);
  std::strcpy(attr.requestMethod, "GET");
  attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
}

void FW::Fetcher::fetch(const std::string& url, Callback onsuccess, Callback onerror) {
  attr.onsuccess = onsuccess;
  attr.onerror = onerror;
  emscripten_fetch(&attr, url.c_str());
}
