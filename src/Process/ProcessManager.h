#ifndef __PROCESS_MANAGER_H__
#define __PROCESS_MANAGER_H__

#include <vector>
#include "Process/Process.h"

namespace FW {
  class ProcessManager {
  public:
    void update(double dt);
    
    void attachProcess(Process::StrongPtr);

    void failAll(bool immediate);

  private:
    std::vector<Process::StrongPtr> processes;
  };
}
#endif
