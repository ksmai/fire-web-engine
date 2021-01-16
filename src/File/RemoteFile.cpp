#include <cstring>
#include "App/Logger.h"
#include "File/RemoteFile.h"

FW::RemoteFile::RemoteFile(const std::string& url):
  url{url},
  result{nullptr},
  succeeded{false}
{
}

FW::RemoteFile::~RemoteFile() {
  close();
}

void FW::RemoteFile::open() {
  emscripten_fetch_attr_t attr;
  emscripten_fetch_attr_init(&attr);
  std::strcpy(attr.requestMethod, "GET");
  attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
  attr.userData = this;
  attr.onsuccess = onSuccess;
  attr.onerror = onError;
  emscripten_fetch(&attr, url.c_str());
}

void FW::RemoteFile::onSuccess(emscripten_fetch_t* result) {
  RemoteFile* file{static_cast<RemoteFile*>(result->userData)};
  file->result = result;
  file->succeeded = true;
}

void FW::RemoteFile::onError(emscripten_fetch_t* result) {
  RemoteFile* file{static_cast<RemoteFile*>(result->userData)};
  file->result = result;
  file->succeeded = false;
}

bool FW::RemoteFile::isOpened() const {
  return result && succeeded;
}

bool FW::RemoteFile::isError() const {
  return result && !succeeded;
}

const std::string FW::RemoteFile::getError() const {
  if (!isError()) {
    Logger::error("Trying to get error when there is none");
    return "";
  }
  return result->statusText;
}

FW::Data FW::RemoteFile::getData() const {
  if (!isOpened()) {
    Logger::error("Trying to get data when file is not opened");
    return Data{};
  }
  const char* dataBegin{result->data};
  const char* dataEnd{dataBegin + result->numBytes};
  return Data{dataBegin, dataEnd};
}

void FW::RemoteFile::close() {
  if (result) {
    emscripten_fetch_close(result);
  }
}
