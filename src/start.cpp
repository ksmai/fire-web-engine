#include <iostream>
#include <emscripten.h>

extern "C" {
  EMSCRIPTEN_KEEPALIVE
  void start() {
    std::cout << "using cout\n";
  }
}
