#ifndef __PROCESS_MANAGER_H__
#define __PROCESS_MANAGER_H__

#include <list>
#include "Process/Process.h"

namespace FW {
  class ProcessManager {
  public:
    void update(double dt);
    
    void attachProcess(Process::StrongPtr);

    void abortAll(bool immediate);

    std::size_t count() const;

  private:
    void clearAll();

    std::list<Process::StrongPtr> processes;
  };
}
#endif
