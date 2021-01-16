#ifndef __PROCESS_MANAGER_H__
#define __PROCESS_MANAGER_H__

#include <vector>
#include "Process/Process.h"

namespace FW {
  class ProcessRunner {
  public:
    void update(double dt);
    
    void attachProcess(Process::StrongPtr);

  private:
    std::vector<Process::StrongPtr> processes;

    static const std::size_t MAX_PROCESSES;
  };
}
#endif
