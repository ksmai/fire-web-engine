#ifndef __DELAY_PROCESS_H__
#define __DELAY_PROCESS_H__

#include "Process/Process.h"

namespace FW {
  class DelayProcess: public Process {
  public:
    explicit DelayProcess(double);

    virtual void update(double) override;

    virtual void onSuccess() override;
    

  private:
    double timeToDelay;
    double timeDelayedSoFar;
  };
}

#endif
