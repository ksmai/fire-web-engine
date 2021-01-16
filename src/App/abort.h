#ifndef __ABORT_H__
#define __ABORT_H__

#include <cstdlib>

namespace FW {
  void abort() {
    std::abort();
  }
}
#endif
