#ifndef __REMOTE_FILE_H__
#define __REMOTE_FILE_H__

#include <string>
#include "emscripten/fetch.h"
#include "File/Data.h"

namespace FW {
  class RemoteFile {
  public:
    explicit RemoteFile(const std::string& url);
    RemoteFile(const RemoteFile&) =delete;
    RemoteFile& operator=(const RemoteFile&) =delete;
    ~RemoteFile();

    void open();
    bool isOpened() const;
    Data getData() const;
    bool isError() const;
    const std::string getError() const;
    void close();

  private:
    static void onSuccess(emscripten_fetch_t*);
    static void onError(emscripten_fetch_t*);

    const std::string url;
    emscripten_fetch_t* result;
    bool succeeded;
  };
}

#endif
